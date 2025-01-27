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



#ifndef SC_RANGEUTL_HXX
#define SC_RANGEUTL_HXX

#include "address.hxx"
#include <tools/string.hxx>
#include "scdllapi.h"
#include <com/sun/star/table/CellAddress.hpp>
#include <com/sun/star/table/CellRangeAddress.hpp>
#include <com/sun/star/uno/Sequence.hxx>

//------------------------------------------------------------------------

class SvStream;

class ScArea;
class ScDocument;
class ScRange;
class ScRangeName;
class ScRangeList;
class ScDBCollection;

enum RutlNameScope { RUTL_NONE=0, RUTL_NAMES, RUTL_DBASE };

//------------------------------------------------------------------------

class SC_DLLPUBLIC ScRangeUtil
{
public:
				ScRangeUtil()  {}
				~ScRangeUtil() {}

	sal_Bool	MakeArea			( const String&		rAreaStr,
								  ScArea&			rArea,
								  ScDocument*		pDoc,
								  SCTAB			nTab,
								  ScAddress::Details const & rDetails = ScAddress::detailsOOOa1 ) const;

	void	CutPosString		( const String&		theAreaStr,
								  String&			thePosStr ) const;

	sal_Bool 	IsAbsTabArea		( const String& 	rAreaStr,
								  ScDocument*		pDoc,
								  ScArea***			pppAreas 	= 0,
								  sal_uInt16*			pAreaCount	= 0,
								  sal_Bool				bAcceptCellRef = sal_False,
								  ScAddress::Details const & rDetails = ScAddress::detailsOOOa1 ) const;

	sal_Bool	IsAbsArea			( const String& rAreaStr,
								  ScDocument*	pDoc,
								  SCTAB		nTab,
								  String*		pCompleteStr = 0,
								  ScRefAddress*	pStartPos	 = 0,
								  ScRefAddress*	pEndPos		 = 0,
								  ScAddress::Details const & rDetails = ScAddress::detailsOOOa1 ) const;

    sal_Bool    IsRefArea           ( const String&,
                                  ScDocument*,
                                  SCTAB,
                                  String* = 0,
                                  ScRefAddress* = 0 ) const
									  { return sal_False; }

	sal_Bool	IsAbsPos			( const String&	rPosStr,
								  ScDocument*	pDoc,
								  SCTAB		nTab,
								  String*		pCompleteStr = 0,
								  ScRefAddress*	pPosTripel	 = 0,
								  ScAddress::Details const & rDetails = ScAddress::detailsOOOa1 ) const;

	sal_Bool	MakeRangeFromName	( const String&	rName,
									ScDocument*		pDoc,
									SCTAB			nCurTab,
									ScRange&		rRange,
								  RutlNameScope eScope=RUTL_NAMES,
								  ScAddress::Details const & rDetails = ScAddress::detailsOOOa1 ) const;
};

//------------------------------------------------------------------------

class SC_DLLPUBLIC ScRangeStringConverter
{
public:

// helper methods
	static void			AssignString(
							::rtl::OUString& rString,
							const ::rtl::OUString& rNewStr,
							sal_Bool bAppendStr,
                            sal_Unicode cSeperator = ' ');

	static sal_Int32	IndexOf(
							const ::rtl::OUString& rString,
							sal_Unicode cSearchChar,
							sal_Int32 nOffset,
							sal_Unicode cQuote = '\'');

	static sal_Int32	IndexOfDifferent(
							const ::rtl::OUString& rString,
							sal_Unicode cSearchChar,
							sal_Int32 nOffset );

