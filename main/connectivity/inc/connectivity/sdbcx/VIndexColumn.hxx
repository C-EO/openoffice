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



#ifndef _CONNECTIVITY_SDBCX_INDEXCOLUMN_HXX_
#define _CONNECTIVITY_SDBCX_INDEXCOLUMN_HXX_

#include "connectivity/dbtoolsdllapi.hxx"
#include "connectivity/sdbcx/VColumn.hxx"

namespace connectivity
{
	namespace sdbcx
	{
		class OIndexColumn;
		typedef ::comphelper::OIdPropertyArrayUsageHelper<OIndexColumn> OIndexColumn_PROP;

		class OOO_DLLPUBLIC_DBTOOLS OIndexColumn :
            public OColumn, public OIndexColumn_PROP
		{
		protected:
			sal_Bool	m_IsAscending;
			virtual ::cppu::IPropertyArrayHelper* createArrayHelper( sal_Int32 _nId) const;
			virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();
		public:
			OIndexColumn(	sal_Bool _bCase);
			OIndexColumn(	sal_Bool _IsAscending,
							const ::rtl::OUString& _Name,
							const ::rtl::OUString& _TypeName,
							const ::rtl::OUString& _DefaultValue,
							sal_Int32		_IsNullable,
							sal_Int32		_Precision,
							sal_Int32		_Scale,
							sal_Int32		_Type,
							sal_Bool		_IsAutoIncrement,
							sal_Bool		_IsRowVersion,
							sal_Bool		_IsCurrency,
							sal_Bool		_bCase);

			virtual void construct();
			DECLARE_SERVICE_INFO();
		};
	}
}
#endif // _CONNECTIVITY_SDBCX_INDEXCOLUMN_HXX_
