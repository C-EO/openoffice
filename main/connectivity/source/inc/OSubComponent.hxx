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


#ifndef _CONNECTIVITY_OSUBCOMPONENT_HXX_
#define _CONNECTIVITY_OSUBCOMPONENT_HXX_

#include <cppuhelper/weak.hxx>
#include <cppuhelper/interfacecontainer.h>
#include "connectivity/dbtoolsdllapi.hxx"

namespace com
{
	namespace sun
	{
		namespace star
		{
			namespace lang
			{
				class XComponent;
			}
		}
	}
}
namespace connectivity
{
    OOO_DLLPUBLIC_DBTOOLS
	void release(oslInterlockedCount& _refCount,
				 ::cppu::OBroadcastHelper& rBHelper,
				 ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& _xInterface,
				 ::com::sun::star::lang::XComponent* _pObject);
	//************************************************************
	// OSubComponent
	//************************************************************
	template <class SELF, class WEAK> class OSubComponent
	{
	protected:
		// the parent must support the tunnel implementation
		::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > m_xParent;
		SELF*	m_pDerivedImplementation;

	public:
		OSubComponent(
				const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& _xParent,
				SELF* _pDerivedImplementation)
			:m_xParent(_xParent)
			,m_pDerivedImplementation(_pDerivedImplementation)
		{
		}

	protected:
		void dispose_ChildImpl()
		{
			// avoid ambiguity
			::osl::MutexGuard aGuard( m_pDerivedImplementation->WEAK::rBHelper.rMutex );
		m_xParent.clear();
		}
		void relase_ChildImpl()
		{
			::connectivity::release(m_pDerivedImplementation->m_refCount,
									m_pDerivedImplementation->WEAK::rBHelper,
									m_xParent,
									m_pDerivedImplementation);

			m_pDerivedImplementation->WEAK::release();
		}
	};
}
#endif // _CONNECTIVITY_OSUBCOMPONENT_HXX_
