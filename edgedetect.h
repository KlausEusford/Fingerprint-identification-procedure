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
#ifndef __EDGEDETECT_HEADER__
#define __EDGEDETECT_HEADER__

// This value may be changed if needed, play around with it if you like
#define EDGETHRESHOLD 100

// Logarithmic form of edge dectection
template <class itemType>
FvsError_t EdgeDetect(FvsImage<itemType> & img);
// include implementation
#include "edgedetect.cpp"
#endif // __EDGEDETECT_HEADER__