#include "TPinDiodeHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TGRSIOptions.h"
#include "TEagle.h"

TPinDiodeHit::TPinDiodeHit() : TDetectorHit()
{
   // Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TPinDiodeHit::~TPinDiodeHit() = default;

TPinDiodeHit::TPinDiodeHit(const TPinDiodeHit& rhs) : TDetectorHit()
{
   // Copy Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
   rhs.Copy(*this);
}

TPinDiodeHit::TPinDiodeHit(const uint8_t& id, const uint16_t& energy, const uint64_t& eventTime)
{
   SetAddress(0x200 + id);
   SetCharge(energy);
   SetKValue(1);
   SetTimeStamp(eventTime);
}

void TPinDiodeHit::Copy(TObject& rhs) const
{
   // Copies a TPinDiodeHit
   TDetectorHit::Copy(rhs);
}

void TPinDiodeHit::Copy(TObject& obj, bool waveform) const
{
   Copy(obj);
   if(waveform) {
      CopyWave(obj);
   }
}

TVector3 TPinDiodeHit::GetPosition(Double_t) const
{
   // Gets the position of the current TPinDiodeHit
   auto vec = TEagle::GetPinDiodePosition(GetDetector());
   return TVector3(vec.X(), vec.Y(), vec.Z());
}

TVector3 TPinDiodeHit::GetPosition() const
{
   // Gets the position of the current TPinDiodeHit
   return GetPosition(GetDefaultDistance());
}

void TPinDiodeHit::Clear(Option_t*)
{
   // Clears the PinDiodeHit
   TDetectorHit::Clear();
}

void TPinDiodeHit::Print(Option_t*) const
{
   /// Prints the EagleHit information
   Print(std::cout);
}

void TPinDiodeHit::Print(std::ostream& out) const
{
   /// Prints the PinDiodeHit information
   std::ostringstream str;
   str<<"PinDiode Address:               0x"<<std::setw(4)<<std::hex<<std::setfill('0')<<GetAddress()<<std::setfill(' ')<<std::dec<<std::endl;
   str<<"PinDiode Detector:              "<<std::setw(16)<<GetDetector()<<std::endl;
   str<<"PinDiode hit energy:            "<<std::setw(16)<<GetEnergy()<<std::endl;
   str<<"PinDiode hit time:              "<<std::setw(16)<<GetTime()<<std::endl;
   out<<str.str();
}
