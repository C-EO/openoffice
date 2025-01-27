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



#ifndef ACCESSIBILITY_STANDARD_VCLXACCESSIBLEBOX_HXX
#define ACCESSIBILITY_STANDARD_VCLXACCESSIBLEBOX_HXX

#include <map>
#include <accessibility/standard/vclxaccessibleedit.hxx>
#ifndef _COM_SUN_STAR_ACCESSIBILITY_STANDARD_ACCESSIBLEROLE_HPP_
#include <com/sun/star/accessibility/AccessibleRole.hpp>
#endif
#include <com/sun/star/accessibility/XAccessibleKeyBinding.hpp>
#ifndef _COM_SUN_STAR_ACCESSIBILITY_XACCESSIBLEVALUE_HPP_
#include <com/sun/star/accessibility/XAccessibleValue.hpp>
#endif
#ifndef _CPPUHELPER_IMPLBASE2_HXX
#include <cppuhelper/implbase2.hxx>
#endif


typedef ::cppu::ImplHelper3<
    ::com::sun::star::accessibility::XAccessible,
	::com::sun::star::accessibility::XAccessibleValue,
    ::com::sun::star::accessibility::XAccessibleAction
    > VCLXAccessibleBox_BASE;


/** Base class for list- and combo boxes.  This class manages the box'
    children.  The classed derived from this one have only to implement the
    <member>IsValid</member> method and return the current implementation name.
*/
class VCLXAccessibleBox
    : public VCLXAccessibleComponent,
      public VCLXAccessibleBox_BASE
{
public:
    enum BoxType {COMBOBOX, LISTBOX};

    /** The constructor is initialized with the box type which may be
        either <const>COMBOBOX</const> or <const>LISTBOX</const> and a flag
        indicating whether the box is a drop down box.
    */
    VCLXAccessibleBox (VCLXWindow* pVCLXindow, BoxType aType, bool bIsDropDownBox);

	// XTypeProvider
	DECLARE_XTYPEPROVIDER()

	// XInterface
	DECLARE_XINTERFACE()



    // XAccessible

	virtual ::com::sun::star::uno::Reference<
		::com::sun::star::accessibility::XAccessibleContext > SAL_CALL
			getAccessibleContext(  ) throw (::com::sun::star::uno::RuntimeException);

    // XAccessibleContext

    /** Each object has one or two children: an optional text field and the
        actual list.  The text field is not provided for non drop down list
        boxes.
    */
    sal_Int32 SAL_CALL getAccessibleChildCount (void)
        throw (::com::sun::star::uno::RuntimeException);
    /** For drop down list boxes the text field is a not editable
        <type>VCLXAccessibleTextField</type>, for combo boxes it is an
        editable <type>VLCAccessibleEdit</type>.
    */
    ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible> SAL_CALL
        getAccessibleChild (sal_Int32 i)
        throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
    /** The role is always <const
        scope="com::sun::star::accessibility">AccessibleRole::COMBO_BOX</const>.
    */
	sal_Int16 SAL_CALL getAccessibleRole (void)
        throw (::com::sun::star::uno::RuntimeException);

    sal_Int32 SAL_CALL getAccessibleIndexInParent (void)
        throw (::com::sun::star::uno::RuntimeException);

	// XAccessibleAction

    /** There is one action for drop down boxes and none for others.
    */
    virtual sal_Int32 SAL_CALL getAccessibleActionCount (void)
        throw (::com::sun::star::uno::RuntimeException);
    /** The action for drop down boxes lets the user toggle the visibility of the
        popup menu.
    */
    virtual sal_Bool SAL_CALL doAccessibleAction (sal_Int32 nIndex)
        throw (::com::sun::star::lang::IndexOutOfBoundsException,
            ::com::sun::star::uno::RuntimeException);
    /** The returned string is associated with resource
        <const>RID_STR_ACC_ACTION_TOGGLEPOPUP</const>.
    */
    virtual ::rtl::OUString SAL_CALL getAccessibleActionDescription (sal_Int32 nIndex)
        throw (::com::sun::star::lang::IndexOutOfBoundsException,
            ::com::sun::star::uno::RuntimeException);
    /** No keybinding returned so far.
    */
	virtual ::com::sun::star::uno::Reference<
		::com::sun::star::accessibility::XAccessibleKeyBinding > SAL_CALL
			getAccessibleActionKeyBinding( sal_Int32 nIndex )
				throw (::com::sun::star::lang::IndexOutOfBoundsException,
					   ::com::sun::star::uno::RuntimeException);

	// XComponent

    /** This method is called from the implementation helper during an
        XComponent::dispose() call.
    */
	virtual void SAL_CALL disposing (void);

	//=====  XAccessibleValue  ================================================

    virtual ::com::sun::star::uno::Any SAL_CALL getCurrentValue( )
        throw (::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL setCurrentValue( 
        const ::com::sun::star::uno::Any& aNumber ) 
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getMaximumValue(  ) 
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getMinimumValue(  ) 
        throw (::com::sun::star::uno::RuntimeException);
	bool IsDropDownBox() {return m_bIsDropDownBox;};
	BoxType GetBoxType() { return m_aBoxType;};
protected:
    /** Specifies whether the box is a combo box or a list box.  List boxes
        have multi selection.
    */
    BoxType m_aBoxType;

    /// Specifies whether the box is a drop down box and thus has an action.
    bool m_bIsDropDownBox;

    /// The child that represents the text field if there is one.
    ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible>
        m_xText;

    /// The child that contains the items of this box.
    ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible>
        m_xList;

    /** This flag specifies whether an object has a text field as child
        regardless of whether that child being currently instantiated or
        not.
    */
    bool m_bHasTextChild;

    /** This flag specifies whether an object has a list as child regardless
        of whether that child being currently instantiated or not.  This
        flag is always true in the current implementation because the list
        child is just another wrapper around this object and thus has the
        same life time.
    */
    bool m_bHasListChild;

	virtual ~VCLXAccessibleBox (void);

    /** Returns </true> when the object is valid.
    */
    virtual bool IsValid (void) const = 0;

	virtual void ProcessWindowChildEvent (const VclWindowEvent& rVclWindowEvent);
	virtual void ProcessWindowEvent (const VclWindowEvent& rVclWindowEvent);

	virtual void	FillAccessibleStateSet( utl::AccessibleStateSetHelper& rStateSet );

private:
    /// Index in parent.  This is settable from the outside.
	sal_Int32 m_nIndexInParent;
};

#endif
