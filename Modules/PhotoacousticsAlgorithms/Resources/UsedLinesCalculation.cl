/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

__kernel void ckUsedLines(
  __global unsigned short* dDest, // output buffer
  float partMult,
  unsigned int inputL,
  unsigned int inputS
)
{
  // get thread identifier
  unsigned int globalPosX = get_global_id(0);
  unsigned int globalPosY = get_global_id(1);
  
  unsigned short outputS = get_global_size(1);
  unsigned short outputL = get_global_size(0);
  
  // terminate non-valid threads
  if ( globalPosX < outputL && globalPosY < outputS)
  {
    float l_i = (float)globalPosX / outputL * inputL;
    float s_i = (float)globalPosY / outputS * inputS / 2;

    float part = partMult * s_i;
    if (part < 1)
      part = 1;
    
    unsigned short maxLine = min((l_i + part) + 1, (float)inputL);
    unsigned short minLine = max((l_i - part), 0.0f);
    
    dDest[globalPosY * outputL + globalPosX] = (maxLine - minLine); //usedLines
    dDest[globalPosY * outputL + globalPosX + 1 * outputL * outputS] = minLine; //minLine
    dDest[globalPosY * outputL + globalPosX + 2 * outputL * outputS] = maxLine; //maxLine
  }
}