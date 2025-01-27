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


#ifndef _SVT_FONTSUBSTCONFIG_HXX
#define _SVT_FONTSUBSTCONFIG_HXX

#include "svtools/svtdllapi.h"
#include <unotools/configitem.hxx>

struct SvtFontSubstConfig_Impl;

//-----------------------------------------------------------------------------
struct SubstitutionStruct
{
	rtl::OUString 	sFont;
	rtl::OUString 	sReplaceBy;
	sal_Bool		bReplaceAlways;
	sal_Bool		bReplaceOnScreenOnly;
};
//-----------------------------------------------------------------------------
class SVT_DLLPUBLIC SvtFontSubstConfig : public utl::ConfigItem
{
	sal_Bool					bIsEnabled;
	SvtFontSubstConfig_Impl*	pImpl;
public:
	SvtFontSubstConfig();
	virtual ~SvtFontSubstConfig();

	virtual void			Commit();
	virtual void Notify( const com::sun::star::uno::Sequence< rtl::OUString >& _rPropertyNames);

	sal_Bool 				IsEnabled() const {return bIsEnabled;}
	void	 				Enable(sal_Bool bSet)  {bIsEnabled = bSet; SetModified();}

	sal_Int32					SubstitutionCount() const;
	void						ClearSubstitutions();
	const SubstitutionStruct*	GetSubstitution(sal_Int32 nPos);
	void						AddSubstitution(const SubstitutionStruct&	rToAdd);
	void						Apply();
};

#endif
