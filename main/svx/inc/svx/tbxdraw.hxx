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


#ifndef _TBXDRAW_HXX
#define _TBXDRAW_HXX

// include ---------------------------------------------------------------

#include <sfx2/tbxctrl.hxx>

// class SvxPopupWindowTbxMgr --------------------------------------------

class SvxPopupWindowTbxMgr : public SfxPopupWindow
{
	void*              	pNiemalsDenPointerAufDieErzeugendeToolBoxMerken; // MI!
	ResId 				aRIdWinTemp;
	ResId				aRIdTbxTemp;

	Link				aTbxClickHdl;


	DECL_LINK( TbxSelectHdl, ToolBox*);
	DECL_LINK( TbxClickHdl, ToolBox*);

protected:
    using SfxPopupWindow::StateChanged;

public:
	SvxPopupWindowTbxMgr( 	sal_uInt16 nId,
							SfxToolBoxControl* pTbxCtl,
							ResId aRIdWin,
							ResId aRIdTbx );
	SvxPopupWindowTbxMgr( 	sal_uInt16 nId,
							WindowAlign eAlign,
							ResId aRIdWin,
							ResId aRIdTbx );
	~SvxPopupWindowTbxMgr();

    virtual void                StateChanged( sal_uInt16 nSID, SfxItemState eState,
											  const SfxPoolItem* pState);
	virtual SfxPopupWindow* 	Clone() const;
	virtual void 				PopupModeEnd();

	void 						StartSelection();

    // toolbox click or execute may chage the default selection in the tools toolbox
    virtual void                UserEvent( sal_uIntPtr nEvent, void* pEventData );
};


#endif		// _TBX_DRAW_HXX
