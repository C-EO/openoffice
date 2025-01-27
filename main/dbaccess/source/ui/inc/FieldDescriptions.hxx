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


#ifndef DBAUI_FIELDDESCRIPTIONS_HXX
#define DBAUI_FIELDDESCRIPTIONS_HXX

#ifndef DBAUI_ENUMTYPES_HXX
#include "QEnumTypes.hxx"
#endif
#ifndef _SVX_SVXENUM_HXX 
#include <editeng/svxenum.hxx>
#endif
#ifndef DBAUI_TYPEINFO_HXX
#include "TypeInfo.hxx"
#endif
#ifndef _OSL_DIAGNOSE_H_ 
#include <osl/diagnose.h>
#endif
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif			   
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSETINFO_HPP_
#include <com/sun/star/beans/XPropertySetInfo.hpp>
#endif

namespace dbaui
{
	class OFieldDescription
	{
	private:
		::com::sun::star::uno::Any		m_aDefaultValue;	// the default value from the database
		::com::sun::star::uno::Any		m_aControlDefault;	// the value which the control inserts as default
		::com::sun::star::uno::Any		m_aWidth;				// sal_Int32 or void
		::com::sun::star::uno::Any		m_aRelativePosition;	// sal_Int32 or void

		TOTypeInfoSP	m_pType;

		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >		m_xDest;
		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo >	m_xDestInfo;

		::rtl::OUString		m_sName;
		::rtl::OUString		m_sTypeName;
		::rtl::OUString		m_sDescription;
        ::rtl::OUString		m_sHelpText;
		
		::rtl::OUString		m_sAutoIncrementValue;
		sal_Int32			m_nType;	// only used when m_pType is null
		sal_Int32			m_nPrecision;
		sal_Int32			m_nScale;
		sal_Int32			m_nIsNullable;
		sal_Int32			m_nFormatKey;
		SvxCellHorJustify	m_eHorJustify;
		sal_Bool			m_bIsAutoIncrement;
		sal_Bool			m_bIsPrimaryKey;
		sal_Bool			m_bIsCurrency;
		sal_Bool			m_bHidden;

	public:
		OFieldDescription();
		OFieldDescription( const OFieldDescription& rDescr );
		OFieldDescription(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _xAffectedCol
						 ,sal_Bool _bUseAsDest = sal_False);
		~OFieldDescription();

		void SetName(const ::rtl::OUString& _rName);							
		void SetDescription(const ::rtl::OUString& _rDescription);
        void SetHelpText(const ::rtl::OUString& _sHelptext);
		void SetDefaultValue(const ::com::sun::star::uno::Any& _rDefaultValue);	
		void SetControlDefault(const ::com::sun::star::uno::Any& _rControlDefault);
		void SetAutoIncrementValue(const ::rtl::OUString& _sAutoIncValue);	
		void SetType(TOTypeInfoSP _pType);
		void SetTypeValue(sal_Int32 _nType);
		void SetTypeName(const ::rtl::OUString& _sTypeName);
		void SetPrecision(const sal_Int32& _rPrecision);						
		void SetScale(const sal_Int32& _rScale);								
		void SetIsNullable(const sal_Int32& _rIsNullable);					
		void SetFormatKey(const sal_Int32& _rFormatKey);						
		void SetHorJustify(const SvxCellHorJustify& _rHorJustify);			
		void SetAutoIncrement(sal_Bool _bAuto);								
		void SetPrimaryKey(sal_Bool _bPKey);									
		void SetCurrency(sal_Bool _bIsCurrency);

		/** copies the content of the field description into the column
			@param	_rxColumn the dest
		*/
		void copyColumnSettingsTo(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _rxColumn);

		void FillFromTypeInfo(const TOTypeInfoSP& _pType,sal_Bool _bForce,sal_Bool _bReset);

		::rtl::OUString				GetName()				const;
		::rtl::OUString				GetDescription()		const;
        ::rtl::OUString				GetHelpText()		    const;
		::com::sun::star::uno::Any	GetControlDefault()		const;
		::rtl::OUString				GetAutoIncrementValue()	const;
		sal_Int32					GetType()				const;
		::rtl::OUString				GetTypeName()			const;
		sal_Int32					GetPrecision()			const;
		sal_Int32					GetScale()				const;
		sal_Int32					GetIsNullable()			const;
		sal_Int32					GetFormatKey()			const;
		SvxCellHorJustify			GetHorJustify()			const;
		TOTypeInfoSP				getTypeInfo()			const;
        TOTypeInfoSP				getSpecialTypeInfo()	const;
		sal_Bool					IsAutoIncrement()		const;
		sal_Bool					IsPrimaryKey()			const;
		sal_Bool					IsCurrency()			const;
		sal_Bool					IsNullable()			const;
	};
}
#endif // DBAUI_FIELDDESCRIPTIONS_HXX


