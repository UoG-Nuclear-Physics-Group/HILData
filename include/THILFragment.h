/** \addtogroup Sorting
 *  @{
 */

#ifndef THILFRAGMENT_H
#define THILFRAGMENT_H

#include <vector>

#include "TFragment.h"

/////////////////////////////////////////////////////////////////
///
/// \class THILFragment
///
/// This Class contains all of the information in an event
/// fragment for HIL data. This is used to pass an event straight
/// from the raw-file to the event queue (the event building loop
/// does nothing apart from passing this straight on).
///
/////////////////////////////////////////////////////////////////

class THILFragment : public TFragment {
public:
   THILFragment();
   THILFragment(const THILFragment&);
   ~THILFragment();

   THILFragment& operator=(const THILFragment&) = default;   // use default assignment operator (to shut up gcc 9.1)

   bool Good(bool verbose = false) const;

   size_t GermaniumMultiplicity() const { return fGermaniumId.size(); }
   size_t SiliconMultiplicity() const { return fSiliconId.size(); }

   std::vector<uint8_t>  GermaniumId() const { return fGermaniumId; }
   std::vector<uint16_t> GermaniumEnergy() const { return fGermaniumEnergy; }
   std::vector<uint16_t> GermaniumTime() const { return fGermaniumTime; }
   std::vector<uint8_t>  SiliconId() const { return fSiliconId; }
   std::vector<uint16_t> SiliconEnergy() const { return fSiliconEnergy; }
   uint8_t               GermaniumId(size_t index) const { return fGermaniumId.at(index); }
   uint16_t              GermaniumEnergy(size_t index) const { return fGermaniumEnergy.at(index); }
   uint16_t              GermaniumTime(size_t index) const { return fGermaniumTime.at(index); }
   uint8_t               SiliconId(size_t index) const { return fSiliconId.at(index); }
   uint16_t              SiliconEnergy(size_t index) const { return fSiliconEnergy.at(index); }
   uint32_t              EventNumber() const { return fEventNumber; }
   uint64_t              EventTime() const { return fEventTime; }
   uint16_t              MicrosecondTime() const { return fUsTime; }

   void GermaniumId(const std::vector<uint8_t>& geId) { fGermaniumId = geId; }
   void GermaniumEnergy(const std::vector<uint16_t>& geEnergy) { fGermaniumEnergy = geEnergy; }
   void GermaniumTime(const std::vector<uint16_t>& geTime) { fGermaniumTime = geTime; }
   void SiliconId(const std::vector<uint8_t>& siId) { fSiliconId = siId; }
   void SiliconEnergy(const std::vector<uint16_t>& siEnergy) { fSiliconEnergy = siEnergy; }
   void EventNumber(const uint32_t& eventNumber) { fEventNumber = eventNumber; }
   void EventTime(const uint64_t& eventTime) { fEventTime = eventTime; }
   void MicrosecondTime(const uint16_t& usTime) { fUsTime = usTime; }

   void Clear(Option_t* opt = "") override;
   void Print(std::ostream& out) const override;

   TObject* Clone(const char* name = "") const override;

private:
   std::vector<uint8_t>  fGermaniumId;
   std::vector<uint16_t> fGermaniumEnergy;
   std::vector<uint16_t> fGermaniumTime;
   std::vector<uint8_t>  fSiliconId;
   std::vector<uint16_t> fSiliconEnergy;
   uint32_t              fEventNumber{0};
   uint64_t              fEventTime{0};
   uint16_t              fUsTime{0};

   /// \cond CLASSIMP
   ClassDefOverride(THILFragment, 1);   // HIL Fragments
                                        /// \endcond
};
/*! @} */
#endif   // THILFRAGMENT_H
