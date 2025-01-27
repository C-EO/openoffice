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



#ifndef _UNO_LINGU_HXX
#define _UNO_LINGU_HXX

#include <i18npool/lang.h>
#include <tools/string.hxx>
#include <vos/refernce.hxx>
#include <com/sun/star/util/Language.hpp>
#include <com/sun/star/lang/Locale.hpp>
#include <com/sun/star/linguistic2/XLinguServiceManager.hpp>
#include <com/sun/star/linguistic2/XSpellChecker1.hpp>
#include <com/sun/star/linguistic2/XHyphenator.hpp>
#include <com/sun/star/linguistic2/XThesaurus.hpp>
#include <com/sun/star/linguistic2/XDictionaryList.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include "editeng/editengdllapi.h"

class LinguMgrExitLstnr;

class Window;

///////////////////////////////////////////////////////////////////////////
// SvxLinguConfigUpdate
// class to update configuration items when (before!) the linguistic is used.
//
// This class is called by all the dummy implementations to update all of the
// configuration (list of used/available services) when the linguistic is
// accessed for the first time.

class SvxLinguConfigUpdate
{
    static sal_Int32    nCurrentDataFilesChangedCheckValue;
    static sal_Int16    nNeedUpdating;  // n == -1 => needs to be checked
                                    // n ==  0 => already updated, nothing to be done
                                    // n ==  1 => needs to be updated

    static sal_Int32 CalcDataFilesChangedCheckValue();

public:

    EDITENG_DLLPUBLIC static void UpdateAll( sal_Bool bForceCheck = sal_False );
    static sal_Bool IsNeedUpdateAll( sal_Bool bForceCheck = sal_False );
};

///////////////////////////////////////////////////////////////////////////

class EDITENG_DLLPUBLIC LinguMgr
{
	friend class LinguMgrExitLstnr;

    //static ::VOS::ORefCount aRefCount;

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XLinguServiceManager > xLngSvcMgr;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XSpellChecker1 > xSpell;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XHyphenator >	xHyph;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XThesaurus >		xThes;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionaryList > xDicList;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet > 		xProp;

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary >	xIgnoreAll;
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary >	xChangeAll;

	static LinguMgrExitLstnr						   *pExitLstnr;
	static sal_Bool										bExiting;

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XSpellChecker1 > GetSpell();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XHyphenator > 	GetHyph();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XThesaurus > 	GetThes();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionaryList > GetDicList();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet > 		GetProp();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary >	GetStandard();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary >	GetIgnoreAll();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary >	GetChangeAll();

	// disallow access to copy-constructor and assignment-operator
	LinguMgr(const LinguMgr &);
	LinguMgr & operator = (const LinguMgr &);

public:

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XSpellChecker1 > GetSpellChecker();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XHyphenator > 	GetHyphenator();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XThesaurus > 	GetThesaurus();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionaryList > GetDictionaryList();
	static ::com::sun::star::uno::Reference<
		::com::sun::star::beans::XPropertySet > 		GetLinguPropertySet();

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XLinguServiceManager > GetLngSvcMgr();

	static ::com::sun::star::uno::Reference<
		::com::sun::star::linguistic2::XDictionary > 	GetStandardDic();
	static ::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XDictionary > 	GetIgnoreAllList();
	static ::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XDictionary > 	GetChangeAllList();

    // update all configuration entries
    static void UpdateAll();
};

///////////////////////////////////////////////////////////////////////////

namespace com { namespace sun { namespace star { namespace linguistic2 {
	class XHyphenatedWord;
}}}}


struct SvxAlternativeSpelling
{
	String		aReplacement;
	::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XHyphenatedWord >	xHyphWord;
	sal_Int16  		nChangedPos,
		 		nChangedLength;
	sal_Bool		bIsAltSpelling;

	inline SvxAlternativeSpelling();
};

inline SvxAlternativeSpelling::SvxAlternativeSpelling() :
	nChangedPos(-1), nChangedLength(-1), bIsAltSpelling(sal_False)
{
}


EDITENG_DLLPUBLIC SvxAlternativeSpelling SvxGetAltSpelling( 
		const ::com::sun::star::uno::Reference< 
			::com::sun::star::linguistic2::XHyphenatedWord > & rHyphWord );


///////////////////////////////////////////////////////////////////////////

class EDITENG_DLLPUBLIC SvxDicListChgClamp
{
private:
	::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XDictionaryList > 	xDicList;

	// disallow access to copy-constructor and assignment-operator
	EDITENG_DLLPRIVATE SvxDicListChgClamp(const SvxDicListChgClamp &);
	EDITENG_DLLPRIVATE SvxDicListChgClamp & operator = (const SvxDicListChgClamp &);

public:
	SvxDicListChgClamp( ::com::sun::star::uno::Reference< 
		::com::sun::star::linguistic2::XDictionaryList >  &rxDicList );
	~SvxDicListChgClamp();
};

///////////////////////////////////////////////////////////////////////////

//TL:TODO: remove those functions or make them inline
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XSpellChecker1 >	SvxGetSpellChecker();
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XHyphenator >  	SvxGetHyphenator();
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XThesaurus >  	SvxGetThesaurus();
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XDictionaryList > SvxGetDictionaryList();
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::beans::XPropertySet > 		SvxGetLinguPropertySet();
//TL:TODO: remove argument or provide SvxGetIgnoreAllList with the same one
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XDictionary >  	SvxGetOrCreatePosDic( 
			::com::sun::star::uno::Reference< 
				::com::sun::star::linguistic2::XDictionaryList >  xDicList );
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XDictionary >  	SvxGetIgnoreAllList();
EDITENG_DLLPUBLIC ::com::sun::star::uno::Reference< 
	::com::sun::star::linguistic2::XDictionary >  	SvxGetChangeAllList();

///////////////////////////////////////////////////////////////////////////
// misc functions
//

EDITENG_DLLPUBLIC LanguageType 						SvxLocaleToLanguage( 
		const ::com::sun::star::lang::Locale& rLocale );
EDITENG_DLLPUBLIC ::com::sun::star::lang::Locale& 	SvxLanguageToLocale( 
		::com::sun::star::lang::Locale& rLocale, LanguageType eLang );
EDITENG_DLLPUBLIC ::com::sun::star::lang::Locale    SvxCreateLocale( LanguageType eLang );


EDITENG_DLLPUBLIC short	SvxDicError( Window *pParent, sal_Int16 nError );


#endif
