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


#ifndef _SFXDISPATCH_HXX
#define _SFXDISPATCH_HXX

#include "sal/config.h"
#include "sfx2/dllapi.h"
#include "sal/types.h"

#include <stdarg.h>

#define _SVSTDARR_USHORTS
#include <svl/svstdarr.hxx>		// SvUShorts

#include <sfx2/bindings.hxx>
#include <sfx2/viewfrm.hxx>

class SfxSlotServer;
class SfxShell;
class SfxRequest;
class SfxShellStack_Impl;
class SfxHintPoster;
class SfxViewFrame;
class SfxBindings;
class SfxItemSet;
class SfxPopupMenuManager;
class SfxModule;
struct SfxDispatcher_Impl;
struct SfxPlugInInfo_Impl;

namespace com
{
    namespace sun
    {
        namespace star
        {
            namespace frame
            {
                class XDispatch;
            }
        }
    }
}

//=========================================================================

#define SFX_SHELL_POP_UNTIL 	4
#define SFX_SHELL_POP_DELETE    2
#define SFX_SHELL_PUSH			1

//=========================================================================

typedef SfxPoolItem* SfxPoolItemPtr;
SV_DECL_PTRARR_DEL( SfxItemPtrArray, SfxPoolItemPtr, 4, 4 )

// fuer  shell.cxx
typedef SfxItemPtrArray SfxItemArray_Impl;

class SfxExecuteItem : public SfxItemPtrArray, public SfxPoolItem
{
	sal_uInt16 nSlot;
	SfxCallMode eCall;
	sal_uInt16 nModifier;
public:
	sal_uInt16                   GetSlot() const { return nSlot; }
	sal_uInt16                   GetModifier() const { return nModifier; }
	void                     SetModifier( sal_uInt16 nModifierP ) { nModifier = nModifierP; }
	SfxCallMode              GetCallMode() const { return eCall; }
	void                     SetCallMode( SfxCallMode eMode ) { eCall = eMode; }
	virtual int 			 operator==( const SfxPoolItem& ) const;

	virtual SfxPoolItem*	 Clone( SfxItemPool *pPool = 0 ) const;
							 SfxExecuteItem(
								 sal_uInt16 nWhich, sal_uInt16 nSlot, SfxCallMode eMode,
								 const SfxPoolItem *pArg1, ... );
							 SfxExecuteItem(
								 sal_uInt16 nWhich, sal_uInt16 nSlot, SfxCallMode eMode );
							 SfxExecuteItem( const SfxExecuteItem& );
};

//=========================================================================

class SFX2_DLLPUBLIC SfxDispatcher
{
	SfxDispatcher_Impl* 			pImp;
	sal_Bool							bFlushed;

private:
	// auf temporaer ausgewerteten Todos suchen
	SAL_DLLPRIVATE sal_Bool CheckVirtualStack( const SfxShell& rShell, sal_Bool bDeep );

#ifndef _SFX_HXX

friend class SfxApplication;
friend class SfxViewFrame;

	DECL_DLLPRIVATE_LINK( EventHdl_Impl, void * );
	DECL_DLLPRIVATE_LINK( PostMsgHandler, SfxRequest * );

	SAL_DLLPRIVATE int Call_Impl( SfxShell& rShell, const SfxSlot &rSlot, SfxRequest &rReq, sal_Bool bRecord );
	SAL_DLLPRIVATE void _Update_Impl( sal_Bool,sal_Bool,sal_Bool,SfxWorkWindow*);
	SAL_DLLPRIVATE void CollectTools_Impl(SfxWorkWindow*);

protected:
friend class SfxBindings;
friend class SfxStateCache;
friend class SfxPopupMenuManager;
friend class SfxHelp;
						// Fuer die Bindings: Finden einer Message; Level fuer
						// erneuten Zugriff
	SAL_DLLPRIVATE sal_Bool _TryIntercept_Impl( sal_uInt16 nId, SfxSlotServer &rServer, sal_Bool bModal );
	sal_Bool				_FindServer( sal_uInt16 nId, SfxSlotServer &rServer, sal_Bool bModal );
	sal_Bool				_FillState( const SfxSlotServer &rServer,
									SfxItemSet &rState, const SfxSlot *pRealSlot );
	const SfxPoolItem*	_Execute( const SfxSlotServer &rServer );
	void				_Execute( SfxShell &rShell, const SfxSlot &rSlot,
								  SfxRequest &rReq,
								  SfxCallMode eCall = SFX_CALLMODE_STANDARD);
	const SfxPoolItem*	_Execute( sal_uInt16 nSlot, SfxCallMode eCall,
								  va_list pArgs, const SfxPoolItem *pArg1 );

#endif
protected:
	void FlushImpl();

public:
						SfxDispatcher( SfxDispatcher* pParent );
						SfxDispatcher( SfxViewFrame *pFrame = 0 );

