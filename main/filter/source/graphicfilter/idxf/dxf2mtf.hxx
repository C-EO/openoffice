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



#ifndef _DXF2MTF_HXX
#define _DXF2MTF_HXX

#include "dxfreprd.hxx"
#include <vcl/font.hxx>
#include <vcl/lineinfo.hxx>

// MT: NOOLDSV, someone should change the code...
enum PenStyle { PEN_NULL, PEN_SOLID, PEN_DOT, PEN_DASH, PEN_DASHDOT };
enum BrushStyle { BRUSH_NULL, BRUSH_SOLID, BRUSH_HORZ, BRUSH_VERT,
				  BRUSH_CROSS, BRUSH_DIAGCROSS, BRUSH_UPDIAG, BRUSH_DOWNDIAG,
				  BRUSH_25, BRUSH_50, BRUSH_75,
				  BRUSH_BITMAP };


class DXF2GDIMetaFile {
private:

	VirtualDevice * pVirDev;
	const DXFRepresentation * pDXF;
	sal_Bool bStatus;

	sal_uInt16 OptPointsPerCircle;

	sal_uLong nMinPercent;
	sal_uLong nMaxPercent;
	sal_uLong nLastPercent;
	sal_uLong nMainEntitiesCount;

	long		nBlockColor;
	DXFLineInfo	aBlockDXFLineInfo;
	long		nParentLayerColor;
	DXFLineInfo	aParentLayerDXFLineInfo;
	Color		aActLineColor;
	Color		aActFillColor;
	Font		aActFont;

	sal_uLong CountEntities(const DXFEntities & rEntities);

	void MayCallback(sal_uLong nMainEntitiesProcessed);

	Color ConvertColor(sal_uInt8 nColor);

	long GetEntityColor(const DXFBasicEntity & rE);

	DXFLineInfo LTypeToDXFLineInfo(const char * sLineType);

	DXFLineInfo GetEntityDXFLineInfo(const DXFBasicEntity & rE);

	sal_Bool SetLineAttribute(const DXFBasicEntity & rE, sal_uLong nWidth=0);

	sal_Bool SetAreaAttribute(const DXFBasicEntity & rE);

	sal_Bool SetFontAttribute(const DXFBasicEntity & rE, short nAngle,
						  sal_uInt16 nHeight, double fWidthScale);

	void DrawLineEntity(const DXFLineEntity & rE, const DXFTransform & rTransform);

	void DrawPointEntity(const DXFPointEntity & rE, const DXFTransform & rTransform);

	void DrawCircleEntity(const DXFCircleEntity & rE, const DXFTransform & rTransform);

	void DrawArcEntity(const DXFArcEntity & rE, const DXFTransform & rTransform);

	void DrawTraceEntity(const DXFTraceEntity & rE, const DXFTransform & rTransform);

	void DrawSolidEntity(const DXFSolidEntity & rE, const DXFTransform & rTransform);

	void DrawTextEntity(const DXFTextEntity & rE, const DXFTransform & rTransform);

	void DrawInsertEntity(const DXFInsertEntity & rE, const DXFTransform & rTransform);

	void DrawAttribEntity(const DXFAttribEntity & rE, const DXFTransform & rTransform);

	void DrawPolyLineEntity(const DXFPolyLineEntity & rE, const DXFTransform & rTransform);

	void Draw3DFaceEntity(const DXF3DFaceEntity & rE, const DXFTransform & rTransform);

	void DrawDimensionEntity(const DXFDimensionEntity & rE, const DXFTransform & rTransform);

	void DrawLWPolyLineEntity( const DXFLWPolyLineEntity & rE, const DXFTransform & rTransform );

	void DrawHatchEntity( const DXFHatchEntity & rE, const DXFTransform & rTransform );

	void DrawEntities(const DXFEntities & rEntities,
					  const DXFTransform & rTransform,
					  sal_Bool bTopEntities);

public:

	DXF2GDIMetaFile();
	~DXF2GDIMetaFile();

	sal_Bool Convert( const DXFRepresentation & rDXF, GDIMetaFile & rMTF, sal_uInt16 nMinPercent, sal_uInt16 nMaxPercent);

};


#endif
