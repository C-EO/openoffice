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



#ifndef SC_PAGEDATA_HXX
#define SC_PAGEDATA_HXX

#include "global.hxx"
#include "address.hxx"

class ScDocShell;

//============================================================================

class ScPrintRangeData
{
private:
	ScRange		aPrintRange;
	size_t		nPagesX;
	SCCOL*		pPageEndX;
	size_t		nPagesY;
	SCROW*		pPageEndY;
	long		nFirstPage;
	sal_Bool		bTopDown;
	sal_Bool		bAutomatic;

public:
				ScPrintRangeData();
				~ScPrintRangeData();

	void			SetPrintRange( const ScRange& rNew )	{ aPrintRange = rNew; }
	const ScRange&	GetPrintRange() const					{ return aPrintRange; }

	void			SetPagesX( size_t nCount, const SCCOL* pEnd );
	void			SetPagesY( size_t nCount, const SCROW* pEnd );

	size_t			GetPagesX() const		{ return nPagesX;   }
	const SCCOL*	GetPageEndX() const		{ return pPageEndX; }
	size_t			GetPagesY() const		{ return nPagesY;   }
	const SCROW*	GetPageEndY() const		{ return pPageEndY; }

	void			SetFirstPage( long nNew )	{ nFirstPage = nNew; }
	long			GetFirstPage() const		{ return nFirstPage; }
	void			SetTopDown( sal_Bool bSet )		{ bTopDown = bSet; }
	sal_Bool			IsTopDown() const			{ return bTopDown; }
	void			SetAutomatic( sal_Bool bSet )	{ bAutomatic = bSet; }
	sal_Bool			IsAutomatic() const			{ return bAutomatic; }
};

class ScPageBreakData
{
private:
	size_t				nAlloc;
	size_t				nUsed;
	ScPrintRangeData*	pData;			// Array

public:
				ScPageBreakData(size_t nMax);
				~ScPageBreakData();

	size_t				GetCount() const			{ return nUsed; }
	ScPrintRangeData&	GetData(size_t i);

	sal_Bool				IsEqual( const ScPageBreakData& rOther ) const;

	void				AddPages();
};



#endif
