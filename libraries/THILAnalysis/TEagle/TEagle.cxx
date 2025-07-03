#include <iostream>
#include "TEagle.h"
#include "TRandom.h"
#include "TMath.h"
#include "THILFragment.h"

// positions from http://www.old.slcj.uw.edu.pl:8090/HIL093_S/4
ROOT::Math::Polar3DVector TEagle::gDetectorPosition[17] = {
   ROOT::Math::Polar3DVector(1., 101. / 180. * TMath::Pi(), 0. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 37. / 180. * TMath::Pi(), 324. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 79. / 180. * TMath::Pi(), 36. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 101. / 180. * TMath::Pi(), 144. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 143. / 180. * TMath::Pi(), 288. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 37. / 180. * TMath::Pi(), 180. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 101. / 180. * TMath::Pi(), 72. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 143. / 180. * TMath::Pi(), 0. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 143. / 180. * TMath::Pi(), 72. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 37. / 180. * TMath::Pi(), 108. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 79. / 180. * TMath::Pi(), 324. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 143. / 180. * TMath::Pi(), 144. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 101. / 180. * TMath::Pi(), 216. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 79. / 180. * TMath::Pi(), 180. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 37. / 180. * TMath::Pi(), 36. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 37. / 180. * TMath::Pi(), 252. / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 143. / 180. * TMath::Pi(), 216. / 180. * TMath::Pi())};

// positions taken from komora.xlsx and reshuffled according to http://www.old.slcj.uw.edu.pl:8090/HIL093_S/3
ROOT::Math::Polar3DVector TEagle::gDiodePosition[49] = {
   ROOT::Math::Polar3DVector(1., 130.9188 / 180. * TMath::Pi(), -114.1559 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 112.2018 / 180. * TMath::Pi(), -133.4987 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), -130.8261 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 167.5319 / 180. * TMath::Pi(), -144.0201 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), -157.1848 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 142.6894 / 180. * TMath::Pi(), -115.697 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 142.7579 / 180. * TMath::Pi(), -171.1808 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 160.0227 / 180. * TMath::Pi(), -108.0073 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 135.0056 / 180. * TMath::Pi(), -142.6367 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 126.3285 / 180. * TMath::Pi(), -162.3313 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), 157.1848 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 135.0056 / 180. * TMath::Pi(), 145.3544 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 138.9843 / 180. * TMath::Pi(), 131.4479 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 139.0054 / 180. * TMath::Pi(), 158.3867 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 167.5319 / 180. * TMath::Pi(), 144.0201 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 126.8803 / 180. * TMath::Pi(), 162.8018 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 160.0227 / 180. * TMath::Pi(), -180.0000 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 0.0000 / 180. * TMath::Pi(), 0.0000 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 130.6994 / 180. * TMath::Pi(), 114.8989 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 110.0793 / 180. * TMath::Pi(), 134.1141 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 167.5319 / 180. * TMath::Pi(), 71.97365 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 123.8915 / 180. * TMath::Pi(), 80.31562 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 160.0227 / 180. * TMath::Pi(), 108.0073 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 142.7579 / 180. * TMath::Pi(), 44.80589 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 138.9843 / 180. * TMath::Pi(), 59.44141 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 126.3285 / 180. * TMath::Pi(), 53.67318 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 142.6894 / 180. * TMath::Pi(), 100.2885 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), 85.17818 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), 58.82094 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 139.0054 / 180. * TMath::Pi(), 86.38944 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), 130.8261 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 130.9188 / 180. * TMath::Pi(), 29.85166 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 139.0054 / 180. * TMath::Pi(), 14.37406 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 126.3285 / 180. * TMath::Pi(), -18.33714 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 160.0227 / 180. * TMath::Pi(), 36.01454 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 167.5319 / 180. * TMath::Pi(), 0.00000 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 112.2018 / 180. * TMath::Pi(), 10.50944 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 135.0056 / 180. * TMath::Pi(), 1.370451 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), -13.16529 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), 13.16529 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 142.7579 / 180. * TMath::Pi(), -27.17891 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), -58.82094 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 135.0056 / 180. * TMath::Pi(), -70.6425 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 123.8915 / 180. * TMath::Pi(), -63.6885 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 160.0227 / 180. * TMath::Pi(), -36.01454 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 138.9843 / 180. * TMath::Pi(), -84.54967 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 167.5319 / 180. * TMath::Pi(), -71.97365 / 180. * TMath::Pi()),
   ROOT::Math::Polar3DVector(1., 151.6914 / 180. * TMath::Pi(), -85.17818 / 180. * TMath::Pi())};

// unused positions:
//22	ROOT::Math::Polar3DVector(1., 142.6894/180.*TMath::Pi(), -43.70832/180.*TMath::Pi()),
//54	ROOT::Math::Polar3DVector(1., 126.3285/180.*TMath::Pi(), -90.32652/180.*TMath::Pi()),
//79	ROOT::Math::Polar3DVector(1., 119.0059/180.*TMath::Pi(), -41.34825/180.*TMath::Pi()),
//80	ROOT::Math::Polar3DVector(1., 110.0793/180.*TMath::Pi(), -81.8824/180.*TMath::Pi()),
//100	ROOT::Math::Polar3DVector(1., 112.2616/180.*TMath::Pi(), 72.59155/180.*TMath::Pi()),

TEagle::TEagle()
{
   // Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TEagle::TEagle(const TEagle& rhs) : TDetector()
{
   // Copy Contructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
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
   static_cast<TEagle&>(rhs).fUsTime      = fUsTime;
}

void TEagle::Clear(Option_t* opt)
{
   /// Clears the parent and all of the hits
   TDetector::Clear(opt);
   fPinDiodeHits.clear();
   fEventNumber = 0;
   fUsTime      = 0;
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
      Hits().push_back(hit);
   }
   for(size_t h = 0; h < hilFragment->SiliconMultiplicity(); ++h) {
      TPinDiodeHit* hit = new TPinDiodeHit(hilFragment->SiliconId(h), hilFragment->SiliconEnergy(h), hilFragment->EventTime());
      fPinDiodeHits.push_back(hit);
   }
   fEventNumber = hilFragment->EventNumber();
   fUsTime      = hilFragment->MicrosecondTime();
}

TPinDiodeHit* TEagle::GetPinDiodeHit(const int& i) const
{
   try {
      return fPinDiodeHits.at(i);   //static_cast<TPinDiodeHit*>(fPinDiodeHits.at(i));
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
   str<<this<<": "<<Hits().size()<<" germanium and "<<fPinDiodeHits.size()<<" silicon hits"<<std::endl
      <<"event #"<<fEventNumber<<", "<<fUsTime<<" microseconds"<<std::endl;
   for(const auto& hit : Hits()) {
      hit->Print(str);
   }
   for(auto hit : fPinDiodeHits) {
      hit->Print(str);
   }
   str<<"----------------------------------------"<<std::endl;
   out<<str.str();
}
