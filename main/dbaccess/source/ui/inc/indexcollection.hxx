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



#ifndef _DBAUI_INDEXCOLLECTION_HXX_
#define _DBAUI_INDEXCOLLECTION_HXX_

#ifndef _COM_SUN_STAR_CONTAINER_XNAMEACCESS_HPP_
#include <com/sun/star/container/XNameAccess.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_SQLEXCEPTION_HPP_
#include <com/sun/star/sdbc/SQLException.hpp>
#endif
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif
#ifndef _DBAUI_INDEXES_HXX_
#include "indexes.hxx"
#endif

//......................................................................
namespace dbaui
{
//......................................................................

	//==================================================================
	//= OIndexCollection
	//==================================================================
	class OIndexCollection
	{
	protected:
		::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >
					m_xIndexes;

		// cached information
		Indexes		m_aIndexes;

	public:
		// construction
		OIndexCollection();
		OIndexCollection(const OIndexCollection& _rSource);
		//	OIndexCollection(const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& _rxIndexes);

		const OIndexCollection& operator=(const OIndexCollection& _rSource);

		// iterating through the collection
		typedef OIndex* iterator;
		typedef OIndex const* const_iterator;

		/// get access to the first element of the index collection
		Indexes::const_iterator begin() const { return m_aIndexes.begin(); }
		/// get access to the first element of the index collection
		Indexes::iterator begin() { return m_aIndexes.begin(); }
		/// get access to the (last + 1st) element of the index collection
		Indexes::const_iterator end() const { return m_aIndexes.end(); }
		/// get access to the (last + 1st) element of the index collection
		Indexes::iterator end() { return m_aIndexes.end(); }

		// searching
		Indexes::const_iterator find(const String& _rName) const;
		Indexes::iterator find(const String& _rName);
		Indexes::const_iterator findOriginal(const String& _rName) const;
		Indexes::iterator findOriginal(const String& _rName);

		// inserting without committing
		// the OriginalName of the newly inserted index will be empty, thus indicating that it's new
		Indexes::iterator insert(const String& _rName);
		// commit a new index, which is already part if the collection, but does not have an equivalent in the
		// data source, yet
		void commitNewIndex(const Indexes::iterator& _rPos) SAL_THROW((::com::sun::star::sdbc::SQLException));

		// reset the data for the given index
		void resetIndex(const Indexes::iterator& _rPos) SAL_THROW((::com::sun::star::sdbc::SQLException));

		// attach to a new key container
		void attach(const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& _rxIndexes);
		// detach from the container
		void detach();

		// is the object valid?
		sal_Bool	isValid() const { return m_xIndexes.is(); }
		// number of contained indexes
		sal_Int32	size() const { return m_aIndexes.size(); }

		/// drop an index, and remove it from the collection
		sal_Bool	drop(const Indexes::iterator& _rPos) SAL_THROW((::com::sun::star::sdbc::SQLException));
		/// simply drop the index described by the name, but don't remove the descriptor from the collection
		sal_Bool	dropNoRemove(const Indexes::iterator& _rPos) SAL_THROW((::com::sun::star::sdbc::SQLException));

	protected:
		void implConstructFrom(const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& _rxIndexes);
		void implFillIndexInfo(OIndex& _rIndex, ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet > _rxDescriptor) SAL_THROW((::com::sun::star::uno::Exception));
		void implFillIndexInfo(OIndex& _rIndex) SAL_THROW((::com::sun::star::uno::Exception));
	};


//......................................................................
}	// namespace dbaui
//......................................................................

#endif // _DBAUI_INDEXCOLLECTION_HXX_
