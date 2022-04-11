#ifndef THILDETECTORINFORMATION_H
#define THILDETECTORINFORMATION_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class THILDetectorInformation
///
/// This Class is designed to store setup dependent
/// information. It is used to store existence of
/// detector systems, etc.
///
/////////////////////////////////////////////////////////////////

#include "Globals.h"

#include "TDetectorInformation.h"
#include "TChannel.h"

class THILDetectorInformation : public TDetectorInformation {
public:
   THILDetectorInformation();
   ~THILDetectorInformation() override;
	
	TEventBuildingLoop::EBuildMode BuildMode() const override;

	void Print(Option_t* opt = "") const override;
	void Clear(Option_t* = "") override;
	void Set() override;

   inline void SetEagle(bool flag = true)       { fEagle       = flag; }
   inline void SetPinDiode(bool flag = true)    { fPinDiode       = flag; }
   inline void SetEaglePulser(bool flag = true) { fEaglePulser = flag; }

   inline bool Eagle()       const { return fEagle; }
   inline bool PinDiode()    const { return fPinDiode; }
   inline bool EaglePulser() const { return fEaglePulser; }

private:
   //  detector types to switch over in Set()

   bool fEagle{false}; // flag for Eagle on/off
   bool fPinDiode{false}; // flag for pin diode on/off
   bool fEaglePulser{false};

   /// \cond CLASSIMP
   ClassDefOverride(THILDetectorInformation, 1); // Contains the run-dependent information.
   /// \endcond
};
/*! @} */
#endif
