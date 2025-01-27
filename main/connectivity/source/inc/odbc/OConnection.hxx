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


#ifndef _CONNECTIVITY_ODBC_OCONNECTION_HXX_
#define _CONNECTIVITY_ODBC_OCONNECTION_HXX_

#include <com/sun/star/sdbc/SQLWarning.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include "odbc/OFunctiondefs.hxx"
#include "odbc/odbcbasedllapi.hxx"
#include "OSubComponent.hxx"
#include "connectivity/CommonTools.hxx"
#include "TConnection.hxx"
#include "OTypeInfo.hxx"
#include <cppuhelper/weakref.hxx>
#include "AutoRetrievingBase.hxx"
#include <osl/module.h>


#include <map>

namespace connectivity
{
	namespace odbc
	{

		class OStatement_Base;
		class ODBCDriver;
		class ODatabaseMetaData;

		typedef connectivity::OMetaConnection OConnection_BASE;
		typedef ::std::vector< ::connectivity::OTypeInfo>	TTypeInfoVector;

		class OOO_DLLPUBLIC_ODBCBASE OConnection :
                            public OConnection_BASE,
							public connectivity::OSubComponent<OConnection, OConnection_BASE>,
							public OAutoRetrievingBase
		{
			friend class connectivity::OSubComponent<OConnection, OConnection_BASE>;

		protected:
			//====================================================================
			// Data attributes
			//====================================================================
			::std::map< SQLHANDLE,OConnection*>	m_aConnections;	// holds all connectionas which are need for serveral statements
			TTypeInfoVector						m_aTypeInfo;	//	vector containing an entry
														//  for each row returned by
														//  DatabaseMetaData.getTypeInfo.
			

            ::com::sun::star::sdbc::SQLWarning  m_aLastWarning; //      Last SQLWarning generated by
														//  an operation
			::rtl::OUString	m_sUser;		// the user name
			ODBCDriver*		m_pDriver;		//	Pointer to the owning
											//  driver object
							
			SQLHANDLE		m_aConnectionHandle;
			SQLHANDLE		m_pDriverHandleCopy;	// performance reason
			sal_Int32		m_nStatementCount;
			sal_Bool		m_bClosed;
			sal_Bool		m_bUseCatalog;	// should we use the catalog on filebased databases
			sal_Bool		m_bUseOldDateFormat;
			sal_Bool		m_bParameterSubstitution;
			sal_Bool		m_bIgnoreDriverPrivileges;
            sal_Bool        m_bPreventGetVersionColumns;    // #i60273#
            sal_Bool        m_bReadOnly;


			SQLRETURN		OpenConnection(const ::rtl::OUString& aConnectStr,sal_Int32 nTimeOut, sal_Bool bSilent);

			virtual OConnection*	cloneConnection(); // creates a new connection
			
		public:
			oslGenericFunction  getOdbcFunction(sal_Int32 _nIndex)  const;
            virtual SQLRETURN       Construct( const ::rtl::OUString& url,const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& info) throw(::com::sun::star::sdbc::SQLException);

			OConnection(const SQLHANDLE _pDriverHandle,ODBCDriver*	_pDriver);
			//	OConnection(const SQLHANDLE _pConnectionHandle);
			virtual ~OConnection();

            void closeAllStatements () throw( ::com::sun::star::sdbc::SQLException);

			// OComponentHelper
			virtual void SAL_CALL disposing(void);
			// XInterface
			virtual void SAL_CALL release() throw();

			// XServiceInfo
			DECLARE_SERVICE_INFO();
			// XConnection
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XStatement > SAL_CALL createStatement(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XPreparedStatement > SAL_CALL prepareStatement( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XPreparedStatement > SAL_CALL prepareCall( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::rtl::OUString SAL_CALL nativeSQL( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setAutoCommit( sal_Bool autoCommit ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL getAutoCommit(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL commit(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL rollback(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL isClosed(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDatabaseMetaData > SAL_CALL getMetaData(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setReadOnly( sal_Bool readOnly ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL isReadOnly(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setCatalog( const ::rtl::OUString& catalog ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::rtl::OUString SAL_CALL getCatalog(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setTransactionIsolation( sal_Int32 level ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Int32 SAL_CALL getTransactionIsolation(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess > SAL_CALL getTypeMap(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setTypeMap( const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& typeMap ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			// XCloseable
			virtual void SAL_CALL close(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			// XWarningsSupplier
			virtual ::com::sun::star::uno::Any SAL_CALL getWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			virtual void SAL_CALL clearWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			//
			SQLHANDLE		getConnection() { return m_aConnectionHandle; }
			
			// should we use the catalog on filebased databases
			inline sal_Bool		isCatalogUsed()						const { return m_bUseCatalog; }
			inline sal_Bool		isParameterSubstitutionEnabled()	const { return m_bParameterSubstitution; }
			inline sal_Bool		isIgnoreDriverPrivilegesEnabled()	const { return m_bIgnoreDriverPrivileges; }
            inline sal_Bool     preventGetVersionColumns()          const { return m_bPreventGetVersionColumns; }
			inline sal_Bool		useOldDateFormat()					const { return m_bUseOldDateFormat; }
			inline SQLHANDLE		getDriverHandle()				const { return m_pDriverHandleCopy;}
			inline ODBCDriver*		getDriver()						const { return m_pDriver;}
			inline ::rtl::OUString getUserName()					const { return m_sUser; }

			SQLHANDLE		createStatementHandle();
			// close and free the handle and set it to SQL_NULLHANDLE
			void			freeStatementHandle(SQLHANDLE& _pHandle);

            void                    buildTypeInfo() throw( ::com::sun::star::sdbc::SQLException);
            const TTypeInfoVector&	getTypeInfo() const { return m_aTypeInfo; }
		};
	}
}
#endif // _CONNECTIVITY_ODBC_OCONNECTION_HXX_
