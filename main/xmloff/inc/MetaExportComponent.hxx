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



#ifndef _XMLOFF_METAEXPORTCOMPONENT_HXX
#define _XMLOFF_METAEXPORTCOMPONENT_HXX

#include <com/sun/star/document/XDocumentProperties.hpp>

#include <xmloff/xmlexp.hxx>

class XMLMetaExportComponent : public SvXMLExport
{
	::com::sun::star::uno::Reference<
        ::com::sun::star::document::XDocumentProperties > mxDocProps;

public:
	// #110680#
	// XMLMetaExportComponent();
	XMLMetaExportComponent(
		const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& xServiceFactory,
		sal_uInt16 nFlags
		);

	virtual ~XMLMetaExportComponent();

protected:
	// export the events off all autotexts
	virtual sal_uInt32 exportDoc(
        enum ::xmloff::token::XMLTokenEnum eClass = xmloff::token::XML_TOKEN_INVALID );

    // accept XDocumentProperties in addition to XModel
	virtual void SAL_CALL setSourceDocument( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& xDoc ) throw(::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

    // override
	virtual void _ExportMeta();

	// methods without content:
	virtual void _ExportAutoStyles();
	virtual void _ExportMasterStyles();
	virtual void _ExportContent();
};

// global functions to support the component

::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL
	XMLMetaExportComponent_getSupportedServiceNames()
	throw();

::rtl::OUString SAL_CALL XMLMetaExportComponent_getImplementationName()
	throw();

::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > SAL_CALL
	XMLMetaExportComponent_createInstance(
		const ::com::sun::star::uno::Reference<
			::com::sun::star::lang::XMultiServiceFactory > & )
	throw( ::com::sun::star::uno::Exception );

#endif