	SAL_DLLPRIVATE void Construct_Impl( SfxDispatcher* pParent );

	virtual				~SfxDispatcher();

	const SfxPoolItem*  Execute( const SfxExecuteItem& rItem );
	virtual	sal_uInt16		ExecuteFunction( sal_uInt16 nSID, SfxPoolItem** ppArgs=0, sal_uInt16 nMode=0 );
    sal_uInt16              ExecuteFunction( sal_uInt16 nSID, const SfxItemSet& rArgs , sal_uInt16 nMode=0 );

	virtual void		SetExecuteMode( sal_uInt16 );

	const SfxPoolItem*	Execute( sal_uInt16 nSlot,
								 SfxCallMode nCall = SFX_CALLMODE_SLOT,
								 const SfxPoolItem **pArgs = 0,
								 sal_uInt16 nModi = 0,
								 const SfxPoolItem **pInternalArgs = 0);

	const SfxPoolItem*	Execute( sal_uInt16 nSlot,
								 SfxCallMode nCall,
								 SfxItemSet* pArgs,
								 SfxItemSet* pInternalArgs,
								 sal_uInt16 nModi = 0);

	const SfxPoolItem*	Execute( sal_uInt16 nSlot,
								 SfxCallMode nCall,
								 const SfxPoolItem *pArg1, ... );

	const SfxPoolItem*	Execute( sal_uInt16 nSlot,
								 SfxCallMode nCall,
								 const SfxItemSet &rArgs );

    const SfxPoolItem*  Execute( sal_uInt16 nSlot,
                                 SfxCallMode nCall,
                                 sal_uInt16 nModi,
                                 const SfxItemSet &rArgs );

	sal_uInt16				GetSlotId( const String& rCommand );
    const SfxSlot*      GetSlot( const String& rCommand );

	sal_Bool				IsActive( const SfxShell& rShell );
	sal_Bool				IsOnTop( const SfxShell& rShell );
	sal_uInt16				GetShellLevel( const SfxShell &rShell );
	SfxBindings*		GetBindings() const;

	void				Push( SfxShell& rShell );
	void				Pop( SfxShell& rShell, sal_uInt16 nMode = 0 );

	SfxShell*			GetShell(sal_uInt16 nIdx) const;
	SfxViewFrame*       GetFrame() const;
	SfxModule*		GetModule() const;
	// caller has to clean up the Manager on his own
	static SfxPopupMenuManager* Popup( sal_uInt16 nConfigId,Window *pWin, const Point *pPos );

	void				ExecutePopup( const ResId &rId,
							  Window *pWin = 0, const Point *pPosPixel = 0 );
	static void 		ExecutePopup( sal_uInt16 nConfigId = 0,
							  Window *pWin = 0, const Point *pPosPixel = 0 );
	static void 		ExecutePopup( sal_uInt16 nConfigId,
									  Window *pWin, const Point *pPosPixel,
									  const SfxPoolItem *pArg1, ...  );

	sal_Bool			IsAppDispatcher() const;
	sal_Bool			IsFlushed() const;
	void				Flush();
	void				Lock( sal_Bool bLock );
	sal_Bool				IsLocked( sal_uInt16 nSID = 0 ) const;
	void				SetSlotFilter( sal_Bool bEnable = sal_False,
									   sal_uInt16 nCount = 0, const sal_uInt16 *pSIDs = 0 );

	void				HideUI( sal_Bool bHide = sal_True );
	void				ShowObjectBar(sal_uInt16 nId, SfxShell *pShell=0) const;
	sal_uInt32			GetObjectBarId( sal_uInt16 nPos ) const;

	SfxItemState		QueryState( sal_uInt16 nSID, const SfxPoolItem* &rpState );
    SfxItemState        QueryState( sal_uInt16 nSID, ::com::sun::star::uno::Any& rAny );

