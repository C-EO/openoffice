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


#ifndef DBAUI_IUPDATEHELPER_HXX
#define DBAUI_IUPDATEHELPER_HXX

#include <com/sun/star/util/Date.hpp>
#include <com/sun/star/util/DateTime.hpp>
#include <com/sun/star/util/Time.hpp>

namespace dbaui
{
    class SAL_NO_VTABLE IUpdateHelper
    {
    public:
        virtual void updateString(sal_Int32 _nPos, const ::rtl::OUString& _sValue) = 0;
        virtual void updateDouble(sal_Int32 _nPos,const double& _nValue) = 0;
        virtual void updateInt(sal_Int32 _nPos,const sal_Int32& _nValue) = 0;
        virtual void updateNull(sal_Int32 _nPos, ::sal_Int32 sqlType) = 0;
        virtual void updateDate(sal_Int32 _nPos,const ::com::sun::star::util::Date& _nValue) = 0;
        virtual void updateTime(sal_Int32 _nPos,const ::com::sun::star::util::Time& _nValue) = 0;
        virtual void updateTimestamp(sal_Int32 _nPos,const ::com::sun::star::util::DateTime& _nValue) = 0;
        virtual void moveToInsertRow() = 0;
        virtual void insertRow() = 0;
    };
}

#endif // DBAUI_IUPDATEHELPER_HXX
