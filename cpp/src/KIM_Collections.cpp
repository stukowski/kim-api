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
// Copyright (c) 2016--2020, Regents of the University of Minnesota.
// All rights reserved.
//
// Contributors:
//    Ryan S. Elliott
//

//
// Release: This file is part of the kim-api.git repository.
//


#include <cstddef>

#ifndef KIM_LOG_VERBOSITY_HPP_
#include "KIM_LogVerbosity.hpp"
#endif

#ifndef KIM_COLLECTIONS_HPP_
#include "KIM_Collections.hpp"
#endif

#ifndef KIM_COLLECTION_HPP_
#include "KIM_Collection.hpp"
#endif

#ifndef KIM_COLLECTION_ITEM_TYPE_HPP_
#include "KIM_CollectionItemType.hpp"
#endif

#ifndef KIM_COLLECTIONS_IMPLEMENTATION_HPP_
#include "KIM_CollectionsImplementation.hpp"
#endif

namespace KIM
{
int Collections::Create(Collections ** const collections)
{
  *collections = new Collections();

  int error = CollectionsImplementation::Create(&((*collections)->pimpl));
  if (error)
  {
    delete *collections;
    *collections = NULL;
    return true;
  }
  else
  {
    return false;
  }
}

void Collections::Destroy(Collections ** const collections)
{
  CollectionsImplementation::Destroy(&((*collections)->pimpl));
  delete *collections;
  *collections = NULL;
}

int Collections::GetItemType(std::string const & itemName,
                             CollectionItemType * const itemType) const
{
  return pimpl->GetItemType(itemName, itemType);
}

int Collections::GetItemLibraryFileNameAndCollection(
    CollectionItemType const itemType,
    std::string const & itemName,
    std::string const ** const fileName,
    Collection * const collection) const
{
  return pimpl->GetItemLibraryFileNameAndCollection(
      itemType, itemName, fileName, collection);
}

int Collections::CacheListOfItemMetadataFiles(CollectionItemType const itemType,
                                              std::string const & itemName,
                                              int * const extent)
{
  return pimpl->CacheListOfItemMetadataFiles(itemType, itemName, extent);
}

int Collections::GetItemMetadataFile(
    int const index,
    std::string const ** const fileName,
    unsigned int * const fileLength,
    unsigned char const ** const fileRawData,
    int * const availableAsString,
    std::string const ** const fileString) const
{
  return pimpl->GetItemMetadataFile(
      index, fileName, fileLength, fileRawData, availableAsString, fileString);
}

int Collections::CacheListOfItemNamesByType(CollectionItemType const itemType,
                                            int * const extent)
{
  return pimpl->CacheListOfItemNamesByType(itemType, extent);
}

int Collections::GetItemNameByType(int const index,
                                   std::string const ** const itemName) const
{
  return pimpl->GetItemNameByType(index, itemName);
}

int Collections::CacheListOfItemNamesByCollectionAndType(
    Collection const collection,
    CollectionItemType const itemType,
    int * const extent)
{
  return pimpl->CacheListOfItemNamesByCollectionAndType(
      collection, itemType, extent);
}

int Collections::GetItemNameByCollectionAndType(
    int const index, std::string const ** const itemNames) const
{
  return pimpl->GetItemNameByCollectionAndType(index, itemNames);
}

int Collections::GetItemLibraryFileNameByCollectionAndType(
    Collection const collection,
    CollectionItemType const itemType,
    std::string const & itemName,
    std::string const ** const fileName) const
{
  return pimpl->GetItemLibraryFileNameByCollectionAndType(
      collection, itemType, itemName, fileName);
}

int Collections::CacheListOfItemMetadataFilesByCollectionAndType(
    Collection const collection,
    CollectionItemType const itemType,
    std::string const & itemName,
    int * const extent)
{
  return pimpl->CacheListOfItemMetadataFilesByCollectionAndType(
      collection, itemType, itemName, extent);
}

int Collections::GetItemMetadataFileByCollectionAndType(
    int const index,
    std::string const ** const fileName,
    unsigned int * const fileLength,
    unsigned char const ** const fileRawData,
    int * const availableAsString,
    std::string const ** const fileString) const
{
  return pimpl->GetItemMetadataFileByCollectionAndType(
      index, fileName, fileLength, fileRawData, availableAsString, fileString);
}

void Collections::GetProjectNameAndSemVer(
    std::string const ** const projectName,
    std::string const ** const semVer) const
{
  pimpl->GetProjectNameAndSemVer(projectName, semVer);
}

int Collections::GetEnvironmentVariableName(
    CollectionItemType const itemType, std::string const ** const name) const
{
  return pimpl->GetEnvironmentVariableName(itemType, name);
}

void Collections::GetConfigurationFileEnvironmentVariable(
    std::string const ** const name, std::string const ** const value) const
{
  pimpl->GetConfigurationFileEnvironmentVariable(name, value);
}

void Collections::GetConfigurationFileName(
    std::string const ** const fileName) const
{
  pimpl->GetConfigurationFileName(fileName);
}

int Collections::CacheListOfDirectoryNames(Collection const collection,
                                           CollectionItemType const itemType,
                                           int * const extent)
{
  return pimpl->CacheListOfDirectoryNames(collection, itemType, extent);
}

int Collections::GetDirectoryName(
    int const index, std::string const ** const directoryName) const
{
  return pimpl->GetDirectoryName(index, directoryName);
}

void Collections::SetLogID(std::string const & logID)
{
  pimpl->SetLogID(logID);
}

void Collections::PushLogVerbosity(LogVerbosity const logVerbosity)
{
  pimpl->PushLogVerbosity(logVerbosity);
}

void Collections::PopLogVerbosity() { pimpl->PopLogVerbosity(); }

Collections::Collections() : pimpl(NULL) {}

Collections::~Collections() {}

}  // namespace KIM
