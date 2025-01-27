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



#ifndef SC_TPSUBT_HXX
#define SC_TPSUBT_HXX

#include <sfx2/tabdlg.hxx>
#include <svx/checklbx.hxx>
#include <vcl/fixed.hxx>
#include "global.hxx"

//------------------------------------------------------------------------

// +1 because one field is reserved for the "- none -" entry
#define SC_MAXFIELDS	MAXCOLCOUNT+1

class ScViewData;
class ScDocument;

//========================================================================
// Gruppenseiten: Basisklasse

class ScTpSubTotalGroup : public SfxTabPage
{
protected:
			ScTpSubTotalGroup( Window* pParent, sal_uInt16 nResId,
							   const SfxItemSet& rArgSet );

public:
	virtual ~ScTpSubTotalGroup();

	static sal_uInt16*	GetRanges		();
	sal_Bool			DoReset			( sal_uInt16			nGroupNo,
									  const SfxItemSet&	rArgSet  );
	sal_Bool			DoFillItemSet	( sal_uInt16		nGroupNo,
									  SfxItemSet&	rArgSet  );
protected:
	FixedText		aFtGroup;
	ListBox			aLbGroup;
	FixedText		aFtColumns;
	SvxCheckListBox	aLbColumns;
	FixedText		aFtFunctions;
	ListBox			aLbFunctions;
	const String	aStrNone;
	const String	aStrColumn;

	ScViewData*				pViewData;
	ScDocument*				pDoc;

	const sal_uInt16			nWhichSubTotals;
	const ScSubTotalParam&	rSubTotalData;
	SCCOL					nFieldArr[SC_MAXFIELDS];
	const sal_uInt16			nFieldCount;

private:
	void 			Init			();
	void 			FillListBoxes	();
	ScSubTotalFunc	LbPosToFunc		( sal_uInt16 nPos );
	sal_uInt16			FuncToLbPos		( ScSubTotalFunc eFunc );
	sal_uInt16			GetFieldSelPos	( SCCOL nField );

	// Handler ------------------------
	DECL_LINK( SelectHdl, ListBox * );
	DECL_LINK( CheckHdl, ListBox * );
};

//------------------------------------------------------------------------

class ScTpSubTotalGroup1 : public ScTpSubTotalGroup
{
protected:
			ScTpSubTotalGroup1( Window*				 pParent,
								const SfxItemSet&	 rArgSet );

public:
	virtual ~ScTpSubTotalGroup1();

	static	SfxTabPage*	Create		( Window*				pParent,
									  const SfxItemSet& 	rArgSet );
	virtual	sal_Bool		FillItemSet	( SfxItemSet& rArgSet );
	virtual	void		Reset		( const SfxItemSet& rArgSet );
};

//------------------------------------------------------------------------

class ScTpSubTotalGroup2 : public ScTpSubTotalGroup
{
protected:
			ScTpSubTotalGroup2( Window*				 pParent,
								const SfxItemSet&	 rArgSet );

public:
	virtual ~ScTpSubTotalGroup2();

	static	SfxTabPage*	Create		( Window*				pParent,
									  const SfxItemSet& 	rArgSet );
	virtual	sal_Bool		FillItemSet	( SfxItemSet& rArgSet );
	virtual	void		Reset		( const SfxItemSet& rArgSet );
};

//------------------------------------------------------------------------

class ScTpSubTotalGroup3 : public ScTpSubTotalGroup
{
protected:
			ScTpSubTotalGroup3( Window*				 pParent,
								const SfxItemSet&	 rArgSet );

public:
	virtual ~ScTpSubTotalGroup3();

	static	SfxTabPage*	Create		( Window*				pParent,
									  const SfxItemSet& 	rArgSet );
	virtual	sal_Bool		FillItemSet	( SfxItemSet& rArgSet );
	virtual	void		Reset		( const SfxItemSet& rArgSet );
};

//========================================================================
// Optionen:

class ScTpSubTotalOptions : public SfxTabPage
{
protected:
			ScTpSubTotalOptions( Window*			 pParent,
								  const SfxItemSet&  rArgSet );

public:
	virtual ~ScTpSubTotalOptions();

	static sal_uInt16*		GetRanges	();
	static SfxTabPage*	Create		( Window*				pParent,
									  const SfxItemSet& 	rArgSet );
	virtual	sal_Bool		FillItemSet	( SfxItemSet& rArgSet );
	virtual	void		Reset		( const SfxItemSet& rArgSet );

private:
    FixedLine   aFlGroup;
	CheckBox	aBtnPagebreak;
	CheckBox	aBtnCase;
	CheckBox	aBtnSort;
    FixedLine   aFlSort;
	RadioButton	aBtnAscending;
	RadioButton	aBtnDescending;
	CheckBox	aBtnFormats;
	CheckBox	aBtnUserDef;
	ListBox		aLbUserDef;

	ScViewData*				pViewData;
	ScDocument*				pDoc;
	const sal_uInt16			nWhichSubTotals;
	const ScSubTotalParam&	rSubTotalData;

private:
	void Init					();
	void FillUserSortListBox	();

	// Handler ------------------------
	DECL_LINK( CheckHdl, CheckBox * );
};



#endif // SC_TPSORT_HXX
