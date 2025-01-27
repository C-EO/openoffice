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



#ifndef _DBAUI_UNOSQLMESSAGE_HXX_
#define _DBAUI_UNOSQLMESSAGE_HXX_

#ifndef _SVT_GENERICUNODIALOG_HXX_
#include <svtools/genericunodialog.hxx>
#endif
#ifndef _DBAUI_MODULE_DBU_HXX_
#include "moduledbu.hxx"
#endif
//.........................................................................
namespace dbaui
{
//.........................................................................

typedef ::svt::OGenericUnoDialog OSQLMessageDialogBase;
class OSQLMessageDialog
		:public OSQLMessageDialogBase
		,public ::comphelper::OPropertyArrayUsageHelper< OSQLMessageDialog >
{
	OModuleClient m_aModuleClient;
protected:
	// <properties>
	::com::sun::star::uno::Any	m_aException;
    ::rtl::OUString             m_sHelpURL;
	// </properties>

protected:
	OSQLMessageDialog(const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& _rxORB);

public:
	// XTypeProvider
    virtual com::sun::star::uno::Sequence<sal_Int8> SAL_CALL getImplementationId(  ) throw(com::sun::star::uno::RuntimeException);

	// XServiceInfo
	virtual ::rtl::OUString SAL_CALL getImplementationName() throw(com::sun::star::uno::RuntimeException);
	virtual ::comphelper::StringSequence SAL_CALL getSupportedServiceNames() throw(com::sun::star::uno::RuntimeException);

	// XServiceInfo - static methods
	static com::sun::star::uno::Sequence< ::rtl::OUString > getSupportedServiceNames_Static(void) throw( com::sun::star::uno::RuntimeException );
	static ::rtl::OUString getImplementationName_Static(void) throw( com::sun::star::uno::RuntimeException );
	static com::sun::star::uno::Reference< com::sun::star::uno::XInterface >
			SAL_CALL Create(const com::sun::star::uno::Reference< com::sun::star::lang::XMultiServiceFactory >&);

	// XPropertySet
	virtual com::sun::star::uno::Reference<com::sun::star::beans::XPropertySetInfo>  SAL_CALL getPropertySetInfo() throw(com::sun::star::uno::RuntimeException);
	virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

	// OPropertyArrayUsageHelper
	virtual ::cppu::IPropertyArrayHelper* createArrayHelper( ) const;

protected:
// OPropertySetHelper overridables
	// (overwrittin these three, because we have some special handling for our property)
	virtual sal_Bool SAL_CALL convertFastPropertyValue( com::sun::star::uno::Any& _rConvertedValue, com::sun::star::uno::Any& _rOldValue, sal_Int32 _nHandle, const com::sun::star::uno::Any& _rValue) throw(com::sun::star::lang::IllegalArgumentException);

// OGenericUnoDialog overridables
	virtual Dialog*	createDialog(Window* _pParent);
};

//.........................................................................
}	// namespace dbaui
//.........................................................................

#endif // _DBAUI_UNOSQLMESSAGE_HXX_
