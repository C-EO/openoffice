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
#include "precompiled_cui.hxx"

#include <sfx2/frame.hxx>
#include <sfx2/viewfrm.hxx>
#include <sot/formats.hxx>
#include <sfx2/sfxsids.hrc>
#include <svl/macitem.hxx>
#include <ucbhelper/content.hxx>
#include <unotools/localfilehelper.hxx>
#include "hyperdlg.hrc"
#include "cuihyperdlg.hxx"  //CHINA001
#include "hltpbase.hxx"
#include "macroass.hxx"
#include <svx/svxdlg.hxx> //CHINA001
#include <cuires.hrc> //CHINA001

using namespace ::ucbhelper;

//########################################################################
//#                                                                      #
//# ComboBox-Control, which is filled with all current framenames         #
//#                                                                      #
//########################################################################

/*************************************************************************
|*
|* Constructor / Destructor
|*
|************************************************************************/

SvxFramesComboBox::SvxFramesComboBox ( Window* pParent, const ResId& rResId,
                                        SfxDispatcher* pDispatch )
:	ComboBox (pParent, rResId)
{
    TargetList* pList = new TargetList;
    SfxViewFrame* pViewFrame = pDispatch ? pDispatch->GetFrame() : 0;
    SfxFrame* pFrame = pViewFrame ? &pViewFrame->GetTopFrame() : 0;
    if ( pFrame )
    {
        pFrame->GetTargetList(*pList);
        sal_uInt16 nCount = (sal_uInt16)pList->Count();
        if( nCount )
        {
            sal_uInt16 i;
            for ( i = 0; i < nCount; i++ )
            {
                InsertEntry(*pList->GetObject(i));
            }
            for ( i = nCount; i; i-- )
            {
                delete pList->GetObject( i - 1 );
            }
        }
        delete pList;
    }
}

SvxFramesComboBox::~SvxFramesComboBox ()
{
}
//########################################################################
//#																		 #
//# ComboBox-Control for URL's with History and Autocompletion			 #
//#																		 #
//########################################################################

/*************************************************************************
|*
|* Constructor / Destructor
|*
|************************************************************************/

SvxHyperURLBox::SvxHyperURLBox( Window* pParent, INetProtocol eSmart, sal_Bool bAddresses )
: SvtURLBox			( pParent, eSmart ),
  DropTargetHelper  ( this ),
  mbAccessAddress	(bAddresses)
{
}

sal_Int8 SvxHyperURLBox::AcceptDrop( const AcceptDropEvent& /* rEvt */ )
{
    return( IsDropFormatSupported( FORMAT_STRING ) ? DND_ACTION_COPY : DND_ACTION_NONE );
}

sal_Int8 SvxHyperURLBox::ExecuteDrop( const ExecuteDropEvent& rEvt )
{
    TransferableDataHelper	aDataHelper( rEvt.maDropEvent.Transferable );
    String                  aString;
    sal_Int8                nRet = DND_ACTION_NONE;

    if( aDataHelper.GetString( FORMAT_STRING, aString ) )
    {
        SetText( aString );
		nRet = DND_ACTION_COPY;
    }

    return nRet;
}

