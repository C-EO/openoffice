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



#ifndef CONNECTIVITY_MYSQL_TABLE_HXX
#define CONNECTIVITY_MYSQL_TABLE_HXX

#include "connectivity/TTableHelper.hxx"
#include <com/sun/star/sdbc/XDatabaseMetaData.hpp>
#include <comphelper/IdPropArrayHelper.hxx>

namespace connectivity
{
	namespace mysql
	{

        ::rtl::OUString getTypeString(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& xColProp);

		class OMySQLTable;
		typedef ::comphelper::OIdPropertyArrayUsageHelper< OMySQLTable >	OMySQLTable_PROP;
		class OMySQLTable :	public OTableHelper
							,public OMySQLTable_PROP
		{
			sal_Int32 m_nPrivileges; // we have to set our privileges by our own

			/** executes the statmenmt.
				@param	_rStatement
					The statement to execute.
				*/
			void executeStatement(const ::rtl::OUString& _rStatement );
		protected:

			/** creates the column collection for the table
				@param	_rNames
					The column names.
			*/
			virtual sdbcx::OCollection* createColumns(const TStringVector& _rNames);

			/** creates the key collection for the table
				@param	_rNames
					The key names.
			*/
			virtual sdbcx::OCollection* createKeys(const TStringVector& _rNames);

			/** creates the index collection for the table
				@param	_rNames
					The index names.
			*/
			virtual sdbcx::OCollection* createIndexes(const TStringVector& _rNames);

            /** Returns always "RENAME TABLE " even for views.
            *
            * \return The start of the rename statement.
            * @see http://dev.mysql.com/doc/refman/5.1/de/rename-table.html
            */
            virtual ::rtl::OUString getRenameStart() const;

			/** used to implement the creation of the array helper which is shared amongst all instances of the class.
				This method needs to be implemented in derived classes.
				<BR>
				The method gets called with s_aMutex acquired.
				<BR>
				as long as IPropertyArrayHelper has no virtual destructor, the implementation of ~OPropertyArrayUsageHelper
				assumes that you created an ::cppu::OPropertyArrayHelper when deleting s_pProps.
				@return							an pointer to the newly created array helper. Must not be NULL.
			*/
			virtual ::cppu::IPropertyArrayHelper* createArrayHelper(sal_Int32 nId) const;
			virtual ::cppu::IPropertyArrayHelper & SAL_CALL getInfoHelper();
			
		public:
			OMySQLTable(	sdbcx::OCollection* _pTables,
							const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _xConnection);
			OMySQLTable(	sdbcx::OCollection* _pTables,
							const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _xConnection,
							const ::rtl::OUString& _Name,
							const ::rtl::OUString& _Type,
							const ::rtl::OUString& _Description = ::rtl::OUString(),
							const ::rtl::OUString& _SchemaName = ::rtl::OUString(),
							const ::rtl::OUString& _CatalogName = ::rtl::OUString(),
							sal_Int32 _nPrivileges = 0
				);

			// ODescriptor
			virtual void construct();
			// com::sun::star::lang::XUnoTunnel
            virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& aIdentifier ) throw(::com::sun::star::uno::RuntimeException);
            static ::com::sun::star::uno::Sequence< sal_Int8 > getUnoTunnelImplementationId();

			// XAlterTable
            virtual void SAL_CALL alterColumnByName( const ::rtl::OUString& colName, const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& descriptor ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::container::NoSuchElementException, ::com::sun::star::uno::RuntimeException);
			/** returns the ALTER TABLE XXX statement
			*/
			::rtl::OUString getAlterTableColumnPart();

			// some methods to alter table structures
			void alterColumnType(sal_Int32 nNewType,const ::rtl::OUString& _rColName,const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _xDescriptor);
			void alterDefaultValue(const ::rtl::OUString& _sNewDefault,const ::rtl::OUString& _rColName);
			void dropDefaultValue(const ::rtl::OUString& _sNewDefault);

            virtual ::rtl::OUString getTypeCreatePattern() const;
		};
	}
}
#endif // CONNECTIVITY_MYSQL_TABLE_HXX
