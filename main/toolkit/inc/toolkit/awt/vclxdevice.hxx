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



#ifndef _TOOLKIT_AWT_VCLXDEVICE_HXX_
#define _TOOLKIT_AWT_VCLXDEVICE_HXX_

#include <toolkit/dllapi.h>
#include <com/sun/star/awt/XDevice.hpp>
#include <com/sun/star/lang/XTypeProvider.hpp>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <cppuhelper/weak.hxx>
#include <vos/mutex.hxx>

// #include <com/sun/star/awt/XTextConstraints.hpp>
#include <com/sun/star/awt/XUnitConversion.hpp>

class OutputDevice;
class VirtualDevice;

//	----------------------------------------------------
//	class VCLXDevice
//	----------------------------------------------------

// For using nDummy, no incompatible update, add a sal_Bool bCreatedWithToolkitMember later...
#define FLAGS_CREATEDWITHTOOLKIT    0x00000001

class TOOLKIT_DLLPUBLIC VCLXDevice :	public ::com::sun::star::awt::XDevice, 
					public ::com::sun::star::lang::XTypeProvider,
					public ::com::sun::star::lang::XUnoTunnel,
/* public ::com::sun::star::awt::XTextConstraints,*/
					public ::com::sun::star::awt::XUnitConversion,
					public ::cppu::OWeakObject
{
	friend class VCLXGraphics;

private:
	vos::IMutex&	mrMutex;	// Reference to SolarMutex
	OutputDevice*			mpOutputDevice;
	
public:
	void*					pDummy;
	sal_uInt32				nFlags;

protected:
	vos::IMutex&	GetMutex() { return mrMutex; }
	void					DestroyOutputDevice();

public:
							VCLXDevice();
							~VCLXDevice();

	void					SetOutputDevice( OutputDevice* pOutDev ) { mpOutputDevice = pOutDev; }
	OutputDevice*			GetOutputDevice() const { return mpOutputDevice; }
	
	void                    SetCreatedWithToolkit( sal_Bool bCreatedWithToolkit );
	sal_Bool                IsCreatedWithToolkit() const;
	
	// ::com::sun::star::uno::XInterface
    ::com::sun::star::uno::Any					SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
	void										SAL_CALL acquire() throw()	{ OWeakObject::acquire(); }
	void										SAL_CALL release() throw()	{ OWeakObject::release(); }

	// ::com::sun::star::lang::XUnoTunnel
	static const ::com::sun::star::uno::Sequence< sal_Int8 >&	GetUnoTunnelId() throw();
	static VCLXDevice*											GetImplementation( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& rxIFace ) throw();
	sal_Int64													SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& rIdentifier ) throw(::com::sun::star::uno::RuntimeException);

	// ::com::sun::star::lang::XTypeProvider
	::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type >	SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Sequence< sal_Int8 >						SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);

	// ::com::sun::star::awt::XDevice, 
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XGraphics >	SAL_CALL createGraphics(  ) throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XDevice >		SAL_CALL createDevice( sal_Int32 nWidth, sal_Int32 nHeight ) throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::awt::DeviceInfo										SAL_CALL getInfo() throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Sequence< ::com::sun::star::awt::FontDescriptor > SAL_CALL getFontDescriptors(  ) throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XFont >		SAL_CALL getFont( const ::com::sun::star::awt::FontDescriptor& aDescriptor ) throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XBitmap >		SAL_CALL createBitmap( sal_Int32 nX, sal_Int32 nY, sal_Int32 nWidth, sal_Int32 nHeight ) throw(::com::sun::star::uno::RuntimeException);
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XDisplayBitmap > SAL_CALL createDisplayBitmap( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XBitmap >& Bitmap ) throw(::com::sun::star::uno::RuntimeException);

    // ::com::sun::star::awt::XTextConstraints
    // ::sal_Int32 SAL_CALL getTextWidth( const ::rtl::OUString& Text ) throw (::com::sun::star::uno::RuntimeException);
    // ::sal_Int32 SAL_CALL getTextHeight(  ) throw (::com::sun::star::uno::RuntimeException);

    // ::com::sun::star::awt::XUnitConversion
    ::com::sun::star::awt::Point SAL_CALL convertPointToLogic( const ::com::sun::star::awt::Point& aPoint, ::sal_Int16 TargetUnit ) throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
    ::com::sun::star::awt::Point SAL_CALL convertPointToPixel( const ::com::sun::star::awt::Point& aPoint, ::sal_Int16 SourceUnit ) throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
    ::com::sun::star::awt::Size SAL_CALL convertSizeToLogic( const ::com::sun::star::awt::Size& aSize, ::sal_Int16 TargetUnit ) throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
    ::com::sun::star::awt::Size SAL_CALL convertSizeToPixel( const ::com::sun::star::awt::Size& aSize, ::sal_Int16 SourceUnit ) throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);


};

//	----------------------------------------------------
//	class VCLXVirtualDevice
//	----------------------------------------------------

class VCLXVirtualDevice : public VCLXDevice
{
private:
	VirtualDevice*	mpVDev;	

public:
					~VCLXVirtualDevice();

	void			SetVirtualDevice( VirtualDevice* pVDev ) { SetOutputDevice( (OutputDevice*)pVDev ); }
};




#endif // _TOOLKIT_AWT_VCLXDEVICE_HXX_
