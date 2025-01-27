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
#include "precompiled_dbui.hxx"
#ifndef DBAUI_RELATIONCONTROL_HXX
#include "RelationControl.hxx"
#endif
#ifndef DBACCESS_SOURCE_UI_INC_RELATIONCONTROL_HRC
#include "RelationControl.hrc"
#endif

#ifndef _SVTOOLS_EDITBROWSEBOX_HXX_
#include <svtools/editbrowsebox.hxx>
#endif
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif
#ifndef _TOOLS_DEBUG_HXX
#include <tools/debug.hxx>
#endif
#ifndef TOOLS_DIAGNOSE_EX_H
#include <tools/diagnose_ex.h>
#endif
#ifndef DBAUI_TABLECONNECTIONDATA_HXX
#include "TableConnectionData.hxx"
#endif
#ifndef DBAUI_TABLECONNECTION_HXX
#include "TableConnection.hxx"
#endif
#ifndef DBAUI_TABLEWINDOW_HXX
#include "TableWindow.hxx"
#endif
#ifndef _COM_SUN_STAR_SDBC_XDATABASEMETADATA_HPP_
#include <com/sun/star/sdbc/XDatabaseMetaData.hpp>
#endif
#ifndef DBAUI_TOOLS_HXX
#include "UITools.hxx"
#endif
#ifndef _COM_SUN_STAR_SDBCX_XCOLUMNSSUPPLIER_HPP_
#include <com/sun/star/sdbcx/XColumnsSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_CONTAINER_XNAMEACCESS_HPP_
#include <com/sun/star/container/XNameAccess.hpp>
#endif
#ifndef DBAUI_RELCONTROLIFACE_HXX
#include "RelControliFace.hxx"
#endif
#ifndef _DBU_CONTROL_HRC_
#include "dbu_control.hrc"
#endif
#ifndef _DBA_DBACCESS_HELPID_HRC_
#include "dbaccess_helpid.hrc"
#endif
#ifndef _TOOLS_DEBUG_HXX
#include <tools/debug.hxx>
#endif

#include <algorithm>

#define SOURCE_COLUMN	1
#define DEST_COLUMN		2

namespace dbaui
{
	using namespace ::com::sun::star::uno;
	using namespace ::com::sun::star::beans;
	using namespace ::com::sun::star::sdbc;
	using namespace ::com::sun::star::sdbcx;
	using namespace ::com::sun::star::container;
	using namespace svt;

	typedef ::svt::EditBrowseBox ORelationControl_Base;
	class ORelationControl : public ORelationControl_Base
	{
		friend class OTableListBoxControl;

        ::std::auto_ptr< ::svt::ListBoxControl> m_pListCell;
		TTableConnectionData::value_type		m_pConnData;
		const OJoinTableView::OTableWindowMap*	m_pTableMap;
		OTableListBoxControl*					m_pBoxControl;
		long									m_nDataPos;
		Reference< XPropertySet>				m_xSourceDef;
		Reference< XPropertySet>				m_xDestDef;


		void fillListBox(const Reference< XPropertySet>& _xDest,long nRow,sal_uInt16 nColumnId);
		/** returns the column id for the editbrowsebox
			@param	_nColId
					the column id SOURCE_COLUMN or DEST_COLUMN

			@return	the current column id either SOURCE_COLUMN or DEST_COLUMN depends on the connection data
		*/
		sal_uInt16 getColumnIdent( sal_uInt16 _nColId ) const;
	public:
		ORelationControl( OTableListBoxControl* pParent,const OJoinTableView::OTableWindowMap* _pTableMap );
		virtual ~ORelationControl();

		/** searches for a connection between these two tables
			@param	_pSource	
					the left table
			@param	_pDest		
					the right window
		*/
		void setWindowTables(const OTableWindow* _pSource,const OTableWindow* _pDest);

		/** allows to access the connection data from outside

			@return rthe connection data
		*/
		inline TTableConnectionData::value_type getData() const { return m_pConnData; }

		void lateInit();

	protected:
		virtual void Resize();

		virtual long PreNotify(NotifyEvent& rNEvt );

		virtual sal_Bool IsTabAllowed(sal_Bool bForward) const;

