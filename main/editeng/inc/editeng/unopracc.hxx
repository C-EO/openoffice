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



#ifndef _SVX_UNOPRACC_HXX
#define _SVX_UNOPRACC_HXX

#include <editeng/unotext.hxx>


class SvxEditSource;

/** Wraps SvxUnoTextRangeBase and provides us with the text properties
 
    Inherits from SvxUnoTextRangeBase and provides XPropertySet and
    XMultiPropertySet interfaces. Just set the selection to the
    required text range and return a reference to a XPropertySet.
 */
class SvxAccessibleTextPropertySet : public SvxUnoTextRangeBase, 
                                     public ::com::sun::star::lang::XTypeProvider,
                                     public ::cppu::OWeakObject
{
public:
    SvxAccessibleTextPropertySet( const SvxEditSource*, const SvxItemPropertySet* );
	virtual ~SvxAccessibleTextPropertySet() throw();

    // XTextRange
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::text::XText > SAL_CALL getText() throw (::com::sun::star::uno::RuntimeException);
 
	// uno::XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryAggregation( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	virtual void SAL_CALL acquire() throw();
	virtual void SAL_CALL release() throw();

	// lang::XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ) throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames() throw (::com::sun::star::uno::RuntimeException);

	// lang::XTypeProvider
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);

	// XServiceName
    ::rtl::OUString SAL_CALL getServiceName() throw (::com::sun::star::uno::RuntimeException);
};

#endif
