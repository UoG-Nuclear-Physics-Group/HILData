#include "THILDetectorInformation.h"

#include <iostream>
#include <unordered_map>

#include "TROOT.h"

#include "THILMnemonic.h"

/// \cond CLASSIMP
ClassImp(THILDetectorInformation)
/// \endcond

THILDetectorInformation::THILDetectorInformation() : TDetectorInformation()
{
   /// Default ctor for THILDetectorInformation.
   Clear();
	Set();
}

THILDetectorInformation::~THILDetectorInformation() = default;

TEventBuildingLoop::EBuildMode THILDetectorInformation::BuildMode() const
{
	return TEventBuildingLoop::EBuildMode::kSkip;
}

void THILDetectorInformation::Print(Option_t* opt) const
{
   // Prints the THILDetectorInformation. Options:
   // a: Print out more details.
   if(strchr(opt, 'a') != nullptr) {
      printf("\t\tEAGLE:            %s\n", Eagle() ? "true" : "false");
      printf("\t\tEAGLEPULSER:      %s\n", EaglePulser() ? "true" : "false");
      printf("\t\tPINDIODE:         %s\n", PinDiode() ? "true" : "false");
      printf("\n");
   }
}

void THILDetectorInformation::Clear(Option_t*)
{
   // Clears the THILDetectorInformation. Currently, there are no available
   // options.

	TDetectorInformation::Clear();

   fEagle = false;
}

void THILDetectorInformation::Set()
{
   /// Sets the run info. This figures out what systems are available.
   std::unordered_map<unsigned int, TChannel*>::iterator iter;
   for(iter = TChannel::GetChannelMap()->begin(); iter != TChannel::GetChannelMap()->end(); iter++) {
      std::string channelname = iter->second->GetName();

      //  detector system type.
      //  for more info, see: https://www.triumf.info/wiki/tigwiki/index.php/Detector_Nomenclature
      switch(static_cast<const THILMnemonic*>(iter->second->GetMnemonic())->System()) {
			case THILMnemonic::ESystem::kEagle:
				SetEagle();
				break;
			case THILMnemonic::ESystem::kEaglePulser:
				SetEaglePulser();
				break;
			case THILMnemonic::ESystem::kPinDiode:
				SetPinDiode();
				break;
			default:
				break;
		};
	}
}

