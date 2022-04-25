#include "TEaglePulser.h"

#include "TClass.h"
#include <cmath>
#include "TMath.h"

#include "TGRSIOptions.h"

/// \cond CLASSIMP
ClassImp(TEaglePulser)
/// \endcond


TEaglePulser::TEaglePulser()
{
   Clear();
}

TEaglePulser::~TEaglePulser() = default;

TEaglePulser& TEaglePulser::operator=(const TEaglePulser& rhs)
{
   rhs.Copy(*this);
   return *this;
}

TEaglePulser::TEaglePulser(const TEaglePulser& rhs) : TDetector()
{
   rhs.Copy(*this);
}

void TEaglePulser::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel* chan)
{
	
   /// This function creates TEaglePulserHits for each fragment and stores them in separate front and back vectors
   if(frag == nullptr || chan == nullptr) {
      return;
   }

   auto hit = new TDetectorHit(*frag);
   fHits.push_back(hit);
}

void TEaglePulser::Print(Option_t*) const
{
	std::cout<<__PRETTY_FUNCTION__<<"\tnot yet written."<<std::endl;
}
