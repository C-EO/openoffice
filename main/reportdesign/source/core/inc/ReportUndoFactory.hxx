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


#ifndef REPORT_UNDOFACTORY_HXX_INCLUDED
#define REPORT_UNDOFACTORY_HXX_INCLUDED

#include <svx/svdundo.hxx>
#include <memory>
#include "ModuleHelper.hxx"

namespace rptui
{
    class OReportUndoFactory : public SdrUndoFactory
    {
        OModuleClient	    m_aModuleClient;
        ::std::auto_ptr<SdrUndoFactory> m_pUndoFactory;

        OReportUndoFactory(const OReportUndoFactory&);
        OReportUndoFactory& operator=(const OReportUndoFactory&);
    public:
        OReportUndoFactory();
        virtual ~OReportUndoFactory();

       	// shapes
	    virtual SdrUndoAction* CreateUndoMoveObject( SdrObject& rObject );
	    virtual SdrUndoAction* CreateUndoMoveObject( SdrObject& rObject, const Size& rDist );
	    virtual SdrUndoAction* CreateUndoGeoObject( SdrObject& rObject );
	    virtual SdrUndoAction* CreateUndoAttrObject( SdrObject& rObject, bool bStyleSheet1 = false, bool bSaveText = false );
	    virtual SdrUndoAction* CreateUndoRemoveObject( SdrObject& rObject, bool bOrdNumDirect = false);
	    virtual SdrUndoAction* CreateUndoInsertObject( SdrObject& rObject, bool bOrdNumDirect = false);
	    virtual SdrUndoAction* CreateUndoDeleteObject( SdrObject& rObject, bool bOrdNumDirect = false);
	    virtual SdrUndoAction* CreateUndoNewObject( SdrObject& rObject, bool bOrdNumDirect = false);
	    virtual SdrUndoAction* CreateUndoCopyObject( SdrObject& rObject, bool bOrdNumDirect = false);

	    virtual SdrUndoAction* CreateUndoObjectOrdNum( SdrObject& rObject, sal_uInt32 nOldOrdNum1, sal_uInt32 nNewOrdNum1);

	    virtual SdrUndoAction* CreateUndoReplaceObject( SdrObject& rOldObject, SdrObject& rNewObject, bool bOrdNumDirect = false );
	    virtual SdrUndoAction* CreateUndoObjectLayerChange( SdrObject& rObject, SdrLayerID aOldLayer, SdrLayerID aNewLayer );
	    virtual SdrUndoAction* CreateUndoObjectSetText( SdrObject& rNewObj, sal_Int32 nText );

	    // layer
	    virtual SdrUndoAction* CreateUndoNewLayer(sal_uInt16 nLayerNum, SdrLayerAdmin& rNewLayerAdmin, SdrModel& rNewModel);
	    virtual SdrUndoAction* CreateUndoDeleteLayer(sal_uInt16 nLayerNum, SdrLayerAdmin& rNewLayerAdmin, SdrModel& rNewModel);
	    virtual SdrUndoAction* CreateUndoMoveLayer(sal_uInt16 nLayerNum, SdrLayerAdmin& rNewLayerAdmin, SdrModel& rNewModel, sal_uInt16 nNeuPos1);

	    // page
	    virtual SdrUndoAction*	CreateUndoDeletePage(SdrPage& rPage);
	    virtual SdrUndoAction* CreateUndoNewPage(SdrPage& rPage);
	    virtual SdrUndoAction* CreateUndoCopyPage(SdrPage& rPage);
	    virtual SdrUndoAction* CreateUndoSetPageNum(SdrPage& rNewPg, sal_uInt16 nOldPageNum1, sal_uInt16 nNewPageNum1);

	    // master page
	    virtual SdrUndoAction* CreateUndoPageRemoveMasterPage(SdrPage& rChangedPage);
	    virtual SdrUndoAction* CreateUndoPageChangeMasterPage(SdrPage& rChangedPage);

    };
//==============================================================================
} // rptui
//==============================================================================

#endif // REPORT_UNDOFACTORY_HXX_INCLUDED
