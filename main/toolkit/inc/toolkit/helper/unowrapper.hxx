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



#ifndef _TOOLKIT_HELPER_UNOWRAPPER_HXX_
#define _TOOLKIT_HELPER_UNOWRAPPER_HXX_

#include <com/sun/star/awt/XToolkit.hpp>
#include <com/sun/star/awt/XGraphics.hpp>
#include <com/sun/star/awt/XWindowPeer.hpp>
#include <com/sun/star/accessibility/XAccessible.hpp>
#include <toolkit/helper/accessibilityclient.hxx>

#include <vcl/unowrap.hxx>
#include <vcl/window.hxx>

//	----------------------------------------------------
//	class UnoWrapper
//	----------------------------------------------------

class UnoWrapper : public UnoWrapperBase
{
private:
	::com::sun::star::uno::Reference< ::com::sun::star::awt::XToolkit>  mxToolkit;
    ::toolkit::AccessibilityClient                                      maAccessibleFactoryAccess;

public:	
	UnoWrapper( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XToolkit>& rxToolkit );
	
    virtual void        Destroy();
	
	// Toolkit
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::awt::XToolkit> GetVCLToolkit();

	// Graphics
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::awt::XGraphics>	CreateGraphics( OutputDevice* pOutDev );
	virtual void				ReleaseAllGraphics( OutputDevice* pOutDev );
	
	// Window
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowPeer> GetWindowInterface( Window* pWindow, sal_Bool bCreate );
	virtual void				SetWindowInterface( Window* pWindow, ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowPeer> xIFace );

	void				WindowDestroyed( Window* pWindow );
	void 				WindowEvent_Move( Window* pWindow );
	void 				WindowEvent_Resize( Window* pWindow );
	void 				WindowEvent_Show( Window* pWindow, sal_Bool bShow );
	void 				WindowEvent_Close( Window* pWindow );
	void 				WindowEvent_Minimize( Window* pWindow );
	void 				WindowEvent_Normalize( Window* pWindow );
	void 				WindowEvent_Activate( Window* pWindow, sal_Bool bActivated );
	void 				WindowEvent_MouseButtonUp( Window* pWindow, const MouseEvent& rEvt );
	void 				WindowEvent_MouseButtonDown( Window* pWindow, const MouseEvent& rEvt );
	void 				WindowEvent_MouseMove( Window* pWindow, const MouseEvent& rEvt );
	void 				WindowEvent_Command( Window* pWindow, const CommandEvent& rCEvt );
	void 				WindowEvent_KeyInput( Window* pWindow, const KeyEvent& rEvt );
	void 				WindowEvent_KeyUp( Window* pWindow, const KeyEvent& rEvt );
	void 				WindowEvent_GetFocus( Window* pWindow );
	void 				WindowEvent_LoseFocus( Window* pWindow );
	void 				WindowEvent_Paint( Window* pWindow, const Rectangle& rRect );

	// Accessibility
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible >
						CreateAccessible( Menu* pMenu, sal_Bool bIsMenuBar );

private:
    virtual ~UnoWrapper();
};

#endif // _TOOLKIT_HELPER_UNOWRAPPER_HXX_
