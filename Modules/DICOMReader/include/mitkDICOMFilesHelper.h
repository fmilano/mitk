/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITKDICOMFILESHELPER_H
#define MITKDICOMFILESHELPER_H

#include <string>
#include <vector>

#include <MitkDICOMReaderExports.h>

namespace mitk {

typedef std::vector<std::string> DICOMFilePathList;

/** Helper functions. Searches for all files in the directory of the passed file path.
  All files will be checked if they are DICOM files. All DICOM files will be added to the result and
  returned.
  @remark The helper does no sorting of any kind.*/
DICOMFilePathList GetDICOMFilesInSameDirectory(const std::string& filePath);

/** All passed files will be checked if they are DICOM files.
All DICOM files will be added to the result and returned.
@remark The helper does no sorting of any kind.*/
DICOMFilePathList FilterForDICOMFiles(const DICOMFilePathList& fileList);
}

#endif // MITKDICOMFILESHELPER_H
