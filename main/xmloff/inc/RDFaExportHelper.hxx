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



#ifndef RDFAEXPORTHELPER_HXX
#define RDFAEXPORTHELPER_HXX

#include <com/sun/star/uno/Reference.h>

#include <rtl/ustring.hxx>

#include <map>


namespace com { namespace sun { namespace star {
    namespace rdf { class XBlankNode; }
    namespace rdf { class XMetadatable; }
    namespace rdf { class XDocumentRepository; }
} } }

class SvXMLExport;

namespace xmloff {

class SAL_DLLPRIVATE RDFaExportHelper
{
private:
    SvXMLExport & m_rExport;

    ::com::sun::star::uno::Reference<
        ::com::sun::star::rdf::XDocumentRepository> m_xRepository;

    typedef ::std::map< ::rtl::OUString, ::rtl::OUString >
        BlankNodeMap_t;

    BlankNodeMap_t m_BlankNodeMap;

    long m_Counter;

    ::rtl::OUString
    LookupBlankNode( ::com::sun::star::uno::Reference<
        ::com::sun::star::rdf::XBlankNode> const & i_xBlankNode);

public:
    RDFaExportHelper(SvXMLExport & i_rExport);

    void
    AddRDFa(::com::sun::star::uno::Reference<
        ::com::sun::star::rdf::XMetadatable> const & i_xMetadatable);
};

} // namespace xmloff

#endif // RDFAEXPORTHELPER_HXX
