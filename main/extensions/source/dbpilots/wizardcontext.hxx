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



#ifndef _EXTENSIONS_DBP_WIZARDCONTEXT_HXX_
#define _EXTENSIONS_DBP_WIZARDCONTEXT_HXX_

#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/sdbc/XRowSet.hpp>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/drawing/XDrawPage.hpp>
#include <com/sun/star/drawing/XControlShape.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <comphelper/stl_types.hxx>

//.........................................................................
namespace dbp
{
//.........................................................................

	//=====================================================================
	//= OControlWizardContext
	//=====================================================================
	struct OControlWizardContext
	{
		// the global data source context
		::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >
					xDatasourceContext;

		// the control mode
		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >
					xObjectModel;
		// the form the control model belongs to
		::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >
					xForm;
		// the form as rowset
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XRowSet >
					xRowSet;

		// the model of the document
		::com::sun::star::uno::Reference< ::com::sun::star::frame::XModel >
					xDocumentModel;
		// the page where the control mode resides
		::com::sun::star::uno::Reference< ::com::sun::star::drawing::XDrawPage >
					xDrawPage;
		// the shape which carries the control
		::com::sun::star::uno::Reference< ::com::sun::star::drawing::XControlShape >
					xObjectShape;

		// the tables or queries of the data source the form is bound to (if any)
		::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >
					xObjectContainer;
		// the column types container of the object the form is bound to (table, query or SQL statement)
		DECLARE_STL_USTRINGACCESS_MAP(sal_Int32,TNameTypeMap);
		TNameTypeMap aTypes;
		// the column names of the object the form is bound to (table, query or SQL statement)
		::com::sun::star::uno::Sequence< ::rtl::OUString >
					aFieldNames;

		sal_Bool bEmbedded;
	};

//.........................................................................
}	// namespace dbp
//.........................................................................

#endif // _EXTENSIONS_DBP_WIZARDCONTEXT_HXX_
