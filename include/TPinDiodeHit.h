#ifndef PINDIODEHIT_H
#define PINDIODEHIT_H

/** \addtogroup Detectors
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TPinDiodeHit
///
/// This is class that contains the information about a sceptar
/// hit. This class is used to find energy, time, etc.
/// The address is set to 0x200 plus the id of the diode.
///
/////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cmath>
#include <vector>

#include "TFragment.h"
#include "TChannel.h"

#include "TVector3.h"

#include "TDetectorHit.h"

class TPinDiodeHit : public TDetectorHit {
public:
   TPinDiodeHit();
   ~TPinDiodeHit() override;
   TPinDiodeHit(const TPinDiodeHit&);
   TPinDiodeHit(const uint8_t& id, const uint16_t& energy, const uint64_t& eventTime);

   TVector3 GetPosition(Double_t dist) const override;   //!<!
   TVector3 GetPosition() const override;                //!<!

   void Clear(Option_t* opt = "") override;         //!<!
   void Print(Option_t* opt = "") const override;   //!<!
   void Print(std::ostream& out) const override;    //!<!
   void Copy(TObject&) const override;              //!<!
   void Copy(TObject&, bool) const override;        //!<!

private:
   Double_t GetDefaultDistance() const { return 0.0; }

   /// \cond CLASSIMP
   ClassDefOverride(TPinDiodeHit, 1)   // Stores the information for a PinDiodeHit
   /// \endcond
};
/*! @} */
#endif
