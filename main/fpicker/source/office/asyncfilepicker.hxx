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



#ifndef SVTOOLS_SOURCE_FILEPICKER_ASYNCFILEPICKER_HXX
#define SVTOOLS_SOURCE_FILEPICKER_ASYNCFILEPICKER_HXX

/** === begin UNO includes === **/
/** === end UNO includes === **/

#include <tools/link.hxx>
#include <tools/string.hxx>
#include <rtl/ref.hxx>
#include <rtl/ustring.hxx>
#include <com/sun/star/uno/Sequence.h>

class SvtFileView;
class SvtFileDialog;

typedef ::com::sun::star::uno::Sequence< ::rtl::OUString >	OUStringList;

//........................................................................
namespace svt
{
//........................................................................
	

	//====================================================================
	//= AsyncPickerAction
	//====================================================================
    class AsyncPickerAction : public ::rtl::IReference
	{
    public:
        enum Action
        {
            ePrevLevel,
            eOpenURL,
            eExecuteFilter
        };

    private:
        mutable oslInterlockedCount m_refCount;
        Action                      m_eAction;
        SvtFileView*                m_pView;
        SvtFileDialog*              m_pDialog;
        String                      m_sURL;
        String                      m_sFileName;
        bool                        m_bRunning;

    public:
        AsyncPickerAction( SvtFileDialog* _pDialog, SvtFileView* _pView, const Action _eAction );

        /** executes the action

            @param _nMinTimeout
                the minimum timeout to wait, in milliseconds. If negative, the action will we done
                synchronously. If between 0 and 999, it will be corrected to 1000, means the
                smallest valid value is 1000 (which equals one second).
            @param _nMaxTimeout
                The maximum time to wait for a result, in milliseconds. If there's no result of
                the action within the given time frame, the action will be cancelled.
                If smaller than or equal to <arg>_nMinTimeout</arg>, it will be corrected to
                <arg>_nMinTimeout</arg> + 30000.
        */
        void execute( 
			const String& _rURL, 
			const String& _rFilter, 
			sal_Int32 _nMinTimeout, 
			sal_Int32 _nMaxTimeout,
			const OUStringList& rBlackList = OUStringList() );

        /// cancels the running action
        void cancel();

        // IReference overridables
	    virtual oslInterlockedCount SAL_CALL acquire();
	    virtual oslInterlockedCount SAL_CALL release();

    protected:
        virtual ~AsyncPickerAction();

    private:
        DECL_LINK( OnActionDone, void* );

        AsyncPickerAction();                                        // never implemented
        AsyncPickerAction( const AsyncPickerAction& );              // never implemented
        AsyncPickerAction& operator=( const AsyncPickerAction& );   // never implemented
	};

//........................................................................
} // namespace svt
//........................................................................

#endif // SVTOOLS_SOURCE_FILEPICKER_ASYNCFILEPICKER_HXX
