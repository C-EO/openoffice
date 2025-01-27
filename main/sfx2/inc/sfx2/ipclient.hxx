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


#ifndef _SFX_IPCLIENT_HXX
#define _SFX_IPCLIENT_HXX

#include "sal/config.h"
#include "sfx2/dllapi.h"
#include "sal/types.h"
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/embed/Aspects.hpp>
#include <com/sun/star/embed/XEmbeddedObject.hpp>

#include <tools/ref.hxx>
#include <tools/gen.hxx>

#include <sfx2/objsh.hxx>

class SfxInPlaceClient_Impl;
class SfxViewShell;
class SfxObjectShell;
class Window;
class Fraction;

//=========================================================================

class SFX2_DLLPUBLIC SfxInPlaceClient
{
friend class SfxInPlaceClient_Impl;

	SfxInPlaceClient_Impl*	m_pImp;
	SfxViewShell*			m_pViewSh;
    Window*                 m_pEditWin;

    // called after the requested new object area was negotiated
    SAL_DLLPRIVATE virtual void ObjectAreaChanged();

    // an active object was resized by the user and now asks for the new space
    SAL_DLLPRIVATE virtual void RequestNewObjectArea( Rectangle& );

    // notify the client that an active object has changed its VisualAreaSize
    SAL_DLLPRIVATE virtual void ViewChanged();

    // an object wants to become visible
    SAL_DLLPRIVATE virtual void MakeVisible();

public:
                        SfxInPlaceClient( SfxViewShell* pViewShell, Window* pDraw, sal_Int64 nAspect = com::sun::star::embed::Aspects::MSOLE_CONTENT );
	virtual 			~SfxInPlaceClient();

	SfxViewShell*		GetViewShell() const { return m_pViewSh; }
    Window*             GetEditWin() const { return m_pEditWin; }
    com::sun::star::uno::Reference < com::sun::star::embed::XEmbeddedObject > GetObject() const;
    void                SetObject( const com::sun::star::uno::Reference < com::sun::star::embed::XEmbeddedObject >& rObject );
    void                SetObjectState( sal_Int32 );
    Size                GetObjectVisAreaSize() const;
    sal_Bool            IsObjectUIActive() const;
    sal_Bool            IsObjectInPlaceActive() const;
    sal_Bool            IsObjectActive() const;
    void                DeactivateObject();
    sal_Bool                SetObjArea( const Rectangle & );
    Rectangle           GetObjArea() const;
    Rectangle           GetScaledObjArea() const;
    void                SetSizeScale( const Fraction & rScaleWidth, const Fraction & rScaleHeight );
    sal_Bool            SetObjAreaAndScale( const Rectangle&, const Fraction&, const Fraction& );
    const Fraction&     GetScaleWidth() const;
    const Fraction&     GetScaleHeight() const;
    void                Invalidate();
    static Window*      GetActiveWindow( SfxObjectShell* pDoc, const com::sun::star::uno::Reference < com::sun::star::embed::XEmbeddedObject >& xObject );
    static SfxInPlaceClient* GetClient( SfxObjectShell* pDoc, const com::sun::star::uno::Reference < com::sun::star::embed::XEmbeddedObject >& xObject );
    sal_Int64           GetAspect() const;
	sal_Int64			GetObjectMiscStatus() const;
    ErrCode             DoVerb( long nVerb );
    void                VisAreaChanged();
    void                ResetObject();
    sal_Bool                IsUIActive();

    // used in Writer
    // Rectangle           PixelObjVisAreaToLogic( const Rectangle & rObjRect ) const;
    // Rectangle           LogicObjAreaToPixel( const Rectangle & rRect ) const;
    virtual void        FormatChanged(); // object format was changed (used for StarMath formulas aligning)
};

#endif
