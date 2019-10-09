/*########################################################################

  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.

  The Initial Developer of the Original Code is Shivang Patel.

  Copyright(C) 2002. All Rights Reserved.

  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
########################################################################*/
#ifndef __MATCHING_SHIFTING_HEADER__
#define __MATCHING_SHIFTING_HEADER__
#include "fvsimage.h"
#include "fvstypes.h"


template <class itemType>
float_t Matching_Shifting(FvsImage<itemType> & TemplateImg, FvsImage<itemType> & VerifyingImg);


// include implementation
#include "matching_shifting.cpp"

#endif // __MATCHING_SHIFTING_HEADER__