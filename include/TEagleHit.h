#ifndef EAGLEHIT_H
#define EAGLEHIT_H

/** \addtogroup Detectors
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TEagleHit
///
/// This is class that contains the information about an eagle
/// hit. This class is used to find energy, time, etc.
/// The address is set to 0x100 plus the id of the detector.
///
/////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cmath>
#include <vector>

#include "TFragment.h"
#include "TChannel.h"

#include "TVector3.h"

#include "TDetectorHit.h"

class TEagleHit : public TDetectorHit {
public:
   TEagleHit();
   ~TEagleHit() override;
   TEagleHit(const TEagleHit&);
	TEagleHit(const uint8_t& id, const uint16_t& energy, const uint16_t& time, const uint64_t& eventTime);

	TVector3 GetPosition(Double_t dist) const override; //!<!
   TVector3 GetPosition() const override;              //!<!

   void Clear(Option_t* opt = "") override;       //!<!
   void Print(Option_t* opt = "") const override; //!<!
   void Print(std::ostream& out) const override; //!<!
   void Copy(TObject&) const override;            //!<!
   void Copy(TObject&, bool) const override;      //!<!

private:
   Double_t GetDefaultDistance() const { return 0.0; }

   /// \cond CLASSIMP
   ClassDefOverride(TEagleHit, 1) // Stores the information for a EagleHit
   /// \endcond
};
/*! @} */
#endif
