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


#ifndef _SWFLDDB_HXX
#define _SWFLDDB_HXX

#include <vcl/fixed.hxx>
#include <vcl/lstbox.hxx>
#ifndef _SV_BUTTON_HXX //autogen
#include <vcl/button.hxx>
#endif
#include <vcl/group.hxx>

#include "condedit.hxx"
#include "dbtree.hxx"
#include "numfmtlb.hxx"

#include "fldpage.hxx"

/*--------------------------------------------------------------------
   Beschreibung:
 --------------------------------------------------------------------*/

class SwFldDBPage : public SwFldPage
{
	FixedText 			aTypeFT;
	ListBox 			aTypeLB;
	FixedText 			aSelectionFT;
	SwDBTreeList		aDatabaseTLB;

    FixedText           aAddDBFT;
    PushButton          aAddDBPB;
            
	FixedText 			aConditionFT;
	ConditionEdit		aConditionED;
	FixedText 			aValueFT;
	Edit 				aValueED;
	RadioButton			aDBFormatRB;
	RadioButton			aNewFormatRB;
	NumFormatListBox	aNumFormatLB;
	ListBox				aFormatLB;
    FixedLine           aFormatFL;
    FixedLine           aFormatVertFL;

	String				sOldDBName;
	String				sOldTableName;
	String				sOldColumnName;
	sal_uLong				nOldFormat;
	sal_uInt16				nOldSubType;
	Link				aOldNumSelectHdl;

	DECL_LINK( TypeHdl, ListBox* );
	DECL_LINK( NumSelectHdl, NumFormatListBox* pLB = 0);
	DECL_LINK( TreeSelectHdl, SvTreeListBox* pBox );
	DECL_LINK( ModifyHdl, Edit *pED = 0 );
    DECL_LINK( AddDBHdl, PushButton* );

	void				CheckInsert();

    using SwFldPage::SetWrtShell;

protected:
	virtual sal_uInt16		GetGroup();

public:
						SwFldDBPage(Window* pParent, const SfxItemSet& rSet);

						~SwFldDBPage();

	static SfxTabPage*  Create(Window* pParent, const SfxItemSet& rAttrSet);

	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );
	virtual void        Reset( const SfxItemSet& rSet );

	virtual void		FillUserData();
    void                ActivateMailMergeAddress();
    
    void                SetWrtShell(SwWrtShell& rSh);
};


#endif