	sal_Bool                IsAllowed( sal_uInt16 nSlot ) const;
    ::com::sun::star::frame::XDispatch*          GetDispatchInterface( const String& );
	void				SetDisableFlags( sal_uInt32 nFlags );
	sal_uInt32				GetDisableFlags() const;

//#if 0 // _SOLAR__PRIVATE
	SAL_DLLPRIVATE sal_Bool HasSlot_Impl( sal_uInt16 );
	SAL_DLLPRIVATE void SetMenu_Impl();
	SAL_DLLPRIVATE void Update_Impl( sal_Bool bForce = sal_False ); // ObjectBars etc.
	SAL_DLLPRIVATE sal_Bool IsUpdated_Impl() const;
	SAL_DLLPRIVATE void DebugOutput_Impl() const;
	SAL_DLLPRIVATE void ResetObjectBars_Impl();
	SAL_DLLPRIVATE int GetShellAndSlot_Impl( sal_uInt16 nSlot, SfxShell **ppShell, const SfxSlot **ppSlot,
											  sal_Bool bOwnShellsOnly, sal_Bool bModal, sal_Bool bRealSlot=sal_True );
	SAL_DLLPRIVATE void LockUI_Impl( sal_Bool bLock = sal_True );
	SAL_DLLPRIVATE void SetReadOnly_Impl( sal_Bool  bOn );
	SAL_DLLPRIVATE sal_Bool GetReadOnly_Impl() const;
	SAL_DLLPRIVATE sal_Bool IsSlotEnabledByFilter_Impl( sal_uInt16 nSID ) const;
	SAL_DLLPRIVATE void SetQuietMode_Impl( sal_Bool bOn );
	SAL_DLLPRIVATE void SetModalMode_Impl( sal_Bool bOn );
	SAL_DLLPRIVATE sal_Bool IsReadOnlyShell_Impl( sal_uInt16 nShell ) const;
	SAL_DLLPRIVATE void RemoveShell_Impl( SfxShell& rShell );
	SAL_DLLPRIVATE void InsertShell_Impl( SfxShell& rShell, sal_uInt16 nPos );
	SAL_DLLPRIVATE void DoParentActivate_Impl();
	SAL_DLLPRIVATE void DoParentDeactivate_Impl();
    SAL_DLLPRIVATE void DoActivate_Impl( sal_Bool bMDI, SfxViewFrame* pOld );
    SAL_DLLPRIVATE void DoDeactivate_Impl( sal_Bool bMDI, SfxViewFrame* pNew );
	SAL_DLLPRIVATE void InvalidateBindings_Impl(sal_Bool);
	SAL_DLLPRIVATE sal_uInt16 GetNextToolBox_Impl( sal_uInt16 nPos, sal_uInt16 nType, String *pStr );
//#endif
};

//--------------------------------------------------------------------

inline sal_Bool SfxDispatcher::IsFlushed() const

/*	[Beschreibung]

	Mit dieser Methode l"a"st sich erfragen, ob der Stack des
	SfxDispatchers geflusht ist, oder noch Push- oder Pop-Befehle
	ausstehen.
*/

{
	 return bFlushed;
}

//--------------------------------------------------------------------

inline void SfxDispatcher::Flush()

/*  [Beschreibung]

	Diese Methode f"uhrt ausstehenden Push- und Pop-Befehle aus.
	F"ur <SfxShell>s, die dabei neu auf den Stack kommen, wird
	<SfxShell::Activate(sal_Bool)> mit bMDI == sal_True aufgerufen, f"ur
	SfxShells, die vom Stack entfernt werden, wird <SfxShell::Deactivate(sal_Bool)>
	mit bMDI == sal_True aufgerufen.
*/

{
	if ( !bFlushed ) FlushImpl();
}

//--------------------------------------------------------------------

inline void SfxDispatcher::Push( SfxShell& rShell )

/*	[Beschreibung]

	Mit dieser Methode wird eine <SfxShell> auf den SfxDispatcher
	gepusht. Die SfxShell wird zun"achst zum pushen vermerkt und
	es wird ein Timer aufgesetzt. Erst bei Ablauf des Timers wird
	tats"achlich gepusht (<SfxDispatcher::Flush()>) und die <SfxBindings>
	werden invalidiert. W"ahrend der Timer l"auft gleichen sich
	entgegengesetzte Push und Pop Befehle mit derselben SfxShell aus.
*/

{
	Pop( rShell, SFX_SHELL_PUSH );
}

//--------------------------------------------------------------------

inline sal_Bool SfxDispatcher::IsActive( const SfxShell& rShell )

/*	[Beschreibung]

	Mit dieser Methode kann abgefragt werden, ob sich eine bestimmte
	<SfxShell>-Instanz auf dem SfxDispatcher befindet.

	[R"uckgabewert]

	sal_Bool				sal_True
						Die SfxShell-Instanz befindet sich auf dem
						SfxDispatcher.

						sal_False
						Die SfxShell-Instanz befindet sich nicht auf dem
						SfxDispatcher.

*/

{
	return CheckVirtualStack( rShell, sal_True );
}
//--------------------------------------------------------------------

inline sal_Bool SfxDispatcher::IsOnTop( const SfxShell& rShell )

/*  [Beschreibung]

	Mit dieser Methode kann abgefragt werden, ob sich eine bestimmte
	<SfxShell>-Instanz zuoberst auf dem SfxDispatcher befindet.

	[R"uckgabewert]

	sal_Bool				sal_True
						Die SfxShell-Instanz befindet sich als oberste
						SfxShell auf dem SfxDispatcher.

						sal_False
						Die SfxShell-Instanz befindet sich nicht als
						oberste SfxShell auf dem SfxDispatcher.

*/

{
	return CheckVirtualStack( rShell, sal_False );
}

//--------------------------------------------------------------------

#endif
