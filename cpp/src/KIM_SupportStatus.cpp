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

#ifndef KIM_SUPPORT_STATUS_HPP_
#include "KIM_SupportStatus.hpp"
#endif

namespace KIM
{

// Order doesn't matter as long as all values are unique
namespace SUPPORT_STATUS
{
SupportStatus const requiredByAPI(0);
SupportStatus const notSupported(1);
SupportStatus const required(2);
SupportStatus const optional(3);

namespace
{
typedef std::map<SupportStatus const, std::string, SUPPORT_STATUS::Comparator>
StringMap;

StringMap const GetStringMap()
{
  StringMap m;
  m[requiredByAPI] = "requiredByAPI";
  m[notSupported] = "notSupported";
  m[required] = "required";
  m[optional] = "optional";
  return m;
}
}  // namespace
extern StringMap const supportStatusToString = GetStringMap();

void GetNumberOfSupportStatuses(int * const numberOfSupportStatuses)
{
  *numberOfSupportStatuses = supportStatusToString.size();
}

int GetSupportStatus(int const index, SupportStatus * const supportStatus)
{
  int numberOfSupportStatuses;
  GetNumberOfSupportStatuses(&numberOfSupportStatuses);
  if ((index < 0) || (index >= numberOfSupportStatuses)) return true;

  StringMap::const_iterator iter = supportStatusToString.begin();
  for (int i=0; i<index; ++i) ++iter;
  *supportStatus = iter->first;
  return false;  // no error
}
}  // namespace SUPPORT_STATUS

// implementation of SupportStatus
SupportStatus::SupportStatus() : supportStatusID(0){}
SupportStatus::SupportStatus(int const id) : supportStatusID(id){}
SupportStatus::SupportStatus(std::string const & str)
{
  supportStatusID = -1;
  for (SUPPORT_STATUS::StringMap::const_iterator iter
           = SUPPORT_STATUS::supportStatusToString.begin();
       iter != SUPPORT_STATUS::supportStatusToString.end();
       ++iter)
  {
    if (iter->second == str)
    {
      supportStatusID = (iter->first).supportStatusID;
      break;
    }
  }
}

bool SupportStatus::operator==(SupportStatus const & rhs) const
{return supportStatusID == rhs.supportStatusID;}
bool SupportStatus::operator!=(SupportStatus const & rhs) const
{return supportStatusID != rhs.supportStatusID;}

std::string SupportStatus::String() const
{
  std::string result;
  SUPPORT_STATUS::StringMap::const_iterator iter
      = SUPPORT_STATUS::supportStatusToString.find(*this);
  if (iter == SUPPORT_STATUS::supportStatusToString.end())
    result = "unknown";
  else
    result = iter->second;

  return result;
}
}  // namespace KIM
