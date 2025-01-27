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



#ifndef RPTUI_FORMULA_HXX
#define RPTUI_FORMULA_HXX

#include <formula/formula.hxx>
#include <formula/IFunctionDescription.hxx>
#include <formula/IControlReferenceHandler.hxx>
#include <boost/shared_ptr.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/report/meta/XFormulaParser.hpp>

class SvLBoxEntry;
//============================================================================
namespace rptui
{
//============================================================================
class FunctionManager;
class OAddFieldWindow;

//============================================================================
class FormulaDialog : public formula::FormulaModalDialog,
                      public formula::IFormulaEditorHelper,
                      public formula::IControlReferenceHandler
{
    ::boost::shared_ptr< formula::IFunctionManager > m_aFunctionManager;
    formula::FormEditData*      m_pFormulaData;
    OAddFieldWindow*		    m_pAddField;
    ::com::sun::star::uno::Reference < ::com::sun::star::beans::XPropertySet >          m_xRowSet;
    ::com::sun::star::uno::Reference< ::com::sun::star::report::meta::XFormulaParser>   m_xParser;
    ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XFormulaOpCodeMapper>    m_xOpCodeMapper;
    formula::RefEdit*           m_pEdit;
    String                      m_sFormula;
    xub_StrLen                  m_nStart;
    xub_StrLen                  m_nEnd;

    DECL_LINK( OnClickHdl, OAddFieldWindow*);
public:
	FormulaDialog( Window* pParent
        , const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory>& _xServiceFactory
        , const ::boost::shared_ptr< formula::IFunctionManager >& _pFunctionMgr
        , const ::rtl::OUString& _sFormula
        , const ::com::sun::star::uno::Reference < ::com::sun::star::beans::XPropertySet >& _xRowSet);
	virtual ~FormulaDialog();

    // IFormulaEditorHelper
    virtual void notifyChange();
    virtual void fill();
    virtual bool calculateValue(const String& _sExpression,String& _rResult);
    virtual void doClose(sal_Bool _bOk);
    virtual void insertEntryToLRUList(const formula::IFunctionDescription*	pDesc);
    virtual void showReference(const String& _sFormula);
    virtual void dispatch(sal_Bool _bOK,sal_Bool _bMartixChecked);
    virtual void setDispatcherLock( sal_Bool bLock );
    virtual void setReferenceInput(const formula::FormEditData* _pData);
    virtual void deleteFormData();
    virtual void clear();
    virtual void switchBack();
    virtual formula::FormEditData* getFormEditData() const;
    virtual void setCurrentFormula(const String& _sReplacement);
    virtual void setSelection(xub_StrLen _nStart,xub_StrLen _nEnd);
    virtual void getSelection(xub_StrLen& _nStart,xub_StrLen& _nEnd) const;
    virtual String getCurrentFormula() const;

    virtual formula::IFunctionManager* getFunctionManager();
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XFormulaParser> getFormulaParser() const;
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XFormulaOpCodeMapper> getFormulaOpCodeMapper() const;
    virtual ::com::sun::star::table::CellAddress getReferencePosition() const;

    virtual ::std::auto_ptr<formula::FormulaTokenArray> convertToTokenArray(const ::com::sun::star::uno::Sequence< ::com::sun::star::sheet::FormulaToken >& _aTokenList);

    // IControlReferenceHandler
    virtual void ShowReference(const String& _sRef);
    virtual void HideReference( sal_Bool bDoneRefMode = sal_True );
    virtual void ReleaseFocus( formula::RefEdit* pEdit, formula::RefButton* pButton = NULL );
    virtual void ToggleCollapsed( formula::RefEdit* pEdit, formula::RefButton* pButton = NULL );

protected:
	void		 HighlightFunctionParas(const String& aFormula);
};

// =============================================================================
} // rptui
// =============================================================================

#endif // RPTUI_FORMULA_HXX
