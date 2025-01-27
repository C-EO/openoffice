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


#ifndef _SVX_CHARSCALEITEM_HXX
#define _SVX_CHARSCALEITEM_HXX

// include ---------------------------------------------------------------

#include <svl/intitem.hxx>
#include <editeng/editengdllapi.h>

// class SvxCharScaleItem ----------------------------------------------

/* [Description]

	This item defines a character scaling factor as percent value.
	A value of 100 is identical to 100% and means normal width
	A value of 50 is identical to 50% and means 1/2 width.

*/

class EDITENG_DLLPUBLIC SvxCharScaleWidthItem : public SfxUInt16Item
{
public:
	TYPEINFO();

    SvxCharScaleWidthItem( sal_uInt16 nValue /*= 100*/,
                            const sal_uInt16 nId );

	virtual SfxPoolItem*	Clone( SfxItemPool *pPool = 0 ) const;
	virtual SfxPoolItem*	Create(SvStream &, sal_uInt16) const;
	virtual SvStream&		Store( SvStream& , sal_uInt16 nItemVersion ) const;
	virtual sal_uInt16			GetVersion( sal_uInt16 nFileVersion ) const;

	virtual SfxItemPresentation GetPresentation( SfxItemPresentation ePres,
									SfxMapUnit eCoreMetric,
									SfxMapUnit ePresMetric,
									String &rText,
                                    const IntlWrapper * = 0 ) const;

	virtual sal_Bool PutValue( const com::sun::star::uno::Any& rVal,
									sal_uInt8 nMemberId );
	virtual sal_Bool QueryValue( com::sun::star::uno::Any& rVal,
								sal_uInt8 nMemberId ) const;

	inline SvxCharScaleWidthItem& operator=(const SvxCharScaleWidthItem& rItem )
	{
		SetValue( rItem.GetValue() );
		return *this;
	}

};

#endif
