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



#ifndef FORMS_SOURCE_INC_URLTRANSFORMER_HXX
#define FORMS_SOURCE_INC_URLTRANSFORMER_HXX

/** === begin UNO includes === **/
#include <com/sun/star/util/XURLTransformer.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/util/URL.hpp>
/** === end UNO includes === **/

//........................................................................
namespace frm
{
//........................................................................

	//====================================================================
	//= UrlTransformer
	//====================================================================
	class UrlTransformer
	{
    private:
        ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >
                        m_xORB;
        mutable ::com::sun::star::uno::Reference< ::com::sun::star::util::XURLTransformer >
                        m_xTransformer;
        mutable bool    m_bTriedToCreateTransformer;

    public:
        UrlTransformer( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& _rxORB );

        /** returns an URL object for the given URL string
        */
        ::com::sun::star::util::URL
            getStrictURL( const ::rtl::OUString& _rURL ) const;

        /** returns an URL object for the given URL ASCII string
        */
        ::com::sun::star::util::URL
            getStrictURLFromAscii( const sal_Char* _pAsciiURL ) const;

        /** parses a given URL smartly, with a protocol given by ASCII string
        */
        void
            parseSmartWithAsciiProtocol( ::com::sun::star::util::URL& _rURL, const sal_Char* _pAsciiURL ) const;

    private:
        /** ensures that we have an URLTransformer instance in <member>m_xTransformer</member>

            @return
                <TRUE/> if and only if m_xTransformer is not <NULL/>
        */
        bool    implEnsureTransformer() const;
	};

//........................................................................
} // namespace frm
//........................................................................

#endif // FORMS_SOURCE_INC_URLTRANSFORMER_HXX
