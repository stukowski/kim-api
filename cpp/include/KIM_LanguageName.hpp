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


#ifndef KIM_LANGUAGE_NAME_HPP_
#define KIM_LANGUAGE_NAME_HPP_

#include <string>

namespace KIM
{
class LanguageName
{
 public:
  int languageNameID;

  LanguageName();
  LanguageName(int const id);
  LanguageName(std::string const str);
  bool operator==(LanguageName const & rhs) const;
  bool operator!=(LanguageName const & rhs) const;
  std::string String() const;
};

namespace LANGUAGE_NAME
{
extern LanguageName const cpp;
extern LanguageName const c;
extern LanguageName const fortran;

void GetNumberOfLanguageNames(int * const numberOfLanguageNames);
int GetLanguageName(int const index, LanguageName * const languageName);

struct Comparator
{
  bool operator()(LanguageName const & a, LanguageName const & b) const
  {
    return a.languageNameID < b.languageNameID;
  }
};
}  // namespace LANGUAGE_NAME
}  // namespace KIM
#endif  // KIM_LANGUAGE_NAME_HPP_
