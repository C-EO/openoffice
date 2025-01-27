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



#ifndef _EXTENSIONS_PROPCTRLR_CONTROLFONTDIALOG_HXX_
#define _EXTENSIONS_PROPCTRLR_CONTROLFONTDIALOG_HXX_

#include <svtools/genericunodialog.hxx>
#include "modulepcr.hxx"

class SfxItemSet;
class SfxItemPool;
class SfxPoolItem;
//........................................................................
namespace pcr
{
//........................................................................

	//====================================================================
	//= OControlFontDialog
	//====================================================================
	class OControlFontDialog;
	typedef ::svt::OGenericUnoDialog										OControlFontDialog_DBase;
	typedef ::comphelper::OPropertyArrayUsageHelper< OControlFontDialog >	OControlFontDialog_PBase;

	class OControlFontDialog
				:public OControlFontDialog_DBase
				,public OControlFontDialog_PBase
				,public PcrClient
	{
	protected:
		// <properties>
		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >
				m_xControlModel;
		// </properties>

		SfxItemSet*				m_pFontItems;			// item set for the dialog
		SfxItemPool*			m_pItemPool;			// item pool for the item set for the dialog
		SfxPoolItem**			m_pItemPoolDefaults;	// pool defaults

	public:
		OControlFontDialog(const com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >& _rxContext);
		~OControlFontDialog();

		// XTypeProvider
		virtual ::com::sun::star::uno::Sequence<sal_Int8> SAL_CALL getImplementationId(  ) throw(::com::sun::star::uno::RuntimeException);

		// XServiceInfo
		virtual ::rtl::OUString SAL_CALL getImplementationName() throw(::com::sun::star::uno::RuntimeException);
		virtual ::comphelper::StringSequence SAL_CALL getSupportedServiceNames() throw(::com::sun::star::uno::RuntimeException);

		// XServiceInfo - static methods
		static ::com::sun::star::uno::Sequence< ::rtl::OUString > getSupportedServiceNames_static(void) throw( ::com::sun::star::uno::RuntimeException );
		static ::rtl::OUString getImplementationName_static(void) throw( ::com::sun::star::uno::RuntimeException );
		static ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >
				SAL_CALL Create(const ::com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >&);

		// XPropertySet
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo>  SAL_CALL getPropertySetInfo() throw(::com::sun::star::uno::RuntimeException);
		virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

		// OPropertyArrayUsageHelper
		virtual ::cppu::IPropertyArrayHelper* createArrayHelper( ) const;

	protected:
	// OGenericUnoDialog overridables
		virtual Dialog*	createDialog(Window* _pParent);
		virtual void	destroyDialog();
		virtual void	executedDialog(sal_Int16 _nExecutionResult);
	};

//........................................................................
}	// namespace pcr
//........................................................................

#endif // _EXTENSIONS_PROPCTRLR_CONTROLFONTDIALOG_HXX_
