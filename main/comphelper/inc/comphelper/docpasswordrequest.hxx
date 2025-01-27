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



#ifndef COMPHELPER_DOCPASSWORDREQUEST_HXX
#define COMPHELPER_DOCPASSWORDREQUEST_HXX

#include "comphelper/comphelperdllapi.h"
#include <com/sun/star/task/PasswordRequestMode.hpp>
#include <com/sun/star/task/XInteractionRequest.hpp>
#include <cppuhelper/implbase1.hxx>
#include <cppuhelper/weak.hxx>


namespace comphelper {

class AbortContinuation;
class PasswordContinuation;
    
// ============================================================================

/** Selects which UNO document password request type to use. */
enum DocPasswordRequestType
{
    DocPasswordRequestType_STANDARD,    /// Uses the standard com.sun.star.task.DocumentPasswordRequest request.
    DocPasswordRequestType_MS           /// Uses the com.sun.star.task.DocumentMSPasswordRequest request.
};

// ============================================================================

class COMPHELPER_DLLPUBLIC SimplePasswordRequest : 
        public ::com::sun::star::task::XInteractionRequest,
        public ::cppu::OWeakObject
{
public:
    explicit    SimplePasswordRequest( com::sun::star::task::PasswordRequestMode eMode );
    virtual     ~SimplePasswordRequest();

    // XInterface / OWeakObject
    virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& aType ) throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL acquire(  ) throw ();
    virtual void SAL_CALL release(  ) throw ();

    sal_Bool            isAbort() const;
    sal_Bool            isPassword() const;

    ::rtl::OUString     getPassword() const;

private:
    // XInteractionRequest                
    virtual ::com::sun::star::uno::Any SAL_CALL getRequest() throw( ::com::sun::star::uno::RuntimeException );
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::task::XInteractionContinuation > > SAL_CALL getContinuations() throw( ::com::sun::star::uno::RuntimeException );

private:
    ::com::sun::star::uno::Any      maRequest;
    ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::task::XInteractionContinuation > > maContinuations;
    AbortContinuation *             mpAbort;
    PasswordContinuation *          mpPassword;
};

// ============================================================================

/** Implements the task.XInteractionRequest interface for requesting a password
    string for a document.
 */
class COMPHELPER_DLLPUBLIC DocPasswordRequest : 
        public ::com::sun::star::task::XInteractionRequest,
        public ::cppu::OWeakObject
{
public:
    explicit            DocPasswordRequest(
                            DocPasswordRequestType eType,
                            ::com::sun::star::task::PasswordRequestMode eMode,
                            const ::rtl::OUString& rDocumentName,
                            sal_Bool bPasswordToModify = sal_False );
    virtual             ~DocPasswordRequest();

    // XInterface / OWeakObject
    virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& aType ) throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL acquire(  ) throw ();
    virtual void SAL_CALL release(  ) throw ();

    sal_Bool            isAbort() const;
    sal_Bool            isPassword() const;

    ::rtl::OUString     getPassword() const;

    ::rtl::OUString     getPasswordToModify() const;
    sal_Bool            getRecommendReadOnly() const;

private:
    // XInteractionRequest                
    virtual ::com::sun::star::uno::Any SAL_CALL getRequest() throw( ::com::sun::star::uno::RuntimeException );
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::task::XInteractionContinuation > > SAL_CALL getContinuations() throw( ::com::sun::star::uno::RuntimeException );

private:
    ::com::sun::star::uno::Any      maRequest;
    ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::task::XInteractionContinuation > > maContinuations;
    AbortContinuation *             mpAbort;
    PasswordContinuation *          mpPassword;
};

// ============================================================================

} // namespace comphelper

#endif
