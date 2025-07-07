#include "THILDataParser.h"
#include "THILDataParserException.h"

#include <bitset>

#include "TChannel.h"
#include "Globals.h"

#include "TScalerQueue.h"

#include "TEpicsFrag.h"
#include "TParsingDiagnostics.h"

#include "Rtypes.h"

#include "THILFragment.h"
#include "TBadFragment.h"

THILDataParser::THILDataParser()
   : TDataParser()
{
   fState = EDataParserState::kGood;
}

THILDataParser::~THILDataParser()
{
}

int THILDataParser::Process(std::shared_ptr<TRawEvent> rawEvent)
{
   /// Process this TRlmdEvent using the provided data parser.
   /// Returns the total number of fragments read (good and bad).
   // right now the parser only returns the total number of fragments read
   // so we assume (for now) that all fragments are good fragments
   int                         eventsProcessed = 0;
   std::shared_ptr<TRlmdEvent> rlmdEvent       = std::static_pointer_cast<TRlmdEvent>(rawEvent);
   uint32_t                    size            = rlmdEvent->GetDataSize();

   try {
      eventsProcessed += EagleEventToFragment(size, reinterpret_cast<uint16_t*>(rlmdEvent->GetData()));
      for(int i = 0; i < eventsProcessed; ++i) rawEvent->IncrementGoodFrags();
   } catch(THILDataParserException& e) {
      eventsProcessed = -e.GetFailedWord();
      if(!TGRSIOptions::Get()->SuppressErrors()) {
         if(!TGRSIOptions::Get()->LogErrors()) {
            std::cout << std::endl
                      << e.what();
         }
      }
   }

   IncrementItemsPopped();
   DecrementInputSize();

   return eventsProcessed;
}

