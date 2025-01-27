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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_framework.hxx"

//_________________________________________________________________________________________________________________
//	my own includes
//_________________________________________________________________________________________________________________
#include <pattern/window.hxx>
#include <helper/persistentwindowstate.hxx>
#include <threadhelp/writeguard.hxx>
#include <threadhelp/readguard.hxx>
#include <macros/generic.hxx>
#include <services.h>

//_________________________________________________________________________________________________________________
//	interface includes
//_________________________________________________________________________________________________________________
#include <com/sun/star/awt/XWindow.hpp>

#ifndef _COM_SUN_STAR_LANG_XSERVICXEINFO_HPP_
#include <com/sun/star/lang/XServiceInfo.hpp>
#endif
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <com/sun/star/frame/XModuleManager.hpp>

//_________________________________________________________________________________________________________________
//	other includes
//_________________________________________________________________________________________________________________
#include <comphelper/configurationhelper.hxx>
#include <vcl/window.hxx>
#include <vcl/syswin.hxx>

#ifndef _TOOLKIT_HELPER_VCLUNOHELPER_HXX_
#include <toolkit/helper/vclunohelper.hxx>
#endif
#include <vcl/svapp.hxx>
#include <vcl/wrkwin.hxx>
#include <rtl/string.hxx>

//_________________________________________________________________________________________________________________
//	namespace

