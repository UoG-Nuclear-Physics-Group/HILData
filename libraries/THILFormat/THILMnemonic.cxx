#include "THILMnemonic.h"

#include <algorithm>

// Detector dependent includes
#include "TEagle.h"
#include "TEaglePulser.h"

ClassImp(THILMnemonic)

void THILMnemonic::Clear(Option_t*)
{
	TMnemonic::Clear();
   fSystem = ESystem::kClear;
}

void THILMnemonic::EnumerateSystem()
{
   // Enumerating the fSystemString must come after the total mnemonic has been parsed as the details of other parts of
   // the mnemonic must be known
   if(fSystemString.compare("EA") == 0) {
		fSystem = ESystem::kEagle;
   } else if(fSystemString.compare("PD") == 0) {
       fSystem = ESystem::kPinDiode;
   } else if(fSystemString.compare("PU") == 0) {
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
	int tmpUnit = 10;
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
	std::cout<<"====== HILMNEMONIC ============"<<std::endl;
	std::cout<<"fArrayPosition           = "<<fArrayPosition<<std::endl;
	std::cout<<"fSegment                 = "<<fSegment<<std::endl;
	std::cout<<"fSystemString            = "<<fSystemString<<std::endl;
	std::cout<<"fSubSystemString         = "<<fSubSystemString<<std::endl;
	std::cout<<"fArraySubPositionString  = "<<fArraySubPositionString<<std::endl;
	std::cout<<"fCollectedChargeString   = "<<fCollectedChargeString<<std::endl;
	std::cout<<"fOutputSensorString      = "<<fOutputSensorString<<std::endl;
	std::cout<<"==============================="<<std::endl;
}

TClass* THILMnemonic::GetClassType() const
{
	if(fClassType != nullptr) {
		return fClassType;
	}

	switch(System()) {
		case ESystem::kEagle:        fClassType = TEagle::Class(); break;
		case ESystem::kPinDiode:     fClassType = TEagle::Class(); break;
		case ESystem::kEaglePulser:  fClassType = TEaglePulser::Class(); break;
		default:              fClassType = nullptr;
	};
	return fClassType;
}

double THILMnemonic::GetTime(Long64_t timestamp, Float_t, double energy, const TChannel* channel) const
{
	if(channel == nullptr) {
      Error("GetTime", "No TChannel provided");
      return static_cast<Double_t>((timestamp) + gRandom->Uniform());
   }
	Double_t dTime = static_cast<Double_t>((timestamp) + gRandom->Uniform()) * channel->GetTimeStampUnit();
	return dTime - channel->GetTZero(energy);
}
