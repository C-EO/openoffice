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


#ifndef _FRMSIDEBARWINCONTAINER_HXX
#define _FRMSIDEBARWINCONTAINER_HXX

#include <sal/types.h>
#include <vector>

class SwFrm;
class SwFmtFld;
class Window;

namespace sw { namespace sidebarwindows {

class SwSidebarWin;
class FrmSidebarWinContainer;

class SwFrmSidebarWinContainer
{
    public:
        SwFrmSidebarWinContainer();
        ~SwFrmSidebarWinContainer();

        bool insert( const SwFrm& rFrm,
                     const SwFmtFld& rFmtFld,
                     SwSidebarWin& rSidebarWin );

        bool remove( const SwFrm& rFrm,
                     const SwSidebarWin& rSidebarWin );

        bool empty( const SwFrm& rFrm );

        SwSidebarWin* get( const SwFrm& rFrm,
                           const sal_Int32 nIndex );

        void getAll( const SwFrm& rFrm,
                     std::vector< Window* >* pSidebarWins );

    private:
        FrmSidebarWinContainer* mpFrmSidebarWinContainer;
};

} } // eof of namespace sw::sidebarwindows::

#endif
