#include "THILFragment.h"
#include "Globals.h"

THILFragment::THILFragment() : TFragment()
{
   /// Default constructor
   Clear();
}

THILFragment::THILFragment(const THILFragment& rhs) : TFragment(rhs)
{
   /// Copy constructor
   fGermaniumId     = rhs.fGermaniumId;
   fGermaniumEnergy = rhs.fGermaniumEnergy;
   fGermaniumTime   = rhs.fGermaniumTime;
   fSiliconId       = rhs.fSiliconId;
   fSiliconEnergy   = rhs.fSiliconEnergy;
   fEventNumber     = rhs.fEventNumber;
   fEventTime       = rhs.fEventTime;
   fUsTime          = rhs.fUsTime;
}

THILFragment::~THILFragment()
{
   /// Default destructor
}

void THILFragment::Clear(Option_t* opt)
{
   /// Clears all fields of the THILFragment and TFragment
   TFragment::Clear(opt);
   fGermaniumId.clear();
   fGermaniumEnergy.clear();
   fGermaniumTime.clear();
   fSiliconId.clear();
   fSiliconEnergy.clear();
   fEventNumber = 0;
   fEventTime   = 0;
   fUsTime      = 0;
}

TObject* THILFragment::Clone(const char*) const
{
   auto result = new THILFragment;
   *result     = *this;
   result->ClearTransients();
   return result;
}

bool THILFragment::Good(bool verbose) const
{
   /// This function checks whether this event is good, i.e. the length of all germanium
   /// and all silicon vectors matches and there is at least one silicon and one germanium hit.

   if(fGermaniumId.empty() || fSiliconId.empty()) {
      if(verbose) std::cout<<this<<": Either no germanium IDs ("<<fGermaniumId.size()<<") or no silicon IDs ("<<fSiliconId.size()<<")"<<std::endl;
      return false;
   }
   if(fGermaniumId.size() != fGermaniumEnergy.size() || fGermaniumId.size() != fGermaniumTime.size()) {
      if(verbose) std::cout<<this<<": Size of germanium IDs ("<<fGermaniumId.size()<<") doesn't match size of germanium energies ("<<fGermaniumEnergy.size()<<") or size of germanium times ("<<fGermaniumTime.size()<<")"<<std::endl;
      return false;
   }
   if(fSiliconId.size() != fSiliconEnergy.size()) {
      if(verbose) std::cout<<this<<": Size of silicon IDs ("<<fSiliconId.size()<<") doesn't match size of silicon energies ("<<fSiliconEnergy.size()<<")"<<std::endl;
      return false;
   }

   return true;
}

void THILFragment::Print(std::ostream& out) const
{
   std::stringstream str;
   str<<"THILFragment "<<this<<", #"<<fEventNumber<<": "<<fGermaniumId.size()<<" germaniums and "<<fSiliconId.size()<<" silicons at event time "<<fEventTime<<", and microsecond time "<<fUsTime<<std::endl;
   str<<"#   Ge# GeEn GeTime Si# SiEn"<<std::endl;
   for(size_t h = 0; h < fGermaniumId.size(); ++h) {
      if(h < fSiliconId.size()) {
         str<<std::setw(2)<<h<<"  "<<std::setw(3)<<(int)fGermaniumId.at(h)<<" "<<std::setw(4)<<fGermaniumEnergy.at(h)<<" "<<std::setw(6)<<fGermaniumTime.at(h)<<" "<<std::setw(3)<<(int)fSiliconId.at(h)<<" "<<std::setw(4)<<fSiliconEnergy.at(h)<<std::endl;
      } else {
         str<<std::setw(2)<<h<<"  "<<std::setw(3)<<(int)fGermaniumId.at(h)<<" "<<std::setw(4)<<fGermaniumEnergy.at(h)<<" "<<std::setw(6)<<fGermaniumTime.at(h)<<std::endl;
      }
   }
   str<<"============================"<<std::endl;
   out<<str.str();
}
