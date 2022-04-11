#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cassert>

#include "TRlmdEvent.h"

/// \cond CLASSIMP
ClassImp(TRlmdEvent)
/// \endcond

TRlmdEvent::TRlmdEvent()
{
   // Default constructor
   fData.resize(0);
}

void TRlmdEvent::Copy(TObject& rhs) const
{
   // Copies the entire TRlmdEvent.
   static_cast<TRlmdEvent&>(rhs).fData = fData;
}

TRlmdEvent::TRlmdEvent(const TRlmdEvent& rhs) : TRawEvent()
{
   // Copy ctor.
   rhs.Copy(*this);
}

TRlmdEvent::~TRlmdEvent()
{
   Clear();
}

TRlmdEvent& TRlmdEvent::operator=(const TRlmdEvent& rhs)
{
   if(&rhs != this) {
      Clear();
   }

   rhs.Copy(*this);
   return *this;
}

void TRlmdEvent::Clear(Option_t*)
{
   // Clears the TRlmdEvent.
   fData.clear();
}

void TRlmdEvent::SetData(std::vector<char>& buffer)
{
   // Sets the data in the TRlmdEvent as the data argument passed into
   // this function.
   fData = buffer;
   SwapBytes(false);
}

uint32_t TRlmdEvent::GetDataSize() const
{
   return fData.size();
}

char* TRlmdEvent::GetData()
{
   // returns the allocated data.
   return fData.data();
}

void TRlmdEvent::Print(const char* option) const
{
   /// Print data held in this class.
   /// \param [in] option If 'a' (for "all") then the raw data will be
   /// printed out too.
   ///

   printf("Event start:\n");
   if(option[0] == 'a') {
      for(size_t i = 0; i < fData.size() / 4; ++i) {
         printf("0x%08x", ((uint32_t*)fData.data())[i]);
         if(i % 10 == 9) {
            printf("\n");
         } else {
            printf(" ");
         }
      }
   }
}

int TRlmdEvent::SwapBytes(bool)
{
   return 1;
}

// end
