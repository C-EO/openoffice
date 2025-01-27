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



#ifndef CONNECTIVITY_SHAREDRESOURCES_HXX
#define CONNECTIVITY_SHAREDRESOURCES_HXX

/** === begin UNO includes === **/
/** === end UNO includes === **/

#include <rtl/ustring.hxx>
#include <list>
#include "connectivity/dbtoolsdllapi.hxx"

//........................................................................
namespace connectivity
{
//........................................................................

    typedef sal_uInt16  ResourceId;
	//====================================================================
	//= SharedResources
	//====================================================================
    /** helper class for accessing resources shared by different libraries
        in the connectivity module
    */
	class OOO_DLLPUBLIC_DBTOOLS SharedResources
	{
    public:
        SharedResources();
        ~SharedResources();

        /** loads a string from the shared resource file
            @param  _nResId
                the resource ID of the string
            @return
                the string from the resource file
        */
        ::rtl::OUString
            getResourceString(
                ResourceId _nResId
            ) const;

        /** loads a string from the shared resource file, and replaces
            a given ASCII pattern with a given string

            @param  _nResId
                the resource ID of the string to load
            @param  _pAsciiPatternToReplace
                the ASCII string which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute
                the String which should substitute the ASCII pattern.
            
            @return
                the string from the resource file, with applied string substitution
        */
        ::rtl::OUString
            getResourceStringWithSubstitution(
                ResourceId _nResId,
                const sal_Char* _pAsciiPatternToReplace,
                const ::rtl::OUString& _rStringToSubstitute
            ) const;

        /** loads a string from the shared resource file, and replaces
            a given ASCII pattern with a given string

            @param  _nResId
                the resource ID of the string to load
            @param  _pAsciiPatternToReplace1
                the ASCII string (1) which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute1
                the String which should substitute the ASCII pattern (1)
            @param  _pAsciiPatternToReplace2
                the ASCII string (2) which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute2
                the String which should substitute the ASCII pattern (2)
            
            @return
                the string from the resource file, with applied string substitution
        */
        ::rtl::OUString
            getResourceStringWithSubstitution(
                ResourceId _nResId,
                const sal_Char* _pAsciiPatternToReplace1,
                const ::rtl::OUString& _rStringToSubstitute1,
                const sal_Char* _pAsciiPatternToReplace2,
                const ::rtl::OUString& _rStringToSubstitute2
            ) const;

        /** loads a string from the shared resource file, and replaces
            a given ASCII pattern with a given string

            @param  _nResId
                the resource ID of the string to load
            @param  _pAsciiPatternToReplace1
                the ASCII string (1) which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute1
                the String which should substitute the ASCII pattern (1)
            @param  _pAsciiPatternToReplace2
                the ASCII string (2) which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute2
                the String which should substitute the ASCII pattern (2)
            @param  _pAsciiPatternToReplace3
                the ASCII string (3) which is to search in the string. Must not be <NULL/>.
            @param  _rStringToSubstitute3
                the String which should substitute the ASCII pattern (3)
            
            @return
                the string from the resource file, with applied string substitution
        */
        ::rtl::OUString
            getResourceStringWithSubstitution(
                ResourceId _nResId,
                const sal_Char* _pAsciiPatternToReplace1,
                const ::rtl::OUString& _rStringToSubstitute1,
                const sal_Char* _pAsciiPatternToReplace2,
                const ::rtl::OUString& _rStringToSubstitute2,
                const sal_Char* _pAsciiPatternToReplace3,
                const ::rtl::OUString& _rStringToSubstitute3
            ) const;

        /** loads a string from the shared resource file, and replaces a given ASCII pattern with a given string

            @param  _nResId
                the resource ID of the string to load
            @param  _aStringToSubstitutes
                A list of substitutions.

            @return
                the string from the resource file, with applied string substitution
        */
        ::rtl::OUString getResourceStringWithSubstitution( ResourceId _nResId,
                    const ::std::list< ::std::pair<const sal_Char* , ::rtl::OUString > > _aStringToSubstitutes) const;
	};

//........................................................................
} // namespace connectivity
//........................................................................

#endif // CONNECTIVITY_SHAREDRESOURCES_HXX
