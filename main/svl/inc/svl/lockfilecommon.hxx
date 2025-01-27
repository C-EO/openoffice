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



#ifndef _SVT_LOCKFILECOMMON_HXX
#define _SVT_LOCKFILECOMMON_HXX

#include <svl/svldllapi.h>

#include <com/sun/star/io/XStream.hpp>
#include <com/sun/star/io/XInputStream.hpp>
#include <com/sun/star/io/XOutputStream.hpp>
#include <com/sun/star/io/XSeekable.hpp>
#include <com/sun/star/io/XTruncate.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>

#include <osl/mutex.hxx>
#include <tools/urlobj.hxx>

#define LOCKFILE_OOOUSERNAME_ID   0
#define LOCKFILE_SYSUSERNAME_ID   1
#define LOCKFILE_LOCALHOST_ID     2
#define LOCKFILE_EDITTIME_ID      3
#define LOCKFILE_USERURL_ID       4
#define LOCKFILE_ENTRYSIZE        5 

namespace svt {

// This is a general implementation that is used in document lock file implementation and in sharing control file implementation
class SVL_DLLPUBLIC LockFileCommon
{
protected:
    ::osl::Mutex m_aMutex;

    ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory > m_xFactory;
    ::rtl::OUString m_aURL;


    INetURLObject ResolveLinks( const INetURLObject& aDocURL );

public:
    LockFileCommon( const ::rtl::OUString& aOrigURL, const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& xFactory, const ::rtl::OUString& aPrefix );
    ~LockFileCommon();

    static ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence< ::rtl::OUString > > ParseList( const ::com::sun::star::uno::Sequence< sal_Int8 >& aBuffer );
    static ::com::sun::star::uno::Sequence< ::rtl::OUString > ParseEntry( const ::com::sun::star::uno::Sequence< sal_Int8 >& aBuffer, sal_Int32& o_nCurPos );
    static ::rtl::OUString ParseName( const ::com::sun::star::uno::Sequence< sal_Int8 >& aBuffer, sal_Int32& o_nCurPos );
    static ::rtl::OUString EscapeCharacters( const ::rtl::OUString& aSource );
    static ::rtl::OUString GetOOOUserName();
    static ::rtl::OUString GetCurrentLocalTime();
    static ::com::sun::star::uno::Sequence< ::rtl::OUString > GenerateOwnEntry();
};

}

#endif
