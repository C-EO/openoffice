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


#ifndef _DBTREE_HXX
#define _DBTREE_HXX

#include <svtools/svtreebx.hxx>
#include <com/sun/star/uno/Reference.h>

namespace com{namespace sun{namespace star{namespace container{
	class XNameAccess;
}}}}
#include "swdllapi.h"
#include <swtypes.hxx>
#include <swunodef.hxx>

class SwDBTreeList_Impl;
class SwWrtShell;

class SW_DLLPUBLIC SwDBTreeList : public SvTreeListBox
{
    ImageList       aImageList;
    ImageList       aImageListHC;
	Image			aDBBMP;
	Image			aTableBMP;
	Image			aQueryBMP;

	String			sDefDBName;
	sal_Bool			bInitialized;
	sal_Bool			bShowColumns;

    SwDBTreeList_Impl* pImpl;

    SW_DLLPRIVATE DECL_LINK( DBCompare, SvSortData* );

	SW_DLLPRIVATE void			InitTreeList();
	SW_DLLPRIVATE virtual void	RequestingChilds( SvLBoxEntry* pParent );

	SW_DLLPRIVATE virtual sal_Int8 AcceptDrop( const AcceptDropEvent& rEvt );
	SW_DLLPRIVATE virtual void 	StartDrag( sal_Int8 nAction, const Point& rPosPixel );

    using SvTreeListBox::Select;

public:
	 SwDBTreeList( Window* pParent, const ResId& rResId,
                    SwWrtShell* pSh,
                    const String& rDefDBName = aEmptyStr,
                    const sal_Bool bShowCol = sal_False );
	virtual ~SwDBTreeList();

	String  GetDBName( String& rTableName, String& rColumnName, sal_Bool* pbIsTable = 0);

	void	Select( const String& rDBName, const String& rTableName,
					const String& rColumnName );

	void	ShowColumns(sal_Bool bShowCol);
    void    SetWrtShell(SwWrtShell& rSh);
    
    void    AddDataSource(const String& rSource);
};

#endif
