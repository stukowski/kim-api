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
// Copyright (c) 2013--2018, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api.git repository.
//

#include <dlfcn.h>
#include <iostream>
//#include <string>

void usage(std::string name)
{
  size_t beg = name.find_last_of("/");
  if (beg != std::string::npos) name = name.substr(beg + 1, std::string::npos);

  // Follows docopt.org format
  std::cerr << "Usage:\n"
            << "  " << name << " <shared-library-name>" << std::endl;
  // note: this interface is likely to change in future kim-api releases
}

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    usage(argv[0]);
    return 1;
  }
  else
  {
    void * sharedLibraryHandle = dlopen(argv[1], RTLD_NOW);
    if (sharedLibraryHandle == NULL)
    {
      std::cout << "Unable to open shared library.\n" << dlerror() << std::endl;
      return 2;
    }
    else
    {
      std::cout << "Successfully opened shared library." << std::endl;
      dlclose(sharedLibraryHandle);
      return 0;
    }
  }
}
