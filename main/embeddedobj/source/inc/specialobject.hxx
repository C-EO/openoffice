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



#ifndef _INC_SPECIALOBJECT_HXX_
#define _INC_SPECIALOBJECT_HXX_

#include <com/sun/star/awt/Size.hpp>

#include "commonembobj.hxx"

class OSpecialEmbeddedObject : public OCommonEmbeddedObject
{
private:
    com::sun::star::awt::Size         maSize;
public:
    OSpecialEmbeddedObject(
		const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& xFactory,
        const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::NamedValue >& aObjectProps );

    // XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& rType )
		throw( ::com::sun::star::uno::RuntimeException ) ;

    // XTypeProvider
	virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes()
		throw( ::com::sun::star::uno::RuntimeException ) ;

    // XVisualObject
    virtual ::com::sun::star::embed::VisualRepresentation SAL_CALL getPreferredVisualRepresentation( ::sal_Int64 nAspect )
		throw ( ::com::sun::star::lang::IllegalArgumentException,
				::com::sun::star::embed::WrongStateException,
				::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL setVisualAreaSize( sal_Int64 nAspect, const ::com::sun::star::awt::Size& aSize )
		throw ( ::com::sun::star::lang::IllegalArgumentException,
				::com::sun::star::embed::WrongStateException,
				::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException );

    virtual ::com::sun::star::awt::Size SAL_CALL getVisualAreaSize( sal_Int64 nAspect )
		throw ( ::com::sun::star::lang::IllegalArgumentException,
				::com::sun::star::embed::WrongStateException,
				::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException );

    virtual sal_Int32 SAL_CALL getMapUnit( sal_Int64 nAspect )
		throw ( ::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL changeState( sal_Int32 nNewState )
		throw ( ::com::sun::star::embed::UnreachableStateException,
				::com::sun::star::embed::WrongStateException,
				::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL doVerb( sal_Int32 nVerbID )
		throw ( ::com::sun::star::lang::IllegalArgumentException,
				::com::sun::star::embed::WrongStateException,
				::com::sun::star::embed::UnreachableStateException,
				::com::sun::star::uno::Exception,
				::com::sun::star::uno::RuntimeException );
};

#endif
