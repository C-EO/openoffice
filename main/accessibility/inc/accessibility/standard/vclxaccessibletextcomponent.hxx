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



#ifndef ACCESSIBILITY_STANDARD_VCLXACCESSIBLETEXTCOMPONENT_HXX
#define ACCESSIBILITY_STANDARD_VCLXACCESSIBLETEXTCOMPONENT_HXX

#include <toolkit/awt/vclxaccessiblecomponent.hxx>
#include <comphelper/accessibletexthelper.hxx>


//	----------------------------------------------------
//	class VCLXAccessibleTextComponent
//	----------------------------------------------------

typedef ::cppu::ImplHelper1	<	
	::com::sun::star::accessibility::XAccessibleText >	VCLXAccessibleTextComponent_BASE;

class VCLXAccessibleTextComponent : public VCLXAccessibleComponent,
									public ::comphelper::OCommonAccessibleText,
									public VCLXAccessibleTextComponent_BASE		
{
protected:
	::rtl::OUString			                m_sText;

	void					                SetText( const ::rtl::OUString& sText );

	virtual void							ProcessWindowEvent( const VclWindowEvent& rVclWindowEvent );

	// OCommonAccessibleText
	virtual ::rtl::OUString					implGetText();
	virtual ::com::sun::star::lang::Locale	implGetLocale();
	virtual void							implGetSelection( sal_Int32& nStartIndex, sal_Int32& nEndIndex );

	// XComponent
	virtual void SAL_CALL	                disposing();

public:
	VCLXAccessibleTextComponent( VCLXWindow* pVCLXWindow );
	~VCLXAccessibleTextComponent();

	// XInterface
	DECLARE_XINTERFACE()

	// XTypeProvider
	DECLARE_XTYPEPROVIDER()

	// XAccessibleText
	virtual sal_Int32 SAL_CALL getCaretPosition() throw (::com::sun::star::uno::RuntimeException);
	virtual sal_Bool SAL_CALL setCaretPosition( sal_Int32 nIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
	virtual sal_Unicode SAL_CALL getCharacter( sal_Int32 nIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue > SAL_CALL getCharacterAttributes( sal_Int32 nIndex, const ::com::sun::star::uno::Sequence< ::rtl::OUString >& aRequestedAttributes ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
	virtual ::com::sun::star::awt::Rectangle SAL_CALL getCharacterBounds( sal_Int32 nIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
	virtual sal_Int32 SAL_CALL getCharacterCount() throw (::com::sun::star::uno::RuntimeException);
	virtual sal_Int32 SAL_CALL getIndexAtPoint( const ::com::sun::star::awt::Point& aPoint ) throw (::com::sun::star::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getSelectedText() throw (::com::sun::star::uno::RuntimeException);
	virtual sal_Int32 SAL_CALL getSelectionStart() throw (::com::sun::star::uno::RuntimeException);
	virtual sal_Int32 SAL_CALL getSelectionEnd() throw (::com::sun::star::uno::RuntimeException);
	virtual sal_Bool SAL_CALL setSelection( sal_Int32 nStartIndex, sal_Int32 nEndIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getText() throw (::com::sun::star::uno::RuntimeException);
	virtual ::rtl::OUString SAL_CALL getTextRange( sal_Int32 nStartIndex, sal_Int32 nEndIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::accessibility::TextSegment SAL_CALL getTextAtIndex( sal_Int32 nIndex, sal_Int16 aTextType ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::accessibility::TextSegment SAL_CALL getTextBeforeIndex( sal_Int32 nIndex, sal_Int16 aTextType ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::accessibility::TextSegment SAL_CALL getTextBehindIndex( sal_Int32 nIndex, sal_Int16 aTextType ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
	virtual sal_Bool SAL_CALL copyText( sal_Int32 nStartIndex, sal_Int32 nEndIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
};


#endif // ACCESSIBILITY_STANDARD_VCLXACCESSIBLETEXTCOMPONENT_HXX
