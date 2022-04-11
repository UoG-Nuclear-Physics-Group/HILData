#ifndef THILDATAPARSEREXCEPTION_H
#define THILDATAPARSEREXCEPTION_H
/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

#include "THILDataParser.h"

class THILDataParserException : public std::exception {
public:
   THILDataParserException(THILDataParser::EDataParserState state, int failedWord);
   ~THILDataParserException() override;

   const char* what() const noexcept override;

   int                              GetFailedWord() { return fFailedWord; }
   THILDataParser::EDataParserState GetParserState() { return fParserState; }

private:
   THILDataParser::EDataParserState fParserState;
   int                              fFailedWord;
   std::string                      fMessage;
};
/*! @} */
#endif