/*
Diese Methode parsed eine EMailadresse aus einem D&D-DataObjekt aus der Adre�datenbank heraus

#ifndef _OFF_OFADBMGR_HXX
#include <offmgr/ofadbmgr.hxx>
#endif
#ifndef _SFX_APP_HXX
#include <offmgr/app.hxx>
#endif
#ifndef _SDB_SDBCURS_HXX
#include <sdb/sdbcurs.hxx>
#endif

#define DB_DD_DELIM			((char)11)

String SvxHyperURLBox::GetAllEmailNamesFromDragItem( sal_uInt16 nItem )
{
#if !defined( ICC ) && !defined( SOLARIS )
	String aAddress;

	if (  DragServer::HasFormat( nItem, SOT_FORMATSTR_ID_SBA_DATAEXCHANGE ) )
	{
		sal_uLong nLen = DragServer::GetDataLen( nItem, SOT_FORMATSTR_ID_SBA_DATAEXCHANGE ) - 1;
		sal_uInt16 i = 0;
		String aTxt;
		char *pTxt = aTxt.AllocStrBuf( (sal_uInt16)nLen );
		DragServer::PasteData( nItem, pTxt, nLen, SOT_FORMATSTR_ID_SBA_DATAEXCHANGE );
		String aDBName = aTxt.GetToken( i++, DB_DD_DELIM );
		String aTableName = aTxt.GetToken( i++, DB_DD_DELIM );
		i++;	// Format"anderung
		String aStatement = aTxt.GetToken( i++, DB_DD_DELIM );
		if ( !aStatement )
		{
			aDBName += DB_DELIM;
			aDBName += aTableName;
		}
		else
		{
			// use here another delimiter, because the OfaDBMgr uses two
			// delimiters ( (char)11 and ; )
			aDBName += ';';
			aDBName += aStatement;
		}
		SbaSelectionListRef pSelectionList;
		pSelectionList.Clear();
		pSelectionList = new SbaSelectionList;
		sal_uInt16 nTokCnt = aTxt.GetTokenCount( DB_DD_DELIM );

		for ( ; i < nTokCnt; ++i )
			pSelectionList->Insert(
				(void*)(sal_uInt16)aTxt.GetToken( i, DB_DD_DELIM ), LIST_APPEND );

        OfaDBMgr* pDBMgr = SFX_APP()->GetOfaDBMgr();
		sal_Bool bBasic = DBMGR_STD;

		if ( !pDBMgr->OpenDB( bBasic, aDBName ) )
			return aAddress;

		OfaDBParam& rParam = pDBMgr->GetDBData( bBasic );
		pDBMgr->ChangeStatement( bBasic, aStatement );
		rParam.pSelectionList->Clear();

		if ( pSelectionList.Is() &&
				(long)pSelectionList->GetObject(0) != -1L )
			*rParam.pSelectionList = *pSelectionList;

		if ( !pDBMgr->ToFirstSelectedRecord( bBasic ) )
		{
			pDBMgr->CloseAll();
			return aAddress;
		}

		if ( pDBMgr->IsSuccessful( bBasic ) )
		{
			//	Spaltenk"opfe
			SbaDBDataDefRef aDBDef = pDBMgr->OpenColumnNames( bBasic );

			if ( aDBDef.Is() )
			{
				SbaColumn* pCol = aDBDef->GetColumn("EMAIL");
				sal_uLong nPos = 0;

				if ( pCol )
					nPos = aDBDef->GetOriginalColumns().GetPos( pCol ) + 1;
				for ( i = 0 ; nPos && i < pSelectionList->Count(); ++i )
				{
					sal_uLong nIndex = (sal_uLong)pSelectionList->GetObject(i);

					// N"achsten zu lesenden Datensatz ansteuern

					sal_Bool bEnd = rParam.GetCursor()->IsOffRange();

					if ( !bEnd )
					{
						const ODbRowRef aRow =
							pDBMgr->GetSelectedRecord( bBasic, i );

						if ( aRow.Is() )
						{
							aAddress += pDBMgr->
								ImportDBField( (sal_uInt16)nPos, &aDBDef, aRow.getBodyPtr() );
							aAddress += ',';
						}
						else
						{
							aAddress.Erase();
							break;
						}
					}
					else
						break;
				}
			}
		}
		pDBMgr->CloseAll();
	}
	aAddress.EraseTrailingChars( ',' );
	return aAddress;
#else
	return String();
#endif
}
*/

void SvxHyperURLBox::Select()
{
	SvtURLBox::Select();
}
void SvxHyperURLBox::Modify()
{
	SvtURLBox::Modify();
}
long SvxHyperURLBox::Notify( NotifyEvent& rNEvt )
{
	return SvtURLBox::Notify( rNEvt );
}
long SvxHyperURLBox::PreNotify( NotifyEvent& rNEvt )
{
	return SvtURLBox::PreNotify( rNEvt );
}

//########################################################################
//#                                                                      #
//# Hyperlink-Dialog: Tabpages-Baseclass                                 #
//#                                                                      #
//########################################################################

/*************************************************************************
|*
|* Con/Destructor, Initialize
|*
\************************************************************************/

SvxHyperlinkTabPageBase::SvxHyperlinkTabPageBase ( Window *pParent,
												   const ResId &rResId,
												   const SfxItemSet& rItemSet )
