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
#include "precompiled_scui.hxx"




//------------------------------------------------------------------

#define _TPHFEDIT_CXX
#include "scitems.hxx"
#include <editeng/eeitem.hxx>

//CHINA001 #include <svx/chardlg.hxx>
#include <editeng/editobj.hxx>
#include <editeng/editstat.hxx>
#include <editeng/editview.hxx>
#include <editeng/flditem.hxx>
#include <sfx2/basedlgs.hxx>
#include <sfx2/objsh.hxx>
#include <vcl/msgbox.hxx>
#include <vcl/svapp.hxx>
#include <unotools/useroptions.hxx>
#define _SVSTDARR_USHORTS
#include <svl/svstdarr.hxx>

//CHINA001 #include "tphfedit.hxx"
#include "editutil.hxx"
#include "global.hxx"
#include "attrib.hxx"
#include "patattr.hxx"
#include "scresid.hxx"
#include "sc.hrc"
#include "globstr.hrc"
#include "tabvwsh.hxx"
#include "prevwsh.hxx"
#include "hfedtdlg.hrc"
#include "textdlgs.hxx"
#include "AccessibleEditObject.hxx"

#include "scuitphfedit.hxx" //CHINA001
#include <memory> // header file for auto_ptr

// STATIC DATA -----------------------------------------------------------

static sal_uInt16 pPageRightHeaderRanges[] = { SID_SCATTR_PAGE_HEADERRIGHT,
										   SID_SCATTR_PAGE_HEADERRIGHT,
										   0 };

static sal_uInt16 pPageRightFooterRanges[] = { SID_SCATTR_PAGE_FOOTERRIGHT,
										   SID_SCATTR_PAGE_FOOTERRIGHT,
										   0 };

static sal_uInt16 pPageLeftHeaderRanges[] =  { SID_SCATTR_PAGE_HEADERLEFT,
										   SID_SCATTR_PAGE_HEADERLEFT,
										   0 };

static sal_uInt16 pPageLeftFooterRanges[] =  { SID_SCATTR_PAGE_FOOTERLEFT,
										   SID_SCATTR_PAGE_FOOTERLEFT,
										   0 };


static ScEditWindow* pActiveEdWnd = NULL;


//========================================================================
// class ScHFEditPage
//

