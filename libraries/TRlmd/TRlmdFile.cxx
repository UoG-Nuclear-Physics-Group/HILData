#include <iostream>
#include <bitset>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cassert>
#include <cstdlib>
#include <locale>
#include <time.h>

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

#include "TString.h"

#include "TRlmdFile.h"
#include "TRlmdEvent.h"
#include "TRunInfo.h"
#include "THILDetectorInformation.h"
#include "THILMnemonic.h"
#include "HILDataVersion.h"

/// \cond CLASSIMP
ClassImp(TRlmdFile)
/// \endcond

TRlmdFile::TRlmdFile()
{
   // Default Constructor
   fBytesRead = 0;
   fFileSize  = 0;
}

TRlmdFile::TRlmdFile(const char* filename, TRawFile::EOpenType open_type) : TRlmdFile()
{
   switch(open_type) {
		case TRawFile::EOpenType::kRead: Open(filename); break;
		case TRawFile::EOpenType::kWrite: break;
   }
}

TRlmdFile::~TRlmdFile()
{
   // Default dtor. It closes the read in lmd file as well as the output lmd file.
   Close();
}

std::string TRlmdFile::Status(bool)
{
   return Form(HIDE_CURSOR " Processed event, have processed %.2fMB/%.2f MB              " SHOW_CURSOR "\r",
               (fInputStream.tellg() / 1000000.0), (fFileSize / 1000000.0));
}

/// Open a lmd .lmd file with given file name.
///
/// Remote files can be accessed using these special file names:
/// - pipein://command - read data produced by given command, see examples below
/// - ssh://username\@hostname/path/file.mid - read remote file through an ssh pipe
/// - ssh://username\@hostname/path/file.mid.gz and file.mid.bz2 - same for compressed files
/// - dccp://path/file.mid (also file.mid.gz and file.mid.bz2) - read data from dcache, requires dccp in the PATH
///
/// Examples:
/// - ./event_dump.exe /ladd/data9/t2km11/data/run02696.mid.gz - read normal compressed file
/// - ./event_dump.exe ssh://ladd09//ladd/data9/t2km11/data/run02696.mid.gz - read compressed file through ssh to ladd09
/// (note double "/")
/// - ./event_dump.exe pipein://"cat /ladd/data9/t2km11/data/run02696.mid.gz | gzip -dc" - read data piped from a
/// command or script (note quotes)
/// - ./event_dump.exe pipein://"gzip -dc /ladd/data9/t2km11/data/run02696.mid.gz" - another way to read compressed
/// files
/// - ./event_dump.exe dccp:///pnfs/triumf.ca/data/t2km11/aug2008/run02837.mid.gz - read file directly from a dcache
/// pool (note triple "/")
///
/// \param[in] filename The file to open.
/// \returns "true" for succes, "false" for error, use GetLastError() to see why
bool TRlmdFile::Open(const char* filename)
{
   fFilename = filename;

   try {
      fInputStream.open(GetFilename(), std::ifstream::in | std::ifstream::binary);
      fInputStream.seekg(0, std::ifstream::end);
      if(fInputStream.tellg() < 0) {
         std::cout<<R"(Failed to open ")"<<GetFilename()<<"/"<<fFilename<<R"("!)"<<std::endl;
         return false;
      }
      fFileSize = fInputStream.tellg();
      fInputStream.seekg(0, std::ifstream::beg);
		//std::ofstream debugFile("debug.txt", std::ios::app);
		//debugFile<<"Total file size is "<<fFileSize<<" bytes"<<std::endl;
      
      // Read Header Information
		RlmdFileHeader header;
      fInputStream.read(reinterpret_cast<char *>(&header), sizeof(RlmdFileHeader));
      
		//debugFile<<"read header of size "<<sizeof(RlmdFileHeader)<<" bytes"<<std::endl;
		fStartDate = header.date;
		fStartDate = fStartDate.substr(0, 12);
		fStartTime = header.time;
		fStartTime = fStartTime.substr(0, 8);
		fTemplate = header.templateName;
		fTemplate = fTemplate.substr(0, 1480);
   } catch(std::exception& e) {
      std::cout<<"Caught "<<e.what()<<" at "<<__FILE__<<" : "<<__LINE__<<std::endl;
   }

	// setup TChannel to use our mnemonics
	TChannel::SetMnemonicClass(THILMnemonic::Class());

   TRunInfo::SetRunInfo(GetRunNumber(), GetSubRunNumber());
   TRunInfo::SetRunLength(300); 
   TRunInfo::ClearVersion();
   TRunInfo::SetVersion(HILDATA_RELEASE);

	fBytesRead = fInputStream.tellg();
   std::cout<<"Successfully read "<<fBytesRead<<" bytes of header with start date "<<fStartDate<<" "<<fStartTime<<" and template "<<fTemplate<<"!"<<std::endl;

	THILDetectorInformation* detInfo = new THILDetectorInformation();
	TRunInfo::SetDetectorInformation(detInfo);

   return true;
}

