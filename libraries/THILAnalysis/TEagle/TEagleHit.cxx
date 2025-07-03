#include "TEagleHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TEagle.h"
#include "TGRSIOptions.h"

TEagleHit::TEagleHit() : TDetectorHit()
{
   // Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TEagleHit::~TEagleHit() = default;

TEagleHit::TEagleHit(const TEagleHit& rhs) : TDetectorHit()
{
   // Copy Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
   rhs.Copy(*this);
}

TEagleHit::TEagleHit(const uint8_t& id, const uint16_t& energy, const uint16_t& time, const uint64_t& eventTime)
{
   SetAddress(0x100 + id);
   SetCharge(energy);
   SetKValue(1);
   SetTimeStamp(eventTime);
   SetCfd(time);
}

void TEagleHit::Copy(TObject& rhs) const
{
   // Copies a TEagleHit
   TDetectorHit::Copy(rhs);
}

void TEagleHit::Copy(TObject& obj, bool waveform) const
{
   Copy(obj);
   if(waveform) {
      CopyWave(obj);
   }
}

TVector3 TEagleHit::GetPosition(Double_t) const
{
   // Gets the position of the current TEagleHit
   auto vec = TEagle::GetPosition(GetDetector());
   return TVector3(vec.X(), vec.Y(), vec.Z());
}

TVector3 TEagleHit::GetPosition() const
{
   // Gets the position of the current TEagleHit
   return GetPosition(GetDefaultDistance());
}

void TEagleHit::Clear(Option_t*)
{
   // Clears the EagleHit
   TDetectorHit::Clear();
}

void TEagleHit::Print(Option_t*) const
{
   /// Prints the EagleHit information
   Print(std::cout);
}

void TEagleHit::Print(std::ostream& out) const
{
   /// Prints the EagleHit information in a thread-safe way
   std::ostringstream str;
   str << "Eagle Address:                  0x" << std::setw(4) << std::hex << std::setfill('0') << GetAddress() << std::setfill(' ') << std::dec << std::endl;
   str << "Eagle Detector:       " << std::setw(16) << GetDetector() << std::endl;
   str << "Eagle hit energy:     " << std::setw(16) << GetEnergy() << std::endl;
   str << "Eagle hit time stamp: " << std::setw(16) << GetTimeStamp() << std::endl;
   str << "Eagle CFD:            " << std::setw(16) << GetCfd() << std::endl;
   out << str.str();
}