:	IconChoicePage			( pParent, rResId, rItemSet ),
	mpGrpMore				( NULL ),
	mpFtFrame				( NULL ),
	mpCbbFrame				( NULL ),
	mpFtForm				( NULL ),
	mpLbForm				( NULL ),
	mpFtIndication			( NULL ),
	mpEdIndication			( NULL ),
	mpFtText				( NULL ),
	mpEdText				( NULL ),
	mpBtScript				( NULL ),
    mbIsCloseDisabled       ( sal_False ),
	mpDialog				( pParent ),
    mbStdControlsInit       ( sal_False ),
    aEmptyStr()
{
	// create bookmark-window
	mpMarkWnd = new SvxHlinkDlgMarkWnd ( this );
}

SvxHyperlinkTabPageBase::~SvxHyperlinkTabPageBase ()
{
	maTimer.Stop();

	if ( mbStdControlsInit )
	{
		delete mpGrpMore;
		delete mpFtFrame;
		delete mpCbbFrame;
		delete mpFtForm;
		delete mpLbForm;
		delete mpFtIndication;
		delete mpEdIndication;
		delete mpFtText;
		delete mpEdText	;
		delete mpBtScript;
	}

	delete mpMarkWnd;
}

void SvxHyperlinkTabPageBase::ActivatePage()
{
	TabPage::ActivatePage();
}

void SvxHyperlinkTabPageBase::DeactivatePage()
{
    TabPage::DeactivatePage();
}

sal_Bool SvxHyperlinkTabPageBase::QueryClose()
{
    return !mbIsCloseDisabled;
}

void SvxHyperlinkTabPageBase::InitStdControls ()
{
	if ( !mbStdControlsInit )
	{
		mpGrpMore	  = new FixedLine			( this, ResId (GRP_MORE, *m_pResMgr) );
		mpFtFrame	  = new FixedText			( this, ResId (FT_FRAME, *m_pResMgr) );
        mpCbbFrame    = new SvxFramesComboBox   ( this, ResId (CB_FRAME, *m_pResMgr), GetDispatcher() );
		mpFtForm	  = new FixedText			( this, ResId (FT_FORM, *m_pResMgr) );
		mpLbForm	  = new ListBox				( this, ResId (LB_FORM, *m_pResMgr) );
		mpFtIndication= new FixedText			( this, ResId (FT_INDICATION, *m_pResMgr) );
		mpEdIndication= new Edit				( this, ResId (ED_INDICATION, *m_pResMgr) );
		mpFtText	  = new FixedText			( this, ResId (FT_TEXT, *m_pResMgr) );
		mpEdText	  = new Edit				( this, ResId (ED_TEXT, *m_pResMgr) );
		mpBtScript	  = new ImageButton			( this, ResId (BTN_SCRIPT, *m_pResMgr) );

		mpBtScript->SetClickHdl ( LINK ( this, SvxHyperlinkTabPageBase, ClickScriptHdl_Impl ) );

		mpBtScript->SetModeImage( Image( ResId( IMG_SCRIPT_HC, *m_pResMgr ) ), BMP_COLOR_HIGHCONTRAST );
        mpBtScript->EnableTextDisplay (sal_False);

		mpBtScript->SetAccessibleRelationMemberOf( mpGrpMore );
		mpBtScript->SetAccessibleRelationLabeledBy( mpFtForm );
	}

	mbStdControlsInit = sal_True;
}

/*************************************************************************
|*
|* Move Extra-Window
|*
\************************************************************************/

sal_Bool SvxHyperlinkTabPageBase::MoveToExtraWnd( Point aNewPos, sal_Bool bDisConnectDlg )
{
	sal_Bool bReturn = 	mpMarkWnd->MoveTo ( aNewPos );

	if( bDisConnectDlg )
		mpMarkWnd->ConnectToDialog( sal_False );

	return ( !bReturn && IsMarkWndVisible() );
}

/*************************************************************************
|*
|* Show Extra-Window
|*
\************************************************************************/

