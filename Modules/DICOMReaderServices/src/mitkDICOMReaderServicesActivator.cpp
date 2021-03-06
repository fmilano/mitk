/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkDICOMReaderServicesActivator.h"

#include "mitkAutoSelectingDICOMReaderService.h"
#include "mitkClassicDICOMSeriesReaderService.h"
#include "mitkDICOMTagsOfInterestService.h"
#include "mitkSimpleVolumeDICOMSeriesReaderService.h"

#include <usModuleContext.h>

namespace mitk {

  void DICOMReaderServicesActivator::Load(us::ModuleContext* context)
  {
    m_AutoSelectingDICOMReader.reset(new AutoSelectingDICOMReaderService());
    m_SimpleVolumeDICOMSeriesReader.reset(new SimpleVolumeDICOMSeriesReaderService());

    m_DICOMTagsOfInterestService.reset(new DICOMTagsOfInterestService());
    context->RegisterService<mitk::IDICOMTagsOfInterest>(m_DICOMTagsOfInterestService.get());

    DICOMTagPathMapType tagmap = GetDefaultDICOMTagsOfInterest();
    for (auto tag : tagmap)
    {
      m_DICOMTagsOfInterestService->AddTagOfInterest(tag.first);
    }
  }

  void DICOMReaderServicesActivator::Unload(us::ModuleContext*)
  {
  }

}

US_EXPORT_MODULE_ACTIVATOR(mitk::DICOMReaderServicesActivator)
