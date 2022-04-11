#ifndef TRLMDFILE_H
#define TRLMDFILE_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TRlmdFile
///
/// This Class is used to read and write RLMD files in the
/// root framework.
///
/////////////////////////////////////////////////////////////////

#include <string>

#ifdef __APPLE__
#include <_types/_uint32_t.h>
#else
#include <cstdint>
#endif

#include "TRawFile.h"

#include "TRlmdEvent.h"

/// Reader for HIL .rrlmd files

#define COMMENT_LEN 362-12
#define FNAME_LEN 12
#define TEMPLEN 1480

typedef struct
{
	// apart from date, time, and templateName nothing seems to change from the default?
	uint16_t dataLength; // 1000 16bit-words of data in this header
	uint16_t totalLength; // 1024 16bit-words total in this header
	uint16_t type; // 1
	uint16_t subType; // 1
	uint16_t usedLength; // 1000 16bit-words of data used
	uint8_t fragment; // 0
	uint8_t lastFragment; // 0
	uint32_t bufferNumTS; // 0
	uint32_t fragments; // 0
	uint32_t free[7];
	uint32_t file_number; // 1
	char label[8]; // F0000XX0 (?)
	char date[12]; // "dd-mmm-yyyy"
	char time[8]; // "hh:mm:ss"
	uint16_t runIdLength; // 0
	char runId[66];
	uint16_t expNameLength; // 0
	char expName[34];
	uint16_t commentLength; // 0
	char comment[COMMENT_LEN];
	char fileName[FNAME_LEN]; // empty
	uint16_t eventFileFormat; // always 0 ?
	uint16_t bhFlag; // 0
	uint32_t numberOfShortParameters; // 0
	uint32_t numberOfLongParameters; // 0
	uint16_t templateNameLength; // 0x5c6 = 1478 (compared to TEMPLEN 1480)
	uint16_t numberOfEvents; // not used ?
	uint16_t eAbLength; // no idea what eAb stands for, but this seems to always be 0x414 = 1044
	uint16_t eAbNf; // 0
	char templateName[TEMPLEN];
} RlmdFileHeader;

typedef struct {
	uint16_t dataLength; // always 0x3e8?
	uint16_t totalLength; // always 0x400?
	uint16_t type; // always 3
	uint16_t subType; // always 0?
	uint16_t usedLength; // always 0?
	char date[12]; // "dd-mmm-yyyy"
	char time[8]; // "hh:mm:ss"
	uint32_t expTime; // different values, e.g. 0, 2, or 3
} RlmdFileFooter;

typedef struct {
	uint16_t dataLength;
	uint16_t totalLength;
	uint16_t type;
	uint16_t subType;
	uint16_t usedLength;
	uint8_t  fragment;
	uint8_t  lastFragment;
	uint32_t bufferNumTS;
	uint32_t numberOfFragments;
	uint32_t free[7];
} RlmdBufferHeader;

class TRlmdFile : public TRawFile {
public:
   TRlmdFile(); ///< default constructor
   TRlmdFile(const char* filename, TRawFile::EOpenType open_type = TRawFile::EOpenType::kRead);
   ~TRlmdFile() override; ///< destructor

   bool Open(const char* filename) override; ///< Open input file

   void Close() override; ///< Close input file

   using TObject::Read;
   using TObject::Write;
#ifndef __CINT__
   int Read(std::shared_ptr<TRawEvent> rlmdEvent) override; ///< Read one event from the file
#endif
	void Skip(size_t nofEvents) override; ///< Skip nofEvents from the file
   std::string Status(bool long_file_description = true) override;

   int GetRunNumber() override;
   int GetSubRunNumber() override;

#ifndef __CINT__
   std::shared_ptr<TRawEvent> NewEvent() override { return std::make_shared<TRlmdEvent>(); }
#endif
private:
	time_t ConvertToEpoch(const std::string& date, const std::string& time);
	std::string fStartDate;
	std::string fStartTime;
	std::string fStopDate;
	std::string fStopTime;
	std::string fTemplate;
   std::ifstream fInputStream;

	RlmdBufferHeader fBufferHeader;
	size_t fBuffersRead{0};

protected:
   /// \cond CLASSIMP
   ClassDefOverride(TRlmdFile, 0) // Used to open and write RLMD Files
   /// \endcond
};
/*! @} */
#endif // TRlmdFile.h
