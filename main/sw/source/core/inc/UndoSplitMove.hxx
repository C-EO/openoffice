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



#ifndef SW_UNDO_MOVE_HXX
#define SW_UNDO_MOVE_HXX

#include <undobj.hxx>


class SwUndoSplitNode: public SwUndo
{
    SwHistory* pHistory;
    SwRedlineData* pRedlData;
    sal_uLong nNode;
    xub_StrLen nCntnt;
    sal_Bool bTblFlag : 1;
    sal_Bool bChkTblStt : 1;

public:
    SwUndoSplitNode( SwDoc* pDoc, const SwPosition& rPos, sal_Bool bChkTbl );

    virtual ~SwUndoSplitNode();

    virtual void UndoImpl( ::sw::UndoRedoContext & );
    virtual void RedoImpl( ::sw::UndoRedoContext & );
    virtual void RepeatImpl( ::sw::RepeatContext & );

    void SetTblFlag()       { bTblFlag = sal_True; }
};


class SwUndoMove : public SwUndo, private SwUndRng, private SwUndoSaveCntnt
{
    // nDest.. - destination range of move (after move!)
    // nIns..  - source Position of move (after move!)
    // nMv..   - destination position of move (before move!); for REDO
    sal_uLong nDestSttNode, nDestEndNode, nInsPosNode, nMvDestNode;
    xub_StrLen nDestSttCntnt, nDestEndCntnt, nInsPosCntnt, nMvDestCntnt;

    sal_uInt16 nFtnStt; // StartPos of Footnotes in History

    sal_Bool bJoinNext : 1,
         bJoinPrev : 1,
         bMoveRange : 1;

    bool bMoveRedlines; // use DOC_MOVEREDLINES when calling SwDoc::Move

    void DelFtn( const SwPaM& );

public:
    SwUndoMove( const SwPaM&, const SwPosition& );
    SwUndoMove( SwDoc* pDoc, const SwNodeRange&, const SwNodeIndex& );

    virtual void UndoImpl( ::sw::UndoRedoContext & );
    virtual void RedoImpl( ::sw::UndoRedoContext & );

    /// set the destination range after the move
    void SetDestRange( const SwPaM&, const SwPosition&, sal_Bool, sal_Bool );
    void SetDestRange( const SwNodeIndex& rStt, const SwNodeIndex& rEnd,
                        const SwNodeIndex& rInsPos );

    sal_Bool IsMoveRange() const        { return bMoveRange; }
    sal_uLong GetEndNode() const        { return nEndNode; }
    sal_uLong GetDestSttNode() const    { return nDestSttNode; }
    xub_StrLen GetDestSttCntnt() const  { return nDestSttCntnt; }

    void SetMoveRedlines( bool b )       { bMoveRedlines = b; }

};

#endif // SW_UNDO_MOVE_HXX
