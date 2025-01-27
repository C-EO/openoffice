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



#ifndef _OSL_SECURITY_HXX_
#define _OSL_SECURITY_HXX_

#include <rtl/ustring.hxx>

#ifndef _OSL_SECURITY_DECL_HXX
#	include <osl/security_decl.hxx>
#endif

namespace osl
{     

inline Security::Security()
{
	m_handle = osl_getCurrentSecurity();
}

inline Security::~Security()
{
    osl_freeSecurityHandle(m_handle);
}

inline sal_Bool Security::logonUser(const rtl::OUString& strName,
									const rtl::OUString& strPasswd)
{
    osl_freeSecurityHandle(m_handle);
	
    m_handle = 0;
    
	return (osl_loginUser( strName.pData, strPasswd.pData, &m_handle) 
	        == osl_Security_E_None);
}

inline sal_Bool Security::logonUser( const rtl::OUString& strName,
									  const rtl::OUString& strPasswd,
									  const rtl::OUString& strFileServer )
{
    osl_freeSecurityHandle(m_handle);
	
    m_handle = NULL;
	
    return (osl_loginUserOnFileServer(strName.pData, strPasswd.pData, strFileServer.pData, &m_handle) 
	        == osl_Security_E_None);
}

inline sal_Bool Security::getUserIdent( rtl::OUString& strIdent) const
{
    return osl_getUserIdent( m_handle, &strIdent.pData );
}


inline sal_Bool Security::getUserName( rtl::OUString& strName ) const
{
    return osl_getUserName( m_handle, &strName.pData );
}


inline sal_Bool Security::getHomeDir( rtl::OUString& strDirectory) const
{
    return osl_getHomeDir(m_handle, &strDirectory.pData );
}


inline sal_Bool Security::getConfigDir( rtl::OUString& strDirectory ) const
{
    return osl_getConfigDir( m_handle, &strDirectory.pData );
}

inline sal_Bool Security::isAdministrator() const
{
    return osl_isAdministrator(m_handle);
}

inline oslSecurity Security::getHandle() const
{
    return m_handle;
}


}

#endif	// _OSL_SECURITY_HXX_
