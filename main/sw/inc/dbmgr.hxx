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


#ifndef _SWDBMGR_HXX
#define _SWDBMGR_HXX


#include <tools/string.hxx>
#include <tools/link.hxx>
#include <svl/svarray.hxx>
#include <com/sun/star/util/Date.hpp>
#include "swdllapi.h"
#include <swdbdata.hxx>
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/lang/Locale.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
namespace com{namespace sun{namespace star{
	namespace sdbc{
		class XConnection;
		class XStatement;
		class XDataSource;
		class XResultSet;
	}
	namespace beans{

		class XPropertySet;
		struct PropertyValue;
	}
	namespace sdbcx{
		class XColumnsSupplier;
	}
	namespace util{
		class XNumberFormatter;
	}
    namespace mail{
        class XSmtpService;
    }
}}}
namespace svx {
	class ODataAccessDescriptor;
}

struct SwDBFormatData
{
	com::sun::star::util::Date aNullDate;
	com::sun::star::uno::Reference< com::sun::star::util::XNumberFormatter> xFormatter;
	com::sun::star::lang::Locale aLocale;
};

class SwView;
class SwWrtShell;
class SfxProgress;
class ListBox;
class Button;
class SvNumberFormatter;
class SwDbtoolsClient;
class SwXMailMerge;
class SwMailMergeConfigItem;

// -----------------------------------------------------------------------

enum DBMgrOptions
{
	DBMGR_MERGE,			// Datensaetze in Felder
	DBMGR_INSERT,			// Datensaetze in Text
	DBMGR_MERGE_MAILMERGE,	// Serienbriefe drucken
	DBMGR_MERGE_MAILING,	// Serienbriefe als email versenden
    DBMGR_MERGE_MAILFILES,  // Serienbriefe als Datei(en) speichern
    DBMGR_MERGE_DOCUMENTS,   // Print merged documents
    DBMGR_MERGE_SINGLE_FILE  // save merge as single file
};

// -----------------------------------------------------------------------

/*--------------------------------------------------------------------
	 Beschreibung: (neue) Logische Datenbanken verwalten
 --------------------------------------------------------------------*/
#define SW_DB_SELECT_UNKNOWN 	0
#define SW_DB_SELECT_TABLE		1
#define SW_DB_SELECT_QUERY      2

struct SwDSParam : public SwDBData
{
	com::sun::star::util::Date 	aNullDate;

	::com::sun::star::uno::Reference<com::sun::star::util::XNumberFormatter> 	xFormatter;
	::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> 		xConnection;
	::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XStatement>		xStatement;
	::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet>   	xResultSet;
	::com::sun::star::uno::Sequence<  ::com::sun::star::uno::Any > 				aSelection;
	sal_Bool bScrollable;
	sal_Bool bEndOfDB;
	sal_Bool bAfterSelection;
	long nSelectionIndex;

    SwDSParam(const SwDBData& rData) :
        SwDBData(rData),
        bScrollable(sal_False),
		bEndOfDB(sal_False),
		bAfterSelection(sal_False),
		nSelectionIndex(0)
		{}

    SwDSParam(const SwDBData& rData,
		const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet>&   	xResSet,
		const ::com::sun::star::uno::Sequence<  ::com::sun::star::uno::Any >& 	rSelection) :
        SwDBData(rData),
		xResultSet(xResSet),
		aSelection(rSelection),
		bScrollable(sal_True),
		bEndOfDB(sal_False),
		bAfterSelection(sal_False),
		nSelectionIndex(0)
		{}

		void CheckEndOfDB()
		{
			if(bEndOfDB)
				bAfterSelection = sal_True;
		}
};
typedef SwDSParam* SwDSParamPtr;
SV_DECL_PTRARR_DEL(SwDSParamArr, SwDSParamPtr, 0, 5)

struct SwMergeDescriptor
{
    sal_uInt16                                              nMergeType;
    SwWrtShell&                                         rSh;
    const ::svx::ODataAccessDescriptor&                 rDescriptor;
    String                                              sSaveToFilter; //export filter to save resulting files
    String                                              sSaveToFilterOptions; //
    com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue > aSaveToFilterData;

    String                                              sSubject;
    String                                              sAddressFromColumn;
    String                                              sMailBody;
    String                                              sAttachmentName;
    ::com::sun::star::uno::Sequence< ::rtl::OUString >  aCopiesTo;
    ::com::sun::star::uno::Sequence< ::rtl::OUString >  aBlindCopiesTo;

    ::com::sun::star::uno::Reference< com::sun::star::mail::XSmtpService > xSmtpServer;

    sal_Bool                                            bSendAsHTML;
    sal_Bool                                            bSendAsAttachment;

    sal_Bool                                            bPrintAsync;
    sal_Bool                                            bCreateSingleFile;

    SwMailMergeConfigItem*                              pMailMergeConfigItem;

    ::com::sun::star::uno::Sequence<  ::com::sun::star::beans::PropertyValue >  aPrintOptions;