		virtual void Init(const TTableConnectionData::value_type& _pConnData);
		virtual void Init() { ORelationControl_Base::Init(); }
		virtual void InitController( ::svt::CellControllerRef& rController, long nRow, sal_uInt16 nCol );
		virtual ::svt::CellController* GetController( long nRow, sal_uInt16 nCol );
		virtual void PaintCell( OutputDevice& rDev, const Rectangle& rRect, sal_uInt16 nColId ) const;
		virtual sal_Bool SeekRow( long nRow );
		virtual sal_Bool SaveModified();
		virtual String GetCellText( long nRow, sal_uInt16 nColId ) const;

		virtual void CellModified();

        DECL_LINK( AsynchDeactivate, void* );
	private:

		DECL_LINK( AsynchActivate, void* );
		
	};

    //========================================================================
	// class ORelationControl
	//========================================================================
	DBG_NAME(ORelationControl)
	//------------------------------------------------------------------------
	ORelationControl::ORelationControl( OTableListBoxControl* pParent ,const OJoinTableView::OTableWindowMap* _pTableMap)
		:EditBrowseBox( pParent, EBBF_SMART_TAB_TRAVEL | EBBF_NOROWPICTURE, WB_TABSTOP | /*WB_3DLOOK | */WB_BORDER | BROWSER_AUTOSIZE_LASTCOL)
		,m_pTableMap(_pTableMap)
		,m_pBoxControl(pParent)
		,m_xSourceDef( NULL )
		,m_xDestDef( NULL )
	{
		DBG_CTOR(ORelationControl,NULL);
	}

	//------------------------------------------------------------------------
	ORelationControl::~ORelationControl()
	{
		DBG_DTOR(ORelationControl,NULL);
	}

