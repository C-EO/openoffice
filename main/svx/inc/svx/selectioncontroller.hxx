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



#ifndef _SELECTIONCONTROLLER_HXX
#define _SELECTIONCONTROLLER_HXX

#include "svx/svxdllapi.h"

#include <boost/shared_ptr.hpp>

#include <cppuhelper/weak.hxx>

class KeyEvent;
class MouseEvent;
class Window;
class SfxItemSet;
class SfxRequest;
class SfxStyleSheet;
class SdrPage;
class SdrModel;

namespace sdr
{

class SVX_DLLPUBLIC SelectionController: public cppu::OWeakObject
{
public:
	virtual bool onKeyInput(const KeyEvent& rKEvt, Window* pWin);
	virtual bool onMouseButtonDown(const MouseEvent& rMEvt, Window* pWin);
	virtual bool onMouseButtonUp(const MouseEvent& rMEvt, Window* pWin);
	virtual bool onMouseMove(const MouseEvent& rMEvt, Window* pWin);

	virtual void onSelectionHasChanged();

	virtual void GetState( SfxItemSet& rSet );
	virtual void Execute( SfxRequest& rReq );

	virtual bool DeleteMarked();

	virtual bool GetAttributes(SfxItemSet& rTargetSet, bool bOnlyHardAttr) const;
	virtual bool SetAttributes(const SfxItemSet& rSet, bool bReplaceAll);

	virtual bool GetStyleSheet( SfxStyleSheet* &rpStyleSheet ) const;
	virtual bool SetStyleSheet( SfxStyleSheet* pStyleSheet, bool bDontRemoveHardAttr );

	virtual bool GetMarkedObjModel( SdrPage* pNewPage );
	virtual bool PasteObjModel( const SdrModel& rModel );
	
	/** returns a format paint brush set from the current selection */
	virtual bool TakeFormatPaintBrush( boost::shared_ptr< SfxItemSet >& rFormatSet  );
	
	/** applies a format paint brush set from the current selection.
	    if bNoCharacterFormats is true, no character attributes are changed.
	    if bNoParagraphFormats is true, no paragraph attributes are changed.
	*/
	virtual bool ApplyFormatPaintBrush( SfxItemSet& rFormatSet, bool bNoCharacterFormats, bool bNoParagraphFormats );	
};

}

#endif //_SELECTIONCONTROLLER_HXX
