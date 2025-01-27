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



#ifndef _SD_UNDOPAGE_HXX
#define _SD_UNDOPAGE_HXX

#include <tools/gen.hxx>
#include <vcl/prntypes.hxx>

#include "sdundo.hxx"

class SdDrawDocument;
class SdPage;

/************************************************************************/

class SdPageFormatUndoAction : public SdUndoAction
{
	SdPage*     mpPage;

	Size        maOldSize;
	sal_Int32       mnOldLeft;
	sal_Int32       mnOldRight;
	sal_Int32       mnOldUpper;
	sal_Int32       mnOldLower;
	sal_Bool        mbOldScale;
	Orientation meOldOrientation;
	sal_uInt16      mnOldPaperBin;
	sal_Bool        mbOldFullSize;

	Size        maNewSize;
	sal_Int32       mnNewLeft;
	sal_Int32       mnNewRight;
	sal_Int32       mnNewUpper;
	sal_Int32       mnNewLower;
	sal_Bool        mbNewScale;
	Orientation meNewOrientation;
	sal_uInt16      mnNewPaperBin;
	sal_Bool        mbNewFullSize;

public:
	TYPEINFO();
	SdPageFormatUndoAction(	SdDrawDocument*	pDoc,
							SdPage*			pThePage,
							const Size&		rOldSz,
							sal_Int32			nOldLft,
							sal_Int32			nOldRgt,
							sal_Int32			nOldUpr,
							sal_Int32			nOldLwr,
							sal_Bool            bOldScl,
							Orientation     eOldOrient,
							sal_uInt16      	nOPaperBin,
							sal_Bool        	bOFullSize,

							const Size&     rNewSz,
							sal_Int32			nNewLft,
							sal_Int32			nNewRgt,
							sal_Int32			nNewUpr,
							sal_Int32           nNewLwr,
							sal_Bool            bNewScl,
							Orientation     eNewOrient,
							sal_uInt16      	nNPaperBin,
							sal_Bool        	bNFullSize
							) :
		SdUndoAction(pDoc),
		mpPage		(pThePage),
		maOldSize	(rOldSz),
		mnOldLeft	(nOldLft),
		mnOldRight	(nOldRgt),
		mnOldUpper	(nOldUpr),
		mnOldLower	(nOldLwr),
		mbOldScale   (bOldScl),
		meOldOrientation(eOldOrient),
		mnOldPaperBin (nOPaperBin),
		mbOldFullSize (bOFullSize),


		maNewSize	(rNewSz),
		mnNewLeft	(nNewLft),
		mnNewRight	(nNewRgt),
		mnNewUpper	(nNewUpr),
		mnNewLower   (nNewLwr),
		mbNewScale   (bNewScl),
		meNewOrientation(eNewOrient),
		mnNewPaperBin (nNPaperBin),
		mbNewFullSize (bNFullSize)

		{}
	virtual ~SdPageFormatUndoAction();

	virtual void Undo();
	virtual void Redo();
};

/************************************************************************/

class SdPageLRUndoAction : public SdUndoAction
{
	SdPage*	mpPage;

	sal_Int32	mnOldLeft;
	sal_Int32	mnOldRight;
	sal_Int32	mnNewLeft;
	sal_Int32	mnNewRight;

public:
	TYPEINFO();
	SdPageLRUndoAction( SdDrawDocument* pDoc, SdPage* pThePage,
						sal_Int32 nOldLft, sal_Int32 nOldRgt,
						sal_Int32 nNewLft, sal_Int32 nNewRgt ) :
		SdUndoAction(pDoc),
		mpPage		(pThePage),
		mnOldLeft	(nOldLft),
		mnOldRight	(nOldRgt),
		mnNewLeft	(nNewLft),
		mnNewRight	(nNewRgt)
		{}
	virtual ~SdPageLRUndoAction();

	virtual void Undo();
	virtual void Redo();
};

/************************************************************************/

class SdPageULUndoAction : public SdUndoAction
{
	SdPage*	mpPage;

	sal_Int32	mnOldUpper;
	sal_Int32	mnOldLower;
	sal_Int32	mnNewUpper;
	sal_Int32	mnNewLower;

public:
	TYPEINFO();
	SdPageULUndoAction( SdDrawDocument* pDoc, SdPage* pThePage,
						sal_Int32 nOldUpr, sal_Int32 nOldLwr,
						sal_Int32 nNewUpr, sal_Int32 nNewLwr ) :
		SdUndoAction(pDoc),
		mpPage		(pThePage),
		mnOldUpper	(nOldUpr),
		mnOldLower	(nOldLwr),
		mnNewUpper	(nNewUpr),
		mnNewLower	(nNewLwr)
		{}
	virtual ~SdPageULUndoAction();

	virtual void Undo();
	virtual void Redo();
};



#endif		// _SD_UNDOPAGE_HXX
