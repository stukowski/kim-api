//
// CDDL HEADER START
//
// The contents of this file are subject to the terms of the Common Development
// and Distribution License Version 1.0 (the "License").
//
// You can obtain a copy of the license at
// http://www.opensource.org/licenses/CDDL-1.0.  See the License for the
// specific language governing permissions and limitations under the License.
//
// When distributing Covered Code, include this CDDL HEADER in each file and
// include the License file in a prominent location with the name LICENSE.CDDL.
// If applicable, add the following below this CDDL HEADER, with the fields
// enclosed by brackets "[]" replaced with your own identifying information:
//
// Portions Copyright (c) [yyyy] [name of copyright owner]. All rights reserved.
//
// CDDL HEADER END
//

//
// Copyright (c) 2016--2017, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api.git repository.
//

#include <map>

#ifndef KIM_LANGUAGE_NAME_HPP_
#include "KIM_LanguageName.hpp"
#endif

namespace KIM
{

// Order doesn't matter as long as all values are unique
namespace LANGUAGE_NAME
{
LanguageName const cpp(0);
LanguageName const c(1);
LanguageName const fortran(2);

namespace
{
typedef std::map<LanguageName const, std::string, LANGUAGE_NAME::Comparator>
StringMap;

StringMap const GetStringMap()
{
  StringMap m;
  m[cpp] = "cpp";
  m[c] = "c";
  m[fortran] = "fortran";
  return m;
}
}  // namespace
extern StringMap const languageNameToString = GetStringMap();

void GetNumberOfLanguageNames(int * const numberOfLanguageNames)
{
  *numberOfLanguageNames = languageNameToString.size();
}

int GetLanguageName(int const index, LanguageName * const languageName)
{
  int numberOfLanguageNames;
  GetNumberOfLanguageNames(&numberOfLanguageNames);
  if ((index < 0) || (index >= numberOfLanguageNames)) return true;

  StringMap::const_iterator iter = languageNameToString.begin();
  for (int i=0; i<index; ++i) ++iter;
  *languageName = iter->first;
  return true;  // no error
}
}  // namespace LANGUAGE_NAME

// implementation of LanguageName
LanguageName::LanguageName(): languageNameID(0){}
LanguageName::LanguageName(int const id): languageNameID(id){}
LanguageName::LanguageName(std::string const str)
{
  languageNameID = -1;
  for (LANGUAGE_NAME::StringMap::const_iterator iter
           = LANGUAGE_NAME::languageNameToString.begin();
       iter != LANGUAGE_NAME::languageNameToString.end();
       ++iter)
  {
    if (iter->second == str)
    {
      languageNameID = (iter->first).languageNameID;
      break;
    }
  }
}

bool LanguageName::operator==(LanguageName const & rhs) const
{return languageNameID == rhs.languageNameID;}
bool LanguageName::operator!=(LanguageName const & rhs) const
{return languageNameID != rhs.languageNameID;}

std::string LanguageName::String() const
{
  std::string result;
  LANGUAGE_NAME::StringMap::const_iterator iter
      = LANGUAGE_NAME::languageNameToString.find(*this);
  if (iter == LANGUAGE_NAME::languageNameToString.end())
    result = "unknown";
  else
    result = iter->second;

  return result;
}
}  // namespace KIM