    SwMergeDescriptor( sal_uInt16 nType, SwWrtShell& rShell, ::svx::ODataAccessDescriptor& rDesc ) :
        nMergeType(nType),
        rSh(rShell),
        rDescriptor(rDesc),
        bSendAsHTML( sal_True ),
        bSendAsAttachment( sal_False ),
        bPrintAsync( sal_False ),
        bCreateSingleFile( sal_False ),
        pMailMergeConfigItem(0)
        {}

};

struct SwNewDBMgr_Impl;
class SwConnectionDisposedListener_Impl;
class AbstractMailMergeDlg;

class SW_DLLPUBLIC SwNewDBMgr
{
friend class SwConnectionDisposedListener_Impl;

    static SwDbtoolsClient* pDbtoolsClient;

    String              sEMailAddrFld;  // Mailing: Spaltenname der E-Mail Adresse
	String				sSubject;		// Mailing: Subject
	String				sAttached;		// Mailing: Attachte Files
	sal_uInt16				nMergeType;
	sal_Bool				bInitDBFields : 1;	// sal_True: Datenbank an Feldern beim Mergen
    sal_Bool                bSingleJobs : 1;    // Einzelne Druckjobs bei Aufruf aus Basic
	sal_Bool				bCancel : 1;		// Serienbrief-Save abgebrochen

	sal_Bool 				bInMerge	: 1;	//merge process active
    sal_Bool                bMergeSilent : 1;   // suppress display of dialogs/boxes (used when called over API)
    sal_Bool                bMergeLock : 1;     // prevent update of database fields while document is
                                            // actually printed at the ViewShell
	SwDSParamArr		aDataSourceParams;
    SwNewDBMgr_Impl*    pImpl;
    const SwXMailMerge* pMergeEvtSrc;   // != 0 if mail merge events are to be send

    SW_DLLPRIVATE SwDSParam*          FindDSData(const SwDBData& rData, sal_Bool bCreate);
    SW_DLLPRIVATE SwDSParam*          FindDSConnection(const ::rtl::OUString& rSource, sal_Bool bCreate);


	SW_DLLPRIVATE DECL_LINK( PrtCancelHdl, Button * );

	// Datensaetze als Text ins Dokument einfuegen
	SW_DLLPRIVATE void ImportFromConnection( SwWrtShell* pSh);

	// Einzelnen Datensatz als Text ins Dokument einfuegen
	SW_DLLPRIVATE void ImportDBEntry(SwWrtShell* pSh);

    // merge to file _and_ merge to e-Mail
    SW_DLLPRIVATE sal_Bool          MergeMailFiles(SwWrtShell* pSh,
                                        const SwMergeDescriptor& rMergeDescriptor );
    SW_DLLPRIVATE sal_Bool          ToNextRecord(SwDSParam* pParam);

public:
	SwNewDBMgr();
	~SwNewDBMgr();

	// Art des aktellen Mergens. Siehe DBMgrOptions-enum
	inline sal_uInt16	GetMergeType() const			{ return nMergeType; }
	inline void 	SetMergeType( sal_uInt16 nTyp ) 	{ nMergeType = nTyp; }

    // MailMergeEvent source
    const SwXMailMerge *    GetMailMergeEvtSrc() const  { return pMergeEvtSrc; }
    void SetMailMergeEvtSrc( const SwXMailMerge *pSrc ) { pMergeEvtSrc = pSrc; }

    inline sal_Bool     IsMergeSilent() const           { return bMergeSilent != 0; }
    inline void     SetMergeSilent( sal_Bool bVal )     { bMergeSilent = bVal; }

	// Mischen von Datensaetzen in Felder
    sal_Bool            MergeNew( const SwMergeDescriptor& rMergeDesc );
    sal_Bool            Merge(SwWrtShell* pSh);

    // Datenbankfelder mit fehlendem Datenbankname initialisieren
	inline sal_Bool 	IsInitDBFields() const 	{ return bInitDBFields;	}
	inline void 	SetInitDBFields(sal_Bool b)	{ bInitDBFields = b;	}

	// Serienbriefe einzelnd oder alle zusammen drucken/speichern
    inline sal_Bool     IsSingleJobs() const    { return bSingleJobs;   }
    inline void     SetSingleJobs(sal_Bool b)   { bSingleJobs = b;  }

	// Mailing
	// email-Daten setzen
	inline void		SetEMailColumn(const String& sColName) { sEMailAddrFld = sColName; }
	inline void		SetSubject(const String& sSbj) { sSubject = sSbj; }
	inline void		SetAttachment(const String& sAtt) { sAttached = sAtt; }


	// Listbox mit allen Tabellennamen einer Datenbank fuellen
	sal_Bool			GetTableNames(ListBox* pListBox, const String& rDBName );