void SvxHyperlinkTabPageBase::ShowMarkWnd ()
{
	( ( Window* ) mpMarkWnd )->Show();

	// Size of dialog-window in screen pixels
    Rectangle aDlgRect( mpDialog->GetWindowExtentsRelative( NULL ) );
	Point aDlgPos ( aDlgRect.TopLeft() );
	Size aDlgSize (	mpDialog->GetSizePixel () );

	// Absolute size of the screen
    Rectangle aScreen( mpDialog->GetDesktopRectPixel() );

	// Size of Extrawindow
	Size aExtraWndSize( mpMarkWnd->GetSizePixel () );

    // mpMarkWnd is a child of mpDialog, so coordinates for positioning must be relative to mpDialog
	if( aDlgPos.X()+(1.05*aDlgSize.Width())+aExtraWndSize.Width() > aScreen.Right() )
	{
		if( aDlgPos.X() - ( 0.05*aDlgSize.Width() ) - aExtraWndSize.Width() < 0 )
		{
			// Pos Extrawindow anywhere
			MoveToExtraWnd( Point(10,10) );  // very unlikely
			mpMarkWnd->ConnectToDialog( sal_False );
		}
		else
		{
			// Pos Extrawindow on the left side of Dialog
			MoveToExtraWnd( Point(0,0) - Point( long(0.05*aDlgSize.Width()), 0 ) - Point( aExtraWndSize.Width(), 0 ) );
		}
	}
	else
	{
		// Pos Extrawindow on the right side of Dialog
		MoveToExtraWnd ( Point( long(1.05*aDlgSize.getWidth()), 0 ) );
	}

	// Set size of Extra-Window
	mpMarkWnd->SetSizePixel( Size( aExtraWndSize.Width(), aDlgSize.Height() ) );
}

/*************************************************************************
|*
|* Fill Dialogfields
|*
\************************************************************************/

void SvxHyperlinkTabPageBase::FillStandardDlgFields ( SvxHyperlinkItem* pHyperlinkItem )
{
	// Frame
	sal_uInt16 nPos = mpCbbFrame->GetEntryPos ( pHyperlinkItem->GetTargetFrame() );
	if ( nPos != LISTBOX_ENTRY_NOTFOUND)
		mpCbbFrame->SetText ( pHyperlinkItem->GetTargetFrame() );

	// Form
	String aStrFormText = CUI_RESSTR( RID_SVXSTR_HYPERDLG_FROM_TEXT );
	String aStrFormButton = CUI_RESSTR( RID_SVXSTR_HYPERDLG_FORM_BUTTON );

	if( pHyperlinkItem->GetInsertMode() & HLINK_HTMLMODE )
	{
		mpLbForm->Clear();
		mpLbForm->InsertEntry( aStrFormText );
		mpLbForm->SelectEntryPos ( 0 );
	}
	else
	{
		mpLbForm->Clear();
		mpLbForm->InsertEntry( aStrFormText );
		mpLbForm->InsertEntry( aStrFormButton );
		mpLbForm->SelectEntryPos ( pHyperlinkItem->GetInsertMode() == HLINK_BUTTON ? 1 : 0 );
	}

	// URL
	mpEdIndication->SetText ( pHyperlinkItem->GetName() );

	// Name
	mpEdText->SetText ( pHyperlinkItem->GetIntName() );

	// Script-button
	if ( !pHyperlinkItem->GetMacroEvents() )
		mpBtScript->Disable();
	else
		mpBtScript->Enable();
}

/*************************************************************************
|*
|* Any action to do after apply-button is pressed
|*
\************************************************************************/

void SvxHyperlinkTabPageBase::DoApply ()
{
	// default-implemtation : do nothing
}

/*************************************************************************
|*
|* Ask page whether an insert is possible
|*
\************************************************************************/

sal_Bool SvxHyperlinkTabPageBase::AskApply ()
{
	// default-implementation
	return sal_True;
}

/*************************************************************************
|*
|* This method would be called from bookmark-window to set new mark-string
|*
\************************************************************************/

void SvxHyperlinkTabPageBase::SetMarkStr ( String& /*aStrMark*/ )
{
	// default-implemtation : do nothing
}

/*************************************************************************
|*
|* This method will be called from the dialog-class if the state off
|* the online-mode has changed.
|*
\************************************************************************/

void SvxHyperlinkTabPageBase::SetOnlineMode( sal_Bool /*bEnable*/ )
{
	// default-implemtation : do nothing
}

/*************************************************************************
|*
|* Set initial focus
|*
|************************************************************************/