int THILDataParser::EagleEventToFragment(uint32_t size, uint16_t* data)
{
   std::shared_ptr<THILFragment> eventFrag;
   int                           nofEvents = 0;

   // all data needs to be byte swapped!
   // 0 - length of used data
   uint16_t usedData = ByteSwap(data[0]);
   if(usedData % 2 == 1) {
      throw THILDataParserException(EDataParserState::kOddUsedLength, 0);
   }
   // convert used data from bytes to 16bit words
   usedData /= 2;
   // 1 - always 0x0200 w/o byte swapping
   if(data[1] != 0x0200) {
      throw THILDataParserException(EDataParserState::kWrongBufferType, 1);
   }
   // 2+3 - buffer number
   uint32_t bufferNumber                = ByteSwap(data[2]);
   bufferNumber                         = (bufferNumber << 16) | ByteSwap(data[3]);
   uint16_t              eventLength    = 0;
   uint16_t              eventId        = 0;
   uint32_t              acquisitionClk = 0;
   uint32_t              cpuClk         = 0;
   uint32_t              eventNumber    = 0;
   uint64_t              eventTime      = 0;
   uint16_t              pattern        = 0;
   uint16_t              usTime         = 0;
   std::bitset<16>       bitPattern;
   std::vector<uint8_t>  geId;
   std::vector<uint16_t> geTime;
   std::vector<uint16_t> geEnergy;
   std::vector<uint8_t>  siId;
   std::vector<uint16_t> siEnergy;
   //std::ofstream debugFile("debug.txt", std::ios::app);
   //debugFile<<std::hex;
   for(size_t i = 4; i < usedData && i < size;) {   // incrementation is done within the loop
      //debugFile<<"Parsing buffer from 0x"<<i<<" to 0x"<<usedData<<std::endl;
      // 4+x - length should not be zero
      eventLength = ByteSwap(data[i++]);
      if(eventLength == 0) {
         throw THILDataParserException(EDataParserState::kZeroEventLength, i - 1);
      }
      if(eventLength % 2 == 1) {
         throw THILDataParserException(EDataParserState::kOddEventLength, i - 1);
      }
      // 5+x - event id
      eventId = ByteSwap(data[i++]);
      if(eventId == 0x0001) {
         acquisitionClk = ByteSwap(data[i++]);
         acquisitionClk = (acquisitionClk << 16) | ByteSwap(data[i++]);
         cpuClk         = ByteSwap(data[i++]);
         cpuClk         = (cpuClk << 16) | ByteSwap(data[i++]);
         // we advanced i 6 times so the event length should be 12 and we are done with this event
         if(eventLength != 0x000c) {
            throw THILDataParserException(EDataParserState::kWrongEventLength, i - 6);
         }
         //debugFile<<"acquisitionClk 0x"<<acquisitionClk<<", cpuClk 0x"<<cpuClk<<std::endl;
         continue;
      }
      eventLength /= 2;   // convert from bytes to 16bit words
      // for all other IDs we only care about the high nibble
      // we already read two 16bit words of the event
      switch((eventId >> 8) & 0xff) {
      case 0x11:   // sync
         //debugFile<<"found sync pattern, moving index from "<<i;
         i += eventLength - 2;
         //debugFile<<" to "<<i<<std::endl;
         break;
      case 0x12:   // scaler ??
         //debugFile<<"found scaler pattern, moving index from "<<i;
         i += eventLength - 2;
         //debugFile<<" to "<<i<<std::endl;
         break;
      case 0x70:
         //debugFile<<"found 0x70 pattern, moving index from "<<i;
         i += eventLength - 2;
         //debugFile<<" to "<<i<<std::endl;
         break;
      case 0x74:
         //debugFile<<"found 0x74 pattern, moving index from "<<i;
         i += eventLength - 2;
         //debugFile<<" to "<<i<<std::endl;
         break;
      case 0x75:   // CEFE bit pattern
         eventNumber = ByteSwap(data[i++]);
         eventNumber = (eventNumber << 16) | ByteSwap(data[i++]);
         eventTime   = ByteSwap(data[i++]);
         eventTime   = (eventTime << 16) | ByteSwap(data[i++]);
         eventTime   = (eventTime << 16) | ByteSwap(data[i++]);
         eventTime   = (eventTime << 16) | ByteSwap(data[i++]);
         // CEFE 0
         // pattern and time in microseconds
         pattern = ByteSwap(data[i++]);
         if((pattern & 0xdf00) != 0x5500) {
            throw THILDataParserException(EDataParserState::kWrongUsPattern, i - 1);
         }
         usTime = ByteSwap(data[i++]);
         // pattern and hit pattern (plus loop for energies, ids, and times)
         pattern = ByteSwap(data[i++]);
         if((pattern & 0xdf00) != 0x5500) {
            throw THILDataParserException(EDataParserState::kWrongGeHPPattern, i - 1);
         }
         //debugFile<<eventNumber<<": time 0x"<<eventTime<<" microseconds 0x"<<usTime<<" pattern 0x"<<pattern<<" at "<<i-1;
         bitPattern = ByteSwap(data[i++]);
         //debugFile<<" bit pattern 0x"<<bitPattern.to_ulong()<<" at "<<i-1<<std::endl;
         if(bitPattern.any()) {
            //debugFile<<"found Ge ";
            for(int ch = 0; ch < 16; ++ch) {
               if(bitPattern.test(ch)) {
                  //debugFile<<ch<<" ";
                  // for each bit that is set we should find one energy word and a combined ID and time word
                  // each preceded by a "pattern word"
                  pattern = ByteSwap(data[i++]);
                  if((pattern & 0xdf00) != 0x5500) {
                     throw THILDataParserException(EDataParserState::kWrongGeEnPattern, i - 1);
                  }
                  geEnergy.push_back(ByteSwap(data[i++]));
                  pattern = ByteSwap(data[i++]);
                  if((pattern & 0xdf00) != 0x5500) {
                     throw THILDataParserException(EDataParserState::kWrongGeTimePattern, i - 1);
                  }
                  geId.push_back((ByteSwap(data[i]) >> 12) & 0xf);
                  geTime.push_back(ByteSwap(data[i++]) & 0xfff);
               }
            }
            //debugFile<<std::endl;
         }
         // CEFE 1
         pattern = ByteSwap(data[i++]);
         if((pattern & 0xdf00) != 0x5500) {
            throw THILDataParserException(EDataParserState::kWrongSi1HPPattern, i - 1);
         }
         bitPattern = ByteSwap(data[i++]);
         if(bitPattern.any()) {
            //debugFile<<"found Si1 ";
            for(int ch = 0; ch < 16; ++ch) {
               if(bitPattern.test(ch)) {
                  //debugFile<<ch<<" ";
                  // for each bit that is set we should find one combined ID and energy word
                  pattern = ByteSwap(data[i++]);
                  if((pattern & 0xdf00) != 0x5500) {
                     throw THILDataParserException(EDataParserState::kWrongSi1EnPattern, i - 1);
                  }
                  siId.push_back((ByteSwap(data[i]) >> 12) & 0xf);
                  siEnergy.push_back(ByteSwap(data[i++]) & 0xfff);
               }
            }
            //debugFile<<std::endl;
         }
         // CEFE 2
         pattern = ByteSwap(data[i++]);
         if((pattern & 0xdf00) != 0x5500) {
            throw THILDataParserException(EDataParserState::kWrongSi2HPPattern, i - 1);
         }
         bitPattern = ByteSwap(data[i++]);
         if(bitPattern.any()) {
            //debugFile<<"found Si2 ";
            for(int ch = 0; ch < 16; ++ch) {
               if(bitPattern.test(ch)) {
                  //debugFile<<ch<<" ";
                  // for each bit that is set we should find one combined ID and energy word
                  pattern = ByteSwap(data[i++]);
                  if((pattern & 0xdf00) != 0x5500) {
                     throw THILDataParserException(EDataParserState::kWrongSi2EnPattern, i - 1);
                  }
                  siId.push_back(((ByteSwap(data[i]) >> 12) & 0xf) + 16);
                  siEnergy.push_back(ByteSwap(data[i++]) & 0xfff);
               }
            }
            //debugFile<<std::endl;
         }
         // CEFE 3
         pattern = ByteSwap(data[i++]);
         if((pattern & 0xdf00) != 0x5500) {
            throw THILDataParserException(EDataParserState::kWrongSi2HPPattern, i - 1);
         }
         bitPattern = ByteSwap(data[i++]);
         if(bitPattern.any()) {
            //debugFile<<"found Si3 ";
            for(int ch = 0; ch < 16; ++ch) {
               if(bitPattern.test(ch)) {
                  //debugFile<<ch<<" ";
                  // for each bit that is set we should find one combined ID and energy word
                  pattern = ByteSwap(data[i++]);
                  if((pattern & 0xdf00) != 0x5500) {
                     throw THILDataParserException(EDataParserState::kWrongSi3EnPattern, i - 1);
                  }
                  siId.push_back(((ByteSwap(data[i]) >> 12) & 0xf) + 32);
                  siEnergy.push_back(ByteSwap(data[i++]) & 0xfff);
               }
            }
            //debugFile<<std::endl;
         }
         //debugFile<<geId.size()<<" Ge and "<<siId.size()<<" Si"<<std::endl;
         //debugFile<<"#   Ge# GeEn GeTime Si# SiEn"<<std::endl;
         //for(size_t h = 0; h < geId.size(); ++h) {
         //	if(h < siId.size()) {
         //		debugFile<<std::setw(2)<<h<<"  "<<std::setw(3)<<(int)geId.at(h)<<" "<<std::setw(4)<<geEnergy.at(h)<<" "<<std::setw(6)<<geTime.at(h)<<" "<<std::setw(3)<<(int)siId.at(h)<<" "<<std::setw(4)<<siEnergy.at(h)<<std::endl;
         //	} else {
         //		debugFile<<std::setw(2)<<h<<"  "<<std::setw(3)<<(int)geId.at(h)<<" "<<std::setw(4)<<geEnergy.at(h)<<" "<<std::setw(6)<<geTime.at(h)<<std::endl;
         //	}
         //}
         //debugFile<<"============================"<<std::endl;

         eventFrag = std::make_shared<THILFragment>();
         // default address as this event contains multiple hits with different addresses
         // but this makes the unpacking loop create the right detector
         eventFrag->SetAddress(0x100);
         eventFrag->GermaniumId(geId);
         eventFrag->GermaniumEnergy(geEnergy);
         eventFrag->GermaniumTime(geTime);
         eventFrag->SiliconId(siId);
         eventFrag->SiliconEnergy(siEnergy);
         //eventFrag->AcquisitionClock(acquisitionClk);
         //eventFrag->CpuClock(cpuClk);
         eventFrag->EventNumber(eventNumber);
         eventFrag->EventTime(eventTime);
         eventFrag->MicrosecondTime(usTime);
         if(eventFrag->Good()) {
            Push(GoodOutputQueues(), eventFrag);
            ++nofEvents;
         } else {
            Push(*BadOutputQueue(), std::make_shared<TBadFragment>(*eventFrag));
         }
         if(RecordDiag()) {
            TParsingDiagnostics::Get()->GoodFragment(eventFrag->GetDetectorType());
         }

         geId.clear();
         geEnergy.clear();
         geTime.clear();
         siId.clear();
         siEnergy.clear();
         break;
      case 0xaa:   // start
         //debugFile<<"found start pattern, moving index from "<<i;
         i += eventLength - 2;
         //debugFile<<" to "<<i<<std::endl;
         break;
      default:
         throw THILDataParserException(EDataParserState::kWrongEventId, i - 1);
         break;
      };
   }
   //debugFile.close();

   return nofEvents;
}
