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



#ifndef SC_HTMLEXP_HXX
#define SC_HTMLEXP_HXX

#include "global.hxx"
#include <rtl/textenc.h>
#include <tools/gen.hxx>
#include <tools/color.hxx>

#include "expbase.hxx"


class ScDocument;
class SfxItemSet;
class SdrPage;
class Graphic;
class SdrObject;
class OutputDevice;
class ScDrawLayer;
class SvStringsSortDtor;
class ScEditCell;
class SvxBorderLine;

struct ScHTMLStyle
{	// Defaults aus StyleSheet
	Color				aBackgroundColor;
	String 				aFontFamilyName;
	sal_uInt32				nFontHeight;		// Item-Value
    sal_uInt16              nFontSizeNumber;    // HTML value 1-7
    sal_uInt8                nDefaultScriptType; // Font values are valid for the default script type
    sal_Bool                bInitialized;

    ScHTMLStyle() : nFontHeight(0), nFontSizeNumber(2), nDefaultScriptType(0),
        bInitialized(0) {}

	const ScHTMLStyle& operator=( const ScHTMLStyle& r )
		{
			aBackgroundColor	= r.aBackgroundColor;
			aFontFamilyName		= r.aFontFamilyName;
			nFontHeight			= r.nFontHeight;
            nFontSizeNumber     = r.nFontSizeNumber;
            nDefaultScriptType  = r.nDefaultScriptType;
            bInitialized        = r.bInitialized;
			return *this;
		}
};

struct ScHTMLGraphEntry
{
	ScRange				aRange;			// ueberlagerter Zellbereich
	Size				aSize;			// Groesse in Pixeln
	Size				aSpace;			// Spacing in Pixeln
	SdrObject*			pObject;
	sal_Bool				bInCell;		// ob in Zelle ausgegeben wird
	sal_Bool				bWritten;

	ScHTMLGraphEntry( SdrObject* pObj, const ScRange& rRange,
		const Size& rSize, 	sal_Bool bIn, const Size& rSpace ) :
        aRange( rRange ), aSize( rSize ), aSpace( rSpace ),
        pObject( pObj ), bInCell( bIn ), bWritten( sal_False ) {}
};

DECLARE_LIST( ScHTMLGraphList, ScHTMLGraphEntry* )

#define SC_HTML_FONTSIZES 7
const short nIndentMax = 23;

class ScHTMLExport : public ScExportBase
{
	// default HtmlFontSz[1-7]
	static const sal_uInt16	nDefaultFontSize[SC_HTML_FONTSIZES];
	// HtmlFontSz[1-7] in s*3.ini [user]
	static sal_uInt16		nFontSize[SC_HTML_FONTSIZES];
	static const char*	pFontSizeCss[SC_HTML_FONTSIZES];
	static const sal_uInt16	nCellSpacing;
	static const sal_Char __FAR_DATA sIndentSource[];

	ScHTMLGraphList		aGraphList;
	ScHTMLStyle			aHTMLStyle;
    String              aBaseURL;
	String				aStreamPath;
	String				aCId;			// Content-Id fuer Mail-Export
	OutputDevice*		pAppWin;		// fuer Pixelei
	SvStringsSortDtor*	pSrcArr;		// fuer CopyLocalFileToINet
	SvStringsSortDtor*	pDestArr;
    String              aNonConvertibleChars;   // collect nonconvertible characters
	rtl_TextEncoding	eDestEnc;
	SCTAB				nUsedTables;
	short				nIndent;
	sal_Char			sIndent[nIndentMax+1];
	sal_Bool				bAll;			// ganzes Dokument
	sal_Bool				bTabHasGraphics;
	sal_Bool				bTabAlignedLeft;
	sal_Bool				bCalcAsShown;
	sal_Bool				bCopyLocalFileToINet;
    sal_Bool                bTableDataWidth;
    sal_Bool                bTableDataHeight;

	const SfxItemSet&	PageDefaults( SCTAB nTab );

    void                WriteBody();
	void 				WriteHeader();
	void 				WriteOverview();
	void 				WriteTables();
	void 				WriteCell( SCCOL nCol, SCROW nRow, SCTAB nTab );
	void				WriteGraphEntry( ScHTMLGraphEntry* );
    void                WriteImage( String& rLinkName,
									const Graphic&, const ByteString& rImgOptions,
									sal_uLong nXOutFlags = 0 );
							// nXOutFlags fuer XOutBitmap::WriteGraphic

						// write to stream if and only if URL fields in edit cell
	sal_Bool				WriteFieldText( const ScEditCell* pCell );

						// kopiere ggfs. eine lokale Datei ins Internet
	sal_Bool 				CopyLocalFileToINet( String& rFileNm,
							const String& rTargetNm, sal_Bool bFileToFile = sal_False );
	sal_Bool				HasCId() { return aCId.Len() > 0; }
	void				MakeCIdURL( String& rURL );

	void				PrepareGraphics( ScDrawLayer*, SCTAB nTab,
										SCCOL nStartCol, SCROW nStartRow,
										SCCOL nEndCol, SCROW nEndRow );
	void				FillGraphList( const SdrPage*, SCTAB nTab,
										SCCOL nStartCol, SCROW nStartRow,
										SCCOL nEndCol, SCROW nEndRow );

	void				BorderToStyle( ByteString& rOut, const char* pBorderName,
									   const SvxBorderLine* pLine, bool& bInsertSemicolon );

	sal_uInt16				GetFontSizeNumber( sal_uInt16 nHeight );
	const char*			GetFontSizeCss( sal_uInt16 nHeight );
	sal_uInt16				ToPixel( sal_uInt16 nTwips );
	Size				MMToPixel( const Size& r100thMMSize );
	void				IncIndent( short nVal );
	const sal_Char*			GetIndentStr() { return sIndent; }

public:
                        ScHTMLExport( SvStream&, const String&, ScDocument*, const ScRange&,
										sal_Bool bAll, const String& aStreamPath );
	virtual				~ScHTMLExport();
    sal_uLong               Write();
    const String&       GetNonConvertibleChars() const
                            { return aNonConvertibleChars; }
};

#endif
