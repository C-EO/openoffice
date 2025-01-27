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



#ifndef DBACCESS_CORE_API_WRAPPEDRESULTSET_HXX
#define DBACCESS_CORE_API_WRAPPEDRESULTSET_HXX

#ifndef DBACCESS_CORE_API_CACHESET_HXX
#include "CacheSet.hxx"
#endif
#include <com/sun/star/sdbc/XRowUpdate.hpp>
#include <com/sun/star/sdbc/XResultSetUpdate.hpp>

namespace dbaccess
{
	// this set is used when we have a bookmarkable set from the driver
	class WrappedResultSet : public OCacheSet
	{
		::com::sun::star::uno::Reference< ::com::sun::star::sdbcx::XRowLocate> m_xRowLocate;
        ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSetUpdate> m_xUpd;
        ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XRowUpdate>       m_xUpdRow;

		void updateColumn(sal_Int32 nPos,::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XRowUpdate > _xParameter,const connectivity::ORowSetValue& _rValue);
	public:
        WrappedResultSet(sal_Int32 i_nMaxRows) : OCacheSet(i_nMaxRows)
		{}
		~WrappedResultSet()
		{
			m_xRowLocate = NULL;
		}

		virtual void construct(const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet>& _xDriverSet,const ::rtl::OUString& i_sRowSetFilter);
		virtual void fillValueRow(ORowSetRow& _rRow,sal_Int32 _nPosition);
		// ::com::sun::star::sdbcx::XRowLocate
		virtual ::com::sun::star::uno::Any SAL_CALL getBookmark() throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Bool SAL_CALL moveToBookmark( const ::com::sun::star::uno::Any& bookmark ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Bool SAL_CALL moveRelativeToBookmark( const ::com::sun::star::uno::Any& bookmark, sal_Int32 rows ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Int32 SAL_CALL compareBookmarks( const ::com::sun::star::uno::Any& first, const ::com::sun::star::uno::Any& second ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Bool SAL_CALL hasOrderedBookmarks(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Int32 SAL_CALL hashBookmark( const ::com::sun::star::uno::Any& bookmark ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		// ::com::sun::star::sdbcx::XDeleteRows
		virtual ::com::sun::star::uno::Sequence< sal_Int32 > SAL_CALL deleteRows( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& rows ,const connectivity::OSQLTable& _xTable) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		// ::com::sun::star::sdbc::XResultSetUpdate
		virtual void SAL_CALL insertRow( const ORowSetRow& _rInsertRow,const connectivity::OSQLTable& _xTable ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL updateRow(const ORowSetRow& _rInsertRow,const ORowSetRow& _rOrginalRow,const connectivity::OSQLTable& _xTable   ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL deleteRow(const ORowSetRow& _rInsertRow,const connectivity::OSQLTable& _xTable   ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL cancelRowUpdates(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL moveToInsertRow(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL moveToCurrentRow(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
	};
}
#endif // DBACCESS_CORE_API_WRAPPEDRESULTSET_HXX
