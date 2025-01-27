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



#ifndef _SVX_FRMDIRLBOX_HXX
#define _SVX_FRMDIRLBOX_HXX

#ifndef _LSTBOX_HXX
#include <vcl/lstbox.hxx>
#endif
#include <sfx2/itemconnect.hxx>
#include <editeng/frmdir.hxx>
#include "svx/svxdllapi.h"

class SvxFrameDirectionItem;

namespace svx {

// ============================================================================

/** This listbox contains entries to select horizontal text direction.

    The control works on the SvxFrameDirection enumeration (i.e. left-to-right,
    right-to-left), used i.e. in conjunction with the SvxFrameDirectionItem.
 */
class SVX_DLLPUBLIC FrameDirectionListBox : public ListBox
{
public:
    explicit            FrameDirectionListBox( Window* pParent, WinBits nStyle = WB_BORDER );
    explicit            FrameDirectionListBox( Window* pParent, const ResId& rResId );
    virtual             ~FrameDirectionListBox();

    /** Inserts a string with corresponding direction enum into the listbox. */
    void                InsertEntryValue(
                            const String& rString,
                            SvxFrameDirection eDirection,
                            sal_uInt16 nPos = LISTBOX_APPEND );
    /** Removes the entry, that represents the specified frame direction. */
    void                RemoveEntryValue( SvxFrameDirection eDirection );

    /** Selects the specified frame direction. */
    void                SelectEntryValue( SvxFrameDirection eDirection );
    /** Returns the currently selected frame direction. */
    SvxFrameDirection   GetSelectEntryValue() const;

    /** Saves the currently selected frame direction. */
    inline void         SaveValue() { meSaveValue = GetSelectEntryValue(); }
    /** Returns the frame direction saved with SaveValue(). */
    inline SvxFrameDirection GetSavedValue() const { return meSaveValue; }

private:
    SvxFrameDirection   meSaveValue;    /// Saved value for later comparison.
};

typedef FrameDirectionListBox FrameDirListBox;

// ============================================================================

/** Wrapper for usage of a FrameDirectionListBox in item connections. */
class SVX_DLLPUBLIC FrameDirListBoxWrapper : public sfx::SingleControlWrapper< FrameDirListBox, SvxFrameDirection >
{
public:
    explicit            FrameDirListBoxWrapper( FrameDirListBox& rListBox );

    virtual bool        IsControlDontKnow() const;
    virtual void        SetControlDontKnow( bool bSet );

    virtual SvxFrameDirection GetControlValue() const;
    virtual void        SetControlValue( SvxFrameDirection eValue );
};

/** Wrapper for usage of a SvxFrameDirectionItem in item connections. */
typedef sfx::ValueItemWrapper< SvxFrameDirectionItem, SvxFrameDirection, sal_uInt16 > FrameDirItemWrapper;

/** An item<->control connection for a FrameDirectionListBox. */
typedef sfx::ItemControlConnection< FrameDirItemWrapper, FrameDirListBoxWrapper > FrameDirListBoxConnection;

// ============================================================================

} // namespace svx

#endif
