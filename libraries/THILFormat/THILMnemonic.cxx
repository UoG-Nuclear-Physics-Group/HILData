#include "THILMnemonic.h"

#include <algorithm>

// Detector dependent includes
#include "TEagle.h"
#include "TEaglePulser.h"

void THILMnemonic::Clear(Option_t*)
{
   TMnemonic::Clear();
   fSystem = ESystem::kClear;
}

void THILMnemonic::EnumerateSystem()
{
   // Enumerating the fSystemString must come after the total mnemonic has been parsed as the details of other parts of
   // the mnemonic must be known
   if(SystemString() == "EA") {
      fSystem = ESystem::kEagle;
   } else if(SystemString() == "PD") {
      fSystem = ESystem::kPinDiode;
   } else if(SystemString() == "PU") {
      fSystem = ESystem::kEaglePulser;
   } else {
      fSystem = ESystem::kClear;
   }
}

void THILMnemonic::EnumerateDigitizer(TPriorityValue<std::string>& digitizerName, TPriorityValue<EDigitizer>& digitizerType, TPriorityValue<int>& timeStampUnit)
{
   std::string name = digitizerName.Value();
   std::transform(name.begin(), name.end(), name.begin(), ::toupper);
   EDigitizer tmpType = EDigitizer::kDefault;
   int        tmpUnit = 10;
   digitizerType.Set(tmpType, digitizerName.Priority());
   timeStampUnit.Set(tmpUnit, digitizerName.Priority());
}

void THILMnemonic::Parse(std::string* name)
{
   if((name == nullptr) || name->length() < 9) {
      return;
   }
   TMnemonic::Parse(name);
   // Enumerating the fSystemString must come last as the details of other parts of
   // the mnemonic must be known
   EnumerateSystem();
}

void THILMnemonic::Print(Option_t*) const
{
   std::ostringstream str;
   std::cout << "====== HILMNEMONIC ============" << std::endl;
   TMnemonic::Print(str);
   std::cout << "===============================" << std::endl;
   std::cout << str.str();
}

TClass* THILMnemonic::GetClassType() const
{
   if(TMnemonic::GetClassType() != nullptr) {
      return TMnemonic::GetClassType();
   }

   switch(System()) {
   case ESystem::kEagle: SetClassType(TEagle::Class()); break;
   case ESystem::kPinDiode: SetClassType(TEagle::Class()); break;
   case ESystem::kEaglePulser: SetClassType(TEaglePulser::Class()); break;
   default: SetClassType(nullptr);
   };
   return TMnemonic::GetClassType();
}

double THILMnemonic::GetTime(Long64_t timestamp, Float_t, double energy, const TChannel* channel) const
{
   if(channel == nullptr) {
      Error("GetTime", "No TChannel provided");
      return static_cast<Double_t>((timestamp) + gRandom->Uniform());
   }
   Double_t dTime = static_cast<Double_t>((timestamp) + gRandom->Uniform()) * channel->GetTimeStampUnit() + channel->CalibrateCFD(0.);
   return dTime - channel->GetTZero(energy) - channel->GetTimeOffset();
}
