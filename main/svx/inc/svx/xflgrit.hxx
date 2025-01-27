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



#ifndef _SVX_XFLGRIT_HXX
#define _SVX_XFLGRIT_HXX

#include <svx/xit.hxx>
#include <svx/xgrad.hxx>
#include "svx/svxdllapi.h"

class SdrModel;

//------------------------
// class XFillGradientItem
//------------------------
class SVX_DLLPUBLIC XFillGradientItem : public NameOrIndex
{
	XGradient   aGradient;

public:
			TYPEINFO();
			XFillGradientItem() : NameOrIndex(XATTR_FILLGRADIENT, -1) {}
			XFillGradientItem(sal_Int32 nIndex, const XGradient& rTheGradient);
			XFillGradientItem(const UniString& rName, const XGradient& rTheGradient);
			XFillGradientItem(SfxItemPool* pPool, const XGradient& rTheGradient);
			XFillGradientItem(SfxItemPool* pPool );
			XFillGradientItem(const XFillGradientItem& rItem);
			XFillGradientItem(SvStream& rIn, sal_uInt16 nVer = 0);

	virtual int             operator==(const SfxPoolItem& rItem) const;
	virtual SfxPoolItem*    Clone(SfxItemPool* pPool = 0) const;
	virtual SfxPoolItem*    Create(SvStream& rIn, sal_uInt16 nVer) const;
	virtual SvStream&       Store(SvStream& rOut, sal_uInt16 nItemVersion ) const;
	virtual sal_uInt16          GetVersion( sal_uInt16 nFileFormatVersion ) const;

	virtual	sal_Bool        	 QueryValue( com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 ) const;
	virtual	sal_Bool			 PutValue( const com::sun::star::uno::Any& rVal, sal_uInt8 nMemberId = 0 );
	virtual SfxItemPresentation GetPresentation( SfxItemPresentation ePres,
									SfxMapUnit eCoreMetric,
									SfxMapUnit ePresMetric,
                                    String &rText, const IntlWrapper * = 0 ) const;
	const XGradient&		GetGradientValue(const XGradientList* pTable = 0) const; // GetValue -> GetGradientValue
	void					SetGradientValue(const XGradient& rNew) { aGradient = rNew; Detach(); } // SetValue -> SetGradientValue

	static sal_Bool CompareValueFunc( const NameOrIndex* p1, const NameOrIndex* p2 );
	XFillGradientItem* checkForUniqueItem( SdrModel* pModel ) const;
};

#endif
