#include "THILDataParserException.h"
#include "THILDataParser.h"

THILDataParserException::THILDataParserException(THILDataParser::EDataParserState state, int failedWord)
   : fParserState(state), fFailedWord(failedWord)
{
   /// default constructor for THILDataParserException, stores the data parser state and the word the parser failed on
   /// and creates a message based on them that can be accessed via THILDataParserException::what()
   std::ostringstream stream;
   stream<<"THILDataParser failed on "<<fFailedWord<<". word: ";
   switch(fParserState) {
   case THILDataParser::EDataParserState::kGood:
      stream<<"state is good, no idea what went wrong!"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kOddUsedLength:
      stream<<"odd used data length"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongBufferType:
      stream<<"wrong buffer type, should have been 0x0200"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kZeroEventLength:
      stream<<"event length is zero"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kOddEventLength:
      stream<<"odd event length"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongEventLength:
      stream<<"wrong event length"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongUsPattern:
      stream<<"wrong pattern for time in microseconds"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongGeHPPattern:
      stream<<"wrong pattern for hitpattern of germanium"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongGeEnPattern:
      stream<<"wrong pattern for energy of germanium"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongGeTimePattern:
      stream<<"wrong pattern for time of germanium"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi1HPPattern:
      stream<<"wrong pattern for hitpattern of Si 1"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi1EnPattern:
      stream<<"wrong pattern for energy of Si 1"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi2HPPattern:
      stream<<"wrong pattern for hitpattern of Si 2"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi2EnPattern:
      stream<<"wrong pattern for energy of Si 2"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi3HPPattern:
      stream<<"wrong pattern for hitpattern of Si 3"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongSi3EnPattern:
      stream<<"wrong pattern for energy of Si 3"<<std::endl;
      break;
   case THILDataParser::EDataParserState::kWrongEventId:
      stream<<"wrong event ID"<<std::endl;
      break;
   default: break;
   };

   fMessage = stream.str();
}

THILDataParserException::~THILDataParserException()
{
   /// default destructor
}

const char* THILDataParserException::what() const noexcept
{
   /// return message string built in default constructor
   return fMessage.c_str();
}
