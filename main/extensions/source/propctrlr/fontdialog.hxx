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



#ifndef _EXTENSIONS_PROPCTRLR_FONTDIALOG_HXX_
#define _EXTENSIONS_PROPCTRLR_FONTDIALOG_HXX_

#include <sfx2/tabdlg.hxx>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/NamedValue.hpp>

class SvxFontListItem;
class FontList;

//............................................................................
namespace pcr
{
//............................................................................

	//========================================================================
	//= ControlCharacterDialog
	//========================================================================
	class ControlCharacterDialog : public SfxTabDialog
	{
	public:
		ControlCharacterDialog(Window* _pParent, const SfxItemSet& _rCoreSet);
		~ControlCharacterDialog();

		/// creates an item set to be used with this dialog
		static SfxItemSet*	createItemSet(SfxItemSet*& _rpSet, SfxItemPool*& _rpPool, SfxPoolItem**& _rppDefaults);

		/// destroys an item previously created with <method>createItemSet</method>
		static void			destroyItemSet(SfxItemSet*& _rpSet, SfxItemPool*& _rpPool, SfxPoolItem**& _rppDefaults);

		/// fills the given item set with values obtained from the given property set
		static void			translatePropertiesToItems(
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _rxModel,
			SfxItemSet* _pSet);

		/** fills the given property set with values obtained from the given item set
		*/
		static void translateItemsToProperties(
			const SfxItemSet& _rSet,
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _rxModel);

		/** fills the given property set with values obtained from the given item set
		*/
        static void translateItemsToProperties(
			const SfxItemSet& _rSet,
			::com::sun::star::uno::Sequence< ::com::sun::star::beans::NamedValue >& _out_properties );

	protected:
		virtual void PageCreated(sal_uInt16 _nId, SfxTabPage& _rPage);
	};
//............................................................................
}	// namespace pcr
//............................................................................

#endif // _EXTENSIONS_PROPCTRLR_FONTDIALOG_HXX_