void SvxHyperlinkTabPageBase::SetInitFocus()
{
	GrabFocus();
}

/*************************************************************************
|*
|* Ask dialog whether the curretn doc is a HTML-doc
|*
|************************************************************************/

sal_Bool SvxHyperlinkTabPageBase::IsHTMLDoc() const
{
	return ((SvxHpLinkDlg*)mpDialog)->IsHTMLDoc();
}

/*************************************************************************
|*
|* retrieve dispatcher
|*
|************************************************************************/

SfxDispatcher* SvxHyperlinkTabPageBase::GetDispatcher() const
{
	return ((SvxHpLinkDlg*)mpDialog)->GetDispatcher();
}

/*************************************************************************
|*
|* Click on imagebutton : Script
|*
|************************************************************************/

IMPL_LINK ( SvxHyperlinkTabPageBase, ClickScriptHdl_Impl, void *, EMPTYARG )
{
	SvxHyperlinkItem *pHyperlinkItem = (SvxHyperlinkItem *)
		                               GetItemSet().GetItem (SID_HYPERLINK_GETLINK);

	if ( pHyperlinkItem->GetMacroEvents() )
	{
		// get macros from itemset
		const SvxMacroTableDtor* pMacroTbl = pHyperlinkItem->GetMacroTbl();
		SvxMacroItem aItem ( GetWhich(SID_ATTR_MACROITEM) );
		if( pMacroTbl )
			aItem.SetMacroTable( *pMacroTbl );

		// create empty itemset for macro-dlg
		SfxItemSet* pItemSet = new SfxItemSet(SFX_APP()->GetPool(),
			                                  SID_ATTR_MACROITEM,
											  SID_ATTR_MACROITEM );
		pItemSet->Put ( aItem, SID_ATTR_MACROITEM );

        // --> PB 2006-01-13 #123474#
        /*  disable HyperLinkDlg for input while the MacroAssignDlg is working
            because if no JAVA is installed an error box occurs and then it is possible
            to close the HyperLinkDlg before its child (MacroAssignDlg) -> GPF
         */
        sal_Bool bIsInputEnabled = GetParent()->IsInputEnabled();
        if ( bIsInputEnabled )
            GetParent()->EnableInput( sal_False );
        // <--
        SfxMacroAssignDlg aDlg( this, mxDocumentFrame, *pItemSet );

		// add events
		SfxMacroTabPage *pMacroPage = (SfxMacroTabPage*) aDlg.GetTabPage();

		if ( pHyperlinkItem->GetMacroEvents() & HYPERDLG_EVENT_MOUSEOVER_OBJECT )
			pMacroPage->AddEvent( String( CUI_RESSTR(RID_SVXSTR_HYPDLG_MACROACT1) ),
			                      SFX_EVENT_MOUSEOVER_OBJECT );
		if ( pHyperlinkItem->GetMacroEvents() & HYPERDLG_EVENT_MOUSECLICK_OBJECT )
			pMacroPage->AddEvent( String( CUI_RESSTR(RID_SVXSTR_HYPDLG_MACROACT2) ),
			                      SFX_EVENT_MOUSECLICK_OBJECT);
		if ( pHyperlinkItem->GetMacroEvents() & HYPERDLG_EVENT_MOUSEOUT_OBJECT )
			pMacroPage->AddEvent( String( CUI_RESSTR(RID_SVXSTR_HYPDLG_MACROACT3) ),
			                      SFX_EVENT_MOUSEOUT_OBJECT);

        // --> PB 2006-01-13 #123474#
        if ( bIsInputEnabled )
            GetParent()->EnableInput( sal_True );
        // <--
        // execute dlg
        DisableClose( sal_True );
        short nRet = aDlg.Execute();
        DisableClose( sal_False );
        if ( RET_OK == nRet )
		{
			const SfxItemSet* pOutSet = aDlg.GetOutputItemSet();
			const SfxPoolItem* pItem;
			if( SFX_ITEM_SET == pOutSet->GetItemState( SID_ATTR_MACROITEM, sal_False, &pItem ))
			{
				pHyperlinkItem->SetMacroTable( ((SvxMacroItem*)pItem)->GetMacroTable() );
			}
		}
		delete pItemSet;
	}

	return( 0L );
}

