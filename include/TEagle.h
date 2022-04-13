#ifndef TEAGLE_H
#define TEAGLE_H

/** \addtogroup Detectors
 *  @{
 */

/////////////////////////////////////////////////////////////
///
/// \class TEagle
///
/// The TEagle class defines the observables and algorithms used
/// when analyzing EAGLE data. It includes detector positions,
/// etc.
///
/////////////////////////////////////////////////////////////

#include <vector>
#include <cstdio>

#include "Math/Vector3D.h"

#include "Globals.h"
#include "TDetector.h"
#include "TEagleHit.h"
#include "TPinDiodeHit.h"

class TEagle : public TDetector {
public:
   TEagle();
   ~TEagle() override;
   TEagle(const TEagle& rhs);

   TEagleHit* GetEagleHit(const int& i) const { return static_cast<TEagleHit*>(GetHit(i)); }
   TPinDiodeHit* GetPinDiodeHit(const int& i) const;
#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
	void BuildHits() override {} // no need to build any hits, everything already done in AddFragment

   static ROOT::Math::Polar3DVector GetPosition(int DetNbr) { return gDetectorPosition[DetNbr]; } //!<!
   static ROOT::Math::Polar3DVector GetPinDiodePosition(int DetNbr) { return gDiodePosition[DetNbr]; } //!<!

   TEagle& operator=(const TEagle&); //!<!

private:
   static ROOT::Math::Polar3DVector gDetectorPosition[17]; //!<!  Position of each detector (plus one default position)
   static ROOT::Math::Polar3DVector gDiodePosition[49]; //!<!  Position of each diode

	std::vector<TPinDiodeHit*> fPinDiodeHits; ///< vector of pin diode hits
	uint32_t fEventNumber; ///< event number
	uint16_t fUsTime;      ///< time in microseconds

public:
	void Copy(TObject&) const override;            //!<!
	void Clear(Option_t* opt = "all") override;    //!<!
   void Print(Option_t* opt = "") const override; //!<!
   void Print(std::ostream& out) const override;  //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TEagle, 1) // Eagle Physics structure
	/// \endcond
};
/*! @} */
#endif