void TRlmdFile::Close()
{
   fInputStream.close();
	fBytesRead = fFileSize; // just to make sure we don't try to read more data after the file was closed
}

/// \param [in] Event Pointer to an empty TRlmdEvent
/// \returns "true" for success, "false" for failure, see GetLastError() to see why
///
///  EDITED FROM THE ORIGINAL TO RETURN TOTAL SUCESSFULLY BYTES READ INSTEAD OF TRUE/FALSE,  PCB
///
int TRlmdFile::Read(std::shared_ptr<TRawEvent> Event)
{
   if(Event == nullptr) return -1;

   size_t LastReadSize = 0;
   std::shared_ptr<TRlmdEvent> RlmdEvent = std::static_pointer_cast<TRlmdEvent>(Event);
   RlmdEvent->Clear();

   if(fBytesRead < fFileSize) {
		//std::ofstream debugFile("debug.txt", std::ios::app);
		//debugFile<<std::hex;
		//debugFile<<"buffer 0x"<<fBuffersRead<<": starting to read at 0x"<<fInputStream.tellg()<<std::endl;
		// Read the buffer header
		fInputStream.read(reinterpret_cast<char*>(&fBufferHeader), sizeof(RlmdBufferHeader));
		LastReadSize += static_cast<size_t>(fInputStream.gcount());

		switch(fBufferHeader.type) {
			case 2: // normal event so we read the buffer data into the event and send it on
				//debugFile<<"read buffer header of size 0x"<<sizeof(RlmdBufferHeader)<<", reading buffer of size 0x"<<2*fBufferHeader.dataLength<<" starting at 0x"<<fInputStream.tellg()<<std::endl;
				try {
					// the data length never changes, so this should only resize the vector once
					fReadBuffer.resize(2*fBufferHeader.dataLength); // 2* accounts for the size of uint16_t
					fInputStream.read(fReadBuffer.data(), 2*fBufferHeader.dataLength);
					LastReadSize += static_cast<size_t>(fInputStream.gcount());
					fBytesRead += LastReadSize;
					//std::cout<<"Read normal buffer with "<<2*fBufferHeader.dataLength<<"/"<<LastReadSize<<" bytes, now at byte "<<fBytesRead<<std::endl;
					RlmdEvent->SetData(fReadBuffer);
					++fBuffersRead;
				} catch(std::exception& e) {
					std::cout<<"Caught "<<e.what()<<" at "<<__FILE__<<" : "<<__LINE__<<std::endl;
					return -1;
				}
				break;
			case 3: // file footer so we "unread" the buffer header, read the footer and then close the file
				fInputStream.seekg(-sizeof(RlmdBufferHeader), std::ifstream::cur);
				RlmdFileFooter footer;
				//debugFile<<"tail buffer 0x"<<fBuffersRead<<": starting to read at 0x"<<fInputStream.tellg()<<std::endl;
				fInputStream.read(reinterpret_cast<char *>(&footer), sizeof(RlmdFileFooter));
				fStopDate = footer.date;
				fStopDate = fStopDate.substr(0,12);
				fStopTime = footer.time;
				fStopTime = fStopTime.substr(0,8);
				TRunInfo::SetRunStart(ConvertToEpoch(fStartDate, fStartTime));
				TRunInfo::SetRunStop(ConvertToEpoch(fStopDate, fStopTime));
				TRunInfo::SetRunLength();
				Close();
				LastReadSize = 0; // setting this to zero as it doesn't matter in this case?
				break;
			default:
				std::cerr<<"Unknown buffer type "<<fBufferHeader.type<<std::endl;
				break;
		}
   }
	return LastReadSize;
}

void TRlmdFile::Skip(size_t)
{
	// this might be wrong, in the case of the rlmd file we could check the data length from the buffer header, skip that far ahead
	// and read the next buffer header
	std::cerr<<"Sorry, but we can't skip events in an RLMD file, the whole file is treated as a single event!"<<std::endl;
	return;
}

int TRlmdFile::GetRunNumber()
{
   // Parse the run number from the current TRlmdFile. This assumes a format of
   // <name>###.rlmd.
   if(fFilename.length() == 0) {
      return 0;
   }
   std::size_t found      = fFilename.rfind(".rlmd");
   if(found == std::string::npos) {
      return 0;
   }
   std::string temp;
	temp = fFilename.substr(found - 3, 3);
   return atoi(temp.c_str());
}

int TRlmdFile::GetSubRunNumber()
{
   // There are no subruns in .lmd files
   return -1;
}

time_t TRlmdFile::ConvertToEpoch(const std::string& date, const std::string& time)
{
	std::stringstream str;
	str<<date<<time;
	std::tm t = {};
	str>>std::get_time(&t, "%d-%b-%Y %H:%M:S");
	return std::mktime(&t);
}

// end
