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
#include "precompiled_vcl.hxx"

#include "vcl/outdev.hxx"
#include "vcl/window.hxx"
#include "vcl/salnativewidgets.hxx"
#include "vcl/pdfextoutdevdata.hxx"

#include "salgdi.hxx"

// -----------------------------------------------------------------------

static bool lcl_enableNativeWidget( const OutputDevice& i_rDevice )
{
    const OutDevType eType( i_rDevice.GetOutDevType() );
    switch ( eType )
    {

    case OUTDEV_WINDOW:
        return dynamic_cast< const Window* >( &i_rDevice )->IsNativeWidgetEnabled();

    case OUTDEV_VIRDEV:
    {
        const ::vcl::ExtOutDevData* pOutDevData( i_rDevice.GetExtOutDevData() );
        const ::vcl::PDFExtOutDevData* pPDFData( dynamic_cast< const ::vcl::PDFExtOutDevData* >( pOutDevData ) );
        if ( pPDFData != NULL )
            return false;
        return true;
    }

    default:
        return false;
    }
}

ImplControlValue::~ImplControlValue()
{
}

ScrollbarValue::~ScrollbarValue()
{
}

SliderValue::~SliderValue()
{
}

TabitemValue::~TabitemValue()
{
}

SpinbuttonValue::~SpinbuttonValue()
{
}

ToolbarValue::~ToolbarValue()
{
}

MenubarValue::~MenubarValue()
{
}

MenupopupValue::~MenupopupValue()
{
}

PushButtonValue::~PushButtonValue()
{
}

// -----------------------------------------------------------------------
// These functions are mainly passthrough functions that allow access to
// the SalFrame behind a Window object for native widget rendering purposes.
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------

sal_Bool OutputDevice::IsNativeControlSupported( ControlType nType, ControlPart nPart )
{
    if( !lcl_enableNativeWidget( *this ) )
        return sal_False;

    if ( !mpGraphics )
        if ( !ImplGetGraphics() )
            return sal_False;

    return( mpGraphics->IsNativeControlSupported(nType, nPart) );
}


// -----------------------------------------------------------------------

sal_Bool OutputDevice::HitTestNativeControl( ControlType nType,
                              ControlPart nPart,
                              const Rectangle& rControlRegion,
                              const Point& aPos,
                              sal_Bool& rIsInside )
{
    if( !lcl_enableNativeWidget( *this ) )
        return sal_False;

    if ( !mpGraphics )
        if ( !ImplGetGraphics() )
            return sal_False;

    Point aWinOffs( mnOutOffX, mnOutOffY );
    Rectangle screenRegion( rControlRegion );
    screenRegion.Move( aWinOffs.X(), aWinOffs.Y());

    return( mpGraphics->HitTestNativeControl(nType, nPart, screenRegion, Point( aPos.X() + mnOutOffX, aPos.Y() + mnOutOffY ),
        rIsInside, this ) );
}

// -----------------------------------------------------------------------

static boost::shared_ptr< ImplControlValue > lcl_transformControlValue( const ImplControlValue& rVal, OutputDevice& rDev )
{
    boost::shared_ptr< ImplControlValue > aResult;
    switch( rVal.getType() )
    {
    case CTRL_SLIDER:
        {
            const SliderValue* pSlVal = static_cast<const SliderValue*>(&rVal);
            SliderValue* pNew = new SliderValue( *pSlVal );
            aResult.reset( pNew );
            pNew->maThumbRect = rDev.ImplLogicToDevicePixel( pSlVal->maThumbRect );
        }
        break;
    case CTRL_SCROLLBAR:
        {
            const ScrollbarValue* pScVal = static_cast<const ScrollbarValue*>(&rVal);
            ScrollbarValue* pNew = new ScrollbarValue( *pScVal );
            aResult.reset( pNew );
            pNew->maThumbRect = rDev.ImplLogicToDevicePixel( pScVal->maThumbRect );
            pNew->maButton1Rect = rDev.ImplLogicToDevicePixel( pScVal->maButton1Rect );
            pNew->maButton2Rect = rDev.ImplLogicToDevicePixel( pScVal->maButton2Rect );
        }
        break;
    case CTRL_SPINBUTTONS:
        {
            const SpinbuttonValue* pSpVal = static_cast<const SpinbuttonValue*>(&rVal);
            SpinbuttonValue* pNew = new SpinbuttonValue( *pSpVal );
            aResult.reset( pNew );
            pNew->maUpperRect = rDev.ImplLogicToDevicePixel( pSpVal->maUpperRect );
            pNew->maLowerRect = rDev.ImplLogicToDevicePixel( pSpVal->maLowerRect );
        }
        break;
    case CTRL_TOOLBAR:
        {
            const ToolbarValue* pTVal = static_cast<const ToolbarValue*>(&rVal);
            ToolbarValue* pNew = new ToolbarValue( *pTVal );
            aResult.reset( pNew );
            pNew->maGripRect = rDev.ImplLogicToDevicePixel( pTVal->maGripRect );
        }
        break;
    case CTRL_TAB_ITEM:
        {
            const TabitemValue* pTIVal = static_cast<const TabitemValue*>(&rVal);
            TabitemValue* pNew = new TabitemValue( *pTIVal );
            aResult.reset( pNew );
        }
        break;
    case CTRL_MENUBAR:
        {
            const MenubarValue* pMVal = static_cast<const MenubarValue*>(&rVal);
            MenubarValue* pNew = new MenubarValue( *pMVal );
            aResult.reset( pNew );
        }
        break;
    case CTRL_PUSHBUTTON:
        {
            const PushButtonValue* pBVal = static_cast<const PushButtonValue*>(&rVal);
            PushButtonValue* pNew = new PushButtonValue( *pBVal );
            aResult.reset( pNew );
        }
        break;
    case CTRL_GENERIC:
            aResult.reset( new ImplControlValue( rVal ) );
            break;
	case CTRL_MENU_POPUP:
		{
			const MenupopupValue* pMVal = static_cast<const MenupopupValue*>(&rVal);
			MenupopupValue* pNew = new MenupopupValue( *pMVal );
            pNew->maItemRect = rDev.ImplLogicToDevicePixel( pMVal->maItemRect );
			aResult.reset( pNew );
		}
		break;
    default:
        OSL_ENSURE( 0, "unknown ImplControlValue type !" );
        break;
    }
    return aResult;
}

