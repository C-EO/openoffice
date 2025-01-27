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


#ifndef _DBA_COREAPI_STATEMENT_HXX_
#define _DBA_COREAPI_STATEMENT_HXX_

#ifndef _COM_SUN_STAR_LANG_XSERVICEINFO_HPP_
#include <com/sun/star/lang/XServiceInfo.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XSTATEMENT_HPP_
#include <com/sun/star/sdbc/XStatement.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XCONNECTION_HPP_
#include <com/sun/star/sdbc/XConnection.hpp>
#endif
#ifndef _COM_SUN_STAR_UTIL_XCANCELLABLE_HPP_
#include <com/sun/star/util/XCancellable.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XWARNINGSSUPPLIER_HPP_
#include <com/sun/star/sdbc/XWarningsSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XCLOSEABLE_HPP_
#include <com/sun/star/sdbc/XCloseable.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XMULTIPLERESULTS_HDL_
#include <com/sun/star/sdbc/XMultipleResults.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XPREPAREDBATCHEXECUTION_HDL_
#include <com/sun/star/sdbc/XPreparedBatchExecution.hpp>
#endif
#include <com/sun/star/sdbc/XBatchExecution.hpp>
#ifndef _COM_SUN_STAR_SDBC_XGENERATEDRESULTSET_HPP_
#include <com/sun/star/sdbc/XGeneratedResultSet.hpp>
#endif
#ifndef _COM_SUN_STAR_SDB_XSINGLESELECTQUERYCOMPOSER_HPP_
#include <com/sun/star/sdb/XSingleSelectQueryComposer.hpp>
#endif
#ifndef _CPPUHELPER_PROPSHLP_HXX
#include <cppuhelper/propshlp.hxx>
#endif
#ifndef _COMPHELPER_PROPERTY_ARRAY_HELPER_HXX_
#include <comphelper/proparrhlp.hxx>
#endif
#ifndef _DBASHARED_APITOOLS_HXX_
#include "apitools.hxx"
#endif
#ifndef _COMPHELPER_BROADCASTHELPER_HXX_
#include <comphelper/broadcasthelper.hxx>
#endif

#include <cppuhelper/implbase3.hxx>

//************************************************************
//  OStatementBase
//************************************************************
class OStatementBase :	public comphelper::OBaseMutex,
						public OSubComponent,
						public ::cppu::OPropertySetHelper,
						public ::comphelper::OPropertyArrayUsageHelper < OStatementBase >,
						public ::com::sun::star::util::XCancellable,
						public ::com::sun::star::sdbc::XWarningsSupplier,
						public ::com::sun::star::sdbc::XPreparedBatchExecution,
						public ::com::sun::star::sdbc::XMultipleResults,
						public ::com::sun::star::sdbc::XCloseable,
						public ::com::sun::star::sdbc::XGeneratedResultSet
{
protected:
	::osl::Mutex			m_aCancelMutex;

	::com::sun::star::uno::WeakReferenceHelper	m_aResultSet;
	::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > m_xAggregateAsSet;
	::com::sun::star::uno::Reference< ::com::sun::star::util::XCancellable > m_xAggregateAsCancellable;
	sal_Bool				m_bUseBookmarks;
    sal_Bool                m_bEscapeProcessing;

	virtual ~OStatementBase();

public:
	OStatementBase(const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection > & _xConn,
				   const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > & _xStatement);
	

// ::com::sun::star::lang::XTypeProvider
	virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes() throw (::com::sun::star::uno::RuntimeException);

// ::com::sun::star::uno::XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw (::com::sun::star::uno::RuntimeException);
	virtual void SAL_CALL acquire() throw();
    virtual void SAL_CALL release() throw();

// OComponentHelper
	virtual void SAL_CALL disposing(void);

// com::sun::star::beans::XPropertySet
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo > SAL_CALL getPropertySetInfo(  ) throw(::com::sun::star::uno::RuntimeException);

// comphelper::OPropertyArrayUsageHelper
	virtual ::cppu::IPropertyArrayHelper* createArrayHelper( ) const;

// cppu::OPropertySetHelper
	virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

	virtual sal_Bool SAL_CALL convertFastPropertyValue(
							::com::sun::star::uno::Any & rConvertedValue,
							::com::sun::star::uno::Any & rOldValue,
							sal_Int32 nHandle,
							const ::com::sun::star::uno::Any& rValue )
								throw (::com::sun::star::lang::IllegalArgumentException);
	virtual void SAL_CALL setFastPropertyValue_NoBroadcast(
								sal_Int32 nHandle,
								const ::com::sun::star::uno::Any& rValue
												 )
												 throw (::com::sun::star::uno::Exception);
	virtual void SAL_CALL getFastPropertyValue( ::com::sun::star::uno::Any& rValue, sal_Int32 nHandle ) const;

// ::com::sun::star::sdbc::XWarningsSupplier
    virtual ::com::sun::star::uno::Any SAL_CALL getWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL clearWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

// ::com::sun::star::util::XCancellable
	virtual void SAL_CALL cancel(  ) throw(::com::sun::star::uno::RuntimeException);

// ::com::sun::star::sdbc::XCloseable
	virtual void SAL_CALL close(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

// ::com::sun::star::sdbc::XMultipleResults
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet > SAL_CALL getResultSet(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Int32 SAL_CALL getUpdateCount(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL getMoreResults(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

// ::com::sun::star::sdbc::XPreparedBatchExecution
    virtual void SAL_CALL addBatch(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL clearBatch(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< sal_Int32 > SAL_CALL executeBatch(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
// ::com::sun::star::sdbc::XGeneratedResultSet
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet > SAL_CALL getGeneratedValues(  ) throw (::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

// Helper
	void disposeResultSet();

protected:
    using ::cppu::OPropertySetHelper::getFastPropertyValue;
};

//************************************************************
//  OStatement
//************************************************************
typedef ::cppu::ImplHelper3 <   ::com::sun::star::sdbc::XStatement
                            ,   ::com::sun::star::lang::XServiceInfo
                            ,   ::com::sun::star::sdbc::XBatchExecution
                            >   OStatement_IFACE;
class OStatement    :public OStatementBase
                    ,public OStatement_IFACE
{
private:
	::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XStatement >                  m_xAggregateStatement;
    ::com::sun::star::uno::Reference< ::com::sun::star::sdb::XSingleSelectQueryComposer >   m_xComposer;
    bool                                                                                    m_bAttemptedComposerCreation;

public:
	OStatement(const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection > & _xConn,
			   const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > & _xStatement);

    DECLARE_XINTERFACE()
    DECLARE_XTYPEPROVIDER()

// ::com::sun::star::lang::XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName(  ) throw(::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ServiceName ) throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames(  ) throw(::com::sun::star::uno::RuntimeException);

// ::com::sun::star::sdbc::XStatement
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet > SAL_CALL executeQuery( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Int32 SAL_CALL executeUpdate( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL execute( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection > SAL_CALL getConnection(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

    // OComponentHelper
	virtual void SAL_CALL disposing();

    // XBatchExecution
	virtual void SAL_CALL addBatch( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
	virtual void SAL_CALL clearBatch(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Sequence< sal_Int32 > SAL_CALL executeBatch(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

    using OStatementBase::addBatch;

private:
    /** does escape processing for the given SQL command, if the our EscapeProcessing
        property allows so.
    */
    ::rtl::OUString impl_doEscapeProcessing_nothrow( const ::rtl::OUString& _rSQL ) const;
    bool            impl_ensureComposer_nothrow() const;
};

#endif // _DBA_COREAPI_STATEMENT_HXX_