ScHFEditPage::ScHFEditPage( Window* 			pParent,
							sal_uInt16				nResId,
							const SfxItemSet&	rCoreAttrs,
							sal_uInt16				nWhichId,
							bool bHeader  )

	:	SfxTabPage		( pParent, ScResId( nResId ), rCoreAttrs ),

		aFtLeft 		( this, ScResId( FT_LEFT ) ),
		aWndLeft		( this, ScResId( WND_LEFT ), Left ),
		aFtCenter		( this, ScResId( FT_CENTER ) ),
		aWndCenter		( this, ScResId( WND_CENTER ), Center ),
		aFtRight		( this, ScResId( FT_RIGHT ) ),
		aWndRight		( this, ScResId( WND_RIGHT ), Right ),
		maFtDefinedHF		( this, ScResId( FT_HF_DEFINED ) ),
		maLbDefined		( this, ScResId( LB_DEFINED ) ),
		maFtCustomHF		( this, ScResId( FT_HF_CUSTOM ) ),
		aBtnText		( this, ScResId( BTN_TEXT ) ),
		aBtnFile		( this, ScResId( BTN_FILE ) ),
		aBtnTable		( this, ScResId( BTN_TABLE ) ),
		aBtnPage		( this, ScResId( BTN_PAGE ) ),
		aBtnLastPage	( this, ScResId( BTN_PAGES ) ),
		aBtnDate		( this, ScResId( BTN_DATE ) ),
		aBtnTime		( this, ScResId( BTN_TIME ) ),
		aFlInfo 		( this, ScResId( FL_INFO ) ),
		aFtInfo 		( this, ScResId( FT_INFO ) ),
		aPopUpFile		( ScResId( RID_POPUP_FCOMMAND) ),
		nWhich			( nWhichId )
{
	//!	use default style from current document?
	//!	if font color is used, header/footer background color must be set

	ScPatternAttr	aPatAttr( rCoreAttrs.GetPool() );


	aBtnFile.SetPopupMenu(&aPopUpFile);

	maLbDefined.SetSelectHdl( LINK( this, ScHFEditPage, ListHdl_Impl ) );
	aBtnFile.SetMenuHdl( LINK( this, ScHFEditPage, MenuHdl ) );
	aBtnText	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnPage	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnLastPage.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnDate	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnTime	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnFile	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );
	aBtnTable	.SetClickHdl( LINK( this, ScHFEditPage, ClickHdl ) );

    aBtnText    .SetModeImage( Image( ScResId( IMG_TEXT_H  ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnFile    .SetModeImage( Image( ScResId( IMG_FILE_H  ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnTable   .SetModeImage( Image( ScResId( IMG_TABLE_H ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnPage    .SetModeImage( Image( ScResId( IMG_PAGE_H  ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnLastPage.SetModeImage( Image( ScResId( IMG_PAGES_H ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnDate    .SetModeImage( Image( ScResId( IMG_DATE_H  ) ), BMP_COLOR_HIGHCONTRAST );
    aBtnTime    .SetModeImage( Image( ScResId( IMG_TIME_H  ) ), BMP_COLOR_HIGHCONTRAST );

    if(!bHeader)
    {
        maFtDefinedHF.SetText(ScGlobal::GetRscString( STR_FOOTER ));
        maFtCustomHF.SetText(ScGlobal::GetRscString( STR_HF_CUSTOM_FOOTER ));
    }
	if( Application::GetSettings().GetLayoutRTL() )
	{
		Point pt1 = aWndLeft.GetPosPixel();
		Point pt2 = aWndRight.GetPosPixel();
		aWndLeft.SetPosPixel(pt2);
		aWndRight.SetPosPixel(pt1);

		pt1 = aFtLeft.GetPosPixel();
		pt2 = aFtRight.GetPosPixel();
		aFtLeft.SetPosPixel(pt2);
		aFtRight.SetPosPixel(pt1);
	}
	aWndLeft.	SetFont( aPatAttr );
	aWndCenter. SetFont( aPatAttr );
	aWndRight.	SetFont( aPatAttr );

	aWndLeft.SetObjectSelectHdl( LINK(this,ScHFEditPage,ObjectSelectHdl) );
	aWndCenter.SetObjectSelectHdl( LINK(this,ScHFEditPage,ObjectSelectHdl) );
	aWndRight.SetObjectSelectHdl( LINK(this,ScHFEditPage,ObjectSelectHdl) );
	aBtnText.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnFile.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnTable.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnPage.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnLastPage.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnDate.SetAccessibleRelationMemberOf( &maFtCustomHF );
	aBtnTime.SetAccessibleRelationMemberOf( &maFtCustomHF );	
	FillCmdArr();

	aWndLeft.GrabFocus();

	InitPreDefinedList();

	FreeResource();
}
    IMPL_LINK( ScHFEditPage, ObjectSelectHdl, ScEditWindow*, pEdit )
	{
		(void)pEdit;
	    aBtnText.GrabFocus();
	    return 0;
	}

// -----------------------------------------------------------------------

__EXPORT ScHFEditPage::~ScHFEditPage()
{
}

void ScHFEditPage::SetNumType(SvxNumType eNumType)
{
	aWndLeft.SetNumType(eNumType);
	aWndCenter.SetNumType(eNumType);
	aWndRight.SetNumType(eNumType);
}

// -----------------------------------------------------------------------

#define IS_AVAILABLE(w)(rCoreSet.GetItemState( (w) ) >= SFX_ITEM_AVAILABLE)

void __EXPORT ScHFEditPage::Reset( const SfxItemSet& rCoreSet )
{
	if ( IS_AVAILABLE( nWhich ) )
	{
		const ScPageHFItem& rItem = (const ScPageHFItem&)(rCoreSet.Get( nWhich ));

        if( const EditTextObject* pLeft = rItem.GetLeftArea() )
            aWndLeft.SetText( *pLeft );
        if( const EditTextObject* pCenter = rItem.GetCenterArea() )
            aWndCenter.SetText( *pCenter );
        if( const EditTextObject* pRight = rItem.GetRightArea() )
            aWndRight.SetText( *pRight );

		SetSelectDefinedList();
	}
}

#undef IS_AVAILABLE

// -----------------------------------------------------------------------

sal_Bool __EXPORT ScHFEditPage::FillItemSet( SfxItemSet& rCoreSet )
{
	ScPageHFItem	aItem( nWhich );
	EditTextObject* pLeft	= aWndLeft	.CreateTextObject();
	EditTextObject* pCenter = aWndCenter.CreateTextObject();
	EditTextObject* pRight	= aWndRight .CreateTextObject();

	aItem.SetLeftArea  ( *pLeft );
	aItem.SetCenterArea( *pCenter );
	aItem.SetRightArea ( *pRight );
	delete pLeft;
	delete pCenter;
	delete pRight;

	rCoreSet.Put( aItem );

	return sal_True;
}

// -----------------------------------------------------------------------

#define SET_CMD(i,id) \
	aCmd  = aDel;							\
	aCmd += ScGlobal::GetRscString( id );	\
	aCmd += aDel;							\
	aCmdArr[i] = aCmd;

// -----------------------------------------------------------------------

void ScHFEditPage::FillCmdArr()
{
	String aDel( ScGlobal::GetRscString( STR_HFCMD_DELIMITER ) );
	String aCmd;

	SET_CMD( 0, STR_HFCMD_PAGE )
	SET_CMD( 1, STR_HFCMD_PAGES )
	SET_CMD( 2, STR_HFCMD_DATE )
	SET_CMD( 3, STR_HFCMD_TIME )
	SET_CMD( 4, STR_HFCMD_FILE )
	SET_CMD( 5, STR_HFCMD_TABLE )
}

#undef SET_CMD

void ScHFEditPage::InitPreDefinedList()
{
    SvtUserOptions aUserOpt;

    Color* pTxtColour = NULL;
    Color* pFldColour = NULL;

    // Get the all field values at the outset.
    String aPageFieldValue(aWndLeft.GetEditEngine()->CalcFieldValue(SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD), 0,0, pTxtColour, pFldColour));
    String aSheetFieldValue(aWndLeft.GetEditEngine()->CalcFieldValue(SvxFieldItem(SvxTableField(), EE_FEATURE_FIELD), 0,0, pTxtColour, pFldColour));
    String aFileFieldValue(aWndLeft.GetEditEngine()->CalcFieldValue(SvxFieldItem(SvxFileField(), EE_FEATURE_FIELD), 0,0, pTxtColour, pFldColour));
    String aExtFileFieldValue(aWndLeft.GetEditEngine()->CalcFieldValue(SvxFieldItem(SvxExtFileField(), EE_FEATURE_FIELD), 0,0, pTxtColour, pFldColour));
    String aDateFieldValue(aWndLeft.GetEditEngine()->CalcFieldValue(SvxFieldItem(SvxDateField(), EE_FEATURE_FIELD), 0,0, pTxtColour, pFldColour));

    maLbDefined.Clear();

    maLbDefined.InsertEntry( ScGlobal::GetRscString( STR_HF_NONE_IN_BRACKETS ));

    String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
    aPageEntry += ' ';
    aPageEntry += aPageFieldValue;
    maLbDefined.InsertEntry(aPageEntry);

    String aPageOfEntry(aPageEntry);
    aPageOfEntry += ' ';
    aPageOfEntry += ScGlobal::GetRscString( STR_HF_OF_QUESTION );
    maLbDefined.InsertEntry( aPageOfEntry);

    maLbDefined.InsertEntry(aSheetFieldValue);

    String aConfidentialEntry(aUserOpt.GetCompany());
    aConfidentialEntry += ' ';
    aConfidentialEntry += ScGlobal::GetRscString( STR_HF_CONFIDENTIAL );
    aConfidentialEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aConfidentialEntry += aDateFieldValue;
    aConfidentialEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aConfidentialEntry += aPageEntry;
    maLbDefined.InsertEntry( aConfidentialEntry);

    String aFileNamePageEntry(aFileFieldValue);
    aFileNamePageEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aFileNamePageEntry += aPageEntry;
    maLbDefined.InsertEntry( aFileNamePageEntry);

    maLbDefined.InsertEntry( aExtFileFieldValue);

    String aPageSheetNameEntry(aPageEntry);
    aPageSheetNameEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aPageSheetNameEntry += aSheetFieldValue;
    maLbDefined.InsertEntry( aPageSheetNameEntry);

    String aPageFileNameEntry(aPageEntry);
    aPageFileNameEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aPageFileNameEntry += aFileFieldValue;
    maLbDefined.InsertEntry( aPageFileNameEntry);

    String aPagePathNameEntry(aPageEntry);
    aPagePathNameEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aPagePathNameEntry += aExtFileFieldValue;
    maLbDefined.InsertEntry( aPagePathNameEntry);

    String aUserNameEntry(aUserOpt.GetFirstName());
    aUserNameEntry += ' ';
    aUserNameEntry += (String)aUserOpt.GetLastName();
    aUserNameEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aUserNameEntry += aPageEntry;
    aUserNameEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aUserNameEntry += aDateFieldValue;
    maLbDefined.InsertEntry( aUserNameEntry);

    String aCreatedByEntry(ScGlobal::GetRscString( STR_HF_CREATED_BY ) );
    aCreatedByEntry += ' ';
    aCreatedByEntry += (String)aUserOpt.GetFirstName();
    aCreatedByEntry += ' ';
    aCreatedByEntry += (String)aUserOpt.GetLastName();
    aCreatedByEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aCreatedByEntry += aDateFieldValue;
    aCreatedByEntry.AppendAscii(RTL_CONSTASCII_STRINGPARAM(", "));
    aCreatedByEntry += aPageEntry;
    maLbDefined.InsertEntry( aCreatedByEntry);
}

void ScHFEditPage::InsertToDefinedList()
{
    sal_uInt16 nCount =  maLbDefined.GetEntryCount();
    if(nCount == eEntryCount)
    {
        String aCustomizedEntry(ScGlobal::GetRscString( STR_HF_CUSTOMIZED ) );
        maLbDefined.InsertEntry( aCustomizedEntry);
        maLbDefined.SelectEntryPos(eEntryCount);
    }
}

void ScHFEditPage::RemoveFromDefinedList()
{
    sal_uInt16 nCount =  maLbDefined.GetEntryCount();
    if(nCount > eEntryCount )
        maLbDefined.RemoveEntry( nCount-1);
}

// determine if the header/footer exists in our predefined list and set select to it.
void ScHFEditPage::SetSelectDefinedList()
{
    SvtUserOptions aUserOpt;

    // default to customized
    ScHFEntryId eSelectEntry = eEntryCount;

    ::std::auto_ptr< EditTextObject > pLeftObj;
    ::std::auto_ptr< EditTextObject > pCenterObj;
    ::std::auto_ptr< EditTextObject > pRightObj;

    XubString aLeftEntry;
    XubString aCenterEntry;
    XubString aRightEntry;

    pLeftObj.reset(aWndLeft.GetEditEngine()->CreateTextObject());
    pCenterObj.reset(aWndCenter.GetEditEngine()->CreateTextObject());
    pRightObj.reset(aWndRight.GetEditEngine()->CreateTextObject());

    bool bFound = false;

    sal_uInt16 i;
    sal_uInt16 nCount =  maLbDefined.GetEntryCount();
    for(i = 0; i < nCount && !bFound; i++)
    {
        switch(static_cast<ScHFEntryId>(i))
        {
            case eNoneEntry:
            {
                aLeftEntry = pLeftObj->GetText(0);
                aCenterEntry = pCenterObj->GetText(0);
                aRightEntry = pRightObj->GetText(0);
                if(aLeftEntry == EMPTY_STRING && aCenterEntry == EMPTY_STRING
                    && aRightEntry == EMPTY_STRING)
                {
                    eSelectEntry = eNoneEntry;
                    bFound = true;
                }
            }
            break;

            case ePageEntry:
            {
                aLeftEntry = pLeftObj->GetText(0);
                aRightEntry = pRightObj->GetText(0);
                if(aLeftEntry == EMPTY_STRING && aRightEntry == EMPTY_STRING)
                {
                    if(IsPageEntry(aWndCenter.GetEditEngine(), pCenterObj.get()))
                    {
                        eSelectEntry = ePageEntry;
                        bFound = true;
                    }
                }
            }
            break;


            //TODO
            case ePagesEntry:
            {
            }
            break;

            case eSheetEntry:
            {
                aLeftEntry = pLeftObj->GetText(0);
                aRightEntry = pRightObj->GetText(0);
                if(aLeftEntry == EMPTY_STRING && aRightEntry == EMPTY_STRING)
                {
                    if(pCenterObj->IsFieldObject())
                    {
                        const SvxFieldItem* pFieldItem = pCenterObj->GetField();
                        if(pFieldItem)
                        {
                            const SvxFieldData* pField = pFieldItem->GetField();
                            if(pField && pField->ISA(SvxTableField))
                            {
                                eSelectEntry = eSheetEntry;
                                bFound = true;
                            }
                        }
                    }
                }
            }
            break;

            case eConfidentialEntry:
            {
                if(IsDateEntry(pCenterObj.get()) && IsPageEntry(aWndRight.GetEditEngine(), pRightObj.get()))
                {
                    String aConfidentialEntry(aUserOpt.GetCompany());
                    aConfidentialEntry += ' ';
                    aConfidentialEntry += ScGlobal::GetRscString( STR_HF_CONFIDENTIAL );
                    if(aConfidentialEntry == aWndLeft.GetEditEngine()->GetText(0))
                    {
                        eSelectEntry = eConfidentialEntry;
                        bFound = true;
                    }
                }
            }
            break;

            //TODO
            case eFileNamePageEntry:
            {
            }
            break;

            case eExtFileNameEntry:
            {
                aLeftEntry = pLeftObj->GetText(0);
                aRightEntry = pRightObj->GetText(0);
                if(IsExtFileNameEntry(pCenterObj.get()) && aLeftEntry == EMPTY_STRING
                    && aRightEntry == EMPTY_STRING)
                {
                    eSelectEntry = eExtFileNameEntry;
                    bFound = true;
                }
            }
            break;

            //TODO
            case ePageSheetEntry:
            {
            }
            break;

            //TODO
            case ePageFileNameEntry:
            {
            }
            break;

            case ePageExtFileNameEntry:
            {
                aLeftEntry = pLeftObj->GetText(0);
                if(IsPageEntry(aWndCenter.GetEditEngine(), pCenterObj.get()) &&
                    IsExtFileNameEntry(pRightObj.get()) && aLeftEntry == EMPTY_STRING)
                {
                    eSelectEntry = ePageExtFileNameEntry;
                    bFound = true;
                }
            }
            break;

            case eUserNameEntry:
            {
                if(IsDateEntry(pRightObj.get()) && IsPageEntry(aWndCenter.GetEditEngine(), pCenterObj.get()))
                {
                    String aUserNameEntry(aUserOpt.GetFirstName());
                    aUserNameEntry += ' ';
                    aUserNameEntry += (String)aUserOpt.GetLastName();
                    if(aUserNameEntry == aWndLeft.GetEditEngine()->GetText(0))
                    {
                        eSelectEntry = eUserNameEntry;
                        bFound = true;
                    }
                }
            }
            break;

            case eCreatedByEntry:
            {
                if(IsDateEntry(pCenterObj.get()) && IsPageEntry(aWndRight.GetEditEngine(), pRightObj.get()))
                {
                    String aCreatedByEntry(ScGlobal::GetRscString( STR_HF_CREATED_BY ) );
                    aCreatedByEntry += ' ';
                    aCreatedByEntry += (String)aUserOpt.GetFirstName();
                    aCreatedByEntry += ' ';
                    aCreatedByEntry += (String)aUserOpt.GetLastName();
                    if(aCreatedByEntry == aWndLeft.GetEditEngine()->GetText(0))
                    {
                        eSelectEntry = eCreatedByEntry;
                        bFound = true;
                    }
                }
            }
            break;

            default:
            {
                // added to avoid warnings
            }
        }
    }

    if(eSelectEntry == eEntryCount)
        InsertToDefinedList();

    maLbDefined.SelectEntryPos( sal::static_int_cast<sal_uInt16>( eSelectEntry ) );
}

bool ScHFEditPage::IsPageEntry(EditEngine*pEngine, EditTextObject* pTextObj)
{
    if(!pEngine && !pTextObj)
        return false;

    bool bReturn = false;

    if(!pTextObj->IsFieldObject())
    {
        SvUShorts aPosList;
        pEngine->GetPortions(0,aPosList);
        if(aPosList.Count() == 2)
        {
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            ESelection aSel(0,0,0,0);
            aSel.nEndPos = aPageEntry.Len();
            if(aPageEntry == pEngine->GetText(aSel))
            {
                aSel.nStartPos = aSel.nEndPos;
                aSel.nEndPos++;
                ::std::auto_ptr< EditTextObject > pPageObj;
                pPageObj.reset(pEngine->CreateTextObject(aSel));
                if(pPageObj.get() && pPageObj->IsFieldObject() )
                {
                    const SvxFieldItem* pFieldItem = pPageObj->GetField();
                    if(pFieldItem)
                    {
                        const SvxFieldData* pField = pFieldItem->GetField();
                        if(pField && pField->ISA(SvxPageField))
                            bReturn = true;
                    }
                }
            }
        }
    }
    return bReturn;
}

bool ScHFEditPage::IsDateEntry(EditTextObject* pTextObj)
{
    if(!pTextObj)
        return false;

    bool bReturn = false;
    if(pTextObj->IsFieldObject())
    {
        const SvxFieldItem* pFieldItem = pTextObj->GetField();
        if(pFieldItem)
        {
            const SvxFieldData* pField = pFieldItem->GetField();
            if(pField && pField->ISA(SvxDateField))
                bReturn = true;
        }
    }
    return bReturn;
}

bool ScHFEditPage::IsExtFileNameEntry(EditTextObject* pTextObj)
{
    if(!pTextObj)
        return false;
    bool bReturn = false;
    if(pTextObj->IsFieldObject())
    {
        const SvxFieldItem* pFieldItem = pTextObj->GetField();
        if(pFieldItem)
	{
            const SvxFieldData* pField = pFieldItem->GetField();
            if(pField && pField->ISA(SvxExtFileField))
                bReturn = true;
        }
    }
    return bReturn;
}

void ScHFEditPage::ProcessDefinedListSel(ScHFEntryId eSel, bool bTravelling)
{
    SvtUserOptions aUserOpt;
    ::std::auto_ptr< EditTextObject > pTextObj;

    switch(eSel)
    {
        case eNoneEntry:
            ClearTextAreas();
            if(!bTravelling)
                aWndLeft.GrabFocus();
        break;

        case ePageEntry:
        {
            ClearTextAreas();
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aWndCenter.InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
            if(!bTravelling)
                aWndCenter.GrabFocus();
        }
        break;

        case ePagesEntry:
        {
            ClearTextAreas();
            ESelection aSel(0,0,0,0);
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aSel.nEndPos = aPageEntry.Len();
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            ++aSel.nEndPos;
            String aPageOfEntry = ' ';
            aPageOfEntry += ScGlobal::GetRscString( STR_HF_OF );
            aPageOfEntry += ' ';
            aWndCenter.GetEditEngine()->QuickInsertText(aPageOfEntry,ESelection(aSel.nEndPara,aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            aSel.nEndPos = sal::static_int_cast<xub_StrLen>( aSel.nEndPos + aPageOfEntry.Len() );
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem(SvxPagesField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara,aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            pTextObj.reset(aWndCenter.GetEditEngine()->CreateTextObject());
            aWndCenter.SetText(*pTextObj);
            XubString aEntry(pTextObj.get()->GetText(0));
            if(!bTravelling)
                aWndCenter.GrabFocus();
        }
        break;

        case eSheetEntry:
            ClearTextAreas();
            aWndCenter.InsertField( SvxFieldItem(SvxTableField(), EE_FEATURE_FIELD) );
            if(!bTravelling)
                aWndCenter.GrabFocus();
        break;

        case eConfidentialEntry:
        {
            ClearTextAreas();
            String aConfidentialEntry(aUserOpt.GetCompany());
            aConfidentialEntry += ' ';
            aConfidentialEntry += ScGlobal::GetRscString( STR_HF_CONFIDENTIAL );
            aWndLeft.GetEditEngine()->SetText(aConfidentialEntry);
            aWndCenter.InsertField( SvxFieldItem(SvxDateField(Date(),SVXDATETYPE_VAR), EE_FEATURE_FIELD) );
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndRight.GetEditEngine()->SetText(aPageEntry);
            aWndRight.InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
            if(!bTravelling)
                aWndRight.GrabFocus();
        }
        break;

        case eFileNamePageEntry:
        {
            ClearTextAreas();
            ESelection aSel(0,0,0,0);
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem( SvxFileField(), EE_FEATURE_FIELD ), aSel );
            ++aSel.nEndPos;
            String aPageEntry(RTL_CONSTASCII_STRINGPARAM(", "));
            aPageEntry += ScGlobal::GetRscString( STR_PAGE ) ;
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->QuickInsertText(aPageEntry, ESelection(aSel.nEndPara,aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            aSel.nStartPos = aSel.nEndPos;
            aSel.nEndPos = sal::static_int_cast<xub_StrLen>( aSel.nEndPos + aPageEntry.Len() );
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara,aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            pTextObj.reset(aWndCenter.GetEditEngine()->CreateTextObject());
            aWndCenter.SetText(*pTextObj);
            XubString aEntry2(pTextObj.get()->GetText(0));
            if(!bTravelling)
                aWndCenter.GrabFocus();
        }
        break;

        case eExtFileNameEntry:
            ClearTextAreas();
            aWndCenter.InsertField( SvxFieldItem( SvxExtFileField(
                EMPTY_STRING, SVXFILETYPE_VAR, SVXFILEFORMAT_FULLPATH ), EE_FEATURE_FIELD ) );
            if(!bTravelling)
                aWndCenter.GrabFocus();
        break;

        case ePageSheetEntry:
        {
            ClearTextAreas();
            ESelection aSel(0,0,0,0);
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aSel.nEndPos = aPageEntry.Len();
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            ++aSel.nEndPos;
            String aCommaSpace(RTL_CONSTASCII_STRINGPARAM(", "));
            aWndCenter.GetEditEngine()->QuickInsertText(aCommaSpace,ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            aSel.nEndPos = sal::static_int_cast<xub_StrLen>( aSel.nEndPos + aCommaSpace.Len() );
            aWndCenter.GetEditEngine()->QuickInsertField( SvxFieldItem(SvxTableField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            pTextObj.reset(aWndCenter.GetEditEngine()->CreateTextObject());
            aWndCenter.SetText(*pTextObj);
            if(!bTravelling)
                aWndCenter.GrabFocus();
        }
        break;

        case ePageFileNameEntry:
        {
            ClearTextAreas();
            ESelection aSel(0,0,0,0);
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aSel.nEndPos = aPageEntry.Len();
            aWndCenter.GetEditEngine()->QuickInsertField(SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            ++aSel.nEndPos;
            String aCommaSpace(RTL_CONSTASCII_STRINGPARAM(", "));
            aWndCenter.GetEditEngine()->QuickInsertText(aCommaSpace,ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            aSel.nEndPos = sal::static_int_cast<xub_StrLen>( aSel.nEndPos + aCommaSpace.Len() );
            aWndCenter.GetEditEngine()->QuickInsertField( SvxFieldItem(SvxFileField(), EE_FEATURE_FIELD), ESelection(aSel.nEndPara, aSel.nEndPos, aSel.nEndPara, aSel.nEndPos));
            pTextObj.reset(aWndCenter.GetEditEngine()->CreateTextObject());
            aWndCenter.SetText(*pTextObj);
            if(!bTravelling)
                aWndCenter.GrabFocus();
        }
        break;

        case ePageExtFileNameEntry:
        {
            ClearTextAreas();
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aWndCenter.InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
            aWndRight.InsertField( SvxFieldItem( SvxExtFileField(
                EMPTY_STRING, SVXFILETYPE_VAR, SVXFILEFORMAT_FULLPATH ), EE_FEATURE_FIELD ) );
            if(!bTravelling)
                aWndRight.GrabFocus();
        }
        break;

        case eUserNameEntry:
        {
            ClearTextAreas();
            String aUserNameEntry(aUserOpt.GetFirstName());
            aUserNameEntry += ' ';
            aUserNameEntry += (String)aUserOpt.GetLastName();
            aWndLeft.GetEditEngine()->SetText(aUserNameEntry);
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndCenter.GetEditEngine()->SetText(aPageEntry);
            aWndCenter.InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
            aWndRight.InsertField( SvxFieldItem(SvxDateField(Date(),SVXDATETYPE_VAR), EE_FEATURE_FIELD) );
            if(!bTravelling)
                aWndRight.GrabFocus();
        }
        break;

        case eCreatedByEntry:
        {
            ClearTextAreas();
            String aCreatedByEntry(ScGlobal::GetRscString( STR_HF_CREATED_BY ) );
            aCreatedByEntry += ' ';
            aCreatedByEntry += (String)aUserOpt.GetFirstName();
            aCreatedByEntry += ' ';
            aCreatedByEntry += (String)aUserOpt.GetLastName();
            aWndLeft.GetEditEngine()->SetText(aCreatedByEntry);
            aWndCenter.InsertField( SvxFieldItem(SvxDateField(Date(),SVXDATETYPE_VAR), EE_FEATURE_FIELD) );
            String aPageEntry(ScGlobal::GetRscString( STR_PAGE ) );
            aPageEntry += ' ';
            aWndRight.GetEditEngine()->SetText(aPageEntry);
            aWndRight.InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
            if(!bTravelling)
                aWndRight.GrabFocus();
        }
        break;

        default :
            break;
    }
}

void ScHFEditPage::ClearTextAreas()
{
    aWndLeft.GetEditEngine()->SetText(EMPTY_STRING);
    aWndLeft.Invalidate();
    aWndCenter.GetEditEngine()->SetText(EMPTY_STRING);
    aWndCenter.Invalidate();
    aWndRight.GetEditEngine()->SetText(EMPTY_STRING);
    aWndRight.Invalidate();
}

//-----------------------------------------------------------------------
// Handler:
//-----------------------------------------------------------------------

IMPL_LINK( ScHFEditPage, ListHdl_Impl, ListBox*, pList )
{
    if ( pList && pList == &maLbDefined )
    {
        ScHFEntryId eSel = static_cast<ScHFEntryId>(maLbDefined.GetSelectEntryPos());
        if(!maLbDefined.IsTravelSelect())
        {
            ProcessDefinedListSel(eSel);

            // check if we need to remove the customized entry.
            if(eSel < eEntryCount)
                RemoveFromDefinedList();
        }
        else
        {
            ProcessDefinedListSel(eSel, true);
        }
    }
    return 0;
}

IMPL_LINK( ScHFEditPage, ClickHdl, ImageButton*, pBtn )
{
	pActiveEdWnd = ::GetScEditWindow(); //CHINA001
	if ( !pActiveEdWnd )
		return 0;

	if ( pBtn == &aBtnText )
	{
		pActiveEdWnd->SetCharAttriutes();
	}
	else
	{
		if ( pBtn == &aBtnPage )
            pActiveEdWnd->InsertField( SvxFieldItem(SvxPageField(), EE_FEATURE_FIELD) );
		else if ( pBtn == &aBtnLastPage )
            pActiveEdWnd->InsertField( SvxFieldItem(SvxPagesField(), EE_FEATURE_FIELD) );
		else if ( pBtn == &aBtnDate )
            pActiveEdWnd->InsertField( SvxFieldItem(SvxDateField(Date(),SVXDATETYPE_VAR), EE_FEATURE_FIELD) );
		else if ( pBtn == &aBtnTime )
            pActiveEdWnd->InsertField( SvxFieldItem(SvxTimeField(), EE_FEATURE_FIELD) );
		else if ( pBtn == &aBtnFile )
		{
            pActiveEdWnd->InsertField( SvxFieldItem( SvxFileField(), EE_FEATURE_FIELD ) );
		}
		else if ( pBtn == &aBtnTable )
            pActiveEdWnd->InsertField( SvxFieldItem(SvxTableField(), EE_FEATURE_FIELD) );
	}
	InsertToDefinedList();
	pActiveEdWnd->GrabFocus();

	return 0;
}

IMPL_LINK( ScHFEditPage, MenuHdl, ScExtIButton*, pBtn )
{
	pActiveEdWnd = ::GetScEditWindow(); //CHINA001
	if ( !pActiveEdWnd )
		return 0;

	if(pBtn!=NULL)
	{
		switch(pBtn->GetSelected())
		{
			case FILE_COMMAND_TITEL:
                pActiveEdWnd->InsertField( SvxFieldItem( SvxFileField(), EE_FEATURE_FIELD ) );
				break;
			case FILE_COMMAND_FILENAME:
				pActiveEdWnd->InsertField( SvxFieldItem( SvxExtFileField(
                        EMPTY_STRING, SVXFILETYPE_VAR, SVXFILEFORMAT_NAME_EXT ), EE_FEATURE_FIELD ) );
				break;
			case FILE_COMMAND_PATH:
				pActiveEdWnd->InsertField( SvxFieldItem( SvxExtFileField(
                        EMPTY_STRING, SVXFILETYPE_VAR, SVXFILEFORMAT_FULLPATH ), EE_FEATURE_FIELD ) );
				break;
		}
	}
	return 0;
}

//========================================================================
// class ScRightHeaderEditPage
//========================================================================

ScRightHeaderEditPage::ScRightHeaderEditPage( Window* pParent, const SfxItemSet& rCoreSet )
	: ScHFEditPage( pParent, RID_SCPAGE_HFED_HR, rCoreSet,
					rCoreSet.GetPool()->GetWhich(SID_SCATTR_PAGE_HEADERRIGHT ), true )
	{}

// -----------------------------------------------------------------------

sal_uInt16* __EXPORT ScRightHeaderEditPage::GetRanges()
	{ return pPageRightHeaderRanges; }

// -----------------------------------------------------------------------

SfxTabPage* __EXPORT ScRightHeaderEditPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
	{ return ( new ScRightHeaderEditPage( pParent, rCoreSet ) ); };


//========================================================================
// class ScLeftHeaderEditPage
//========================================================================

ScLeftHeaderEditPage::ScLeftHeaderEditPage( Window* pParent, const SfxItemSet& rCoreSet )
	: ScHFEditPage( pParent, RID_SCPAGE_HFED_HL, rCoreSet,
					rCoreSet.GetPool()->GetWhich(SID_SCATTR_PAGE_HEADERLEFT ), true )
	{}

// -----------------------------------------------------------------------

sal_uInt16* __EXPORT ScLeftHeaderEditPage::GetRanges()
	{ return pPageLeftHeaderRanges; }

// -----------------------------------------------------------------------

SfxTabPage* __EXPORT ScLeftHeaderEditPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
	{ return ( new ScLeftHeaderEditPage( pParent, rCoreSet ) ); };

//========================================================================
// class ScRightFooterEditPage
//========================================================================

ScRightFooterEditPage::ScRightFooterEditPage( Window* pParent, const SfxItemSet& rCoreSet )
	: ScHFEditPage( pParent, RID_SCPAGE_HFED_FR, rCoreSet,
					rCoreSet.GetPool()->GetWhich(SID_SCATTR_PAGE_FOOTERRIGHT ), false )
	{}

// -----------------------------------------------------------------------

sal_uInt16* __EXPORT ScRightFooterEditPage::GetRanges()
	{ return pPageRightFooterRanges; }

// -----------------------------------------------------------------------

SfxTabPage* __EXPORT ScRightFooterEditPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
	{ return ( new ScRightFooterEditPage( pParent, rCoreSet ) ); };

//========================================================================
// class ScLeftFooterEditPage
//========================================================================

ScLeftFooterEditPage::ScLeftFooterEditPage( Window* pParent, const SfxItemSet& rCoreSet )
	: ScHFEditPage( pParent, RID_SCPAGE_HFED_FL, rCoreSet,
					rCoreSet.GetPool()->GetWhich(SID_SCATTR_PAGE_FOOTERLEFT ), false )
	{}

// -----------------------------------------------------------------------

sal_uInt16* __EXPORT ScLeftFooterEditPage::GetRanges()
	{ return pPageLeftFooterRanges; }

// -----------------------------------------------------------------------

SfxTabPage* __EXPORT ScLeftFooterEditPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
	{ return ( new ScLeftFooterEditPage( pParent, rCoreSet ) ); };
