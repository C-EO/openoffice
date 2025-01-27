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



#ifndef _CONNECTIVITY_ADABAS_GROUP_HXX_
#define _CONNECTIVITY_ADABAS_GROUP_HXX_

#include "connectivity/sdbcx/VGroup.hxx"
#include "ado/Awrapadox.hxx"

namespace connectivity
{
	namespace ado
	{
		typedef sdbcx::OGroup OGroup_ADO;
		class OCatalog;

		class OAdoGroup :	public OGroup_ADO
		{
			WpADOGroup		m_aGroup;
			OCatalog*		m_pCatalog;

			sal_Int32		MapRight(RightsEnum _eNum);
			RightsEnum		Map2Right(sal_Int32 _eNum);
			ObjectTypeEnum	MapObjectType(sal_Int32 _ObjType);
		protected:
            virtual void SAL_CALL getFastPropertyValue(::com::sun::star::uno::Any& rValue,sal_Int32 nHandle) const;
            virtual void SAL_CALL setFastPropertyValue_NoBroadcast(sal_Int32 nHandle,const ::com::sun::star::uno::Any& rValue)throw (::com::sun::star::uno::Exception);

		public:
			virtual void refreshUsers();
		public:
			OAdoGroup(OCatalog* _pParent,sal_Bool _bCase, ADOGroup* _pGroup=NULL);
			OAdoGroup(OCatalog* _pParent,sal_Bool _bCase, const ::rtl::OUString& _Name);

			virtual void SAL_CALL acquire() throw();
            virtual void SAL_CALL release() throw();
			// com::sun::star::lang::XUnoTunnel
            virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& aIdentifier ) throw(::com::sun::star::uno::RuntimeException);
            static ::com::sun::star::uno::Sequence< sal_Int8 > getUnoTunnelImplementationId();

			// XAuthorizable
			virtual sal_Int32 SAL_CALL getPrivileges( const ::rtl::OUString& objName, sal_Int32 objType ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			virtual sal_Int32 SAL_CALL getGrantablePrivileges( const ::rtl::OUString& objName, sal_Int32 objType ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			virtual void SAL_CALL grantPrivileges( const ::rtl::OUString& objName, sal_Int32 objType, sal_Int32 objPrivileges ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			virtual void SAL_CALL revokePrivileges( const ::rtl::OUString& objName, sal_Int32 objType, sal_Int32 objPrivileges ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);

			WpADOGroup getImpl() const { return m_aGroup; }
		};
	}
}
#endif // _CONNECTIVITY_ADABAS_GROUP_HXX_
