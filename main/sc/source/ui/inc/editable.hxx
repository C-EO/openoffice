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



#ifndef SC_EDITABLE_HXX
#define SC_EDITABLE_HXX

#include "address.hxx"
#include <tools/solar.h>

class ScDocument;
class ScViewFunc;
class ScMarkData;
class ScRange;


class ScEditableTester
{
	sal_Bool	bIsEditable;
	sal_Bool	bOnlyMatrix;

public:
			// no test in ctor
			ScEditableTester();

			// calls TestBlock
			ScEditableTester( ScDocument* pDoc, SCTAB nTab,
						SCCOL nStartCol, SCROW nStartRow, SCCOL nEndCol, SCROW nEndRow );

			// calls TestSelectedBlock
			ScEditableTester( ScDocument* pDoc, 
						SCCOL nStartCol, SCROW nStartRow, SCCOL nEndCol, SCROW nEndRow,
						const ScMarkData& rMark );

			// calls TestRange
			ScEditableTester( ScDocument* pDoc, const ScRange& rRange );

			// calls TestSelection
			ScEditableTester( ScDocument* pDoc, const ScMarkData& rMark );

			// calls TestView
			ScEditableTester( ScViewFunc* pView );

			~ScEditableTester() {}

			// Several calls to the Test... methods check if *all* of the ranges
			// are editable. For several independent checks, Reset() has to be used.
	void	TestBlock( ScDocument* pDoc, SCTAB nTab,
						SCCOL nStartCol, SCROW nStartRow, SCCOL nEndCol, SCROW nEndRow );
	void	TestSelectedBlock( ScDocument* pDoc, 
						SCCOL nStartCol, SCROW nStartRow, SCCOL nEndCol, SCROW nEndRow,
						const ScMarkData& rMark );
	void	TestRange( ScDocument* pDoc, const ScRange& rRange );
	void	TestSelection( ScDocument* pDoc, const ScMarkData& rMark );
	void	TestView( ScViewFunc* pView );

	sal_Bool	IsEditable() const			{ return bIsEditable; }
	sal_Bool	IsFormatEditable() const	{ return bIsEditable || bOnlyMatrix; }
	sal_uInt16	GetMessageId() const;
};

#endif
