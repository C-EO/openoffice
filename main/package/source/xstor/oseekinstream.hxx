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



#ifndef _INPUTSEEKSTREAM_HXX_
#define _INPUTSEEKSTREAM_HXX_

#include <com/sun/star/io/XSeekable.hpp>

#include "ocompinstream.hxx"

class OInputSeekStream : public OInputCompStream
						, public ::com::sun::star::io::XSeekable
{
protected:
	::com::sun::star::uno::Reference < ::com::sun::star::io::XSeekable > m_xSeekable;

public:
	OInputSeekStream( OWriteStream_Impl& pImpl,
					  ::com::sun::star::uno::Reference < ::com::sun::star::io::XInputStream > xStream,
					  const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aProps,
					  sal_Int32 nStorageType );

	OInputSeekStream( ::com::sun::star::uno::Reference < ::com::sun::star::io::XInputStream > xStream,
					  const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aProps,
					  sal_Int32 nStorageType );

	virtual ~OInputSeekStream();

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes() throw (::com::sun::star::uno::RuntimeException);

	// XInterface
	virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& rType ) throw( ::com::sun::star::uno::RuntimeException );
	virtual void SAL_CALL acquire() throw();
	virtual void SAL_CALL release() throw();

	//XSeekable
    virtual void SAL_CALL seek( sal_Int64 location ) throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::io::IOException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Int64 SAL_CALL getPosition() throw (::com::sun::star::io::IOException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Int64 SAL_CALL getLength() throw (::com::sun::star::io::IOException, ::com::sun::star::uno::RuntimeException);

};

#endif
