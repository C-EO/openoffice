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



#ifndef _SVT_HATCHWINDOW_HXX
#define _SVT_HATCHWINDOW_HXX

#include <com/sun/star/embed/XHatchWindow.hpp>

#include <toolkit/awt/vclxwindow.hxx>
#include <cppuhelper/typeprovider.hxx>

class SvResizeWindow;
class VCLXHatchWindow : public ::com::sun::star::embed::XHatchWindow,
                        public VCLXWindow
{
	::com::sun::star::uno::Reference< ::com::sun::star::embed::XHatchWindowController > m_xController;
    ::com::sun::star::awt::Size aHatchBorderSize;
    SvResizeWindow* pHatchWindow;

public:
	VCLXHatchWindow();
	~VCLXHatchWindow();

	void initializeWindow( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowPeer >& xParent,
				const ::com::sun::star::awt::Rectangle& aBounds,
				const ::com::sun::star::awt::Size& aSize );

	void QueryObjAreaPixel( Rectangle & );
	void RequestObjAreaPixel( const Rectangle & );
	void InplaceDeactivate();
    void Activated();
    void Deactivated();

	// XInterface
	::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& rType ) throw(::com::sun::star::uno::RuntimeException);
	void SAL_CALL acquire() throw();
	void SAL_CALL release() throw();

	// XTypeProvider
	::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);

	// XHatchWindow
	virtual void SAL_CALL setController( const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XHatchWindowController >& xController ) throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::awt::Size SAL_CALL getHatchBorderSize() throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL setHatchBorderSize( const ::com::sun::star::awt::Size& _hatchbordersize ) throw (::com::sun::star::uno::RuntimeException);

	// XComponent
    virtual void SAL_CALL dispose() throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL addEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& xListener ) throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL removeEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& aListener ) throw (::com::sun::star::uno::RuntimeException);
};

#endif // _SVT_HATCHWINDOW_HXX
