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


#ifndef DBUI_TABLECOPYHELPER_HXX
#define DBUI_TABLECOPYHELPER_HXX

#ifndef DBAUI_APPELEMENTTYPE_HXX
#include "AppElementType.hxx"
#endif
#ifndef _DBAUI_COMMON_TYPES_HXX_
#include "commontypes.hxx"
#endif
#ifndef _SVX_DATACCESSDESCRIPTOR_HXX_
#include <svx/dataaccessdescriptor.hxx>
#endif
#ifndef _SOT_STORAGE_HXX
#include <sot/storage.hxx>
#endif
#ifndef _TRANSFER_HXX
#include <svtools/transfer.hxx>
#endif
#ifndef _COM_SUN_STAR_SDBC_XCONNECTION_HPP_
#include <com/sun/star/sdbc/XConnection.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XRESULTSET_HPP_
#include <com/sun/star/sdbc/XResultSet.hpp>
#endif
#include <functional>

class SvLBoxEntry;
//........................................................................
namespace dbaui
{
//........................................................................
    class OGenericUnoController;
    /// unary_function Functor object for class DataFlavorExVector::value_type returntype is bool
    struct TAppSupportedSotFunctor : ::std::unary_function<DataFlavorExVector::value_type,bool>
    {
	    ElementType	eEntryType;
	    sal_Bool	bQueryDrop;
	    TAppSupportedSotFunctor(const ElementType& _eEntryType,sal_Bool _bQueryDrop)
		    : eEntryType(_eEntryType)
		    , bQueryDrop(_bQueryDrop)
	    {
	    }

	    inline bool operator()(const DataFlavorExVector::value_type& _aType)
	    {
		    switch (_aType.mnSotId)
		    {
			    case SOT_FORMAT_RTF:					// RTF data descriptions
			    case SOT_FORMATSTR_ID_HTML:				// HTML data descriptions
			    case SOT_FORMATSTR_ID_DBACCESS_TABLE:	// table descriptor
				    return (E_TABLE == eEntryType);
			    case SOT_FORMATSTR_ID_DBACCESS_QUERY:	// query descriptor
			    case SOT_FORMATSTR_ID_DBACCESS_COMMAND:	// SQL command
				    return ((E_QUERY == eEntryType) || ( !bQueryDrop && E_TABLE == eEntryType));
		    }
		    return false;
	    }
    };

    class OTableCopyHelper
    {
	private:
        OGenericUnoController*  m_pController;
		::rtl::OUString		    m_sTableNameForAppend;

    public:
        // is needed to describe the drop target
		struct DropDescriptor
		{
			::svx::ODataAccessDescriptor	aDroppedData;
			//dyf add 20070601
			//for transfor the tablename
			::rtl::OUString                 sDefaultTableName;
			//dyf add end
			String							aUrl;
			SotStorageStreamRef				aHtmlRtfStorage;
			ElementType						nType;
            SvLBoxEntry*                    pDroppedAt;
			sal_Int8						nAction;
			sal_Bool						bHtml;
			sal_Bool						bError;

			DropDescriptor() : nType(E_TABLE),pDroppedAt(NULL),nAction(DND_ACTION_NONE) { }
		};

        OTableCopyHelper(OGenericUnoController* _pControler);

        /** pastes a table into the data source
			@param	_rPasteData
				The data helper.
            @param	_sDestDataSourceName
				The name of the dest data source.
		*/
		void pasteTable( const TransferableDataHelper& _rTransData
                        ,const ::rtl::OUString& _sDestDataSourceName
                        ,const SharedConnection& _xConnection);

        /** pastes a table into the data source
			@param	_nFormatId
				The format which should be copied.
			@param	_rPasteData
				The data helper.
            @param	_sDestDataSourceName
				The name of the dest data source.
		*/
		void pasteTable( SotFormatStringId _nFormatId
                        ,const TransferableDataHelper& _rTransData
                        ,const ::rtl::OUString& _sDestDataSourceName
                        ,const SharedConnection& _xConnection);

		/** copies a table which was constructed by tags like HTML or RTF
			@param	_rDesc
				The Drop descriptor
			@param	_bCheck
				If set to <TRUE/> than the controller checks only if a copy is possible.
            @param  _xConnection
                The connection
		*/
		sal_Bool copyTagTable(  DropDescriptor& _rDesc,
                                sal_Bool _bCheck,
                                const SharedConnection& _xConnection);

        /** copies a table which was constructed by tags like HTML or RTF
			@param	_rDesc
				The Drop descriptor
			@param	_bCheck
				If set to <TRUE/> than the controller checks only if a copy is possible.
            @param  _xConnection
                The connection
		*/
		void asyncCopyTagTable(  DropDescriptor& _rDesc
                                ,const ::rtl::OUString& _sDestDataSourceName
                                ,const SharedConnection& _xConnection);

        /** copies a table which was constructed by tags like HTML or RTF
            @param  _aDroppedData
                The dropped data
			@param	_rDesc
				IN/OUT parameter
            @param  _xConnection
                The connection
		*/
        sal_Bool copyTagTable(const TransferableDataHelper& _aDroppedData,
                              DropDescriptor& _rAsyncDrop,
                              const SharedConnection& _xConnection);

        /// returns <TRUE/> if the clipboard supports a table format, otherwise <FALSE/>.
		sal_Bool isTableFormat(const TransferableDataHelper& _rClipboard) const;

        inline void                     SetTableNameForAppend( const ::rtl::OUString& _rDefaultTableName ) { m_sTableNameForAppend = _rDefaultTableName; }
        inline void                     ResetTableNameForAppend() { SetTableNameForAppend( ::rtl::OUString() ); }
		inline const ::rtl::OUString&   GetTableNameForAppend() const { return m_sTableNameForAppend ;}

    private:
		/** pastes a table into the data source
			@param	_rPasteData
				The data descriptor.
            @param	_sDestDataSourceName
				The name of the dest data source.
		*/
		void pasteTable(
            const ::svx::ODataAccessDescriptor& _rPasteData,
            const ::rtl::OUString& _sDestDataSourceName,
            const SharedConnection& _xDestConnection
        );

		/** insert a table into the data source. The source can either be a table or a query
		*/
		void insertTable(
			const ::rtl::OUString& i_rSourceDataSource,
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& i_rSourceConnection,
			const ::rtl::OUString& i_rCommand,
            const sal_Int32 i_nCommandType,
            const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet >& i_rSourceRows,
			const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& i_rSelection,
			const sal_Bool i_bBookmarkSelection,
            const ::rtl::OUString& i_rDestDataSource,
            const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& i_rDestConnection
        );

    };
//........................................................................
}	// namespace dbaui
//........................................................................
#endif // DBUI_TABLECOPYHELPER_HXX