/*************************************************************************
|*
|* Get Macro-Infos
|*
|************************************************************************/

sal_uInt16 SvxHyperlinkTabPageBase::GetMacroEvents()
{
	SvxHyperlinkItem *pHyperlinkItem = (SvxHyperlinkItem *)
		                               GetItemSet().GetItem (SID_HYPERLINK_GETLINK);

	return pHyperlinkItem->GetMacroEvents();
}

SvxMacroTableDtor* SvxHyperlinkTabPageBase::GetMacroTable()
{
	SvxHyperlinkItem *pHyperlinkItem = (SvxHyperlinkItem *)
		                               GetItemSet().GetItem (SID_HYPERLINK_GETLINK);

	return ( (SvxMacroTableDtor*)pHyperlinkItem->GetMacroTbl() );
}

/*************************************************************************
|*
|* Does the given file exists ?
|*
|************************************************************************/

sal_Bool SvxHyperlinkTabPageBase::FileExists( const INetURLObject& rURL )
{
	sal_Bool bRet = sal_False;

	if( rURL.GetFull().getLength() > 0 )
	{
		try
		{
			Content		aCnt( rURL.GetMainURL( INetURLObject::NO_DECODE ), ::com::sun::star::uno::Reference< ::com::sun::star::ucb::XCommandEnvironment >() );
			::rtl::OUString	aTitle;

			aCnt.getPropertyValue( ::rtl::OUString::createFromAscii( "Title" ) ) >>= aTitle;
			bRet = ( aTitle.getLength() > 0 );
		}
		catch( ... )
		{
			DBG_ERROR( "FileExists: ucb error" );
		}
	}

	return bRet;
}

/*************************************************************************
|*
|* try to detect the current protocol that is used in aStrURL
|*
|************************************************************************/

String SvxHyperlinkTabPageBase::GetSchemeFromURL( String aStrURL )
{
    String aStrScheme;

	INetURLObject aURL( aStrURL );
	INetProtocol aProtocol = aURL.GetProtocol();

    // #77696#
    // our new INetUrlObject now has the ability
    // to detect if an Url is valid or not :-(
    if ( aProtocol == INET_PROT_NOT_VALID )
    {
	    if ( aStrURL.EqualsIgnoreCaseAscii( INET_HTTP_SCHEME, 0, 7 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_HTTP_SCHEME );
        }
        else if ( aStrURL.EqualsIgnoreCaseAscii( INET_HTTPS_SCHEME, 0, 8 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_HTTPS_SCHEME );
        }
        else if ( aStrURL.EqualsIgnoreCaseAscii( INET_FTP_SCHEME, 0, 6 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_FTP_SCHEME );
        }
        else if ( aStrURL.EqualsIgnoreCaseAscii( INET_MAILTO_SCHEME, 0, 7 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_MAILTO_SCHEME );
        }
        else if ( aStrURL.EqualsIgnoreCaseAscii( INET_NEWS_SCHEME, 0, 5 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_NEWS_SCHEME );
        }
        else if ( aStrURL.EqualsIgnoreCaseAscii( INET_TELNET_SCHEME, 0, 9 ) )
        {
            aStrScheme = String::CreateFromAscii( INET_TELNET_SCHEME );
        }
    }
    else
        aStrScheme = INetURLObject::GetScheme( aProtocol );
    return aStrScheme;
}


void SvxHyperlinkTabPageBase::GetDataFromCommonFields( String& aStrName,
											 String& aStrIntName, String& aStrFrame,
											 SvxLinkInsertMode& eMode )
{
	aStrIntName	= mpEdText->GetText();
	aStrName	= mpEdIndication->GetText();
	aStrFrame	= mpCbbFrame->GetText();
	eMode		= (SvxLinkInsertMode) (mpLbForm->GetSelectEntryPos()+1);
	if( IsHTMLDoc() )
		eMode = (SvxLinkInsertMode) ( sal_uInt16(eMode) | HLINK_HTMLMODE );
}

/*************************************************************************
|*
|* reset dialog-fields
|*
|************************************************************************/

