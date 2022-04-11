#include <iostream>
#include "TEagle.h"
#include "TRandom.h"
#include "TMath.h"
#include "THILFragment.h"

/// \cond CLASSIMP
ClassImp(TEagle)
/// \endcond

TVector3 TEagle::gDetectorPosition[17] = {
	TVector3(0, 0, 1),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
	TVector3(-12.1784, -16.7622, 6.2123),
	TVector3(12.1784, -16.7622, 6.2123),
	TVector3(19.7051, 6.4026, -6.2123),
	TVector3(0, 20.7192, -6.2123),
	TVector3(-19.7051, 6.4026, -6.2123),
	TVector3(-12.1784, -16.7622, -6.2123),
	TVector3(12.1784, -16.7622, -6.2123),
	TVector3(8.8395, -12.1665, -22.8096)};

TVector3 TEagle::gDiodePosition[49] = {
	TVector3(0, 0, 1),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
	TVector3(-12.1784, -16.7622, 6.2123),
	TVector3(12.1784, -16.7622, 6.2123),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
	TVector3(-12.1784, -16.7622, 6.2123),
	TVector3(12.1784, -16.7622, 6.2123),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
	TVector3(-12.1784, -16.7622, 6.2123),
	TVector3(12.1784, -16.7622, 6.2123),
	TVector3(19.7051, 6.4026, -6.2123),
	TVector3(0, 20.7192, -6.2123),
	TVector3(-19.7051, 6.4026, -6.2123),
	TVector3(-12.1784, -16.7622, -6.2123),
	TVector3(12.1784, -16.7622, -6.2123),
	TVector3(14.3025, 4.6472, -22.8096),
	TVector3(0, 15.0386, -22.8096),
	TVector3(-14.3025, 4.6472, -22.8096),
	TVector3(-8.8395, -12.1665, -22.8096),
	TVector3(8.8395, -12.1665, -22.8096)};

TEagle::TEagle()
{
	// Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TEagle::TEagle(const TEagle& rhs) : TDetector()
{
	// Copy Contructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	rhs.Copy(*this);
}

TEagle::~TEagle()
{
	// Default Destructor
}

TEagle& TEagle::operator=(const TEagle& rhs)
{
	rhs.Copy(*this);
	return *this;
}

void TEagle::Copy(TObject& rhs) const
{
	/// Copy function
	TDetector::Copy(rhs);
	static_cast<TEagle&>(rhs).fPinDiodeHits.resize(fPinDiodeHits.size());
	for(size_t i = 0; i < fPinDiodeHits.size(); ++i) {
		static_cast<TEagle&>(rhs).fPinDiodeHits[i] = new TPinDiodeHit(*(fPinDiodeHits[i]));
	}
	static_cast<TEagle&>(rhs).fEventNumber = fEventNumber;
	static_cast<TEagle&>(rhs).fUsTime = fUsTime;
}

void TEagle::Clear(Option_t* opt)
{
	/// Clears the parent and all of the hits
	TDetector::Clear(opt);
	fPinDiodeHits.clear();
	fEventNumber = 0;
	fUsTime = 0;
}

void TEagle::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel*)
{
	auto hilFragment = std::static_pointer_cast<const THILFragment>(frag);
	if(!hilFragment->Good(true)) {
		std::cerr<<"Bad fragment passed along to "<<__PRETTY_FUNCTION__<<std::endl;
		return;
	}
	for(size_t h = 0; h < hilFragment->GermaniumMultiplicity(); ++h) {
		TEagleHit* hit = new TEagleHit(hilFragment->GermaniumId(h), hilFragment->GermaniumEnergy(h), hilFragment->GermaniumTime(h), hilFragment->EventTime());
		fHits.push_back(hit);
	}
	for(size_t h = 0; h < hilFragment->SiliconMultiplicity(); ++h) {
		TPinDiodeHit* hit = new TPinDiodeHit(hilFragment->SiliconId(h), hilFragment->SiliconEnergy(h), hilFragment->EventTime());
		fPinDiodeHits.push_back(hit);
	}
	fEventNumber = hilFragment->EventNumber();
	fUsTime = hilFragment->MicrosecondTime();
}

TPinDiodeHit* TEagle::GetPinDiodeHit(const int& i) const
{
	try {
		return fPinDiodeHits.at(i);//static_cast<TPinDiodeHit*>(fPinDiodeHits.at(i));
	} catch(const std::out_of_range& oor) {
		std::cerr<<ClassName()<<" is out of range: "<<oor.what()<<std::endl;
      throw grsi::exit_exception(1);
   }
}

void TEagle::Print(Option_t*) const
{
	/// Prints out TEagle information
	Print(std::cout);
}

void TEagle::Print(std::ostream& out) const
{
	/// Prints out TEagle information
	std::ostringstream str;
	str<<this<<": "<<fHits.size()<<" germanium and "<<fPinDiodeHits.size()<<" silicon hits"<<std::endl
	   <<"event #"<<fEventNumber<<", "<<fUsTime<<" microseconds"<<std::endl;
	for(auto hit : fHits) {
		hit->Print(str);
	}
	for(auto hit : fPinDiodeHits) {
		hit->Print(str);
	}
	str<<"----------------------------------------"<<std::endl;
	out<<str.str();
}