    static sal_Int32	GetTokenCount(
							const ::rtl::OUString& rString,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');

    static void			GetTokenByOffset(
							::rtl::OUString& rToken,
							const ::rtl::OUString& rString,
							sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
							sal_Unicode cQuote = '\'');

    static void         AppendTableName(
                            ::rtl::OUStringBuffer& rBuf, 
                            const ::rtl::OUString& rTabName,
                            sal_Unicode cQuote = '\'');

// String to Range core
	static sal_Bool		GetAddressFromString(
							ScAddress& rAddress,
							const ::rtl::OUString& rAddressStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');
	static sal_Bool		GetRangeFromString(
							ScRange& rRange,
							const ::rtl::OUString& rRangeStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
							sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');
	static sal_Bool		GetRangeListFromString(
							ScRangeList& rRangeList,
							const ::rtl::OUString& rRangeListStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');

	static sal_Bool		GetAreaFromString(
							ScArea& rArea,
							const ::rtl::OUString& rRangeStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
							sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');

// String to Range API
	static sal_Bool		GetAddressFromString(
							::com::sun::star::table::CellAddress& rAddress,
							const ::rtl::OUString& rAddressStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
							sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');
	static sal_Bool		GetRangeFromString(
							::com::sun::star::table::CellRangeAddress& rRange,
							const ::rtl::OUString& rRangeStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
							sal_Int32& nOffset,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');
	static sal_Bool		GetRangeListFromString(
							::com::sun::star::uno::Sequence< ::com::sun::star::table::CellRangeAddress >& rRangeSeq,
							const ::rtl::OUString& rRangeListStr,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
                            sal_Unicode cQuote = '\'');

// Range to String core
	static void			GetStringFromAddress(
							::rtl::OUString& rString,
							const ScAddress& rAddress,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_Bool bAppendStr = sal_False,
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );
	static void			GetStringFromRange(
							::rtl::OUString& rString,
							const ScRange& rRange,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_Bool bAppendStr = sal_False,
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );
	static void			GetStringFromRangeList(
							::rtl::OUString& rString,
							const ScRangeList* pRangeList,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D));

	static void			GetStringFromArea(
							::rtl::OUString& rString,
							const ScArea& rArea,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_Bool bAppendStr = sal_False,
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );

// Range to String API
	static void			GetStringFromAddress(
							::rtl::OUString& rString,
							const ::com::sun::star::table::CellAddress& rAddress,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_Bool bAppendStr = sal_False,
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );
	static void			GetStringFromRange(
							::rtl::OUString& rString,
							const ::com::sun::star::table::CellRangeAddress& rRange,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_Bool bAppendStr = sal_False,
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );
	static void			GetStringFromRangeList(
							::rtl::OUString& rString,
							const ::com::sun::star::uno::Sequence< ::com::sun::star::table::CellRangeAddress >& rRangeSeq,
							const ScDocument* pDocument,
                            formula::FormulaGrammar::AddressConvention eConv,
                            sal_Unicode cSeperator = ' ',
							sal_uInt16 nFormatFlags = (SCA_VALID | SCA_TAB_3D) );

// XML Range to Calc Range
    static void         GetStringFromXMLRangeString(
                            ::rtl::OUString& rString,
                            const ::rtl::OUString& rXMLRange, 
                            ScDocument* pDoc );
};

//------------------------------------------------------------------------

class ScArea
{
public:
			ScArea( SCTAB tab      = 0,
					SCCOL colStart = 0,
					SCROW rowStart = 0,
					SCCOL colEnd   = 0,
					SCROW rowEnd   = 0 );

			ScArea( const ScArea& r );

	ScArea&	operator=	( const ScArea& r );
	sal_Bool	operator==	( const ScArea& r ) const;
	sal_Bool	operator!=	( const ScArea& r ) const  { return !( operator==(r) ); }

public:
	SCTAB nTab;
	SCCOL nColStart;
	SCROW nRowStart;
	SCCOL nColEnd;
	SCROW nRowEnd;
};

//------------------------------------------------------------------------

//
//	gibt Bereiche mit Referenz und alle DB-Bereiche zurueck
//

class SC_DLLPUBLIC ScAreaNameIterator
{
private:
	ScRangeName*	pRangeName;
	ScDBCollection*	pDBCollection;
	sal_Bool			bFirstPass;
	sal_uInt16			nPos;

public:
			ScAreaNameIterator( ScDocument* pDoc );
			~ScAreaNameIterator() {}

	sal_Bool	Next( String& rName, ScRange& rRange );
	sal_Bool	WasDBName() const	{ return !bFirstPass; }
};


#endif // SC_RANGEUTL_HXX
