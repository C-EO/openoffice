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



#ifndef _TOOLKIT_AWT_VCLXPOINTER_HXX_
#define _TOOLKIT_AWT_VCLXPOINTER_HXX_


#include <com/sun/star/awt/XPointer.hpp>
#include <com/sun/star/lang/XTypeProvider.hpp>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <cppuhelper/weak.hxx>
#include <osl/mutex.hxx>

#include <vcl/pointr.hxx>

//	----------------------------------------------------
//	class VCLXPointer
//	----------------------------------------------------

class VCLXPointer :	public ::com::sun::star::awt::XPointer, 
					public ::com::sun::star::lang::XTypeProvider,
					public ::com::sun::star::lang::XUnoTunnel,
					public ::cppu::OWeakObject
{
private:
	::osl::Mutex	maMutex;
	Pointer			maPointer;

protected:
	::osl::Mutex&	GetMutex() { return maMutex; }

public:	
	VCLXPointer();
	~VCLXPointer();

	const Pointer&		GetPointer() const { return maPointer; }
	
	// ::com::sun::star::uno::XInterface
    ::com::sun::star::uno::Any	SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	void						SAL_CALL acquire() throw()	{ OWeakObject::acquire(); }
	void						SAL_CALL release() throw()	{ OWeakObject::release(); }

	// ::com::sun::star::lang::XUnoTunnel
	static const ::com::sun::star::uno::Sequence< sal_Int8 >&	GetUnoTunnelId() throw();
	static VCLXPointer*											GetImplementation( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& rxIFace ) throw();
	sal_Int64													SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& rIdentifier ) throw(::com::sun::star::uno::RuntimeException);

	// ::com::sun::star::lang::XTypeProvider
	::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type >	SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Sequence< sal_Int8 >						SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);

	// ::com::sun::star::awt::XPointer
    void SAL_CALL setType( sal_Int32 nType ) throw(::com::sun::star::uno::RuntimeException);
    sal_Int32 SAL_CALL getType(  ) throw(::com::sun::star::uno::RuntimeException);
};




#endif // _TOOLKIT_AWT_VCLXPOINTER_HXX_
