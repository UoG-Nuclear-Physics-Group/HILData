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
	printf("====== HILMNEMONIC ======\n");
	printf("fArrayPosition           = %i\n", fArrayPosition);
	printf("fSegment                 = %i\n", fSegment);
	printf("fSystemString            = %s\n", fSystemString.c_str());
	printf("fSubSystemString         = %s\n", fSubSystemString.c_str());
	printf("fArraySubPositionString  = %s\n", fArraySubPositionString.c_str());
	printf("fCollectedChargeString   = %s\n", fCollectedChargeString.c_str());
	printf("fOutputSensorString      = %s\n", fOutputSensorString.c_str());
	printf("===============================\n");
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
