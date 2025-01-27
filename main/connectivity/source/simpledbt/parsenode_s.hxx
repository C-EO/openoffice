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



#ifndef CONNECTIVITY_PARSENODE_SIMPLE_HXX
#define CONNECTIVITY_PARSENODE_SIMPLE_HXX

#include <connectivity/virtualdbtools.hxx>
#include "refbase.hxx"

//........................................................................
namespace connectivity
{
//........................................................................

	class OSQLParseNode;
	//================================================================
	//= OSimpleParseNode
	//================================================================
	class OSimpleParseNode
			:public simple::ISQLParseNode
			,public ORefBase
	{
	protected:
		const OSQLParseNode*	m_pFullNode;
		sal_Bool				m_bOwner;

	public:
		OSimpleParseNode(const OSQLParseNode* _pNode, sal_Bool _bTakeOwnership = sal_True);
		~OSimpleParseNode();

		// ISQLParseNode
		virtual void parseNodeToStr(::rtl::OUString& _rString, 
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
			const IParseContext* _pContext
		) const;

		virtual void parseNodeToPredicateStr(::rtl::OUString& _rString,
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
			const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatter >& _rxFormatter,
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _rxField,
			const ::com::sun::star::lang::Locale& _rIntl,
			const sal_Char _cDecSeparator,
			const IParseContext* _pContext
		) const;

		// disambiguate IReference
		virtual oslInterlockedCount SAL_CALL acquire();
		virtual oslInterlockedCount SAL_CALL release();
	};

//........................................................................
}	// namespace connectivity
//........................................................................

#endif // CONNECTIVITY_PARSENODE_SIMPLE_HXX