void SvxHyperlinkTabPageBase::Reset( const SfxItemSet& rItemSet)
{
	///////////////////////////////////////
	// Set dialog-fields from create-itemset
	maStrInitURL = aEmptyStr;

	SvxHyperlinkItem *pHyperlinkItem = (SvxHyperlinkItem *)
		                               rItemSet.GetItem (SID_HYPERLINK_GETLINK);

	if ( pHyperlinkItem )
	{
		// set dialog-fields
		FillStandardDlgFields (pHyperlinkItem);

		// set all other fields
		FillDlgFields ( (String&)pHyperlinkItem->GetURL() );

		// Store initial URL
		maStrInitURL = pHyperlinkItem->GetURL();
	}
}

/*************************************************************************
|*
|* Fill output-ItemSet
|*
|************************************************************************/

sal_Bool SvxHyperlinkTabPageBase::FillItemSet( SfxItemSet& rOut)
{
	String aStrURL, aStrName, aStrIntName, aStrFrame;
	SvxLinkInsertMode eMode;

	GetCurentItemData ( aStrURL, aStrName, aStrIntName, aStrFrame, eMode);
	if ( !aStrName.Len() ) //automatically create a visible name if the link is created without name
		aStrName = CreateUiNameFromURL(aStrURL);

	sal_uInt16 nEvents = GetMacroEvents();
	SvxMacroTableDtor* pTable = GetMacroTable();

	SvxHyperlinkItem aItem( SID_HYPERLINK_SETLINK, aStrName, aStrURL, aStrFrame,
		                    aStrIntName, eMode, nEvents, pTable );
	rOut.Put (aItem);

	return sal_True;
}

String SvxHyperlinkTabPageBase::CreateUiNameFromURL( const String& aStrURL )
{
    String			aStrUiURL;
    INetURLObject	aURLObj( aStrURL );

    switch(aURLObj.GetProtocol())
    {
        case INET_PROT_FILE:
            utl::LocalFileHelper::ConvertURLToSystemPath( aURLObj.GetMainURL(INetURLObject::NO_DECODE), aStrUiURL );
            break;
    	case INET_PROT_FTP :
            {
                //remove password from name
                INetURLObject	aTmpURL(aURLObj);
                aTmpURL.SetPass(aEmptyStr);
                aStrUiURL = aTmpURL.GetMainURL( INetURLObject::DECODE_UNAMBIGUOUS );
            }
            break;
		default :
            {
                aStrUiURL = aURLObj.GetMainURL(INetURLObject::DECODE_UNAMBIGUOUS);
            }
    }
    if(!aStrUiURL.Len())
        return aStrURL;
    return aStrUiURL;
}

/*************************************************************************
|*
|* Activate / Deactivate Tabpage
|*
|************************************************************************/

void SvxHyperlinkTabPageBase::ActivatePage( const SfxItemSet& rItemSet )
{
	///////////////////////////////////////
	// Set dialog-fields from input-itemset
	SvxHyperlinkItem *pHyperlinkItem = (SvxHyperlinkItem *)
		                               rItemSet.GetItem (SID_HYPERLINK_GETLINK);

	if ( pHyperlinkItem )
	{
		// standard-fields
		FillStandardDlgFields (pHyperlinkItem);
	}

	// show mark-window if it was open before
	if ( ShouldOpenMarkWnd () )
		ShowMarkWnd ();
}

int SvxHyperlinkTabPageBase::DeactivatePage( SfxItemSet* _pSet)
{
	// hide mark-wnd
	SetMarkWndShouldOpen( IsMarkWndVisible () );
	HideMarkWnd ();

	// retrieve data of dialog
	String aStrURL, aStrName, aStrIntName, aStrFrame;
	SvxLinkInsertMode eMode;

	GetCurentItemData ( aStrURL, aStrName, aStrIntName, aStrFrame, eMode);

	sal_uInt16 nEvents = GetMacroEvents();
	SvxMacroTableDtor* pTable = GetMacroTable();

	if( _pSet )
	{
		SvxHyperlinkItem aItem( SID_HYPERLINK_GETLINK, aStrName, aStrURL, aStrFrame,
								aStrIntName, eMode, nEvents, pTable );
		_pSet->Put( aItem );
	}

	return( LEAVE_PAGE );
}

sal_Bool SvxHyperlinkTabPageBase::ShouldOpenMarkWnd()
{
	return sal_False;
}

void SvxHyperlinkTabPageBase::SetMarkWndShouldOpen(sal_Bool)
{
}
