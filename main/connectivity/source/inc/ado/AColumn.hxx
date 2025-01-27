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



#ifndef _CONNECTIVITY_ADO_COLUMN_HXX_
#define _CONNECTIVITY_ADO_COLUMN_HXX_

#include "connectivity/sdbcx/VColumn.hxx"
#include "ado/Awrapadox.hxx"

namespace connectivity
{
	namespace ado
	{
		class OConnection;
		typedef sdbcx::OColumn OColumn_ADO;
		class OAdoColumn :  public OColumn_ADO
		{
			WpADOColumn		m_aColumn;
			OConnection*	m_pConnection;
			::rtl::OUString	m_ReferencedColumn;
			sal_Bool		m_IsAscending;

			void fillPropertyValues();
		protected:
			virtual void SAL_CALL setFastPropertyValue_NoBroadcast(
									sal_Int32 nHandle,
                                    const ::com::sun::star::uno::Any& rValue
									 )
									 throw (::com::sun::star::uno::Exception);
		public:
			OAdoColumn(sal_Bool _bCase,OConnection* _pConnection,_ADOColumn* _pColumn);
			OAdoColumn(sal_Bool _bCase,OConnection* _pConnection);
			// ODescriptor
			virtual void construct();
			virtual void SAL_CALL acquire() throw();
            virtual void SAL_CALL release() throw();
			// com::sun::star::lang::XUnoTunnel
            virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& aIdentifier ) throw(::com::sun::star::uno::RuntimeException);
            static ::com::sun::star::uno::Sequence< sal_Int8 > getUnoTunnelImplementationId();

			WpADOColumn		getColumnImpl() const;
		};
	}
}
#endif // _CONNECTIVITY_ADO_COLUMN_HXX_
