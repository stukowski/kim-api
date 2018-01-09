//
// CDDL HEADER START
//
// The contents of this file are subject to the terms of the Common
// Development and Distribution License Version 1.0 (the "License").
//
// You can obtain a copy of the license at
// http://www.opensource.org/licenses/CDDL-1.0.  See the License for the
// specific language governing permissions and limitations under the License.
//
// When distributing Covered Code, include this CDDL HEADER in each file and
// include the License file in a prominent location with the name
// LICENSE.CDDL.
// If applicable, add the following below this CDDL HEADER, with the fields
// enclosed by brackets "[]" replaced with your own identifying information:
//
// Portions Copyright (c) [yyyy] [name of copyright owner].
// All rights reserved.
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

#ifndef KIM_CALLBACK_NAME_HPP_
#include "KIM_CallbackName.hpp"
#endif
extern "C"
{
#ifndef KIM_CALLBACK_NAME_H_
#include "KIM_CallbackName.h"
#endif
}  // extern "C"


namespace
{
KIM::CallbackName makeCallbackNameCpp(KIM_CallbackName const callbackName)
{
  KIM::CallbackName const * const callbackNameCpp
      = reinterpret_cast<KIM::CallbackName const * const>(&callbackName);
  return *callbackNameCpp;
}

KIM_CallbackName makeCallbackNameC(KIM::CallbackName const callbackName)
{
  KIM_CallbackName const * const callbackNameC
      = reinterpret_cast<KIM_CallbackName const * const>(&callbackName);
  return *callbackNameC;
}
}  // namespace

extern "C"
{
KIM_CallbackName KIM_CallbackNameFromString(char const * const str)
{
  return makeCallbackNameC(KIM::CallbackName(std::string(str)));
}

int KIM_CallbackNameEqual(KIM_CallbackName const left,
                          KIM_CallbackName const right)
{
  return (left.callbackNameID == right.callbackNameID);
}

int KIM_CallbackNameNotEqual(KIM_CallbackName const left,
                             KIM_CallbackName const right)
{
  return (!KIM_CallbackNameEqual(left, right));
}

char const * const KIM_CallbackNameString(KIM_CallbackName callbackName)
{
  static std::string result;
  result = makeCallbackNameCpp(callbackName).String();
  return result.c_str();
}

// Order doesn't matter as long as all values are unique
KIM_CallbackName const KIM_CALLBACK_NAME_GetNeighborList
= {KIM::CALLBACK_NAME::GetNeighborList.callbackNameID};
KIM_CallbackName const KIM_CALLBACK_NAME_ProcessDEDrTerm
= {KIM::CALLBACK_NAME::ProcessDEDrTerm.callbackNameID};
KIM_CallbackName const KIM_CALLBACK_NAME_ProcessD2EDr2Term
= {KIM::CALLBACK_NAME::ProcessD2EDr2Term.callbackNameID};

void KIM_CALLBACK_NAME_GetNumberOfCallbacks(int * const numberOfCallbacks)
{
  KIM::CALLBACK_NAME::GetNumberOfCallbacks(numberOfCallbacks);
}

int KIM_CALLBACK_NAME_GetCallbackName(int const index,
                                      KIM_CallbackName * const callbackName)
{
  KIM::CallbackName callbackNameCpp;
  int err = KIM::CALLBACK_NAME::GetCallbackName(index, &callbackNameCpp);
  if (err) return err;
  KIM_CallbackName * callbackNameC
      = reinterpret_cast<KIM_CallbackName *>(&callbackNameCpp);
  *callbackName = *callbackNameC;
  return false;
}
}  // extern "C"