	//------------------------------------------------------------------------
	void ORelationControl::Init(const TTableConnectionData::value_type& _pConnData)
	{
		DBG_CHKTHIS(ORelationControl,NULL);

		m_pConnData = _pConnData;
		OSL_ENSURE(m_pConnData, "No data supplied!");

		m_pConnData->normalizeLines();
	}
	//------------------------------------------------------------------------------
	void ORelationControl::lateInit()
	{
        if ( !m_pConnData.get() )
            return;
        m_xSourceDef = m_pConnData->getReferencingTable()->getTable();
		m_xDestDef = m_pConnData->getReferencedTable()->getTable();

		if ( ColCount() == 0 )
		{
			InsertDataColumn( SOURCE_COLUMN, m_pConnData->getReferencingTable()->GetWinName(), 100);
			InsertDataColumn( DEST_COLUMN, m_pConnData->getReferencedTable()->GetWinName(), 100);
				// wenn es die Defs noch nicht gibt, dann muessen sie noch mit SetSource-/-DestDef gesetzt werden !

			m_pListCell.reset( new ListBoxControl( &GetDataWindow() ) );

			//////////////////////////////////////////////////////////////////////
			// set browse mode
			SetMode(	BROWSER_COLUMNSELECTION |
						BROWSER_HLINESFULL		| 
						BROWSER_VLINESFULL		|
						BROWSER_HIDECURSOR		| 
						BROWSER_HIDESELECT		|
						BROWSER_AUTO_HSCROLL	|
						BROWSER_AUTO_VSCROLL);
		}
		else
			// not the first call
			RowRemoved(0, GetRowCount());

		RowInserted(0, m_pConnData->GetConnLineDataList()->size() + 1, sal_True); // add one extra row
	}
	//------------------------------------------------------------------------------
	void ORelationControl::Resize()
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		EditBrowseBox::Resize();
		long nOutputWidth = GetOutputSizePixel().Width();
		SetColumnWidth(1, (nOutputWidth / 2));
		SetColumnWidth(2, (nOutputWidth / 2));
	}

	//------------------------------------------------------------------------------
	long ORelationControl::PreNotify(NotifyEvent& rNEvt)
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		if (rNEvt.GetType() == EVENT_LOSEFOCUS && !HasChildPathFocus() )
			PostUserEvent(LINK(this, ORelationControl, AsynchDeactivate));
		else if (rNEvt.GetType() == EVENT_GETFOCUS)
			PostUserEvent(LINK(this, ORelationControl, AsynchActivate));
		
		return EditBrowseBox::PreNotify(rNEvt);
	}

	//------------------------------------------------------------------------------
	IMPL_LINK(ORelationControl, AsynchActivate, void*, EMPTYARG)
	{
		ActivateCell();
		return 0L;
	}

	//------------------------------------------------------------------------------
	IMPL_LINK(ORelationControl, AsynchDeactivate, void*, EMPTYARG)
	{
		DeactivateCell();
		return 0L;
	}

	//------------------------------------------------------------------------------
	sal_Bool ORelationControl::IsTabAllowed(sal_Bool bForward) const
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		long nRow = GetCurRow();
		sal_uInt16 nCol = GetCurColumnId();

		sal_Bool bRet = !((		( bForward && (nCol == DEST_COLUMN)		&& (nRow == GetRowCount() - 1))) 
						||	(!bForward && (nCol == SOURCE_COLUMN)	&& (nRow == 0)));

		return bRet && EditBrowseBox::IsTabAllowed(bForward);
	}

	//------------------------------------------------------------------------------
	sal_Bool ORelationControl::SaveModified()
	{
		DBG_CHKTHIS(ORelationControl,NULL);
        sal_Int32 nRow = GetCurRow();
		if ( nRow != BROWSER_ENDOFSELECTION )
		{
            String sFieldName(m_pListCell->GetSelectEntry());
            OConnectionLineDataVec* pLines = m_pConnData->GetConnLineDataList();
            if ( pLines->size() <= static_cast<sal_uInt32>(nRow) )
            {
                pLines->push_back(new OConnectionLineData());
                nRow = pLines->size() - 1;
            }

            OConnectionLineDataRef pConnLineData = (*pLines)[nRow];

			switch( getColumnIdent( GetCurColumnId() ) )
			{
			case SOURCE_COLUMN:
				pConnLineData->SetSourceFieldName( sFieldName );
				break;
			case DEST_COLUMN:
				pConnLineData->SetDestFieldName( sFieldName );
				break;
			}
		}

		return sal_True;
	}
	//------------------------------------------------------------------------------
	sal_uInt16 ORelationControl::getColumnIdent( sal_uInt16 _nColId ) const
	{
		sal_uInt16 nId = _nColId;
		if ( m_pConnData->getReferencingTable() != m_pBoxControl->getReferencingTable() )
			nId = ( _nColId == SOURCE_COLUMN) ? DEST_COLUMN : SOURCE_COLUMN;
		return nId;
	}

	//------------------------------------------------------------------------------
	String ORelationControl::GetCellText( long nRow, sal_uInt16 nColId ) const
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		String sText;
		if ( m_pConnData->GetConnLineDataList()->size() > static_cast<size_t>(nRow) )
		{
			OConnectionLineDataRef pConnLineData = (*m_pConnData->GetConnLineDataList())[nRow];
			switch( getColumnIdent( nColId ) )
			{
			case SOURCE_COLUMN:
				sText  = pConnLineData->GetSourceFieldName();
				break;
			case DEST_COLUMN:
				sText  = pConnLineData->GetDestFieldName();
				break;
			}
		}
		return sText;
	}

	//------------------------------------------------------------------------------
	void ORelationControl::InitController( CellControllerRef& /*rController*/, long nRow, sal_uInt16 nColumnId )
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		
		rtl::OString sHelpId( HID_RELATIONDIALOG_LEFTFIELDCELL );

		Reference< XPropertySet> xDef;
		switch ( getColumnIdent(nColumnId) )
		{
			case SOURCE_COLUMN:
				xDef	= m_xSourceDef;
				sHelpId = HID_RELATIONDIALOG_LEFTFIELDCELL;
				break;
			case DEST_COLUMN:
				xDef	= m_xDestDef;
				sHelpId = HID_RELATIONDIALOG_RIGHTFIELDCELL;
				break;
			default:
				//  ?????????
				break;
		}

		if ( xDef.is() )
		{
			fillListBox(xDef,nRow,nColumnId);
			String sName = GetCellText( nRow, nColumnId );
			m_pListCell->SelectEntry( sName );
            if ( m_pListCell->GetSelectEntry() != sName )
            {
                m_pListCell->InsertEntry( sName );
                m_pListCell->SelectEntry( sName );
            }

			m_pListCell->SetHelpId(sHelpId);
		}
	}

	//------------------------------------------------------------------------------
	CellController* ORelationControl::GetController( long /*nRow*/, sal_uInt16 /*nColumnId*/ )
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		return new ListBoxCellController( m_pListCell.get() );
	}

	//------------------------------------------------------------------------------
	sal_Bool ORelationControl::SeekRow( long nRow )
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		m_nDataPos = nRow;
		return sal_True;
	}

	//------------------------------------------------------------------------------
	void ORelationControl::PaintCell( OutputDevice& rDev, const Rectangle& rRect, sal_uInt16 nColumnId ) const
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		String aText  =const_cast< ORelationControl*>(this)->GetCellText( m_nDataPos, nColumnId );

		Point aPos( rRect.TopLeft() );			
		Size aTextSize( GetDataWindow().GetTextHeight(),GetDataWindow().GetTextWidth( aText ));

		if( aPos.X() < rRect.Right() || aPos.X() + aTextSize.Width() > rRect.Right() ||
			aPos.Y() < rRect.Top() || aPos.Y() + aTextSize.Height() > rRect.Bottom() )
			rDev.SetClipRegion( rRect );

		rDev.DrawText( aPos, aText );

		if( rDev.IsClipRegion() )
			rDev.SetClipRegion();
	}
	// -----------------------------------------------------------------------------
	void ORelationControl::fillListBox(const Reference< XPropertySet>& _xDest,long /*_nRow*/,sal_uInt16 /*nColumnId*/)
	{
		m_pListCell->Clear();
		try
		{
			if ( _xDest.is() )
			{
				//sal_Int32 nRows = GetRowCount();
				Reference<XColumnsSupplier> xSup(_xDest,UNO_QUERY);
				Reference<XNameAccess> xColumns = xSup->getColumns();
				Sequence< ::rtl::OUString> aNames = xColumns->getElementNames();
				const ::rtl::OUString* pIter = aNames.getConstArray();
				const ::rtl::OUString* pEnd = pIter + aNames.getLength();
				for(;pIter != pEnd;++pIter)
				{
					m_pListCell->InsertEntry( *pIter );
				}
				m_pListCell->InsertEntry(String(), 0);
			}
		}
        catch( const Exception& )
        {
            DBG_UNHANDLED_EXCEPTION();
        }
	}
	// -----------------------------------------------------------------------------
	void ORelationControl::setWindowTables(const OTableWindow* _pSource,const OTableWindow* _pDest)
	{
		// wenn ich hier gerade editiere, ausblenden
		sal_Bool bWasEditing = IsEditing();
		if ( bWasEditing )
			DeactivateCell();

		if ( _pSource && _pDest )
		{
			m_xSourceDef = _pSource->GetTable();
			SetColumnTitle(1, _pSource->GetName());
		

			m_xDestDef = _pDest->GetTable();
			SetColumnTitle(2, _pDest->GetName());

			const OJoinTableView* pView = _pSource->getTableView();
			OTableConnection* pConn = pView->GetTabConn(_pSource,_pDest);
			if ( pConn && !m_pConnData->GetConnLineDataList()->empty() )
			{
				m_pConnData->CopyFrom(*pConn->GetData());
				m_pBoxControl->getContainer()->notifyConnectionChange();
			}
			else
			{
				// no connection found so we clear our data
				OConnectionLineDataVec* pLines = m_pConnData->GetConnLineDataList();
				::std::for_each(pLines->begin(),
								pLines->end(),
								OUnaryRefFunctor<OConnectionLineData>( ::std::mem_fun(&OConnectionLineData::Reset))
								);

				m_pConnData->setReferencingTable(_pSource->GetData());
				m_pConnData->setReferencedTable(_pDest->GetData());
			}
			m_pConnData->normalizeLines();

		}
		// neu zeichnen
		Invalidate();

		if ( bWasEditing )
		{
			GoToRow(0);
			ActivateCell();
		}
	}
	//------------------------------------------------------------------------
	void ORelationControl::CellModified()
	{
		DBG_CHKTHIS(ORelationControl,NULL);
		EditBrowseBox::CellModified();
		SaveModified();
		static_cast<OTableListBoxControl*>(GetParent())->NotifyCellChange();
	}
	//========================================================================
	// class OTableListBoxControl
