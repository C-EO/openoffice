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


#ifndef DBAUI_QUERYDESIGNVIEW_HXX
#define DBAUI_QUERYDESIGNVIEW_HXX

#ifndef DBAUI_QUERYVIEW_HXX
#include "queryview.hxx"
#endif
#ifndef _SV_SPLIT_HXX
#include <vcl/split.hxx>
#endif
#ifndef _STRING_HXX
#include <tools/string.hxx>
#endif
#ifndef DBAUI_ENUMTYPES_HXX
#include "QEnumTypes.hxx"
#endif
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif
#ifndef DBAUI_QUERYCONTROLLER_HXX
#include "querycontroller.hxx"
#endif
#ifndef DBAUI_CONNECTIONLINEDATA_HXX
#include "ConnectionLineData.hxx"
#endif

namespace connectivity
{
	class OSQLParseNode;
}

class ComboBox;
namespace dbaui
{
	enum SqlParseError
	{
		eIllegalJoin,
		eStatementTooLong,
		eNoConnection,
		eNoSelectStatement,
		eStatementTooComplex,
		eColumnInLikeNotFound,
		eNoColumnInLike,
		eColumnNotFound,
		eNativeMode,
		eTooManyTables,
		eTooManyConditions,
		eTooManyColumns,
		eIllegalJoinCondition,
		eOk
	};

	class OQueryViewSwitch;
	class OAddTableDlg;
	class OQueryTableWindow;
	class OSelectionBrowseBox;
	class OTableConnection;
	class OQueryTableConnectionData;
	class OQueryContainerWindow;
	
	class OQueryDesignView : public OQueryView
	{
		enum ChildFocusState
		{
			SELECTION,
			TABLEVIEW,
			NONE
		};

		Splitter							m_aSplitter;
		
		::com::sun::star::lang::Locale		m_aLocale;
		::rtl::OUString						m_sDecimalSep;

		OSelectionBrowseBox*				m_pSelectionBox;	// presents the lower window
		ChildFocusState						m_eChildFocus;
		sal_Bool							m_bInKeyEvent;
		sal_Bool							m_bInSplitHandler;

	public:
		OQueryDesignView(OQueryContainerWindow* pParent, OQueryController& _rController,const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& );
		virtual ~OQueryDesignView();

		virtual sal_Bool isCutAllowed();
		virtual sal_Bool isPasteAllowed();
		virtual sal_Bool isCopyAllowed();
		virtual void copy();
		virtual void cut();
		virtual void paste();
		// clears the whole query
		virtual void clear();
		// set the view readonly or not
		virtual void setReadOnly(sal_Bool _bReadOnly);
		// check if the statement is correct when not returning false
		virtual sal_Bool checkStatement();
		// set the statement for representation
		virtual void setStatement(const ::rtl::OUString& _rsStatement);
		// returns the current sql statement
		virtual ::rtl::OUString getStatement();
		/// late construction
		virtual void Construct();
		virtual void initialize();
		// window overloads
		virtual long PreNotify( NotifyEvent& rNEvt );
		virtual void GetFocus();
		
		sal_Bool isSlotEnabled(sal_Int32 _nSlotId);
		void setSlotEnabled(sal_Int32 _nSlotId,sal_Bool _bEnable);
		void setNoneVisbleRow(sal_Int32 _nRows);

		::com::sun::star::lang::Locale		getLocale() const			{ return m_aLocale;}
		::rtl::OUString						getDecimalSeparator() const { return m_sDecimalSep;}

		SqlParseError   InsertField( const OTableFieldDescRef& rInfo, sal_Bool bVis=sal_True, sal_Bool bActivate = sal_True);
        bool            HasFieldByAliasName(const ::rtl::OUString& rFieldName, OTableFieldDescRef& rInfo) const;
		// save the position of the table window and the pos of the splitters
		// called when fields are deleted
		void DeleteFields( const ::rtl::OUString& rAliasName );
		// called when a table from tabeview was deleted
		void TableDeleted(const ::rtl::OUString& rAliasName);

		sal_Int32 getColWidth( sal_uInt16 _nColPos) const;
		void fillValidFields(const ::rtl::OUString& strTableName, ComboBox* pFieldList);

		void SaveUIConfig();
		void stopTimer();
		void startTimer();
		void reset();

        /** initializes the view from the current parser / parse iterator of the controller

            @param _pErrorInfo
                When not <NULL/>, the instance pointed to by this parameter takes the error
                which happened during the initialization.
                If it is not <NULL/>, then any such error will be displayed, using the controller's
                showError method.

            @return <TRUE/> if and only if the initialization was successful
        */
        bool    initByParseIterator( ::dbtools::SQLExceptionInfo* _pErrorInfo );

        void    initByFieldDescriptions(
                    const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& i_rFieldDescriptions
                );

		::connectivity::OSQLParseNode* getPredicateTreeFromEntry(	OTableFieldDescRef pEntry,
																	const String& _sCriteria,
																	::rtl::OUString& _rsErrorMessage,
																	::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxColumn) const;

        void fillFunctionInfo(   const ::connectivity::OSQLParseNode* pNode
                                ,const ::rtl::OUString& sFunctionTerm
                                ,OTableFieldDescRef& aInfo);
	protected:
		// return the Rectangle where I can paint myself
		virtual void resizeDocumentView(Rectangle& rRect);
		DECL_LINK( SplitHdl, void* );

    private:
        using OQueryView::SaveTabWinUIConfig;
	};
}
#endif // DBAUI_QUERYDESIGNVIEW_HXX