sal_Bool OutputDevice::DrawNativeControl( ControlType nType,
                            ControlPart nPart,
                            const Rectangle& rControlRegion,
                            ControlState nState,
                            const ImplControlValue& aValue,
                            ::rtl::OUString aCaption )
{
    if( !lcl_enableNativeWidget( *this ) )
        return sal_False;

    // make sure the current clip region is initialized correctly
    if ( !mpGraphics )
        if ( !ImplGetGraphics() )
            return sal_False;

    if ( mbInitClipRegion )
        ImplInitClipRegion();
    if ( mbOutputClipped )
        return sal_True;

    if ( mbInitLineColor )
        ImplInitLineColor();
    if ( mbInitFillColor )
        ImplInitFillColor();

    // Convert the coordinates from relative to Window-absolute, so we draw
    // in the correct place in platform code
    boost::shared_ptr< ImplControlValue > aScreenCtrlValue( lcl_transformControlValue( aValue, *this ) );
    Rectangle screenRegion( ImplLogicToDevicePixel( rControlRegion ) );

    Region aTestRegion( GetActiveClipRegion() );
    aTestRegion.Intersect( rControlRegion );
    if( aTestRegion == rControlRegion )
        nState |= CTRL_CACHING_ALLOWED;   // control is not clipped, caching allowed
    
    sal_Bool bRet = mpGraphics->DrawNativeControl(nType, nPart, screenRegion, nState, *aScreenCtrlValue, aCaption, this );

    return bRet;
}


// -----------------------------------------------------------------------

sal_Bool OutputDevice::DrawNativeControlText(ControlType nType,
                            ControlPart nPart,
                            const Rectangle& rControlRegion,
                            ControlState nState,
                            const ImplControlValue& aValue,
                            ::rtl::OUString aCaption )
{
    if( !lcl_enableNativeWidget( *this ) )
        return sal_False;

    // make sure the current clip region is initialized correctly
    if ( !mpGraphics )
        if ( !ImplGetGraphics() )
            return false;

    if ( mbInitClipRegion )
        ImplInitClipRegion();
    if ( mbOutputClipped )
        return true;

    if ( mbInitLineColor )
        ImplInitLineColor();
    if ( mbInitFillColor )
        ImplInitFillColor();

    // Convert the coordinates from relative to Window-absolute, so we draw
    // in the correct place in platform code
    boost::shared_ptr< ImplControlValue > aScreenCtrlValue( lcl_transformControlValue( aValue, *this ) );
    Rectangle screenRegion( ImplLogicToDevicePixel( rControlRegion ) );

    sal_Bool bRet = mpGraphics->DrawNativeControlText(nType, nPart, screenRegion, nState, *aScreenCtrlValue, aCaption, this );
    
    return bRet;
}


// -----------------------------------------------------------------------

sal_Bool OutputDevice::GetNativeControlRegion(  ControlType nType,
                                ControlPart nPart,
                                const Rectangle& rControlRegion,
                                ControlState nState,
                                const ImplControlValue& aValue,
                                ::rtl::OUString aCaption,
                                Rectangle &rNativeBoundingRegion,
                                Rectangle &rNativeContentRegion )
{
    if( !lcl_enableNativeWidget( *this ) )
        return sal_False;

    if ( !mpGraphics )
        if ( !ImplGetGraphics() )
            return sal_False;

    // Convert the coordinates from relative to Window-absolute, so we draw
    // in the correct place in platform code
    boost::shared_ptr< ImplControlValue > aScreenCtrlValue( lcl_transformControlValue( aValue, *this ) );
    Rectangle screenRegion( ImplLogicToDevicePixel( rControlRegion ) );

    sal_Bool bRet = mpGraphics->GetNativeControlRegion(nType, nPart, screenRegion, nState, *aScreenCtrlValue,
                                aCaption, rNativeBoundingRegion,
                                rNativeContentRegion, this );
    if( bRet )
    {
        // transform back native regions    
        rNativeBoundingRegion = ImplDevicePixelToLogic( rNativeBoundingRegion );
        rNativeContentRegion = ImplDevicePixelToLogic( rNativeContentRegion );
    }

    return bRet;
}


