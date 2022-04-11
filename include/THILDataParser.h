#ifndef THILDARAPARSER_H
#define THILDARAPARSER_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class THILDataParser
///
/// The THILDataParser is the DAQ dependent part of GRSISort.
/// It takes a "DAQ-dependent"-flavoured LMD file and
/// converts it into a generic TFragment that the rest of
/// GRSISort can deal with. This is where event word masks
/// are applied, and any changes to the event format must
/// be implemented.
///
/////////////////////////////////////////////////////////////////

#include "Globals.h"
#include <ctime>
#include <sstream>
#include <vector>
#include <map>
#include <limits>

#ifndef __CINT__
#include <memory>
#endif

#include "TDataParser.h"
#include "TChannel.h"
#include "TFragment.h"
#include "TPPG.h"
#include "TScaler.h"
#include "TFragmentMap.h"
#include "ThreadsafeQueue.h"
#include "TEpicsFrag.h"
#include "TRawEvent.h"
#include "TRlmdEvent.h"

class THILDataParser : public TDataParser {
public:
   THILDataParser();
   ~THILDataParser();

   // ENUM(EBank, char, kWFDN,kGRF1,kGRF2,kGRF3,kFME0,kFME1,kFME2,kFME3);
   enum class EBank { kWFDN = 0, kGRF1 = 1, kGRF2 = 2, kGRF3 = 3, kGRF4 = 4, kFME0 = 5, kFME1 = 6, kFME2 = 7, kFME3 = 8 };

   enum class EDigitizer {
       kV1751 = 1,
       kV1724 = 2,
       kV1730_PSD = 3,
       kV1730_PHA = 4,
       kV1725_PHA = 7,
       kV1725_PSD = 8
   };

   enum class EDataParserState {
      kGood,
      kOddUsedLength,
		kWrongBufferType,
		kZeroEventLength,
		kOddEventLength,
		kWrongEventLength,
		kWrongUsPattern,
		kWrongGeHPPattern,
		kWrongGeEnPattern,
		kWrongGeTimePattern,
		kWrongSi1HPPattern,
		kWrongSi1EnPattern,
		kWrongSi2HPPattern,
		kWrongSi2EnPattern,
		kWrongSi3HPPattern,
		kWrongSi3EnPattern,
		kWrongEventId,
		kUndefined
	};

#ifndef __CINT__
	int Process(std::shared_ptr<TRawEvent>) override;
#endif

private:
	uint16_t ByteSwap(const uint16_t& val) {
		return ((val&0xff) << 8) | ((val&0xff00) >> 8);
	}

	int EagleEventToFragment(uint32_t size, uint16_t* data);

	EDataParserState fState;
};
/*! @} */
#endif
