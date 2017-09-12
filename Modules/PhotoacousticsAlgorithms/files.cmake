set(CPP_FILES
  mitkPhotoacousticImage.cpp
  
  Algorithms/mitkPhotoacousticBeamformingFilter.cpp
  
  Algorithms/OpenCLFilter/mitkPhotoacousticOCLBeamformer.cpp
  Algorithms/OpenCLFilter/mitkPhotoacousticBModeFilter.cpp
  Algorithms/OpenCLFilter/mitkPhotoacousticOCLUsedLinesCalculation.cpp
  Algorithms/OpenCLFilter/mitkPhotoacousticOCLDelayCalculation.cpp
  Algorithms/OpenCLFilter/mitkPhotoacousticOCLMemoryLocSum.cpp
)

set(RESOURCE_FILES
  DASQuadratic.cl
  DMASQuadratic.cl
  DASSpherical.cl
  DMASSpherical.cl
  BModeAbs.cl
  BModeAbsLog.cl
  UsedLinesCalculation.cl
  MemoryLocSum.cl
  DelayCalculation.cl
  DMAS.cl
)