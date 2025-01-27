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



#ifndef _UTL_PROPERTSETINFO_HXX_
#define _UTL_PROPERTSETINFO_HXX_

#include <com/sun/star/uno/Type.h>
#include <com/sun/star/beans/XPropertySetInfo.hpp>
#include <cppuhelper/implbase1.hxx>
#include <comphelper/stl_types.hxx>

//=========================================================================
//= property helper classes
//=========================================================================

//... namespace utl .......................................................
namespace utl
{
//.........................................................................

struct PropertyMapEntry
{
	const sal_Char* mpName;
	sal_uInt16 mnNameLen;
	sal_uInt16 mnWhich;
	const com::sun::star::uno::Type* mpType;
	sal_Int16 mnFlags;
	sal_uInt8 mnMemberId;
};

DECLARE_STL_USTRINGACCESS_MAP( PropertyMapEntry*, PropertyMap );

class PropertyMapImpl;

/** this class implements a XPropertySetInfo that is initialized with arrays of PropertyMapEntry.
	It is used by the class PropertySetHelper.
*/
class PropertySetInfo : public ::cppu::WeakImplHelper1< ::com::sun::star::beans::XPropertySetInfo >
{
private:
	PropertyMapImpl* mpMap;
public:
	PropertySetInfo() throw();
	virtual ~PropertySetInfo() throw();

	/** returns a stl map with all PropertyMapEntry pointer.<p>
		The key is the property name.
	*/
	const PropertyMap* getPropertyMap() const throw();

	/** adds an array of PropertyMapEntry to this instance.<p>
		The end is marked with a PropertyMapEntry where mpName equals NULL
	*/
	void add( PropertyMapEntry* pMap ) throw();

	/** removes an already added PropertyMapEntry which string in mpName equals to aName */
	void remove( const rtl::OUString& aName ) throw();

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property > SAL_CALL getProperties() throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::beans::Property SAL_CALL getPropertyByName( const ::rtl::OUString& aName ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL hasPropertyByName( const ::rtl::OUString& Name ) throw(::com::sun::star::uno::RuntimeException);
};

//.........................................................................
}
//... namespace utl .......................................................

#endif // _UTL_PROPERTSETINFO_HXX_
