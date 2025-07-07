#ifndef TRLMDEVENT_H
#define TRLMDEVENT_H

/** \addtogroup Sorting
 *  @{
 */

#include "Globals.h"

#include "TRawEvent.h"

/////////////////////////////////////////////////////////////////
///
/// \class TRlmdEvent
///
/// C++ class representing one RLMD file.
///
/////////////////////////////////////////////////////////////////

/// RLMD event

class TRlmdEvent : public TRawEvent {
public:
   // houskeeping functions
   TRlmdEvent();                                                ///< default constructor
   TRlmdEvent(const TRlmdEvent&);                               ///< copy constructor
   ~TRlmdEvent() override;                                      ///< destructor
   TRlmdEvent& operator=(const TRlmdEvent&);                    ///< assignement operator
   void        Clear(Option_t* opt = "") override;              ///< clear event for reuse
   void        Copy(TObject&) const override;                   ///< copy helper
   void        Print(const char* option = "") const override;   ///< show all event information

   // get event information

   uint32_t GetDataSize() const override;   ///< return the event size

   // helpers for event creation

   char* GetData() override;                  ///< return pointer to the data buffer
   void  SetData(std::vector<char> buffer);   ///< set an externally allocated data buffer

   int SwapBytes(bool) override;   ///< convert event data between little-endian (Linux-x86) and big endian (MacOS-PPC)

protected:
   std::vector<char> fData;   ///< event data buffer

   /// \cond CLASSIMP
   ClassDefOverride(TRlmdEvent, 0)   // All of the data contained in a RLMD Event
   /// \endcond
};
/*! @} */
#endif   // TRlmdEvent.h
