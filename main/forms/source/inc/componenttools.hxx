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



#ifndef FORMS_SOURCE_INC_COMPONENTTOOLS_HXX
#define FORMS_SOURCE_INC_COMPONENTTOOLS_HXX

/** === begin UNO includes === **/
#include <com/sun/star/uno/Type.hxx>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/frame/XModel.hpp>
/** === end UNO includes === **/

#include <set>
#include <functional>

//........................................................................
namespace frm
{
//........................................................................

    struct TypeCompareLess : public ::std::binary_function< ::com::sun::star::uno::Type, ::com::sun::star::uno::Type, bool >
    {
    private:
        typedef ::com::sun::star::uno::Type             Type;

    public:
        bool operator()( const Type& _rLHS, const Type& _rRHS ) const
        {
            return _rLHS.getTypeName() < _rRHS.getTypeName();
        }
    };

	//====================================================================
	//= TypeBag
	//====================================================================
    /** a helper class which merges sequences of <type scope="com::sun::star::uno">Type</type>s,
        so that the resulting sequence contains every type at most once
    */
	class TypeBag
	{
    public:
        typedef ::com::sun::star::uno::Type             Type;
        typedef ::com::sun::star::uno::Sequence< Type > TypeSequence;
        typedef ::std::set< Type, TypeCompareLess >     TypeSet;

    private:
        TypeSet     m_aTypes;

    public:
        TypeBag(
            const TypeSequence& _rTypes1
        );

        TypeBag(
            const TypeSequence& _rTypes1,
            const TypeSequence& _rTypes2
        );
        TypeBag(
            const TypeSequence& _rTypes1,
            const TypeSequence& _rTypes2,
            const TypeSequence& _rTypes3
        );

        void    addType( const Type& i_rType );
        void    addTypes( const TypeSequence& _rTypes );
        void    removeType( const Type& i_rType );

        /** returns the types represented by this bag
        */
        TypeSequence    getTypes() const;
	};

    ::com::sun::star::uno::Reference< ::com::sun::star::frame::XModel >  getXModel(
        const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& _rxComponent );

//........................................................................
} // namespace frm
//........................................................................

#endif // FORMS_SOURCE_INC_COMPONENTTOOLS_HXX
