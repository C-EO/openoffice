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



#ifndef _COMPHELPER_CONTAINERMULTIPLEXER_HXX_
#define _COMPHELPER_CONTAINERMULTIPLEXER_HXX_

#include <com/sun/star/container/XContainer.hpp>
#include <cppuhelper/implbase1.hxx>
#include <osl/mutex.hxx>
#include "comphelper/comphelperdllapi.h"

//.........................................................................
namespace comphelper
{
//.........................................................................

	class OContainerListenerAdapter;
	//=====================================================================
	//= OContainerListener
	//=====================================================================
	/** a non-UNO container listener
		<p>Useful if you have a non-refcountable class which should act as container listener.<br/>
		In this case, derive this class from OContainerListener, and create an adapter
		<type>OContainerListenerAdapter</type> which multiplexes the changes.</p>
	*/
	class COMPHELPER_DLLPUBLIC OContainerListener
	{
		friend class OContainerListenerAdapter;
	protected:
		OContainerListenerAdapter*	m_pAdapter;
		::osl::Mutex&				m_rMutex;

	public:
		OContainerListener(::osl::Mutex& _rMutex);
		virtual ~OContainerListener();

		virtual void _elementInserted( const ::com::sun::star::container::ContainerEvent& _rEvent ) throw(::com::sun::star::uno::RuntimeException);
		virtual void _elementRemoved( const ::com::sun::star::container::ContainerEvent& _Event ) throw(::com::sun::star::uno::RuntimeException);
		virtual void _elementReplaced( const ::com::sun::star::container::ContainerEvent& _rEvent ) throw(::com::sun::star::uno::RuntimeException);
		virtual void _disposing(const ::com::sun::star::lang::EventObject& _rSource) throw( ::com::sun::star::uno::RuntimeException);

	protected:
		void setAdapter(OContainerListenerAdapter* _pAdapter);
	};

	//=====================================================================
	//= OContainerListenerAdapter
	//=====================================================================
	class COMPHELPER_DLLPUBLIC OContainerListenerAdapter
			:public cppu::WeakImplHelper1< ::com::sun::star::container::XContainerListener >
	{
		friend class OContainerListener;

	protected:
		::com::sun::star::uno::Reference< ::com::sun::star::container::XContainer >
								m_xContainer;
		OContainerListener*		m_pListener;
		sal_Int32				m_nLockCount;

		virtual ~OContainerListenerAdapter();

	public:
		OContainerListenerAdapter(OContainerListener* _pListener,
			const  ::com::sun::star::uno::Reference< ::com::sun::star::container::XContainer >& _rxContainer);

		// XEventListener
		virtual void SAL_CALL disposing( const  ::com::sun::star::lang::EventObject& Source ) throw( ::com::sun::star::uno::RuntimeException);

		// XContainerListener
		virtual void SAL_CALL elementInserted( const ::com::sun::star::container::ContainerEvent& Event ) throw(::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL elementRemoved( const ::com::sun::star::container::ContainerEvent& Event ) throw(::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL elementReplaced( const ::com::sun::star::container::ContainerEvent& Event ) throw(::com::sun::star::uno::RuntimeException);

		// locking the multiplexer
		void		lock();
		void		unlock();
		sal_Int32	locked() const { return m_nLockCount; }

		/// dispose the object. No multiplexing anymore
		void		dispose();

		const ::com::sun::star::uno::Reference< ::com::sun::star::container::XContainer >&
					getContainer() const { return m_xContainer; }
	};

//.........................................................................
}	// namespace dbaui
//.........................................................................

#endif // _COMPHELPER_CONTAINERMULTIPLEXER_HXX_