	// Listbox mit allen Spaltennamen einer Datenbanktabelle fuellen
	sal_Bool			GetColumnNames(ListBox* pListBox,
						const String& rDBName, const String& rTableName, sal_Bool bAppend = sal_False);
    sal_Bool            GetColumnNames(ListBox* pListBox,
                        ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> xConnection,
                        const String& rTableName, sal_Bool bAppend = sal_False);

    sal_uLong GetColumnFmt( ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource> xSource,
                        ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> xConnection,
                        ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet> xColumn,
						SvNumberFormatter* pNFmtr,
						long nLanguage );

    sal_uLong GetColumnFmt( const String& rDBName,
						const String& rTableName,
						const String& rColNm,
						SvNumberFormatter* pNFmtr,
						long nLanguage );
	sal_Int32 GetColumnType( const String& rDBName,
						  const String& rTableName,
						  const String& rColNm );

	inline sal_Bool		IsInMerge() const	{ return bInMerge; }
	void			EndMerge();

	void 			ExecuteFormLetter(SwWrtShell& rSh,
                        const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& rProperties,
                        sal_Bool bWithDataSourceBrowser = sal_False);

	void 			InsertText(SwWrtShell& rSh,
						const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue>& rProperties);

    // check if a data source is open
    sal_Bool            IsDataSourceOpen(const String& rDataSource,
                                    const String& rTableOrQuery, sal_Bool bMergeOnly);

	// open the source while fields are updated - for the calculator only!
    sal_Bool            OpenDataSource(const String& rDataSource, const String& rTableOrQuery,
                        sal_Int32 nCommandType = -1, bool bCreate = false);
    sal_uInt32      GetSelectedRecordId(const String& rDataSource, const String& rTableOrQuery, sal_Int32 nCommandType = -1);
	sal_Bool			GetColumnCnt(const String& rSourceName, const String& rTableName,
							const String& rColumnName, sal_uInt32 nAbsRecordId, long nLanguage,
							String& rResult, double* pNumber);
    //create and store or find an already stored connection to a data source for use
    //in SwFldMgr and SwDBTreeList
    ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>
                    RegisterConnection(::rtl::OUString& rSource);

    const SwDSParam* CreateDSData(const SwDBData& rData)
                        {return FindDSData(rData, sal_True);}
	const SwDSParamArr& GetDSParamArray() const {return aDataSourceParams;}


	//close all data sources - after fields were updated
	void			CloseAll(sal_Bool bIncludingMerge = sal_True);

	sal_Bool			GetMergeColumnCnt(const String& rColumnName, sal_uInt16 nLanguage,
								String &rResult, double *pNumber, sal_uInt32 *pFormat);
	sal_Bool			ToNextMergeRecord();
    sal_Bool            ToNextRecord(const String& rDataSource, const String& rTableOrQuery, sal_Int32 nCommandType = -1);

	sal_Bool			ExistsNextRecord()const;
	sal_uInt32 		GetSelectedRecordId();
	sal_Bool 		ToRecordId(sal_Int32 nSet);

	const SwDBData&	GetAddressDBName();

	static String GetDBField(
					::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > xColumnProp,
					const SwDBFormatData& rDBFormatData,
					double *pNumber = NULL);

	static ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>
			GetConnection(const String& rDataSource,
				::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource>& rxSource);

	static ::com::sun::star::uno::Reference< ::com::sun::star::sdbcx::XColumnsSupplier>
			GetColumnSupplier(::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>,
									const String& rTableOrQuery,
									sal_uInt8	eTableOrQuery = SW_DB_SELECT_UNKNOWN);

	static ::com::sun::star::uno::Sequence<rtl::OUString> GetExistingDatabaseNames();

    /**
     Loads a data source from file and registers it. Returns the registered name.
     */
    static String               LoadAndRegisterDataSource();

    static SwDbtoolsClient&    GetDbtoolsClient();
    // has to be called from _FinitUI()
    static void                RemoveDbtoolsClient();

	/** try to get the data source from the given connection through the XChild interface.
		If this is not possible, the data source will be created through its name.
		@param _xConnection
			The connection which should support the XChild interface. (not a must)
		@param _sDataSourceName
			The data source name will be used to create the data source when the connection can not be used for it.
		@return
			The data source.
	*/
	static ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource>
			getDataSourceAsParent(const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& _xConnection,const ::rtl::OUString& _sDataSourceName);

	/** creates a RowSet, which must be disposed after use.
		@param	_sDataSourceName
			The data source name
		@param	_sCommand
			The command.
		@param	_nCommandType
			The type of the command.
		@param	_xConnection
			The active connection which may be <NULL/>.
		@return
			The new created RowSet.

	*/
	static ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet>
			createCursor(	const ::rtl::OUString& _sDataSourceName,
							const ::rtl::OUString& _sCommand,
							sal_Int32 _nCommandType,
							const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& _xConnection
							);
    //merge into one document - returns the number of merged docs
    sal_Int32 MergeDocuments( SwMailMergeConfigItem& rMMConfig, SwView& rSourceView );
};

#endif
