/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _CHECKIT_HXX
#define _CHECKIT_HXX

#include <com/sun/star/uno/Reference.h>

#ifndef _COM_SUN_STAR_I18N_XEXTENDEDINPUTSEQUENCECHECKER_HDL_
#include <com/sun/star/i18n/XExtendedInputSequenceChecker.hpp>
#endif

/*************************************************************************
 *                      class SwCheckIt
 *
 * Wrapper for the XInputSequenceChecker
 *************************************************************************/

class SwCheckIt
{
public:
    com::sun::star::uno::Reference < com::sun::star::i18n::XExtendedInputSequenceChecker > xCheck;

    SwCheckIt();
};

extern SwCheckIt* pCheckIt;

#endif