DBG_NAME(OTableListBoxControl)

//========================================================================

OTableListBoxControl::OTableListBoxControl(  Window* _pParent
										    ,const ResId& _rResId
										    ,const OJoinTableView::OTableWindowMap* _pTableMap
										    ,IRelationControlInterface* _pParentDialog)
	 : Window(_pParent,_rResId)
	 , m_aFL_InvolvedTables(    this, ResId(FL_INVOLVED_TABLES,*_rResId.GetResMgr()))
	 , m_lmbLeftTable(			this, ResId(LB_LEFT_TABLE,*_rResId.GetResMgr()))
	 , m_lmbRightTable(			this, ResId(LB_RIGHT_TABLE,*_rResId.GetResMgr()))
	 , m_aFL_InvolvedFields(    this, ResId(FL_INVOLVED_FIELDS,*_rResId.GetResMgr()))
	 , m_pTableMap(_pTableMap)
	 , m_pParentDialog(_pParentDialog) 
	{
		m_pRC_Tables = new ORelationControl( this,m_pTableMap );
		m_pRC_Tables->SetHelpId(HID_RELDLG_KEYFIELDS);
		m_pRC_Tables->Init( );
        m_pRC_Tables->SetZOrder(&m_lmbRightTable, WINDOW_ZORDER_BEHIND);

        lateUIInit();

		Link aLink(LINK(this, OTableListBoxControl, OnTableChanged));
		m_lmbLeftTable.SetSelectHdl(aLink);
		m_lmbRightTable.SetSelectHdl(aLink);

		FreeResource();
		DBG_CTOR(OTableListBoxControl,NULL);
	}
	// -----------------------------------------------------------------------------
	OTableListBoxControl::~OTableListBoxControl()
	{
		ORelationControl* pTemp = m_pRC_Tables;
		m_pRC_Tables = NULL;
		delete pTemp;
        DBG_DTOR(OTableListBoxControl,NULL);
    }
	// -----------------------------------------------------------------------------
	void OTableListBoxControl::fillListBoxes()
	{
		DBG_ASSERT( !m_pTableMap->empty(), "OTableListBoxControl::fillListBoxes: no table window!");
		OTableWindow* pInitialLeft = NULL;
		OTableWindow* pInitialRight = NULL;

		// die Namen aller TabWins einsammeln
		OJoinTableView::OTableWindowMap::const_iterator aIter = m_pTableMap->begin();
        OJoinTableView::OTableWindowMap::const_iterator aEnd = m_pTableMap->end();
		for(;aIter != aEnd;++aIter)
		{
			m_lmbLeftTable.InsertEntry(aIter->first);
			m_lmbRightTable.InsertEntry(aIter->first);

			if (!pInitialLeft)
			{
				pInitialLeft = aIter->second;
				m_strCurrentLeft = aIter->first;
			}
			else if (!pInitialRight)
			{
				pInitialRight = aIter->second;
				m_strCurrentRight = aIter->first;
			}
		} // for(;aIter != m_pTableMap->end();++aIter)

        if ( !pInitialRight )
        {
            pInitialRight = pInitialLeft;
			m_strCurrentRight = m_strCurrentLeft;
        }

		// die entsprechenden Defs an mein Controls
		m_pRC_Tables->setWindowTables(pInitialLeft,pInitialRight);

		// die in einer ComboBox ausgewaehlte Tabelle darf nicht in der anderen zur Verfuegung stehen
		
		if ( m_pTableMap->size() > 2 )
		{
			m_lmbLeftTable.RemoveEntry(m_strCurrentRight);
			m_lmbRightTable.RemoveEntry(m_strCurrentLeft);
		}

		// links das erste, rechts das zweite selektieren
		m_lmbLeftTable.SelectEntry(m_strCurrentLeft);
		m_lmbRightTable.SelectEntry(m_strCurrentRight);

		m_lmbLeftTable.GrabFocus();
	}
	// -----------------------------------------------------------------------------
	IMPL_LINK( OTableListBoxControl, OnTableChanged, ListBox*, pListBox )
	{
		String strSelected(pListBox->GetSelectEntry());
		OTableWindow* pLeft		= NULL;
		OTableWindow* pRight	= NULL;

		// eine Sonderbehandlung : wenn es nur zwei Tabellen gibt, muss ich bei Wechsel in einer LB auch in der anderen umschalten
		if ( m_pTableMap->size() == 2 )
		{
			ListBox* pOther;
			if ( pListBox == &m_lmbLeftTable )
				pOther = &m_lmbRightTable;
			else
				pOther = &m_lmbLeftTable;
			pOther->SelectEntryPos(1 - pOther->GetSelectEntryPos());

			OJoinTableView::OTableWindowMap::const_iterator aIter = m_pTableMap->begin();
			OTableWindow* pFirst = aIter->second;
			++aIter;
			OTableWindow* pSecond = aIter->second;

			if ( m_lmbLeftTable.GetSelectEntry() == String(pFirst->GetName()) )
			{
				pLeft	= pFirst;
				pRight	= pSecond;
			}
			else
			{
				pLeft	= pSecond;
				pRight	= pFirst;
			}
		}
		else
		{
			// zuerst brauche ich die TableDef zur Tabelle, dazu das TabWin
			OJoinTableView::OTableWindowMap::const_iterator aFind = m_pTableMap->find(strSelected);
			OTableWindow* pLoop = NULL;
			if( aFind != m_pTableMap->end() )
				pLoop = aFind->second;
			DBG_ASSERT(pLoop != NULL, "ORelationDialog::OnTableChanged : ungueltiger Eintrag in ListBox !");
				// da ich die ListBoxen selber mit eben diesen Tabellennamen, mit denen ich sie jetzt vergleiche, gefuellt habe,
				// MUSS ich strSelected finden
			if (pListBox == &m_lmbLeftTable)
			{
				// den vorher links selektierten Eintrag wieder rein rechts
				m_lmbRightTable.InsertEntry(m_strCurrentLeft);
				// und den jetzt selektierten raus
				m_lmbRightTable.RemoveEntry(strSelected);
				m_strCurrentLeft	= strSelected;
				
				pLeft = pLoop;
				
				OJoinTableView::OTableWindowMap::const_iterator aIter = m_pTableMap->find(m_lmbRightTable.GetSelectEntry());
				OSL_ENSURE( aIter != m_pTableMap->end(), "Invalid name");
				if ( aIter != m_pTableMap->end() )
					pRight = aIter->second;

				m_lmbLeftTable.GrabFocus();
			}
			else
			{
				// den vorher rechts selektierten Eintrag wieder rein links
				m_lmbLeftTable.InsertEntry(m_strCurrentRight);
				// und den jetzt selektierten raus
				m_lmbLeftTable.RemoveEntry(strSelected);
				m_strCurrentRight = strSelected;

				pRight = pLoop;
				OJoinTableView::OTableWindowMap::const_iterator aIter = m_pTableMap->find(m_lmbLeftTable.GetSelectEntry());
				OSL_ENSURE( aIter != m_pTableMap->end(), "Invalid name");
				if ( aIter != m_pTableMap->end() )
					pLeft = aIter->second;
			}
		}

		pListBox->GrabFocus();

		m_pRC_Tables->setWindowTables(pLeft,pRight);

		NotifyCellChange();
		return 0;
	}
	// -----------------------------------------------------------------------------
	void OTableListBoxControl::NotifyCellChange()
	{
		// den Ok-Button en- oder disablen, je nachdem, ob ich eine gueltige Situation habe
        TTableConnectionData::value_type pConnData = m_pRC_Tables->getData();
		const OConnectionLineDataVec* pLines = pConnData->GetConnLineDataList();
		m_pParentDialog->setValid(!pLines->empty());

		if ( pLines->size() >= static_cast<sal_uInt32>(m_pRC_Tables->GetRowCount()) )
		{
			m_pRC_Tables->DeactivateCell();
			m_pRC_Tables->RowInserted(m_pRC_Tables->GetRowCount(), pLines->size() - static_cast<sal_uInt32>(m_pRC_Tables->GetRowCount()) + 1, sal_True);
			m_pRC_Tables->ActivateCell();
		}
	}
	// -----------------------------------------------------------------------------
	void fillEntryAndDisable(ListBox& _rListBox,const String& _sEntry)
	{
		_rListBox.InsertEntry(_sEntry);
		_rListBox.SelectEntryPos(0);
		_rListBox.Disable();
	}
	// -----------------------------------------------------------------------------
    void OTableListBoxControl::fillAndDisable(const TTableConnectionData::value_type& _pConnectionData)
	{
		fillEntryAndDisable(m_lmbLeftTable,_pConnectionData->getReferencingTable()->GetWinName());
		fillEntryAndDisable(m_lmbRightTable,_pConnectionData->getReferencedTable()->GetWinName());
	}
	// -----------------------------------------------------------------------------
	void OTableListBoxControl::Init(const TTableConnectionData::value_type& _pConnData)
	{
		m_pRC_Tables->Init(_pConnData);
	}
	// -----------------------------------------------------------------------------
	void OTableListBoxControl::lateUIInit(Window* _pTableSeparator)
	{
        const sal_Int32 nDiff = LogicToPixel( Point(0,6), MAP_APPFONT ).Y();
        Point aDlgPoint = LogicToPixel( Point(12,43), MAP_APPFONT );
        if ( _pTableSeparator )
        {
            _pTableSeparator->SetZOrder(&m_lmbRightTable, WINDOW_ZORDER_BEHIND);
            m_pRC_Tables->SetZOrder(_pTableSeparator, WINDOW_ZORDER_BEHIND);
            //aDlgPoint = m_pTableSeparator->GetPosPixel() + Point(0,aSize.Height()) + LogicToPixel( Point(0,6), MAP_APPFONT );
            _pTableSeparator->SetPosPixel(Point(0,m_aFL_InvolvedFields.GetPosPixel().Y()));
            const Size aSize = _pTableSeparator->GetSizePixel();
            aDlgPoint.Y() = _pTableSeparator->GetPosPixel().Y() + aSize.Height();
            m_aFL_InvolvedFields.SetPosPixel(Point(m_aFL_InvolvedFields.GetPosPixel().X(),aDlgPoint.Y()));
            aDlgPoint.Y() += nDiff + m_aFL_InvolvedFields.GetSizePixel().Height();
        }
        //////////////////////////////////////////////////////////////////////
		// positing BrowseBox control
        const Size aCurrentSize = GetSizePixel();
		Size aDlgSize = LogicToPixel( Size(24,0), MAP_APPFONT );
		aDlgSize.Width() = aCurrentSize.Width() - aDlgSize.Width();
        aDlgSize.Height() = aCurrentSize.Height() - aDlgPoint.Y() - nDiff;

		m_pRC_Tables->SetPosSizePixel( aDlgPoint, aDlgSize );
		m_pRC_Tables->Show();

        lateInit();
    }
    // -----------------------------------------------------------------------------
	void OTableListBoxControl::lateInit()
	{
		m_pRC_Tables->lateInit();
	}
	// -----------------------------------------------------------------------------
	sal_Bool OTableListBoxControl::SaveModified()
	{
		sal_Bool bRet = m_pRC_Tables->SaveModified();
		m_pRC_Tables->getData()->normalizeLines();
		return bRet;
	}
	// -----------------------------------------------------------------------------
	TTableWindowData::value_type OTableListBoxControl::getReferencingTable()	const 
	{ 
        return m_pRC_Tables->getData()->getReferencingTable();
	}
	// -----------------------------------------------------------------------------
    void OTableListBoxControl::enableRelation(bool _bEnable)
    {
        if ( !_bEnable )
            PostUserEvent(LINK(m_pRC_Tables, ORelationControl, AsynchDeactivate));
        m_pRC_Tables->Enable(_bEnable);
        
    }
	// -----------------------------------------------------------------------------
}
// -----------------------------------------------------------------------------

