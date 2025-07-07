#include <string>

#include "TClassRef.h"

#include "TRlmdFile.h"
#include "THILDataParser.h"
#include "HILDataVersion.h"
#include "TChannel.h"
#include "THILMnemonic.h"

extern "C" TRlmdFile* CreateFile(std::string& fileName) { return new TRlmdFile(fileName.c_str()); }
extern "C" void       DestroyFile(TRlmdFile* obj) { delete obj; }

extern "C" THILDataParser* CreateParser() { return new THILDataParser; }
extern "C" void            DestroyParser(THILDataParser* obj) { delete obj; }

extern "C" std::string LibraryVersion() { return std::string(HILDATA_RELEASE); }

extern "C" void InitLibrary() { TChannel::SetMnemonicClass(TClassRef("THILMnemonic")); }
