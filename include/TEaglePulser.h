#ifndef TEAGLEPULSER_H
#define TEAGLEPULSER_H

/** \addtogroup Detectors
 *  @{
 */

#include <iostream>

#include "TDetector.h"
#include "TChannel.h"
#include "TDetectorHit.h"

class TEaglePulser : public TDetector {
public:

   TEaglePulser();
   TEaglePulser(const TEaglePulser&);
   ~TEaglePulser() override;

#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
	void BuildHits() override {} // no need to build any hits, everything already done in AddFragment

   TEaglePulser& operator=(const TEaglePulser&);                    //
   void Print(Option_t* opt = "") const override; //!<!
	void Print(std::ostream& out) const override;

private:
   void  ClearStatus() {  }

   /// \cond CLASSIMP
   ClassDefOverride(TEaglePulser, 1)
   /// \endcond
};
/*! @} */
#endif
