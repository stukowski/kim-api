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
// Copyright (c) 2016--2018, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api.git repository.
//


#ifndef KIM_NUMBERING_HPP_
#define KIM_NUMBERING_HPP_

#include <string>

namespace KIM
{

class Numbering
{
 public:
  int numberingID;

  Numbering();
  Numbering(int const id);
  Numbering(std::string const & str);
  bool operator==(Numbering const & rhs) const;
  bool operator!=(Numbering const & rhs) const;
  std::string String() const;
};

namespace NUMBERING
{
extern Numbering const zeroBased;
extern Numbering const oneBased;

void GetNumberOfNumberings(int * const numberOfNumberings);
int GetNumbering(int const index, Numbering * const numbering);

struct Comparator
{
  bool operator()(Numbering const & a, Numbering const & b) const
  {
    return a.numberingID < b.numberingID;
  }
};
}  // namespace NUMBERING
}  // namespace KIM
#endif  // KIM_NUMBERING_HPP_