namespace framework{

//_________________________________________________________________________________________________________________
//	definitions

//*****************************************************************************************************************
//  XInterface, XTypeProvider

DEFINE_XINTERFACE_4(PersistentWindowState                                                       ,
                    OWeakObject                                                                 ,
                    DIRECT_INTERFACE (css::lang::XTypeProvider                                  ),
                    DIRECT_INTERFACE (css::lang::XInitialization                                ),
                    DIRECT_INTERFACE (css::frame::XFrameActionListener                          ),
                    DERIVED_INTERFACE(css::lang::XEventListener,css::frame::XFrameActionListener))

DEFINE_XTYPEPROVIDER_4(PersistentWindowState           ,
                       css::lang::XTypeProvider        ,
                       css::lang::XInitialization      ,
                       css::frame::XFrameActionListener,
                       css::lang::XEventListener       )

//*****************************************************************************************************************
PersistentWindowState::PersistentWindowState(const css::uno::Reference< css::lang::XMultiServiceFactory >& xSMGR)
    : ThreadHelpBase          (&Application::GetSolarMutex())
    , m_xSMGR                 (xSMGR                        )
    , m_bWindowStateAlreadySet(sal_False                    )
{
}

//*****************************************************************************************************************
PersistentWindowState::~PersistentWindowState()
{
}

//*****************************************************************************************************************
void SAL_CALL PersistentWindowState::initialize(const css::uno::Sequence< css::uno::Any >& lArguments)
    throw(css::uno::Exception       ,
          css::uno::RuntimeException)
{
    // check arguments
    css::uno::Reference< css::frame::XFrame > xFrame;
    if (lArguments.getLength() < 1)
        throw css::lang::IllegalArgumentException(
                DECLARE_ASCII("Empty argument list!"),
                static_cast< ::cppu::OWeakObject* >(this),
                1);
                
    lArguments[0] >>= xFrame;
    if (!xFrame.is())
        throw css::lang::IllegalArgumentException(
                DECLARE_ASCII("No valid frame specified!"),
                static_cast< ::cppu::OWeakObject* >(this),
                1);
                
    // SAFE -> ----------------------------------
    WriteGuard aWriteLock(m_aLock);
    // hold the frame as weak reference(!) so it can die everytimes :-)
    m_xFrame = xFrame;
    aWriteLock.unlock();
    // <- SAFE ----------------------------------
    
    // start listening
    xFrame->addFrameActionListener(this);
}

//*****************************************************************************************************************
void SAL_CALL PersistentWindowState::frameAction(const css::frame::FrameActionEvent& aEvent)
    throw(css::uno::RuntimeException)
{
    // SAFE -> ----------------------------------
    ReadGuard aReadLock(m_aLock);
    css::uno::Reference< css::lang::XMultiServiceFactory > xSMGR = m_xSMGR ;
    css::uno::Reference< css::frame::XFrame >              xFrame(m_xFrame.get(), css::uno::UNO_QUERY);
    sal_Bool                                               bRestoreWindowState = !m_bWindowStateAlreadySet;
    aReadLock.unlock();
    // <- SAFE ----------------------------------

    // frame already gone ? We hold it weak only ...
    if (!xFrame.is())
        return;
    
    // no window -> no position and size available
    css::uno::Reference< css::awt::XWindow > xWindow = xFrame->getContainerWindow();
    if (!xWindow.is())
        return;
    
    // unknown module -> no configuration available!
    ::rtl::OUString sModuleName = PersistentWindowState::implst_identifyModule(xSMGR, xFrame);
    if (!sModuleName.getLength())
        return;

    switch(aEvent.Action)
    {
        case css::frame::FrameAction_COMPONENT_ATTACHED :
            {
                if (bRestoreWindowState)
                {
                    ::rtl::OUString sWindowState = PersistentWindowState::implst_getWindowStateFromConfig(xSMGR, sModuleName);
                    PersistentWindowState::implst_setWindowStateOnWindow(xWindow,sWindowState);
                    // SAFE -> ----------------------------------
                    WriteGuard aWriteLock(m_aLock);
                    m_bWindowStateAlreadySet = sal_True;
                    aWriteLock.unlock();
                    // <- SAFE ----------------------------------
                }
            }
            break;
            
        case css::frame::FrameAction_COMPONENT_REATTACHED :
            {
                // nothing todo here, because its not allowed to change position and size
                // of an already existing frame!
            }
            break;
            
        case css::frame::FrameAction_COMPONENT_DETACHING :
            {
                ::rtl::OUString sWindowState = PersistentWindowState::implst_getWindowStateFromWindow(xWindow);
                PersistentWindowState::implst_setWindowStateOnConfig(xSMGR, sModuleName, sWindowState);
            }
            break;
        default:
            break;    
    }
}

//*****************************************************************************************************************
void SAL_CALL PersistentWindowState::disposing(const css::lang::EventObject&)
    throw(css::uno::RuntimeException)
{
    // nothing todo here - because we hold the frame as weak reference only
}

//*****************************************************************************************************************
::rtl::OUString PersistentWindowState::implst_identifyModule(const css::uno::Reference< css::lang::XMultiServiceFactory >& xSMGR ,
                                                             const css::uno::Reference< css::frame::XFrame >&              xFrame)
{
    ::rtl::OUString sModuleName;
    
    css::uno::Reference< css::frame::XModuleManager > xModuleManager(
        xSMGR->createInstance(SERVICENAME_MODULEMANAGER),
        css::uno::UNO_QUERY_THROW);
    
    try
    {
        sModuleName = xModuleManager->identify(xFrame);
    }
    catch(const css::uno::RuntimeException& exRun)
        { throw exRun; }
    catch(const css::uno::Exception&)
        { sModuleName = ::rtl::OUString(); }
    
    return sModuleName;
}
                                                     
//*****************************************************************************************************************
::rtl::OUString PersistentWindowState::implst_getWindowStateFromConfig(const css::uno::Reference< css::lang::XMultiServiceFactory >& xSMGR      ,
                                                                       const ::rtl::OUString&                                        sModuleName)
{
    ::rtl::OUString sWindowState;
    
    ::rtl::OUStringBuffer sRelPathBuf(256);
    sRelPathBuf.appendAscii("Office/Factories/*[\"");
    sRelPathBuf.append     (sModuleName            );
    sRelPathBuf.appendAscii("\"]"                  );
    
    ::rtl::OUString sPackage = ::rtl::OUString::createFromAscii("org.openoffice.Setup/");
    ::rtl::OUString sRelPath = sRelPathBuf.makeStringAndClear();
    ::rtl::OUString sKey     = ::rtl::OUString::createFromAscii("ooSetupFactoryWindowAttributes");
    
    try
    {
        ::comphelper::ConfigurationHelper::readDirectKey(xSMGR,
                                                                                      sPackage,
                                                                                      sRelPath,
                                                                                      sKey,
                                                                                      ::comphelper::ConfigurationHelper::E_READONLY) >>= sWindowState;
    }
    catch(const css::uno::RuntimeException& exRun)
        { throw exRun; }
    catch(const css::uno::Exception&)
        { sWindowState = ::rtl::OUString(); }
                                                                                  
    return sWindowState;        
}

//*****************************************************************************************************************
void PersistentWindowState::implst_setWindowStateOnConfig(const css::uno::Reference< css::lang::XMultiServiceFactory >& xSMGR       ,
                                                          const ::rtl::OUString&                                        sModuleName ,
                                                          const ::rtl::OUString&                                        sWindowState)
{
    ::rtl::OUStringBuffer sRelPathBuf(256);
    sRelPathBuf.appendAscii("Office/Factories/*[\"");
    sRelPathBuf.append     (sModuleName            );
    sRelPathBuf.appendAscii("\"]"                  );
    
    ::rtl::OUString sPackage = ::rtl::OUString::createFromAscii("org.openoffice.Setup/");
    ::rtl::OUString sRelPath = sRelPathBuf.makeStringAndClear();
    ::rtl::OUString sKey     = ::rtl::OUString::createFromAscii("ooSetupFactoryWindowAttributes");
    
    try
    {
        ::comphelper::ConfigurationHelper::writeDirectKey(xSMGR,
                                                          sPackage,
                                                          sRelPath,
                                                          sKey,
                                                          css::uno::makeAny(sWindowState),
                                                          ::comphelper::ConfigurationHelper::E_STANDARD);
    }
    catch(const css::uno::RuntimeException& exRun)
        { throw exRun; }
    catch(const css::uno::Exception&)
        {}
}

//*****************************************************************************************************************
::rtl::OUString PersistentWindowState::implst_getWindowStateFromWindow(const css::uno::Reference< css::awt::XWindow >& xWindow)
{
    ::rtl::OUString sWindowState;
    
    if (xWindow.is())
    {
        // SOLAR SAFE -> ------------------------
        ::vos::OClearableGuard aSolarLock(Application::GetSolarMutex());
        
        Window* pWindow = VCLUnoHelper::GetWindow(xWindow);
        // check for system window is necessary to guarantee correct pointer cast!
        if (
            (pWindow                  ) &&
            (pWindow->IsSystemWindow())
           )
        {
            sal_uLong nMask  =   WINDOWSTATE_MASK_ALL;
                  nMask &= ~(WINDOWSTATE_MASK_MINIMIZED);
            sWindowState = B2U_ENC(
                            ((SystemWindow*)pWindow)->GetWindowState(nMask),
                            RTL_TEXTENCODING_UTF8);
        }
        
        aSolarLock.clear();
        // <- SOLAR SAFE ------------------------
    }
    
    return sWindowState;
}
                                                               

//*********************************************************************************************************
void PersistentWindowState::implst_setWindowStateOnWindow(const css::uno::Reference< css::awt::XWindow >& xWindow     ,
                                                          const ::rtl::OUString&                          sWindowState)
{
    if (
        (!xWindow.is()                ) ||
        ( sWindowState.getLength() < 1)
       )
        return;

    // SOLAR SAFE -> ------------------------
    ::vos::OClearableGuard aSolarLock(Application::GetSolarMutex());
        
    Window* pWindow = VCLUnoHelper::GetWindow(xWindow);
    if (!pWindow)
        return;
    
    // check for system and work window - its necessary to guarantee correct pointer cast!
    sal_Bool bSystemWindow = pWindow->IsSystemWindow(); 
    sal_Bool bWorkWindow   = (pWindow->GetType() == WINDOW_WORKWINDOW); 

    if (!bSystemWindow && !bWorkWindow)
        return;

    SystemWindow* pSystemWindow = (SystemWindow*)pWindow;
    WorkWindow*   pWorkWindow   = (WorkWindow*  )pWindow;
    
    // dont save this special state!
    if (pWorkWindow->IsMinimized())
        return;
    
    ::rtl::OUString sOldWindowState = ::rtl::OStringToOUString( pSystemWindow->GetWindowState(), RTL_TEXTENCODING_ASCII_US );
    if ( sOldWindowState != sWindowState )
        pSystemWindow->SetWindowState(U2B_ENC(sWindowState,RTL_TEXTENCODING_UTF8));    
    
    aSolarLock.clear();
    // <- SOLAR SAFE ------------------------
}

} // namespace framework
