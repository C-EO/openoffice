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

// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_xmloff.hxx"

#include <tools/debug.hxx>
#include <com/sun/star/document/XEventsSupplier.hpp>
#include <com/sun/star/container/XNameReplace.hpp>
#include <com/sun/star/presentation/ClickAction.hpp>
#include <com/sun/star/drawing/FillStyle.hpp>
#include <com/sun/star/drawing/LineStyle.hpp>
#include "unointerfacetouniqueidentifiermapper.hxx"
#include <com/sun/star/drawing/XGluePointsSupplier.hpp>
#include <com/sun/star/container/XIdentifierAccess.hpp>
#include <com/sun/star/drawing/GluePoint2.hpp>
#include <com/sun/star/drawing/Alignment.hpp>
#include <com/sun/star/drawing/EscapeDirection.hpp>
#include <com/sun/star/media/ZoomLevel.hpp>
#include <com/sun/star/awt/Rectangle.hpp>
#include <com/sun/star/style/XStyleFamiliesSupplier.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <comphelper/extract.hxx>
#include "ximpshap.hxx"
#include <xmloff/XMLBase64ImportContext.hxx>
#include <xmloff/XMLShapeStyleContext.hxx>
#include <xmloff/xmluconv.hxx>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/drawing/CircleKind.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/awt/XControlModel.hpp>
#include <com/sun/star/drawing/XControlShape.hpp>
#include <com/sun/star/drawing/PointSequenceSequence.hpp>
#include <com/sun/star/drawing/PointSequence.hpp>
#include <com/sun/star/style/XStyleFamiliesSupplier.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/util/XCloneable.hpp>
#include <com/sun/star/beans/XMultiPropertyStates.hpp>
#include "xexptran.hxx"
#include <com/sun/star/drawing/PolyPolygonBezierCoords.hpp>
#include <com/sun/star/beans/XPropertySetInfo.hpp>
#include <com/sun/star/drawing/ConnectorType.hpp>
#include <com/sun/star/drawing/HomogenMatrix3.hpp>
#include "PropertySetMerger.hxx"
#include <xmloff/families.hxx>
#include "ximpstyl.hxx"
#include"xmloff/xmlnmspe.hxx"
#include <xmloff/xmltoken.hxx>
#include "EnhancedCustomShapeToken.hxx"
#include "XMLReplacementImageContext.hxx"
#include "XMLImageMapContext.hxx"
#include "sdpropls.hxx"
#include "eventimp.hxx"
#include "descriptionimp.hxx"
#include "ximpcustomshape.hxx"
#include "XMLEmbeddedObjectImportContext.hxx"
#include "xmloff/xmlerror.hxx"
#include <basegfx/matrix/b2dhommatrix.hxx>
#include <tools/string.hxx>
#include <com/sun/star/drawing/XEnhancedCustomShapeDefaulter.hpp>
#include <com/sun/star/container/XChild.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <basegfx/vector/b2dvector.hxx>
#include <basegfx/point/b2dpoint.hxx>
#include <basegfx/polygon/b2dpolygon.hxx>
#include <basegfx/polygon/b2dpolygontools.hxx>
#include <basegfx/polygon/b2dpolypolygontools.hxx>
#include <basegfx/matrix/b2dhommatrixtools.hxx>

using ::rtl::OUString;
using ::rtl::OUStringBuffer;

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::drawing;
using namespace ::com::sun::star::style;
using namespace ::com::sun::star::container;
using namespace ::com::sun::star::document;
using namespace ::xmloff::token;
using namespace ::xmloff::EnhancedCustomShapeToken;

SvXMLEnumMapEntry aXML_GlueAlignment_EnumMap[] =
{
	{ XML_TOP_LEFT, 	drawing::Alignment_TOP_LEFT },
	{ XML_TOP,			drawing::Alignment_TOP },
	{ XML_TOP_RIGHT,	drawing::Alignment_TOP_RIGHT },
	{ XML_LEFT,		    drawing::Alignment_LEFT },
	{ XML_CENTER,		drawing::Alignment_CENTER },
	{ XML_RIGHT,		drawing::Alignment_RIGHT },
	{ XML_BOTTOM_LEFT,	drawing::Alignment_BOTTOM_LEFT },
	{ XML_BOTTOM,		drawing::Alignment_BOTTOM },
	{ XML_BOTTOM_RIGHT, drawing::Alignment_BOTTOM_RIGHT },
	{ XML_TOKEN_INVALID, 0 }
};

SvXMLEnumMapEntry aXML_GlueEscapeDirection_EnumMap[] =
{
	{ XML_AUTO, 		drawing::EscapeDirection_SMART },
	{ XML_LEFT,	    	drawing::EscapeDirection_LEFT },
    { XML_RIGHT,		drawing::EscapeDirection_RIGHT },
    { XML_UP,			drawing::EscapeDirection_UP },
    { XML_DOWN,		    drawing::EscapeDirection_DOWN },
    { XML_HORIZONTAL,	drawing::EscapeDirection_HORIZONTAL },
    { XML_VERTICAL,	    drawing::EscapeDirection_VERTICAL },
	{ XML_TOKEN_INVALID, 0 }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool ImpIsEmptyURL( const ::rtl::OUString& rURL )
{
    if( rURL.getLength() == 0 )
        return true;

    // #i13140# Also compare against 'toplevel' URLs. which also
    // result in empty filename strings.
    if( 0 == rURL.compareToAscii( "#./" ) )
        return true;

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SvXMLShapeContext, SvXMLImportContext );
TYPEINIT1( SdXMLShapeContext, SvXMLShapeContext );

SdXMLShapeContext::SdXMLShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SvXMLShapeContext( rImport, nPrfx, rLocalName, bTemporaryShape )
,	mxShapes( rShapes )
,	mxAttrList(xAttrList)
,	mbListContextPushed( false )
,	mnStyleFamily(XML_STYLE_FAMILY_SD_GRAPHICS_ID)
,	mbIsPlaceholder(sal_False)
,	mbClearDefaultAttributes( true )
,	mbIsUserTransformed(sal_False)
,	mnZOrder(-1)
,	maSize(1, 1)
,	maPosition(0, 0)
,   maUsedTransformation()
,	mbVisible(true)
,	mbPrintable(true)
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLShapeContext::~SdXMLShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

SvXMLImportContext *SdXMLShapeContext::CreateChildContext( sal_uInt16 p_nPrefix,
	const OUString& rLocalName,
	const uno::Reference< xml::sax::XAttributeList>& xAttrList )
{
	SvXMLImportContext * pContext = NULL;

	// #i68101#
	if( p_nPrefix == XML_NAMESPACE_SVG &&	
		(IsXMLToken( rLocalName, XML_TITLE ) || IsXMLToken( rLocalName, XML_DESC ) ) )
	{
		pContext = new SdXMLDescriptionContext( GetImport(), p_nPrefix, rLocalName, xAttrList, mxShape );
	}
	else if( p_nPrefix == XML_NAMESPACE_OFFICE && IsXMLToken( rLocalName, XML_EVENT_LISTENERS ) )
	{
		pContext = new SdXMLEventsContext( GetImport(), p_nPrefix, rLocalName, xAttrList, mxShape );
	}
	else if( p_nPrefix == XML_NAMESPACE_DRAW && IsXMLToken( rLocalName, XML_GLUE_POINT ) )
	{
		addGluePoint( xAttrList );
	}
	else if( p_nPrefix == XML_NAMESPACE_DRAW && IsXMLToken( rLocalName, XML_THUMBNAIL ) )
	{
		// search attributes for xlink:href
		sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
		for(sal_Int16 i=0; i < nAttrCount; i++)
		{
			OUString sAttrName = xAttrList->getNameByIndex( i );
			OUString aLocalName;
			sal_uInt16 nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName( sAttrName, &aLocalName );

			if( nPrefix == XML_NAMESPACE_XLINK )
			{
				if( IsXMLToken( aLocalName, XML_HREF ) )
				{
					maThumbnailURL = xAttrList->getValueByIndex( i );
					break;
				}
			}
		}
	}
	else
	{
		// create text cursor on demand
		if( !mxCursor.is() )
		{
			uno::Reference< text::XText > xText( mxShape, uno::UNO_QUERY );
			if( xText.is() )
			{
				UniReference < XMLTextImportHelper > xTxtImport =
					GetImport().GetTextImport();
				mxOldCursor = xTxtImport->GetCursor();
				mxCursor = xText->createTextCursor();
				if( mxCursor.is() )
				{
					xTxtImport->SetCursor( mxCursor );
				}

				// remember old list item and block (#91964#) and reset them
				// for the text frame
                xTxtImport->PushListContext();
                mbListContextPushed = true;
			}
		}

		// if we have a text cursor, lets  try to import some text
		if( mxCursor.is() )
		{
			pContext = GetImport().GetTextImport()->CreateTextChildContext(
				GetImport(), p_nPrefix, rLocalName, xAttrList );
		}
	}

	// call parent for content
	if(!pContext)
		pContext = SvXMLImportContext::CreateChildContext( p_nPrefix, rLocalName, xAttrList );

	return pContext;
}

void SdXMLShapeContext::addGluePoint( const uno::Reference< xml::sax::XAttributeList>& xAttrList )
{
	// get the glue points container for this shape if its not already there
	if( !mxGluePoints.is() )
	{
		uno::Reference< drawing::XGluePointsSupplier > xSupplier( mxShape, uno::UNO_QUERY );
		if( !xSupplier.is() )
			return;

		mxGluePoints = uno::Reference< container::XIdentifierContainer >::query( xSupplier->getGluePoints() );

		if( !mxGluePoints.is() )
			return;
	}

	drawing::GluePoint2 aGluePoint;
	aGluePoint.IsUserDefined = sal_True;
	aGluePoint.Position.X = 0;
	aGluePoint.Position.Y = 0;
	aGluePoint.Escape = drawing::EscapeDirection_SMART;
	aGluePoint.PositionAlignment = drawing::Alignment_CENTER;
	aGluePoint.IsRelative = sal_True;

	sal_Int32 nId = -1;

	// read attributes for the 3DScene
	sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
	for(sal_Int16 i=0; i < nAttrCount; i++)
	{
		OUString sAttrName = xAttrList->getNameByIndex( i );
		OUString aLocalName;
		sal_uInt16 nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName( sAttrName, &aLocalName );
		const OUString sValue( xAttrList->getValueByIndex( i ) );

		if( nPrefix == XML_NAMESPACE_SVG )
		{
			if( IsXMLToken( aLocalName, XML_X ) )
			{
				GetImport().GetMM100UnitConverter().convertMeasure(aGluePoint.Position.X, sValue);
			}
			else if( IsXMLToken( aLocalName, XML_Y ) )
			{
				GetImport().GetMM100UnitConverter().convertMeasure(aGluePoint.Position.Y, sValue);
			}
		}
		else if( nPrefix == XML_NAMESPACE_DRAW )
		{
			if( IsXMLToken( aLocalName, XML_ID ) )
			{
				nId = sValue.toInt32();
			}
			else if( IsXMLToken( aLocalName, XML_ALIGN ) )
			{
				sal_uInt16 eKind;
				if( SvXMLUnitConverter::convertEnum( eKind, sValue, aXML_GlueAlignment_EnumMap ) )
				{
					aGluePoint.PositionAlignment = (drawing::Alignment)eKind;
					aGluePoint.IsRelative = sal_False;
				}
			}
			else if( IsXMLToken( aLocalName, XML_ESCAPE_DIRECTION ) )
			{
				sal_uInt16 eKind;
				if( SvXMLUnitConverter::convertEnum( eKind, sValue, aXML_GlueEscapeDirection_EnumMap ) )
				{
					aGluePoint.Escape = (drawing::EscapeDirection)eKind;
				}
			}
		}
	}

	if( nId != -1 )
	{
		try
		{
			sal_Int32 nInternalId = mxGluePoints->insert( uno::makeAny( aGluePoint ) );
			GetImport().GetShapeImport()->addGluePointMapping( mxShape, nId, nInternalId );
		}
		catch( uno::Exception& )
		{
			DBG_ERROR( "exception during setting of glue points!");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////

void SdXMLShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>&)
{
    GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );
}

void SdXMLShapeContext::EndElement()
{
	if(mxCursor.is())
	{
		// delete addition newline
		const OUString aEmpty;
		mxCursor->gotoEnd( sal_False );
		mxCursor->goLeft( 1, sal_True );
		mxCursor->setString( aEmpty );

		// reset cursor
		GetImport().GetTextImport()->ResetCursor();
	}

	if(mxOldCursor.is())
		GetImport().GetTextImport()->SetCursor( mxOldCursor );

    // reinstall old list item (if necessary) #91964#
    if (mbListContextPushed) {
        GetImport().GetTextImport()->PopListContext();
    }

	if( msHyperlink.getLength() != 0 ) try
	{
		const OUString sBookmark( RTL_CONSTASCII_USTRINGPARAM( "Bookmark" ) );

        Reference< XEventsSupplier > xEventsSupplier( mxShape, UNO_QUERY );
		if( xEventsSupplier.is() )
		{
			const OUString sEventType( RTL_CONSTASCII_USTRINGPARAM( "EventType" ) );
			const OUString sClickAction( RTL_CONSTASCII_USTRINGPARAM( "ClickAction" ) );

			Reference< XNameReplace > xEvents( xEventsSupplier->getEvents(), UNO_QUERY_THROW );

			uno::Sequence< beans::PropertyValue > aProperties( 3 );
			aProperties[0].Name = sEventType;
			aProperties[0].Handle = -1;
			aProperties[0].Value <<= OUString( RTL_CONSTASCII_USTRINGPARAM("Presentation") );
			aProperties[0].State = beans::PropertyState_DIRECT_VALUE;

			aProperties[1].Name = sClickAction;
			aProperties[1].Handle = -1;
			aProperties[1].Value <<= ::com::sun::star::presentation::ClickAction_DOCUMENT;
			aProperties[1].State = beans::PropertyState_DIRECT_VALUE;

			aProperties[2].Name = sBookmark;
			aProperties[2].Handle = -1;
			aProperties[2].Value <<= msHyperlink;
			aProperties[2].State = beans::PropertyState_DIRECT_VALUE;

			const OUString sAPIEventName( RTL_CONSTASCII_USTRINGPARAM( "OnClick" ) );
			xEvents->replaceByName( sAPIEventName, Any( aProperties ) );
		}
		else
		{
			// in draw use the Bookmark property
			Reference< beans::XPropertySet > xSet( mxShape, UNO_QUERY_THROW );
			xSet->setPropertyValue( sBookmark, Any( msHyperlink ) );
			xSet->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "OnClick" ) ), Any( ::com::sun::star::presentation::ClickAction_DOCUMENT ) );
		}
	}
	catch( Exception& )
	{
		DBG_ERROR("xmloff::SdXMLShapeContext::EndElement(), exception caught while setting hyperlink!");
	}

	if( mxLockable.is() )
		mxLockable->removeActionLock();
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLShapeContext::AddShape(uno::Reference< drawing::XShape >& xShape)
{
	if(xShape.is())
	{
		// set shape local
		mxShape = xShape;

		if(maShapeName.getLength())
		{
			uno::Reference< container::XNamed > xNamed( mxShape, uno::UNO_QUERY );
			if( xNamed.is() )
				xNamed->setName( maShapeName );
		}

		UniReference< XMLShapeImportHelper > xImp( GetImport().GetShapeImport() );
		xImp->addShape( xShape, mxAttrList, mxShapes );

		if( mbClearDefaultAttributes )
		{
	        uno::Reference<beans::XMultiPropertyStates> xMultiPropertyStates(xShape, uno::UNO_QUERY );
		    if (xMultiPropertyStates.is())
			    xMultiPropertyStates->setAllPropertiesToDefault();
		}

		if( !mbVisible || !mbPrintable ) try
		{
			uno::Reference< beans::XPropertySet > xSet( xShape, uno::UNO_QUERY_THROW );
			if( !mbVisible )
				xSet->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Visible" ) ), uno::Any( sal_False ) );

			if( !mbPrintable )
				xSet->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Printable" ) ), uno::Any( sal_False ) );
		}
		catch( Exception& )
		{
			DBG_ERROR( "SdXMLShapeContext::AddShape(), exception caught!" );
		}

		// #107848#
		if(!mbTemporaryShape && (!GetImport().HasTextImport()
			|| !GetImport().GetTextImport()->IsInsideDeleteContext()))
		{
			xImp->shapeWithZIndexAdded( xShape, mnZOrder );
		}

		if( maShapeId.getLength() )
		{
            const SdXMLGraphicObjectShapeContext* pGSC = dynamic_cast< const SdXMLGraphicObjectShapeContext* >(this);

            /* avoid registering when LateRegister is needed. E.g. MultiImage support where in-between multiple
               xShapes with the same ID would be registered. Registration is done after deciding which image
               to keep, see calls to solveMultipleImages */
            if(!pGSC || !pGSC->getLateAddToIdentifierMapper())
            {
                uno::Reference< uno::XInterface > xRef( xShape, uno::UNO_QUERY );
                GetImport().getInterfaceToIdentifierMapper().registerReference( maShapeId, xRef );
            }
		}

		// #91065# count only if counting for shape import is enabled
		if(GetImport().GetShapeImport()->IsHandleProgressBarEnabled())
		{
			// #80365# increment progress bar at load once for each draw object
			GetImport().GetProgressBarHelper()->Increment();
		}
	}

	mxLockable = uno::Reference< document::XActionLockable >::query( xShape );

	if( mxLockable.is() )
		mxLockable->addActionLock();

}

//////////////////////////////////////////////////////////////////////////////

void SdXMLShapeContext::AddShape(const char* pServiceName )
{
	uno::Reference< lang::XMultiServiceFactory > xServiceFact(GetImport().GetModel(), uno::UNO_QUERY);
	if(xServiceFact.is())
	{
        try
        {
            // --> OD 2006-02-22 #b6382898#
            // Since fix for issue i33294 the Writer model doesn't support
            // com.sun.star.drawing.OLE2Shape anymore.
            // To handle Draw OLE objects it's decided to import these
            // objects as com.sun.star.drawing.OLE2Shape and convert these
            // objects after the import into com.sun.star.drawing.GraphicObjectShape.
            uno::Reference< drawing::XShape > xShape;
            if ( OUString::createFromAscii(pServiceName).compareToAscii( "com.sun.star.drawing.OLE2Shape" ) == 0 &&
                 uno::Reference< text::XTextDocument >(GetImport().GetModel(), uno::UNO_QUERY).is() )
            {
                xShape = uno::Reference< drawing::XShape >(xServiceFact->createInstance(OUString::createFromAscii("com.sun.star.drawing.temporaryForXMLImportOLE2Shape")), uno::UNO_QUERY);
            }
            else
            {
                xShape = uno::Reference< drawing::XShape >(xServiceFact->createInstance(OUString::createFromAscii(pServiceName)), uno::UNO_QUERY);
            }
            // <--
            if( xShape.is() )
                AddShape( xShape );
        }
        catch( const uno::Exception& e )
        {
            uno::Sequence<rtl::OUString> aSeq( 1 );
            aSeq[0] = OUString::createFromAscii(pServiceName);
            GetImport().SetError( XMLERROR_FLAG_ERROR | XMLERROR_API,
                                  aSeq, e.Message, NULL );
        }
	}
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLShapeContext::SetTransformation()
{
	if(mxShape.is())
	{
		uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
		if(xPropSet.is())
		{
			maUsedTransformation.identity();

			if(maSize.Width != 1 || maSize.Height != 1)
			{
				// take care there are no zeros used by error
				if(0 == maSize.Width)
					maSize.Width = 1;
				if(0 == maSize.Height)
					maSize.Height = 1;

				// set global size. This should always be used.
				maUsedTransformation.scale(maSize.Width, maSize.Height);
			}

			if(maPosition.X != 0 || maPosition.Y != 0)
			{
				// if global position is used, add it to transformation
				maUsedTransformation.translate(maPosition.X, maPosition.Y);
			}

			if(mnTransform.NeedsAction())
			{
				// transformation is used, apply to object.
				// NOTICE: The transformation is applied AFTER evtl. used
				// global positioning and scaling is used, so any shear or
				// rotate used herein is applied around the (0,0) position
				// of the PAGE object !!!
				::basegfx::B2DHomMatrix aMat;
				mnTransform.GetFullTransform(aMat);

				// now add to transformation
				maUsedTransformation *= aMat;
			}

			// now set transformation for this object
			uno::Any aAny;
			drawing::HomogenMatrix3 aMatrix;

			aMatrix.Line1.Column1 = maUsedTransformation.get(0, 0);
			aMatrix.Line1.Column2 = maUsedTransformation.get(0, 1);
			aMatrix.Line1.Column3 = maUsedTransformation.get(0, 2);

			aMatrix.Line2.Column1 = maUsedTransformation.get(1, 0);
			aMatrix.Line2.Column2 = maUsedTransformation.get(1, 1);
			aMatrix.Line2.Column3 = maUsedTransformation.get(1, 2);

			aMatrix.Line3.Column1 = maUsedTransformation.get(2, 0);
			aMatrix.Line3.Column2 = maUsedTransformation.get(2, 1);
			aMatrix.Line3.Column3 = maUsedTransformation.get(2, 2);

			aAny <<= aMatrix;

			xPropSet->setPropertyValue(
				OUString(RTL_CONSTASCII_USTRINGPARAM("Transformation")), aAny);
        }
	}
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLShapeContext::SetStyle( bool bSupportsStyle /* = true */)
{
	try
	{
		uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
		if( !xPropSet.is() )
			return;

		do
		{
			XMLPropStyleContext* pDocStyle = NULL;

			// set style on shape
			if(maDrawStyleName.getLength() == 0)
				break;

			const SvXMLStyleContext* pStyle = 0L;
			sal_Bool bAutoStyle(sal_False);

			if(GetImport().GetShapeImport()->GetAutoStylesContext())
				pStyle = GetImport().GetShapeImport()->GetAutoStylesContext()->FindStyleChildContext(mnStyleFamily, maDrawStyleName);

			if(pStyle)
				bAutoStyle = sal_True;

			if(!pStyle && GetImport().GetShapeImport()->GetStylesContext())
				pStyle = GetImport().GetShapeImport()->GetStylesContext()->FindStyleChildContext(mnStyleFamily, maDrawStyleName);

			OUString aStyleName = maDrawStyleName;
			uno::Reference< style::XStyle > xStyle;

			if( pStyle && pStyle->ISA(XMLShapeStyleContext) )
			{
				pDocStyle = PTR_CAST( XMLShapeStyleContext, pStyle );

				if( pDocStyle->GetStyle().is() )
				{
					xStyle = pDocStyle->GetStyle();
				}
				else
				{
                    aStyleName = pDocStyle->GetParentName();
				}
			}

			if( !xStyle.is() && aStyleName.getLength() )
			{
				try
				{

					uno::Reference< style::XStyleFamiliesSupplier > xFamiliesSupplier( GetImport().GetModel(), uno::UNO_QUERY );

					if( xFamiliesSupplier.is() )
					{
						uno::Reference< container::XNameAccess > xFamilies( xFamiliesSupplier->getStyleFamilies() );
						if( xFamilies.is() )
						{

							uno::Reference< container::XNameAccess > xFamily;

							if( XML_STYLE_FAMILY_SD_PRESENTATION_ID == mnStyleFamily )
							{
								aStyleName = GetImport().GetStyleDisplayName(
									XML_STYLE_FAMILY_SD_PRESENTATION_ID,
									aStyleName );
								sal_Int32 nPos = aStyleName.lastIndexOf( sal_Unicode('-') );
								if( -1 != nPos )
								{
									OUString aFamily( aStyleName.copy( 0, nPos ) );

									xFamilies->getByName( aFamily ) >>= xFamily;
									aStyleName = aStyleName.copy( nPos + 1 );
								}
							}
							else
							{
								// get graphics familie
								xFamilies->getByName( OUString( RTL_CONSTASCII_USTRINGPARAM( "graphics" ) ) ) >>= xFamily;
								aStyleName = GetImport().GetStyleDisplayName(
									XML_STYLE_FAMILY_SD_GRAPHICS_ID,
									aStyleName );
							}

							if( xFamily.is() )
								xFamily->getByName( aStyleName ) >>= xStyle;
						}
					}
				}
				catch( uno::Exception& )
				{
					DBG_ERROR( "could not find style for shape!" );
				}
			}

			if( bSupportsStyle && xStyle.is() )
			{
				try
				{
					// set style on object
					uno::Any aAny;
					aAny <<= xStyle;
					xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("Style")), aAny);
				}
				catch( uno::Exception& )
				{
					DBG_ERROR( "could not find style for shape!" );
				}
			}

			// if this is an auto style, set its properties
			if(bAutoStyle && pDocStyle)
			{
				// set PropertySet on object
				pDocStyle->FillPropertySet(xPropSet);
			}

		} while(0);

		// try to set text auto style
		do
		{
			// set style on shape
			if( 0 == maTextStyleName.getLength() )
				break;

			if( NULL == GetImport().GetShapeImport()->GetAutoStylesContext())
				break;

            const SvXMLStyleContext* pTempStyle = GetImport().GetShapeImport()->GetAutoStylesContext()->FindStyleChildContext(XML_STYLE_FAMILY_TEXT_PARAGRAPH, maTextStyleName);
			XMLPropStyleContext* pStyle = PTR_CAST( XMLPropStyleContext, pTempStyle ); // use temp var, PTR_CAST is a bad macro, FindStyleChildContext will be called twice
			if( pStyle == NULL )
				break;

			// set PropertySet on object
			pStyle->FillPropertySet(xPropSet);

		} while(0);
	}
	catch( uno::Exception& )
	{
	}
}

void SdXMLShapeContext::SetLayer()
{
	if( maLayerName.getLength() )
	{
		try
		{
			uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
			if(xPropSet.is() )
			{
				uno::Any aAny;
				aAny <<= maLayerName;

				xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("LayerName")), aAny);
				return;
			}
		}
		catch( uno::Exception e )
		{
		}
	}
}

void SdXMLShapeContext::SetThumbnail()
{
	if( 0 == maThumbnailURL.getLength() )
		return;

	try
	{
		uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
		if( !xPropSet.is() )
			return;

		const OUString sProperty(RTL_CONSTASCII_USTRINGPARAM("ThumbnailGraphicURL"));

		uno::Reference< beans::XPropertySetInfo > xPropSetInfo( xPropSet->getPropertySetInfo() );
		if( xPropSetInfo.is() && xPropSetInfo->hasPropertyByName( sProperty ) )
		{
			// load the thumbnail graphic and export it to a wmf stream so we can set
			// it at the api

			const OUString aInternalURL( GetImport().ResolveGraphicObjectURL( maThumbnailURL, sal_False ) );
			xPropSet->setPropertyValue( sProperty, uno::makeAny( aInternalURL ) );
		}
	}
	catch( uno::Exception e )
	{
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
    bool bHaveXmlId( false );
	if( (XML_NAMESPACE_DRAW == nPrefix) || (XML_NAMESPACE_DRAW_EXT == nPrefix) )
	{
		if( IsXMLToken( rLocalName, XML_ZINDEX ) )
		{
			mnZOrder = rValue.toInt32();
		}
		else if( IsXMLToken( rLocalName, XML_ID ) )
        {
            if (!bHaveXmlId) { maShapeId = rValue; };
        }
		else if( IsXMLToken( rLocalName, XML_NAME ) )
		{
			maShapeName = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_STYLE_NAME ) )
		{
			maDrawStyleName = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_TEXT_STYLE_NAME ) )
		{
			maTextStyleName = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_LAYER ) )
		{
			maLayerName = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_TRANSFORM ) )
		{
			mnTransform.SetString(rValue, GetImport().GetMM100UnitConverter());
		}
		else if( IsXMLToken( rLocalName, XML_DISPLAY ) )
		{
			mbVisible = IsXMLToken( rValue, XML_ALWAYS ) || IsXMLToken( rValue, XML_SCREEN );
			mbPrintable = IsXMLToken( rValue, XML_ALWAYS ) || IsXMLToken( rValue, XML_PRINTER );
		}
	}
	else if( XML_NAMESPACE_PRESENTATION == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_USER_TRANSFORMED ) )
		{
			mbIsUserTransformed = IsXMLToken( rValue, XML_TRUE );
		}
		else if( IsXMLToken( rLocalName, XML_PLACEHOLDER ) )
		{
			mbIsPlaceholder = IsXMLToken( rValue, XML_TRUE );
			if( mbIsPlaceholder )
				mbClearDefaultAttributes = false;
		}
		else if( IsXMLToken( rLocalName, XML_CLASS ) )
		{
			maPresentationClass = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_STYLE_NAME ) )
		{
			maDrawStyleName = rValue;
			mnStyleFamily = XML_STYLE_FAMILY_SD_PRESENTATION_ID;
		}
	}
	else if( XML_NAMESPACE_SVG == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_X ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maPosition.X, rValue);
		}
		else if( IsXMLToken( rLocalName, XML_Y ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maPosition.Y, rValue);
		}
		else if( IsXMLToken( rLocalName, XML_WIDTH ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maSize.Width, rValue);
			if( maSize.Width > 0 )
				maSize.Width += 1;
			else if( maSize.Width < 0 )
				maSize.Width -= 1;
		}
		else if( IsXMLToken( rLocalName, XML_HEIGHT ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maSize.Height, rValue);
			if( maSize.Height > 0 )
				maSize.Height += 1;
			else if( maSize.Height < 0 )
				maSize.Height -= 1;
		}
		else if( IsXMLToken( rLocalName, XML_TRANSFORM ) )
		{
			// because of #85127# take svg:transform into account and hanle like
			// draw:transform for compatibility
			mnTransform.SetString(rValue, GetImport().GetMM100UnitConverter());
		}

		// #i68101#
		else if( IsXMLToken( rLocalName, XML_TITLE ) )
		{
			maShapeTitle = rValue;
		}
		else if( IsXMLToken( rLocalName, XML_DESC ) )
		{
			maShapeDescription = rValue;
		}
	}
	else if( (XML_NAMESPACE_NONE == nPrefix) || (XML_NAMESPACE_XML == nPrefix) )
	{
		if( IsXMLToken( rLocalName, XML_ID ) )
		{
			maShapeId = rValue;
            bHaveXmlId = true;
        }
    }
}

sal_Bool SdXMLShapeContext::isPresentationShape() const
{
	if( maPresentationClass.getLength() && (const_cast<SdXMLShapeContext*>(this))->GetImport().GetShapeImport()->IsPresentationShapesSupported() )
	{
		if(XML_STYLE_FAMILY_SD_PRESENTATION_ID == mnStyleFamily)
		{
			return sal_True;
		}

		if( IsXMLToken( maPresentationClass, XML_HEADER ) || IsXMLToken( maPresentationClass, XML_FOOTER ) ||
			IsXMLToken( maPresentationClass, XML_PAGE_NUMBER ) || IsXMLToken( maPresentationClass, XML_DATE_TIME ) )
		{
			return sal_True;
		}
	}

	return sal_False;
}

void SdXMLShapeContext::onDemandRescueUsefulDataFromTemporary( const SvXMLImportContext& rCandidate )
{
    const SdXMLShapeContext* pCandidate = dynamic_cast< const SdXMLShapeContext* >(&rCandidate);

    if(!mxGluePoints.is() && pCandidate)
    {
        // try to rescue GluePoints from rCandidate to local if we not yet have GluePoints by copying them
        uno::Reference< drawing::XGluePointsSupplier > xSourceSupplier( pCandidate->getShape(), uno::UNO_QUERY );
        if( !xSourceSupplier.is() )
            return;

        uno::Reference< container::XIdentifierAccess > xSourceGluePoints( xSourceSupplier->getGluePoints(), uno::UNO_QUERY );
        if( !xSourceGluePoints.is() )
            return;

        uno::Sequence< sal_Int32 > aSourceIdSequence( xSourceGluePoints->getIdentifiers() );
        const sal_Int32 nSourceCount(aSourceIdSequence.getLength());
        UniReference< XMLShapeImportHelper > xSourceShapeImportHelper(const_cast< SdXMLShapeContext* >(pCandidate)->GetImport().GetShapeImport());

        if(nSourceCount)
        {
            // rCandidate has GluePoints; prepare the GluePoint container for the local shape
            uno::Reference< drawing::XGluePointsSupplier > xSupplier( mxShape, uno::UNO_QUERY );
            if( !xSupplier.is() )
                return;

            mxGluePoints = uno::Reference< container::XIdentifierContainer >::query( xSupplier->getGluePoints() );

            if( !mxGluePoints.is() )
                return;

            drawing::GluePoint2 aSourceGluePoint;

            for( sal_Int32 nSourceIndex(0); nSourceIndex < nSourceCount; nSourceIndex++ )
            {
                const sal_Int32 nSourceIdentifier = aSourceIdSequence[nSourceIndex];

                // loop over GluePoints which are UserDefined (avoid the auto mapped ones)
                if((xSourceGluePoints->getByIdentifier( nSourceIdentifier ) >>= aSourceGluePoint) 
                    && aSourceGluePoint.IsUserDefined)
                {
                    // get original mappingID back, this is the draw:id imported with a draw:glue-point
                    const sal_Int32 nDestinnationId = xSourceShapeImportHelper->findGluePointMapping( 
                        pCandidate->getShape(), 
                        nSourceIdentifier );

                    if(-1 != nSourceIdentifier)
                    {
                        // if we got that we are able to add a copy of that GluePoint to the local
                        // context and xShape since we have all information that the source shape
                        // and context had at import time
                        try
                        {
                            const sal_Int32 nInternalId = mxGluePoints->insert( uno::makeAny( aSourceGluePoint ) );
                            GetImport().GetShapeImport()->addGluePointMapping( mxShape, nDestinnationId, nInternalId );
                        }
                        catch( uno::Exception& )
                        {
                            DBG_ERROR( "exception during setting of glue points!");
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLRectShapeContext, SdXMLShapeContext );

SdXMLRectShapeContext::SdXMLRectShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mnRadius( 0L )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLRectShapeContext::~SdXMLRectShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLRectShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_CORNER_RADIUS ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnRadius, rValue);
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLRectShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create rectangle shape
	AddShape("com.sun.star.drawing.RectangleShape");
	if(mxShape.is())
	{
		// Add, set Style and properties from base shape
		SetStyle();
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();

		if(mnRadius)
		{
			uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
			if(xPropSet.is())
			{
				try
				{
					xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("CornerRadius")), uno::makeAny( mnRadius ) );
				}
				catch( uno::Exception& )
				{
					DBG_ERROR( "exception during setting of corner radius!");
				}
			}
		}
		SdXMLShapeContext::StartElement(xAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////3////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLLineShapeContext, SdXMLShapeContext );

SdXMLLineShapeContext::SdXMLLineShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mnX1( 0L ),
	mnY1( 0L ),
	mnX2( 1L ),
	mnY2( 1L )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLLineShapeContext::~SdXMLLineShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLLineShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_SVG == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_X1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnX1, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnY1, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_X2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnX2, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnY2, rValue);
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLLineShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// #85920# use SetTransformation() to handle import of simple lines.
	// This is necessary to kake into account all anchor positions and
	// other things. All shape imports use the same import schemata now.
	// create necessary shape (Line Shape)
	AddShape("com.sun.star.drawing.PolyLineShape");

	if(mxShape.is())
	{
		// Add, set Style and properties from base shape
		SetStyle();
		SetLayer();

		// get sizes and offsets
		awt::Point aTopLeft(mnX1, mnY1);
		awt::Point aBottomRight(mnX2, mnY2);

		if(mnX1 > mnX2)
		{
			aTopLeft.X = mnX2;
			aBottomRight.X = mnX1;
		}

		if(mnY1 > mnY2)
		{
			aTopLeft.Y = mnY2;
			aBottomRight.Y = mnY1;
		}

		// set local parameters on shape
		uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
		if(xPropSet.is())
		{
			drawing::PointSequenceSequence aPolyPoly(1L);
			drawing::PointSequence* pOuterSequence = aPolyPoly.getArray();
			pOuterSequence->realloc(2L);
			awt::Point* pInnerSequence = pOuterSequence->getArray();
			uno::Any aAny;

			*pInnerSequence = awt::Point( mnX1 - aTopLeft.X, mnY1 - aTopLeft.Y);
			pInnerSequence++;
			*pInnerSequence = awt::Point( mnX2 - aTopLeft.X, mnY2 - aTopLeft.Y);

			aAny <<= aPolyPoly;
			xPropSet->setPropertyValue(
				OUString(RTL_CONSTASCII_USTRINGPARAM("Geometry")), aAny);
		}

		// set sizes for transformation
		maSize.Width = aBottomRight.X - aTopLeft.X;
		maSize.Height = aBottomRight.Y - aTopLeft.Y;
		maPosition.X = aTopLeft.X;
		maPosition.Y = aTopLeft.Y;

		// set pos, size, shear and rotate and get copy of matrix
		SetTransformation();

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLEllipseShapeContext, SdXMLShapeContext );

SdXMLEllipseShapeContext::SdXMLEllipseShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mnCX( 0L ),
	mnCY( 0L ),
	mnRX( 1L ),
	mnRY( 1L ),
	meKind( drawing::CircleKind_FULL ),
	mnStartAngle( 0 ),
	mnEndAngle( 0 )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLEllipseShapeContext::~SdXMLEllipseShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLEllipseShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_SVG == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_RX ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnRX, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_RY ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnRY, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_CX ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnCX, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_CY ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnCY, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_R ) )
		{
			// single radius, it's a circle and both radii are the same
			GetImport().GetMM100UnitConverter().convertMeasure(mnRX, rValue);
			mnRY = mnRX;
			return;
		}
	}
	else if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_KIND ) )
		{
			sal_uInt16 eKind;
			if( SvXMLUnitConverter::convertEnum( eKind, rValue, aXML_CircleKind_EnumMap ) )
			{
				meKind = eKind;
			}
			return;
		}
		if( IsXMLToken( rLocalName, XML_START_ANGLE ) )
		{
			double dStartAngle;
			if( SvXMLUnitConverter::convertDouble( dStartAngle, rValue ) )
				mnStartAngle = (sal_Int32)(dStartAngle * 100.0);
			return;
		}
		if( IsXMLToken( rLocalName, XML_END_ANGLE ) )
		{
			double dEndAngle;
			if( SvXMLUnitConverter::convertDouble( dEndAngle, rValue ) )
				mnEndAngle = (sal_Int32)(dEndAngle * 100.0);
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLEllipseShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create rectangle shape
	AddShape("com.sun.star.drawing.EllipseShape");
	if(mxShape.is())
	{
		// Add, set Style and properties from base shape
		SetStyle();
		SetLayer();

        if(mnCX != 0 || mnCY != 0 || mnRX != 1 || mnRY != 1)
        {
            // #121972# center/radius is used, put to pos and size
            maSize.Width = 2 * mnRX;
            maSize.Height = 2 * mnRY;
            maPosition.X = mnCX - mnRX;
            maPosition.Y = mnCY - mnRY;
        }

		// set pos, size, shear and rotate
		SetTransformation();

		if( meKind != drawing::CircleKind_FULL )
		{
			uno::Reference< beans::XPropertySet > xPropSet( mxShape, uno::UNO_QUERY );
			if( xPropSet.is() )
			{
				uno::Any aAny;
				aAny <<= (drawing::CircleKind)meKind;
				xPropSet->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("CircleKind")), aAny );

				aAny <<= mnStartAngle;
				xPropSet->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("CircleStartAngle")), aAny );

				aAny <<= mnEndAngle;
				xPropSet->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("CircleEndAngle")), aAny );
			}
		}

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLPolygonShapeContext, SdXMLShapeContext );

SdXMLPolygonShapeContext::SdXMLPolygonShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes, sal_Bool bClosed, sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mbClosed( bClosed )
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLPolygonShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_SVG == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_VIEWBOX ) )
		{
			maViewBox = rValue;
			return;
		}
	}
	else if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_POINTS ) )
		{
			maPoints = rValue;
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

SdXMLPolygonShapeContext::~SdXMLPolygonShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLPolygonShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
    // Add, set Style and properties from base shape
    if(mbClosed)
        AddShape("com.sun.star.drawing.PolyPolygonShape");
    else
        AddShape("com.sun.star.drawing.PolyLineShape");

    if( mxShape.is() )
    {
        SetStyle();
        SetLayer();

        // set local parameters on shape
        uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
        if(xPropSet.is())
        {
            // set polygon
            if(maPoints.getLength() && maViewBox.getLength())
            {
                const SdXMLImExViewBox aViewBox(maViewBox, GetImport().GetMM100UnitConverter());
                basegfx::B2DVector aSize(aViewBox.GetWidth(), aViewBox.GetHeight());

                // Is this correct? It overrides ViewBox stuff; OTOH it makes no
                // sense to have the geometry content size different from object size
                if(maSize.Width != 0 && maSize.Height != 0)
                {
                    aSize = basegfx::B2DVector(maSize.Width, maSize.Height);
                }

                basegfx::B2DPolygon aPolygon;

                if(basegfx::tools::importFromSvgPoints(aPolygon, maPoints))
                {
                    if(aPolygon.count())
                    {
                        const basegfx::B2DRange aSourceRange(
                            aViewBox.GetX(), aViewBox.GetY(), 
                            aViewBox.GetX() + aViewBox.GetWidth(), aViewBox.GetY() + aViewBox.GetHeight());
                        const basegfx::B2DRange aTargetRange(
                            aViewBox.GetX(), aViewBox.GetY(), 
                            aViewBox.GetX() + aSize.getX(), aViewBox.GetY() + aSize.getY());

                        if(!aSourceRange.equal(aTargetRange))
                        {
                            aPolygon.transform(
                                basegfx::tools::createSourceRangeTargetRangeTransform(
                                    aSourceRange,
                                    aTargetRange));
                        }

                        com::sun::star::drawing::PointSequenceSequence aPointSequenceSequence;
                        uno::Any aAny;

                        basegfx::tools::B2DPolyPolygonToUnoPointSequenceSequence(basegfx::B2DPolyPolygon(aPolygon), aPointSequenceSequence);
                        aAny <<= aPointSequenceSequence;
                        xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("Geometry")), aAny);
                    }
                }
            }
        }

        // set pos, size, shear and rotate and get copy of matrix
        SetTransformation();

        SdXMLShapeContext::StartElement(xAttrList);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLPathShapeContext, SdXMLShapeContext );

SdXMLPathShapeContext::SdXMLPathShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mbClosed( sal_True )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLPathShapeContext::~SdXMLPathShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLPathShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_SVG == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_VIEWBOX ) )
		{
			maViewBox = rValue;
			return;
		}
		else if( IsXMLToken( rLocalName, XML_D ) )
		{
			maD = rValue;
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLPathShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
    // create polygon shape
    if(maD.getLength())
    {
        const SdXMLImExViewBox aViewBox(maViewBox, GetImport().GetMM100UnitConverter());
        basegfx::B2DVector aSize(aViewBox.GetWidth(), aViewBox.GetHeight());

        // Is this correct? It overrides ViewBox stuff; OTOH it makes no
        // sense to have the geometry content size different from object size
        if(maSize.Width != 0 && maSize.Height != 0)
        {
            aSize = basegfx::B2DVector(maSize.Width, maSize.Height);
        }

        basegfx::B2DPolyPolygon aPolyPolygon;

        if(basegfx::tools::importFromSvgD(aPolyPolygon, maD, true, 0))
        {
            if(aPolyPolygon.count())
            {
                const basegfx::B2DRange aSourceRange(
                    aViewBox.GetX(), aViewBox.GetY(), 
                    aViewBox.GetX() + aViewBox.GetWidth(), aViewBox.GetY() + aViewBox.GetHeight());
                const basegfx::B2DRange aTargetRange(
                    aViewBox.GetX(), aViewBox.GetY(), 
                    aViewBox.GetX() + aSize.getX(), aViewBox.GetY() + aSize.getY());

                if(!aSourceRange.equal(aTargetRange))
                {
                    aPolyPolygon.transform(
                        basegfx::tools::createSourceRangeTargetRangeTransform(
                            aSourceRange,
                            aTargetRange));
                }

                // create shape
                const char* pService;

                if(aPolyPolygon.areControlPointsUsed())
                {
                    if(aPolyPolygon.isClosed())
                    {
                        pService = "com.sun.star.drawing.ClosedBezierShape";
                    }
                    else
                    {
                        pService = "com.sun.star.drawing.OpenBezierShape";
                    }
                }
                else
                {
                    if(aPolyPolygon.isClosed())
                    {
                        pService = "com.sun.star.drawing.PolyPolygonShape";
                    }
                    else
                    {
                        pService = "com.sun.star.drawing.PolyLineShape";
                    }
                }

                // Add, set Style and properties from base shape
                AddShape(pService);

                // #89344# test for mxShape.is() and not for mxShapes.is() to support
                // shape import helper classes WITHOUT XShapes (member mxShapes). This
                // is used by the writer.
                if( mxShape.is() )
                {
                    SetStyle();
                    SetLayer();

                    // set local parameters on shape
                    uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);

                    if(xPropSet.is())
                    {
                        uno::Any aAny;

                        // set polygon data
                        if(aPolyPolygon.areControlPointsUsed())
                        {
                            drawing::PolyPolygonBezierCoords aSourcePolyPolygon;

                            basegfx::tools::B2DPolyPolygonToUnoPolyPolygonBezierCoords(
                                aPolyPolygon,
                                aSourcePolyPolygon);
                            aAny <<= aSourcePolyPolygon;
                        }
                        else
                        {
                            drawing::PointSequenceSequence aSourcePolyPolygon;

                            basegfx::tools::B2DPolyPolygonToUnoPointSequenceSequence(
                                aPolyPolygon,
                                aSourcePolyPolygon);
                            aAny <<= aSourcePolyPolygon;
                        }

                        xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("Geometry")), aAny);
                    }

                    // set pos, size, shear and rotate
                    SetTransformation();

                    SdXMLShapeContext::StartElement(xAttrList);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLTextBoxShapeContext, SdXMLShapeContext );

SdXMLTextBoxShapeContext::SdXMLTextBoxShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mnRadius(0)
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLTextBoxShapeContext::~SdXMLTextBoxShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLTextBoxShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_CORNER_RADIUS ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnRadius, rValue);
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLTextBoxShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>&)
{
	// create textbox shape
	sal_Bool bIsPresShape = sal_False;
	bool bClearText = false;

	const char *pService = NULL;

	if( isPresentationShape() )
	{
		// check if the current document supports presentation shapes
		if( GetImport().GetShapeImport()->IsPresentationShapesSupported() )
		{
			if( IsXMLToken( maPresentationClass, XML_PRESENTATION_SUBTITLE ))
			{
				// XmlShapeTypePresSubtitleShape
				pService = "com.sun.star.presentation.SubtitleShape";
			}
			else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_OUTLINE ) )
			{
				// XmlShapeTypePresOutlinerShape
				pService = "com.sun.star.presentation.OutlinerShape";
			}
			else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_NOTES ) )
			{
				// XmlShapeTypePresNotesShape
				pService = "com.sun.star.presentation.NotesShape";
			}
			else if( IsXMLToken( maPresentationClass, XML_HEADER ) )
			{
				// XmlShapeTypePresHeaderShape
				pService = "com.sun.star.presentation.HeaderShape";
				bClearText = true;
			}
			else if( IsXMLToken( maPresentationClass, XML_FOOTER ) )
			{
				// XmlShapeTypePresFooterShape
				pService = "com.sun.star.presentation.FooterShape";
				bClearText = true;
			}
			else if( IsXMLToken( maPresentationClass, XML_PAGE_NUMBER ) )
			{
				// XmlShapeTypePresSlideNumberShape
				pService = "com.sun.star.presentation.SlideNumberShape";
				bClearText = true;
			}
			else if( IsXMLToken( maPresentationClass, XML_DATE_TIME ) )
			{
				// XmlShapeTypePresDateTimeShape
				pService = "com.sun.star.presentation.DateTimeShape";
				bClearText = true;
			}
			else //  IsXMLToken( maPresentationClass, XML_PRESENTATION_TITLE ) )
			{
				// XmlShapeTypePresTitleTextShape
				pService = "com.sun.star.presentation.TitleTextShape";
			}
			bIsPresShape = sal_True;
		}
	}

	if( NULL == pService )
	{
		// normal text shape
		pService = "com.sun.star.drawing.TextShape";
	}

	// Add, set Style and properties from base shape
	AddShape(pService);

	if( mxShape.is() )
	{
		SetStyle();
		SetLayer();

		if(bIsPresShape)
		{
			uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( !mbIsPlaceholder && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( sal_False ) );

					if( mbIsUserTransformed && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}

		if( bClearText )
		{
			uno::Reference< text::XText > xText( mxShape, uno::UNO_QUERY );
			OUString aEmpty;
			xText->setString( aEmpty );
		}

		// set parameters on shape
//A AW->CL: Eventually You need to strip scale and translate from the transformation
//A to reach the same goal again.
//A		if(!bIsPresShape || mbIsUserTransformed)
//A		{
//A			// set pos and size on shape, this should remove binding
//A			// to pres object on masterpage
//A			SetSizeAndPosition();
//A		}

		// set pos, size, shear and rotate
		SetTransformation();

		if(mnRadius)
		{
			uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
			if(xPropSet.is())
			{
				try
				{
					xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("CornerRadius")), uno::makeAny( mnRadius ) );
				}
				catch( uno::Exception& )
				{
					DBG_ERROR( "exception during setting of corner radius!");
				}
			}
		}

		SdXMLShapeContext::StartElement(mxAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLControlShapeContext, SdXMLShapeContext );

SdXMLControlShapeContext::SdXMLControlShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLControlShapeContext::~SdXMLControlShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLControlShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_CONTROL ) )
		{
			maFormId = rValue;
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

void SdXMLControlShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create Control shape
	// add, set style and properties from base shape
	AddShape("com.sun.star.drawing.ControlShape");
	if( mxShape.is() )
	{
		DBG_ASSERT( maFormId.getLength(), "draw:control without a form:id attribute!" );
		if( maFormId.getLength() )
		{
#ifndef SVX_LIGHT
			if( GetImport().IsFormsSupported() )
			{
				uno::Reference< awt::XControlModel > xControlModel( GetImport().GetFormImport()->lookupControl( maFormId ), uno::UNO_QUERY );
				if( xControlModel.is() )
				{
					uno::Reference< drawing::XControlShape > xControl( mxShape, uno::UNO_QUERY );
					if( xControl.is() )
						xControl->setControl(  xControlModel );

				}
			}
#endif // #ifndef SVX_LIGHT
		}

		SetStyle();
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLConnectorShapeContext, SdXMLShapeContext );

SdXMLConnectorShapeContext::SdXMLConnectorShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	maStart(0,0),
	maEnd(1,1),
	mnType( (sal_uInt16)drawing::ConnectorType_STANDARD ),
	mnStartGlueId(-1),
	mnEndGlueId(-1),
	mnDelta1(0),
	mnDelta2(0),
	mnDelta3(0)
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLConnectorShapeContext::~SdXMLConnectorShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLConnectorShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_DRAW:
	{
		if( IsXMLToken( rLocalName, XML_START_SHAPE ) )
		{
			maStartShapeId = rValue;
			return;
		}
		if( IsXMLToken( rLocalName, XML_START_GLUE_POINT ) )
		{
			mnStartGlueId = rValue.toInt32();
			return;
		}
		if( IsXMLToken( rLocalName, XML_END_SHAPE ) )
		{
			maEndShapeId = rValue;
			return;
		}
		if( IsXMLToken( rLocalName, XML_END_GLUE_POINT ) )
		{
			mnEndGlueId = rValue.toInt32();
			return;
		}
		if( IsXMLToken( rLocalName, XML_LINE_SKEW ) )
		{
			SvXMLTokenEnumerator aTokenEnum( rValue );
			OUString aToken;
			if( aTokenEnum.getNextToken( aToken ) )
			{
				GetImport().GetMM100UnitConverter().convertMeasure(mnDelta1, aToken);
				if( aTokenEnum.getNextToken( aToken ) )
				{
					GetImport().GetMM100UnitConverter().convertMeasure(mnDelta2, aToken);
					if( aTokenEnum.getNextToken( aToken ) )
					{
						GetImport().GetMM100UnitConverter().convertMeasure(mnDelta3, aToken);
					}
				}
			}
			return;
		}
		if( IsXMLToken( rLocalName, XML_TYPE ) )
		{
			SvXMLUnitConverter::convertEnum( mnType, rValue, aXML_ConnectionKind_EnumMap );
			return;
		}
        // #121965# draw:transform may be used in ODF1.2, e.g. exports from MS seem to use these
		else if( IsXMLToken( rLocalName, XML_TRANSFORM ) )
		{
			mnTransform.SetString(rValue, GetImport().GetMM100UnitConverter());
		}
	}
	case XML_NAMESPACE_SVG:
	{
		if( IsXMLToken( rLocalName, XML_X1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maStart.X, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maStart.Y, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_X2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maEnd.X, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maEnd.Y, rValue);
			return;
		}
        if( IsXMLToken( rLocalName, XML_D ) )
        {
            basegfx::B2DPolyPolygon aPolyPolygon;

            if(basegfx::tools::importFromSvgD(aPolyPolygon, rValue, true, 0))
            {
                if(aPolyPolygon.count())
                {
                    // set polygon data
                    if(aPolyPolygon.areControlPointsUsed())
                    {
                        drawing::PolyPolygonBezierCoords aSourcePolyPolygon;

                        basegfx::tools::B2DPolyPolygonToUnoPolyPolygonBezierCoords(
                            aPolyPolygon,
                            aSourcePolyPolygon);
                        maPath <<= aSourcePolyPolygon;
                    }
                    else
                    {
                        drawing::PointSequenceSequence aSourcePolyPolygon;

                        basegfx::tools::B2DPolyPolygonToUnoPointSequenceSequence(
                            aPolyPolygon,
                            aSourcePolyPolygon);
                        maPath <<= aSourcePolyPolygon;
                    }
                }
            }
        }
	}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLConnectorShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// #107928#
	// For security reasons, do not add empty connectors. There may have been an error in EA2
	// that created empty, far set off connectors (e.g. 63 meters below top of document). This
	// is not guaranteed, but it's definitely safe to not add empty connectors.
	sal_Bool bDoAdd(sal_True);

	if(    0 == maStartShapeId.getLength()
		&& 0 == maEndShapeId.getLength()
		&& maStart.X == maEnd.X
		&& maStart.Y == maEnd.Y
		&& 0 == mnDelta1
		&& 0 == mnDelta2
		&& 0 == mnDelta3
		)
	{
		bDoAdd = sal_False;
	}

	if(bDoAdd)
	{
		// create Connector shape
		// add, set style and properties from base shape
		AddShape("com.sun.star.drawing.ConnectorShape");
		if(mxShape.is())
		{
            // #121965# if draw:transform is used, apply directly to the start
            // and end positions before using these
            if(mnTransform.NeedsAction())
            {
                // transformation is used, apply to object.
                ::basegfx::B2DHomMatrix aMat;
                mnTransform.GetFullTransform(aMat);

                if(!aMat.isIdentity())
                {
                    basegfx::B2DPoint aStart(maStart.X, maStart.Y);
                    basegfx::B2DPoint aEnd(maEnd.X, maEnd.Y);

                    aStart = aMat * aStart;
                    aEnd = aMat * aEnd;

                    maStart.X = basegfx::fround(aStart.getX());
                    maStart.Y = basegfx::fround(aStart.getY());
                    maEnd.X = basegfx::fround(aEnd.getX());
                    maEnd.Y = basegfx::fround(aEnd.getY());
                }
            }

            // add connection ids
			if( maStartShapeId.getLength() )
				GetImport().GetShapeImport()->addShapeConnection( mxShape, sal_True, maStartShapeId, mnStartGlueId );
			if( maEndShapeId.getLength() )
				GetImport().GetShapeImport()->addShapeConnection( mxShape, sal_False, maEndShapeId, mnEndGlueId );

			uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );
			if( xProps.is() )
			{
				uno::Any aAny;
				aAny <<= maStart;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("StartPosition")), aAny);

				aAny <<= maEnd;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EndPosition")), aAny );

				aAny <<= (drawing::ConnectorType)mnType;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EdgeKind")), aAny );

				aAny <<= mnDelta1;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EdgeLine1Delta")), aAny );

				aAny <<= mnDelta2;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EdgeLine2Delta")), aAny );

				aAny <<= mnDelta3;
				xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EdgeLine3Delta")), aAny );
			}
			SetStyle();
			SetLayer();

			if ( maPath.hasValue() )
            {
                // --> OD #i115492#
                // Ignore svg:d attribute for text documents created by OpenOffice.org 
                // versions before OOo 3.3, because these OOo versions are storing
                // svg:d values not using the correct unit.
                bool bApplySVGD( true );
                if ( uno::Reference< text::XTextDocument >(GetImport().GetModel(), uno::UNO_QUERY).is() )
                {
                    sal_Int32 nUPD( 0 );
                    sal_Int32 nBuild( 0 );
                    const bool bBuildIdFound = GetImport().getBuildIds( nUPD, nBuild );
                    if ( GetImport().IsTextDocInOOoFileFormat() ||
                         ( bBuildIdFound &&
                           ( ( nUPD == 641 ) || ( nUPD == 645 ) ||  // prior OOo 2.0
                             ( nUPD == 680 ) ||                     // OOo 2.x
                             ( nUPD == 300 ) ||                     // OOo 3.0 - OOo 3.0.1
                             ( nUPD == 310 ) ||                     // OOo 3.1 - OOo 3.1.1
                             ( nUPD == 320 ) ) ) )                  // OOo 3.2 - OOo 3.2.1
                    {
                        bApplySVGD = false;
                    }    
                }            

                if ( bApplySVGD )
                {
                    xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("PolyPolygonBezier") ), maPath );
                }                                                                                        
                // <--
            }

			SdXMLShapeContext::StartElement(xAttrList);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLMeasureShapeContext, SdXMLShapeContext );

SdXMLMeasureShapeContext::SdXMLMeasureShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	maStart(0,0),
	maEnd(1,1)
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLMeasureShapeContext::~SdXMLMeasureShapeContext()
{
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLMeasureShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_SVG:
	{
		if( IsXMLToken( rLocalName, XML_X1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maStart.X, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y1 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maStart.Y, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_X2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maEnd.X, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_Y2 ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maEnd.Y, rValue);
			return;
		}
	}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLMeasureShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create Measure shape
	// add, set style and properties from base shape
	AddShape("com.sun.star.drawing.MeasureShape");
	if(mxShape.is())
	{
		SetStyle();
		SetLayer();

		uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );
		if( xProps.is() )
		{
			uno::Any aAny;
			aAny <<= maStart;
			xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("StartPosition")), aAny);

			aAny <<= maEnd;
			xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("EndPosition")), aAny );
		}

		// delete pre created fields
		uno::Reference< text::XText > xText( mxShape, uno::UNO_QUERY );
		if( xText.is() )
		{
			const OUString aEmpty( RTL_CONSTASCII_USTRINGPARAM( " " ) );
			xText->setString( aEmpty );
		}

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

void SdXMLMeasureShapeContext::EndElement()
{
	do
	{
		// delete pre created fields
		uno::Reference< text::XText > xText( mxShape, uno::UNO_QUERY );
		if( !xText.is() )
			break;

		uno::Reference< text::XTextCursor > xCursor( xText->createTextCursor() );
		if( !xCursor.is() )
			break;

		const OUString aEmpty;
		xCursor->collapseToStart();
		xCursor->goRight( 1, sal_True );
		xCursor->setString( aEmpty );
	}
	while(0);

	SdXMLShapeContext::EndElement();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLPageShapeContext, SdXMLShapeContext );

SdXMLPageShapeContext::SdXMLPageShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ), mnPageNumber(0)
{
	mbClearDefaultAttributes = false;
}

//////////////////////////////////////////////////////////////////////////////

SdXMLPageShapeContext::~SdXMLPageShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLPageShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_PAGE_NUMBER ) )
		{
			mnPageNumber = rValue.toInt32();
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLPageShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create Page shape
	// add, set style and properties from base shape

	// #86163# take into account which type of PageShape needs to
	// be constructed. It's an pres shape if presentation:XML_CLASS == XML_PRESENTATION_PAGE.
	sal_Bool bIsPresentation = maPresentationClass.getLength() &&
		   GetImport().GetShapeImport()->IsPresentationShapesSupported();

	uno::Reference< lang::XServiceInfo > xInfo( mxShapes, uno::UNO_QUERY );
	const sal_Bool bIsOnHandoutPage = xInfo.is() && xInfo->supportsService( OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.presentation.HandoutMasterPage")) );

	if( bIsOnHandoutPage )
	{
		AddShape("com.sun.star.presentation.HandoutShape");
	}
	else
	{
		if(bIsPresentation && !IsXMLToken( maPresentationClass, XML_PRESENTATION_PAGE ) )
		{
			bIsPresentation = sal_False;
		}

		if(bIsPresentation)
		{
			AddShape("com.sun.star.presentation.PageShape");
		}
		else
		{
			AddShape("com.sun.star.drawing.PageShape");
		}
	}

	if(mxShape.is())
	{
		SetStyle();
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();

		uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
		if(xPropSet.is())
		{
			uno::Reference< beans::XPropertySetInfo > xPropSetInfo( xPropSet->getPropertySetInfo() );
			const OUString aPageNumberStr(RTL_CONSTASCII_USTRINGPARAM("PageNumber"));
			if( xPropSetInfo.is() && xPropSetInfo->hasPropertyByName(aPageNumberStr))
				xPropSet->setPropertyValue(aPageNumberStr, uno::makeAny( mnPageNumber ));
		}

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLCaptionShapeContext, SdXMLShapeContext );

SdXMLCaptionShapeContext::SdXMLCaptionShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	// #86616# for correct edge rounding import mnRadius needs to be initialized
	mnRadius( 0L )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLCaptionShapeContext::~SdXMLCaptionShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLCaptionShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	// create Caption shape
	// add, set style and properties from base shape
	AddShape("com.sun.star.drawing.CaptionShape");
	if( mxShape.is() )
	{
		SetStyle();
		SetLayer();

		uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );

		// SJ: If AutoGrowWidthItem is set, SetTransformation will lead to the wrong SnapRect
		// because NbcAdjustTextFrameWidthAndHeight() is called (text is set later and center alignment 
		// is the default setting, so the top left reference point that is used by the caption point is
		// no longer correct) There are two ways to solve this problem, temporarily disabling the
		// autogrowwith as we are doing here or to apply the CaptionPoint after setting text
		sal_Bool bIsAutoGrowWidth = sal_False;
		if ( xProps.is() )
		{
			uno::Any aAny( xProps->getPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("TextAutoGrowWidth") ) ) );
			aAny >>= bIsAutoGrowWidth;

			if ( bIsAutoGrowWidth )
				xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("TextAutoGrowWidth")), uno::makeAny( sal_False ) );
		}
			
		// set pos, size, shear and rotate
		SetTransformation();
		if( xProps.is() )
			xProps->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("CaptionPoint")), uno::makeAny( maCaptionPoint ) );

		if ( bIsAutoGrowWidth )
			xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("TextAutoGrowWidth")), uno::makeAny( sal_True ) );

		if(mnRadius)
		{
			uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
			if(xPropSet.is())
			{
				try
				{
					xPropSet->setPropertyValue(OUString(RTL_CONSTASCII_USTRINGPARAM("CornerRadius")), uno::makeAny( mnRadius ) );
				}
				catch( uno::Exception& )
				{
					DBG_ERROR( "exception during setting of corner radius!");
				}
			}
		}

		SdXMLShapeContext::StartElement(xAttrList);
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLCaptionShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_CAPTION_POINT_X ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maCaptionPoint.X, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_CAPTION_POINT_Y ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(maCaptionPoint.Y, rValue);
			return;
		}
		if( IsXMLToken( rLocalName, XML_CORNER_RADIUS ) )
		{
			GetImport().GetMM100UnitConverter().convertMeasure(mnRadius, rValue);
			return;
		}
	}
	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLGraphicObjectShapeContext, SdXMLShapeContext );

SdXMLGraphicObjectShapeContext::SdXMLGraphicObjectShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	maURL(),
    mbLateAddToIdentifierMapper(false)
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLGraphicObjectShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_XLINK == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_HREF ) )
		{
			maURL = rValue;
			return;
		}
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLGraphicObjectShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& )
{
	// create graphic object shape
	const char *pService;

	if( IsXMLToken( maPresentationClass, XML_GRAPHIC ) && GetImport().GetShapeImport()->IsPresentationShapesSupported() )
	{
		pService = "com.sun.star.presentation.GraphicObjectShape";
	}
	else
	{
		pService = "com.sun.star.drawing.GraphicObjectShape";
	}

	AddShape( pService );

	if(mxShape.is())
	{
		SetStyle();
		SetLayer();

		uno::Reference< beans::XPropertySet > xPropset(mxShape, uno::UNO_QUERY);
		if(xPropset.is())
		{
			// since OOo 1.x had no line or fill style for graphics, but may create
			// documents with them, we have to override them here
			sal_Int32 nUPD, nBuildId;
			if( GetImport().getBuildIds( nUPD, nBuildId ) && (nUPD == 645) ) try
			{
				xPropset->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("FillStyle")), Any( FillStyle_NONE ) );
				xPropset->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("LineStyle")), Any( LineStyle_NONE ) );
			}
			catch( Exception& )
			{
			}

			uno::Reference< beans::XPropertySetInfo > xPropsInfo( xPropset->getPropertySetInfo() );
			if( xPropsInfo.is() && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
				xPropset->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( mbIsPlaceholder ) );

			if( !mbIsPlaceholder )
			{
				if( maURL.getLength() )
				{
					uno::Any aAny;
					aAny <<= GetImport().ResolveGraphicObjectURL( maURL, GetImport().isGraphicLoadOnDemandSupported() );
					try
					{
						xPropset->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicURL") ), aAny );
						xPropset->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicStreamURL") ), aAny );
					}
					catch (lang::IllegalArgumentException const &)
					{
					}
				}
			}
		}

		if(mbIsUserTransformed)
		{
			uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}

		// set pos, size, shear and rotate
		SetTransformation();

		SdXMLShapeContext::StartElement(mxAttrList);
	}
}

void SdXMLGraphicObjectShapeContext::EndElement()
{
	if( mxBase64Stream.is() )
	{
		OUString sURL( GetImport().ResolveGraphicObjectURLFromBase64( mxBase64Stream ) );
		if( sURL.getLength() )
		{
			try
			{
				uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
				if(xProps.is())
				{
					const uno::Any aAny( uno::makeAny( sURL ) );
					xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicURL") ), aAny );
					xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicStreamURL") ), aAny );
				}
			}
			catch (lang::IllegalArgumentException const &)
			{
			}
		}
	}

	SdXMLShapeContext::EndElement();
}


//////////////////////////////////////////////////////////////////////////////

SvXMLImportContext* SdXMLGraphicObjectShapeContext::CreateChildContext(
	sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName,
	const uno::Reference<xml::sax::XAttributeList>& xAttrList )
{
	SvXMLImportContext* pContext = NULL;

	if( (XML_NAMESPACE_OFFICE == nPrefix) &&
			 xmloff::token::IsXMLToken( rLocalName, xmloff::token::XML_BINARY_DATA ) )
	{
		if( !maURL.getLength() && !mxBase64Stream.is() )
		{
			mxBase64Stream = GetImport().GetStreamForGraphicObjectURLFromBase64();
			if( mxBase64Stream.is() )
				pContext = new XMLBase64ImportContext( GetImport(), nPrefix,
													rLocalName, xAttrList,
													mxBase64Stream );
		}
	}

	// delegate to parent class if no context could be created
	if ( NULL == pContext )
		pContext = SdXMLShapeContext::CreateChildContext(nPrefix, rLocalName,
														 xAttrList);

	return pContext;
}

//////////////////////////////////////////////////////////////////////////////

SdXMLGraphicObjectShapeContext::~SdXMLGraphicObjectShapeContext()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLChartShapeContext, SdXMLShapeContext );

SdXMLChartShapeContext::SdXMLChartShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
	mpChartContext( NULL )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLChartShapeContext::~SdXMLChartShapeContext()
{
	if( mpChartContext )
		delete mpChartContext;
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLChartShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>& xAttrList)
{
	const sal_Bool bIsPresentation = isPresentationShape();

	AddShape( bIsPresentation ? "com.sun.star.presentation.ChartShape" : "com.sun.star.drawing.OLE2Shape" );

	if(mxShape.is())
	{
		SetStyle();
		SetLayer();

		if( !mbIsPlaceholder )
		{
			uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
					xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( sal_False ) );

				uno::Any aAny;

				const OUString aCLSID( RTL_CONSTASCII_USTRINGPARAM("12DCAE26-281F-416F-a234-c3086127382e"));

				aAny <<= aCLSID;
				xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("CLSID") ), aAny );

#ifndef SVX_LIGHT
				aAny = xProps->getPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("Model") ) );
				uno::Reference< frame::XModel > xChartModel;
				if( aAny >>= xChartModel )
				{
					mpChartContext = GetImport().GetChartImport()->CreateChartContext( GetImport(), XML_NAMESPACE_SVG, GetXMLToken(XML_CHART), xChartModel, xAttrList );
				}
#endif
			}
		}

		if(mbIsUserTransformed)
		{
			uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}


		// set pos, size, shear and rotate
		SetTransformation();

		SdXMLShapeContext::StartElement(xAttrList);

		if( mpChartContext )
			mpChartContext->StartElement( xAttrList );
	}
}

void SdXMLChartShapeContext::EndElement()
{
	if( mpChartContext )
		mpChartContext->EndElement();

	SdXMLShapeContext::EndElement();
}

void SdXMLChartShapeContext::Characters( const ::rtl::OUString& rChars )
{
	if( mpChartContext )
		mpChartContext->Characters( rChars );
}

SvXMLImportContext * SdXMLChartShapeContext::CreateChildContext( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList )
{
	if( mpChartContext )
		return mpChartContext->CreateChildContext( nPrefix, rLocalName, xAttrList );

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLObjectShapeContext, SdXMLShapeContext );

SdXMLObjectShapeContext::SdXMLObjectShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
		com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes,
        sal_Bool bTemporaryShape)
: SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape )
{
}

SdXMLObjectShapeContext::~SdXMLObjectShapeContext()
{
}

void SdXMLObjectShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& )
{
	// #96717# in theorie, if we don't have a url we shouldn't even
	// export this ole shape. But practical its to risky right now
	// to change this so we better dispose this on load
	//if( !mbIsPlaceholder && ImpIsEmptyURL(maHref) )
	//	return;

	// #100592# this BugFix prevents that a shape is created. CL
	// is thinking about an alternative.
    // #i13140# Check for more than empty string in maHref, there are
    // other possibilities that maHref results in empty container
    // storage names
	if( !(GetImport().getImportFlags() & IMPORT_EMBEDDED) && !mbIsPlaceholder && ImpIsEmptyURL(maHref) )
		return;

	const char* pService = "com.sun.star.drawing.OLE2Shape";

	sal_Bool bIsPresShape = maPresentationClass.getLength() && GetImport().GetShapeImport()->IsPresentationShapesSupported();

	if( bIsPresShape )
	{
		if( IsXMLToken( maPresentationClass, XML_PRESENTATION_CHART ) )
		{
			pService = "com.sun.star.presentation.ChartShape";
		}
		else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_TABLE ) )
		{
			pService = "com.sun.star.presentation.CalcShape";
		}
		else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_OBJECT ) )
		{
			pService = "com.sun.star.presentation.OLE2Shape";
		}
	}

	AddShape( pService );

	if( mxShape.is() )
	{
		SetLayer();

		if(bIsPresShape)
		{
			uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( !mbIsPlaceholder && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( sal_False ) );

					if( mbIsUserTransformed && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}

		if( !mbIsPlaceholder && maHref.getLength() )
		{
        	uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );

            if( xProps.is() )
        	{
				OUString aPersistName = GetImport().ResolveEmbeddedObjectURL( maHref, maCLSID );

				if ( GetImport().IsPackageURL( maHref ) )
				{
					const OUString  sURL(RTL_CONSTASCII_USTRINGPARAM( "vnd.sun.star.EmbeddedObject:" ));

					if ( aPersistName.compareTo( sURL, sURL.getLength() ) == 0 )
						aPersistName = aPersistName.copy( sURL.getLength() );

        			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "PersistName" ) ),
											  uno::makeAny( aPersistName ) );
				}
				else
				{
					// this is OOo link object
					xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "LinkURL" ) ),
											  uno::makeAny( aPersistName ) );
				}
			}
		}

		// set pos, size, shear and rotate
		SetTransformation();

		SetStyle();

		GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );
	}
}

void SdXMLObjectShapeContext::EndElement()
{
    // #i67705#
    const sal_uInt16 nGeneratorVersion(GetImport().getGeneratorVersion());

    if(nGeneratorVersion < SvXMLImport::OOo_34x)
    {
        // #i118485#
        // If it's an old file from us written before OOo3.4, we need to correct
        // FillStyle and LineStyle for OLE2 objects. The error was that the old paint
        // implementations just ignored added fill/linestyles completely, thus
        // those objects need to be corrected to not show blue and hairline which
        // always was the default, but would be shown now
		uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);

        if( xProps.is() )
        {
            xProps->setPropertyValue(::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("FillStyle")), uno::makeAny(drawing::FillStyle_NONE));
            xProps->setPropertyValue(::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("LineStyle")), uno::makeAny(drawing::LineStyle_NONE));
        }
    }

    // #100592#
	if( mxBase64Stream.is() )
	{
		OUString aPersistName( GetImport().ResolveEmbeddedObjectURLFromBase64() );
		const OUString  sURL(RTL_CONSTASCII_USTRINGPARAM( "vnd.sun.star.EmbeddedObject:" ));

        aPersistName = aPersistName.copy( sURL.getLength() );

		uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);
		if( xProps.is() )
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "PersistName" ) ), uno::makeAny( aPersistName ) );
	}

	SdXMLShapeContext::EndElement();
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLObjectShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_DRAW:
		if( IsXMLToken( rLocalName, XML_CLASS_ID ) )
		{
			maCLSID = rValue;
			return;
		}
		break;
	case XML_NAMESPACE_XLINK:
		if( IsXMLToken( rLocalName, XML_HREF ) )
		{
			maHref = rValue;
			return;
		}
		break;
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

SvXMLImportContext* SdXMLObjectShapeContext::CreateChildContext(
	sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName,
	const uno::Reference<xml::sax::XAttributeList>& xAttrList )
{
	// #100592#
	SvXMLImportContext* pContext = NULL;

	if((XML_NAMESPACE_OFFICE == nPrefix) && IsXMLToken(rLocalName, XML_BINARY_DATA))
	{
		mxBase64Stream = GetImport().GetStreamForEmbeddedObjectURLFromBase64();
		if( mxBase64Stream.is() )
			pContext = new XMLBase64ImportContext( GetImport(), nPrefix,
												rLocalName, xAttrList,
												mxBase64Stream );
	}
	else if( ((XML_NAMESPACE_OFFICE == nPrefix) && IsXMLToken(rLocalName, XML_DOCUMENT)) ||
				((XML_NAMESPACE_MATH == nPrefix) && IsXMLToken(rLocalName, XML_MATH)) )
	{
		XMLEmbeddedObjectImportContext *pEContext =
			new XMLEmbeddedObjectImportContext( GetImport(), nPrefix,
												rLocalName, xAttrList );
		maCLSID = pEContext->GetFilterCLSID();
		if( maCLSID.getLength() != 0 )
		{
			uno::Reference< beans::XPropertySet > xPropSet(mxShape, uno::UNO_QUERY);
			if( xPropSet.is() )
			{
				xPropSet->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("CLSID") ), uno::makeAny( maCLSID ) );

				uno::Reference< lang::XComponent > xComp;
				xPropSet->getPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("Model") ) ) >>= xComp;
				DBG_ASSERT( xComp.is(), "no xModel for own OLE format" );
				pEContext->SetComponent( xComp );
			}
		}
		pContext = pEContext;
	}

	// delegate to parent class if no context could be created
	if(!pContext)
		pContext = SdXMLShapeContext::CreateChildContext(nPrefix, rLocalName, xAttrList);

	return pContext;
}

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLAppletShapeContext, SdXMLShapeContext );

SdXMLAppletShapeContext::SdXMLAppletShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
		com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes,
        sal_Bool bTemporaryShape)
: SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
  mbIsScript( sal_False )
{
}

SdXMLAppletShapeContext::~SdXMLAppletShapeContext()
{
}

void SdXMLAppletShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& )
{
	const char* pService = "com.sun.star.drawing.AppletShape";
	AddShape( pService );

	if( mxShape.is() )
	{
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();
		GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLAppletShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_DRAW:
		if( IsXMLToken( rLocalName, XML_APPLET_NAME ) )
		{
			maAppletName = rValue;
			return;
		}
		if( IsXMLToken( rLocalName, XML_CODE ) )
		{
			maAppletCode = rValue;
			return;
		}
		if( IsXMLToken( rLocalName, XML_MAY_SCRIPT ) )
		{
			mbIsScript = IsXMLToken( rValue, XML_TRUE );
			return;
		}
		break;
	case XML_NAMESPACE_XLINK:
		if( IsXMLToken( rLocalName, XML_HREF ) )
		{
			maHref = GetImport().GetAbsoluteReference(rValue);
			return;
		}
		break;
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

void SdXMLAppletShapeContext::EndElement()
{
	uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );
	if( xProps.is() )
	{
		uno::Any aAny;

		if ( maSize.Width && maSize.Height )
		{
			// the visual area for applet must be set on loading
			awt::Rectangle aRect( 0, 0, maSize.Width, maSize.Height );
			aAny <<= aRect;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "VisibleArea" ) ), aAny );
		}
	
		if( maParams.getLength() )
		{
			aAny <<= maParams;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletCommands" ) ), aAny );
		}

		if( maHref.getLength() )
		{
			aAny <<= maHref;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletCodeBase" ) ), aAny );
		}

		if( maAppletName.getLength() )
		{
			aAny <<= maAppletName;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletName" ) ), aAny );
		}

		if( mbIsScript )
		{
			aAny <<= mbIsScript;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletIsScript" ) ), aAny );

		}

		if( maAppletCode.getLength() )
		{
			aAny <<= maAppletCode;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletCode" ) ), aAny );
		}

        aAny <<= ::rtl::OUString( GetImport().GetDocumentBase() );
        xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "AppletDocBase" ) ), aAny );

		SetThumbnail();
	}

	SdXMLShapeContext::EndElement();
}

SvXMLImportContext * SdXMLAppletShapeContext::CreateChildContext( sal_uInt16 p_nPrefix, const ::rtl::OUString& rLocalName, const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList )
{
	if( p_nPrefix == XML_NAMESPACE_DRAW && IsXMLToken( rLocalName, XML_PARAM ) )
	{
		OUString aParamName, aParamValue;
		const sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
		// now parse the attribute list and look for draw:name and draw:value
		for(sal_Int16 a(0); a < nAttrCount; a++)
		{
			const OUString& rAttrName = xAttrList->getNameByIndex(a);
			OUString aLocalName;
			sal_uInt16 nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName(rAttrName, &aLocalName);
			const OUString aValue( xAttrList->getValueByIndex(a) );

			if( nPrefix == XML_NAMESPACE_DRAW )
			{
				if( IsXMLToken( aLocalName, XML_NAME ) )
				{
					aParamName = aValue;
				}
				else if( IsXMLToken( aLocalName, XML_VALUE ) )
				{
					aParamValue = aValue;
				}
			}
		}

		if( aParamName.getLength() )
		{
			sal_Int32 nIndex = maParams.getLength();
			maParams.realloc( nIndex + 1 );
			maParams[nIndex].Name = aParamName;
			maParams[nIndex].Handle = -1;
			maParams[nIndex].Value <<= aParamValue;
			maParams[nIndex].State = beans::PropertyState_DIRECT_VALUE;
		}

		return new SvXMLImportContext( GetImport(), p_nPrefix, rLocalName );
	}

	return SdXMLShapeContext::CreateChildContext( p_nPrefix, rLocalName, xAttrList );
}

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLPluginShapeContext, SdXMLShapeContext );

SdXMLPluginShapeContext::SdXMLPluginShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
		com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes,
        sal_Bool bTemporaryShape) :
SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
mbMedia( false )
{
}

SdXMLPluginShapeContext::~SdXMLPluginShapeContext()
{
}

void SdXMLPluginShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& xAttrList)
{
	// watch for MimeType attribute to see if we have a media object
	for( sal_Int16 n = 0, nAttrCount = ( xAttrList.is() ? xAttrList->getLength() : 0 ); n < nAttrCount; ++n )
	{
		OUString 	aLocalName;
		sal_uInt16 	nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName( xAttrList->getNameByIndex( n ), &aLocalName );

		if( nPrefix == XML_NAMESPACE_DRAW && IsXMLToken( aLocalName, XML_MIME_TYPE ) )
		{
			if( 0 == xAttrList->getValueByIndex( n ).compareToAscii( "application/vnd.sun.star.media" ) )
				mbMedia = true;

			// leave this loop
			n = nAttrCount - 1;
		}
	}

	const char* pService;

	sal_Bool bIsPresShape = sal_False;

	if( mbMedia )
	{
		pService = "com.sun.star.drawing.MediaShape";

		bIsPresShape = maPresentationClass.getLength() && GetImport().GetShapeImport()->IsPresentationShapesSupported();
		if( bIsPresShape )
		{
			if( IsXMLToken( maPresentationClass, XML_PRESENTATION_OBJECT ) )
			{
				pService = "com.sun.star.presentation.MediaShape";
			}
		}
	}
	else
		pService = "com.sun.star.drawing.PluginShape";

	AddShape( pService );

	if( mxShape.is() )
	{
		SetLayer();

		if(bIsPresShape)
		{
			uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( !mbIsPlaceholder && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( sal_False ) );

					if( mbIsUserTransformed && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}

		// set pos, size, shear and rotate
		SetTransformation();
		GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLPluginShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_DRAW:
		if( IsXMLToken( rLocalName, XML_MIME_TYPE ) )
		{
			maMimeType = rValue;
			return;
		}
		break;
	case XML_NAMESPACE_XLINK:
		if( IsXMLToken( rLocalName, XML_HREF ) )
		{
			maHref = GetImport().GetAbsoluteReference(rValue);
			return;
		}
		break;
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

void SdXMLPluginShapeContext::EndElement()
{
	uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );

	if( xProps.is() )
	{
		uno::Any aAny;

		if ( maSize.Width && maSize.Height )
		{
			const rtl::OUString sVisibleArea( RTL_CONSTASCII_USTRINGPARAM( "VisibleArea" ) );
			uno::Reference< beans::XPropertySetInfo > aXPropSetInfo( xProps->getPropertySetInfo() );
            if ( !aXPropSetInfo.is() || aXPropSetInfo->hasPropertyByName( sVisibleArea ) )
			{
				// the visual area for a plugin must be set on loading
				awt::Rectangle aRect( 0, 0, maSize.Width, maSize.Height );
				aAny <<= aRect;
				xProps->setPropertyValue( sVisibleArea, aAny );
			}
		}

		if( !mbMedia )
		{
			// in case we have a plugin object
			if( maParams.getLength() )
			{
				aAny <<= maParams;
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "PluginCommands" ) ), aAny );
			}

			if( maMimeType.getLength() )
			{
				aAny <<= maMimeType;
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "PluginMimeType" ) ), aAny );
			}

			if( maHref.getLength() )
			{
				aAny <<= maHref;
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "PluginURL" ) ), aAny );
			}
		}
		else
		{
			// in case we have a media object
			
			OUString sTempRef;
			
			// check for package URL
			if( GetImport().IsPackageURL( maHref ) )
			{
			    sTempRef = OUString( RTL_CONSTASCII_USTRINGPARAM( "vnd.sun.star.Package:" ) );			    
			}
			
			sTempRef += maHref;
			
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "MediaURL" ) ), uno::makeAny( sTempRef ) );

			for( sal_Int32 nParam = 0; nParam < maParams.getLength(); ++nParam )
			{
				const OUString& rName = maParams[ nParam ].Name;

				if( 0 == rName.compareToAscii( "Loop" ) )
				{
					OUString aValueStr;
					maParams[ nParam ].Value >>= aValueStr;
					xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "Loop" ) ),
						uno::makeAny( static_cast< sal_Bool >( 0 == aValueStr.compareToAscii( "true" ) ) ) );
				}
				else if( 0 == rName.compareToAscii( "Mute" ) )
				{
					OUString aValueStr;
					maParams[ nParam ].Value >>= aValueStr;
					xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "Mute" ) ),
						uno::makeAny( static_cast< sal_Bool >( 0 == aValueStr.compareToAscii( "true" ) ) ) );
				}
				else if( 0 == rName.compareToAscii( "VolumeDB" ) )
				{
					OUString aValueStr;
					maParams[ nParam ].Value >>= aValueStr;
					xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "VolumeDB" ) ),
					                            uno::makeAny( static_cast< sal_Int16 >( aValueStr.toInt32() ) ) );
				}
				else if( 0 == rName.compareToAscii( "Zoom" ) )
				{
					OUString 			aZoomStr;
					media::ZoomLevel	eZoomLevel;

					maParams[ nParam ].Value >>= aZoomStr;

					if( 0 == aZoomStr.compareToAscii( "25%" ) )
						eZoomLevel = media::ZoomLevel_ZOOM_1_TO_4;
					else if( 0 == aZoomStr.compareToAscii( "50%" ) )
						eZoomLevel = media::ZoomLevel_ZOOM_1_TO_2;
					else if( 0 == aZoomStr.compareToAscii( "100%" ) )
						eZoomLevel = media::ZoomLevel_ORIGINAL;
					else if( 0 == aZoomStr.compareToAscii( "200%" ) )
						eZoomLevel = media::ZoomLevel_ZOOM_2_TO_1;
					else if( 0 == aZoomStr.compareToAscii( "400%" ) )
						eZoomLevel = media::ZoomLevel_ZOOM_4_TO_1;
					else if( 0 == aZoomStr.compareToAscii( "fit" ) )
						eZoomLevel = media::ZoomLevel_FIT_TO_WINDOW;
					else if( 0 == aZoomStr.compareToAscii( "fixedfit" ) )
						eZoomLevel = media::ZoomLevel_FIT_TO_WINDOW_FIXED_ASPECT;
					else if( 0 == aZoomStr.compareToAscii( "fullscreen" ) )
						eZoomLevel = media::ZoomLevel_FULLSCREEN;
					else
						eZoomLevel = media::ZoomLevel_NOT_AVAILABLE;

					xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "Zoom" ) ), uno::makeAny( eZoomLevel ) );
				}
			}
		}

		SetThumbnail();
	}

	SdXMLShapeContext::EndElement();
}

SvXMLImportContext * SdXMLPluginShapeContext::CreateChildContext( sal_uInt16 p_nPrefix, const ::rtl::OUString& rLocalName, const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList )
{
	if( p_nPrefix == XML_NAMESPACE_DRAW && IsXMLToken( rLocalName, XML_PARAM ) )
	{
		OUString aParamName, aParamValue;
		const sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
		// now parse the attribute list and look for draw:name and draw:value
		for(sal_Int16 a(0); a < nAttrCount; a++)
		{
			const OUString& rAttrName = xAttrList->getNameByIndex(a);
			OUString aLocalName;
			sal_uInt16 nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName(rAttrName, &aLocalName);
			const OUString aValue( xAttrList->getValueByIndex(a) );

			if( nPrefix == XML_NAMESPACE_DRAW )
			{
				if( IsXMLToken( aLocalName, XML_NAME ) )
				{
					aParamName = aValue;
				}
				else if( IsXMLToken( aLocalName, XML_VALUE ) )
				{
					aParamValue = aValue;
				}
			}

			if( aParamName.getLength() )
			{
				sal_Int32 nIndex = maParams.getLength();
				maParams.realloc( nIndex + 1 );
				maParams[nIndex].Name = aParamName;
				maParams[nIndex].Handle = -1;
				maParams[nIndex].Value <<= aParamValue;
				maParams[nIndex].State = beans::PropertyState_DIRECT_VALUE;
			}
		}

		return new SvXMLImportContext( GetImport(), p_nPrefix, rLocalName );
	}

	return SdXMLShapeContext::CreateChildContext( p_nPrefix, rLocalName, xAttrList );
}

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLFloatingFrameShapeContext, SdXMLShapeContext );

SdXMLFloatingFrameShapeContext::SdXMLFloatingFrameShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
		com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes,
        sal_Bool bTemporaryShape)
: SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape )
{
}

SdXMLFloatingFrameShapeContext::~SdXMLFloatingFrameShapeContext()
{
}

void SdXMLFloatingFrameShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& )
{
	const char* pService = "com.sun.star.drawing.FrameShape";
	AddShape( pService );

	if( mxShape.is() )
	{
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();

		uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );
		if( xProps.is() )
		{
			uno::Any aAny;

			if( maFrameName.getLength() )
			{
				aAny <<= maFrameName;
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "FrameName" ) ), aAny );
			}

			if( maHref.getLength() )
			{
				aAny <<= maHref;
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "FrameURL" ) ), aAny );
			}
		}

		SetStyle();

		GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLFloatingFrameShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	switch( nPrefix )
	{
	case XML_NAMESPACE_DRAW:
		if( IsXMLToken( rLocalName, XML_FRAME_NAME ) )
		{
			maFrameName = rValue;
			return;
		}
		break;
	case XML_NAMESPACE_XLINK:
		if( IsXMLToken( rLocalName, XML_HREF ) )
		{
			maHref = GetImport().GetAbsoluteReference(rValue);
			return;
		}
		break;
	}

	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

void SdXMLFloatingFrameShapeContext::EndElement()
{
	uno::Reference< beans::XPropertySet > xProps( mxShape, uno::UNO_QUERY );

	if( xProps.is() )
	{
		if ( maSize.Width && maSize.Height )
		{
			// the visual area for a floating frame must be set on loading
			awt::Rectangle aRect( 0, 0, maSize.Width, maSize.Height );
			uno::Any aAny;
			aAny <<= aRect;
			xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "VisibleArea" ) ), aAny );
		}
	}

	SetThumbnail();
	SdXMLShapeContext::EndElement();
}

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLFrameShapeContext, SdXMLShapeContext );

SdXMLFrameShapeContext::SdXMLFrameShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
		com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes,
        sal_Bool bTemporaryShape)
: SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape ),
    multiImageImportHelper(),
	mbSupportsReplacement( sal_False ),
    mxImplContext(),
    mxReplImplContext()
{
	uno::Reference < util::XCloneable > xClone( xAttrList, uno::UNO_QUERY );
	if( xClone.is() )
		mxAttrList.set( xClone->createClone(), uno::UNO_QUERY );
	else
		mxAttrList = new SvXMLAttributeList( xAttrList );

}

SdXMLFrameShapeContext::~SdXMLFrameShapeContext()
{
}

void SdXMLFrameShapeContext::removeGraphicFromImportContext(const SvXMLImportContext& rContext) const
{
    const SdXMLGraphicObjectShapeContext* pSdXMLGraphicObjectShapeContext = dynamic_cast< const SdXMLGraphicObjectShapeContext* >(&rContext);

    if(pSdXMLGraphicObjectShapeContext)
    {
        try
        {
            uno::Reference< container::XChild > xChild(pSdXMLGraphicObjectShapeContext->getShape(), uno::UNO_QUERY_THROW);

            if(xChild.is())
            {
                uno::Reference< drawing::XShapes > xParent(xChild->getParent(), uno::UNO_QUERY_THROW);
            
                if(xParent.is())
                {
                    // remove from parent
                    xParent->remove(pSdXMLGraphicObjectShapeContext->getShape());

                    // dispose
                    uno::Reference< lang::XComponent > xComp(pSdXMLGraphicObjectShapeContext->getShape(), UNO_QUERY);

                    if(xComp.is())
                    {
                        xComp->dispose();
                    }
                }
            }
        }
        catch( uno::Exception& )
        {
            DBG_ERROR( "Error in cleanup of multiple graphic object import (!)" );
        }
    }
}

rtl::OUString SdXMLFrameShapeContext::getGraphicURLFromImportContext(const SvXMLImportContext& rContext) const
{
    rtl::OUString aRetval;
    const SdXMLGraphicObjectShapeContext* pSdXMLGraphicObjectShapeContext = dynamic_cast< const SdXMLGraphicObjectShapeContext* >(&rContext);

    if(pSdXMLGraphicObjectShapeContext)
    {
        try
        {
            const uno::Reference< beans::XPropertySet > xPropSet(pSdXMLGraphicObjectShapeContext->getShape(), uno::UNO_QUERY_THROW);

            if(xPropSet.is())
            {
                xPropSet->getPropertyValue(rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicStreamURL"))) >>= aRetval;

                if(!aRetval.getLength())
                {
                    // it maybe a link, try GraphicURL
                    xPropSet->getPropertyValue(rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("GraphicURL"))) >>= aRetval;
                }
            }
        }
        catch( uno::Exception& )
        {
            DBG_ERROR( "Error in cleanup of multiple graphic object import (!)" );
        }
    }

    return aRetval;
}

SvXMLImportContext *SdXMLFrameShapeContext::CreateChildContext( sal_uInt16 nPrefix,
	const OUString& rLocalName,
	const uno::Reference< xml::sax::XAttributeList>& xAttrList )
{
	SvXMLImportContext * pContext = 0;

	if( !mxImplContext.Is() )
	{
        pContext = GetImport().GetShapeImport()->CreateFrameChildContext(
            GetImport(), nPrefix, rLocalName, xAttrList, mxShapes, mxAttrList);

        mxImplContext = pContext;
        mbSupportsReplacement = IsXMLToken(rLocalName, XML_OBJECT ) || IsXMLToken(rLocalName, XML_OBJECT_OLE);
        setSupportsMultipleContents(IsXMLToken(rLocalName, XML_IMAGE));

        if(getSupportsMultipleContents())
        {
            SdXMLGraphicObjectShapeContext* pGSC = dynamic_cast< SdXMLGraphicObjectShapeContext* >(pContext);

            if(pGSC)
            {
                // mark context as LateAdd to avoid conflicts with multiple objects registering with the same ID
                pGSC->setLateAddToIdentifierMapper(true);
                addContent(*mxImplContext);
            }
        }
    }
    else if(getSupportsMultipleContents() && XML_NAMESPACE_DRAW == nPrefix && IsXMLToken(rLocalName, XML_IMAGE))
    {
        // read another image
        pContext = GetImport().GetShapeImport()->CreateFrameChildContext(
            GetImport(), nPrefix, rLocalName, xAttrList, mxShapes, mxAttrList);
        mxImplContext = pContext;

        SdXMLGraphicObjectShapeContext* pGSC = dynamic_cast< SdXMLGraphicObjectShapeContext* >(pContext);

        if(pGSC)
        {
            // mark context as LateAdd to avoid conflicts with multiple objects registering with the same ID
            pGSC->setLateAddToIdentifierMapper(true);
            addContent(*mxImplContext);
        }
    }
	else if( mbSupportsReplacement && !mxReplImplContext &&
			 XML_NAMESPACE_DRAW == nPrefix &&
			 IsXMLToken( rLocalName, XML_IMAGE ) )
	{
		// read replacement image
		SvXMLImportContext *pImplContext = &mxImplContext;
		SdXMLShapeContext *pSContext =
			PTR_CAST( SdXMLShapeContext, pImplContext );
		if( pSContext )
		{
			uno::Reference < beans::XPropertySet > xPropSet(
					pSContext->getShape(), uno::UNO_QUERY );
			if( xPropSet.is() )
			{
				pContext = new XMLReplacementImageContext( GetImport(),
									nPrefix, rLocalName, xAttrList, xPropSet );
				mxReplImplContext = pContext;
			}
		}
	}
	else if( 
			( nPrefix == XML_NAMESPACE_SVG &&	// #i68101#
				(IsXMLToken( rLocalName, XML_TITLE ) || IsXMLToken( rLocalName, XML_DESC ) ) ) ||
			 (nPrefix == XML_NAMESPACE_OFFICE && IsXMLToken( rLocalName, XML_EVENT_LISTENERS ) ) ||
			 (nPrefix == XML_NAMESPACE_DRAW && (IsXMLToken( rLocalName, XML_GLUE_POINT ) ||
												IsXMLToken( rLocalName, XML_THUMBNAIL ) ) ) )
	{
		SvXMLImportContext *pImplContext = &mxImplContext;
		pContext = PTR_CAST( SdXMLShapeContext, pImplContext )->CreateChildContext( nPrefix,
																		rLocalName, xAttrList );
	}
	else if ( (XML_NAMESPACE_DRAW == nPrefix) && IsXMLToken( rLocalName, XML_IMAGE_MAP ) )
	{
		SdXMLShapeContext *pSContext = dynamic_cast< SdXMLShapeContext* >( &mxImplContext );
		if( pSContext )
		{
			uno::Reference < beans::XPropertySet > xPropSet( pSContext->getShape(), uno::UNO_QUERY );
			if (xPropSet.is())
			{
				pContext = new XMLImageMapContext(GetImport(), nPrefix, rLocalName, xPropSet);
			}
		}
	}

	// call parent for content
	if(!pContext)
		pContext = SvXMLImportContext::CreateChildContext( nPrefix, rLocalName, xAttrList );

	return pContext;
}

void SdXMLFrameShapeContext::StartElement(const uno::Reference< xml::sax::XAttributeList>&)
{
	// ignore
}

void SdXMLFrameShapeContext::EndElement()
{
    /// solve if multiple image child contexts were imported
    const SvXMLImportContext* pWinner = solveMultipleImages();
    const SdXMLGraphicObjectShapeContext* pGSCWinner = dynamic_cast< const SdXMLGraphicObjectShapeContext* >(pWinner);

    /// if we have a winner and it's on LateAdd, add it now
    if(pGSCWinner && pGSCWinner->getLateAddToIdentifierMapper() && pGSCWinner->getShapeId().getLength())
    {
        uno::Reference< uno::XInterface > xRef( pGSCWinner->getShape(), uno::UNO_QUERY );
        GetImport().getInterfaceToIdentifierMapper().registerReference( pGSCWinner->getShapeId(), xRef );
    }

	if( !mxImplContext.Is() )
	{
		// now check if this is an empty presentation object
		sal_Int16 nAttrCount = mxAttrList.is() ? mxAttrList->getLength() : 0;
		for(sal_Int16 a(0); a < nAttrCount; a++)
		{
			OUString aLocalName;
			sal_uInt16 nPrefix = GetImport().GetNamespaceMap().GetKeyByAttrName(mxAttrList->getNameByIndex(a), &aLocalName);

			if( nPrefix == XML_NAMESPACE_PRESENTATION )
			{
				if( IsXMLToken( aLocalName, XML_PLACEHOLDER ) )
				{
					mbIsPlaceholder = IsXMLToken( mxAttrList->getValueByIndex(a), XML_TRUE );
				}
				else if( IsXMLToken( aLocalName, XML_CLASS ) )
				{
					maPresentationClass = mxAttrList->getValueByIndex(a);
				}
			}
		}

		if( (maPresentationClass.getLength() != 0) && mbIsPlaceholder )
		{
			uno::Reference< xml::sax::XAttributeList> xEmpty;

			enum XMLTokenEnum eToken = XML_TEXT_BOX;

			if( IsXMLToken( maPresentationClass, XML_GRAPHIC ) )
			{
				eToken = XML_IMAGE;

			}
			else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_PAGE ) )
			{
				eToken = XML_PAGE_THUMBNAIL;
			}
			else if( IsXMLToken( maPresentationClass, XML_PRESENTATION_CHART ) ||
					 IsXMLToken( maPresentationClass, XML_PRESENTATION_TABLE ) ||
					 IsXMLToken( maPresentationClass, XML_PRESENTATION_OBJECT ) )
			{
				eToken = XML_OBJECT;
			}

			mxImplContext = GetImport().GetShapeImport()->CreateFrameChildContext(
					GetImport(), XML_NAMESPACE_DRAW, GetXMLToken( eToken ), mxAttrList, mxShapes, xEmpty );

			if( mxImplContext.Is() )
			{
				mxImplContext->StartElement( mxAttrList );
				mxImplContext->EndElement();
			}
		}
	}

	mxImplContext = 0;
	SdXMLShapeContext::EndElement();
}

void SdXMLFrameShapeContext::processAttribute( sal_uInt16,
		const ::rtl::OUString&, const ::rtl::OUString& )
{
	// ignore
}

TYPEINIT1( SdXMLCustomShapeContext, SdXMLShapeContext );

SdXMLCustomShapeContext::SdXMLCustomShapeContext(
	SvXMLImport& rImport,
	sal_uInt16 nPrfx,
	const OUString& rLocalName,
	const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList,
	uno::Reference< drawing::XShapes >& rShapes,
    sal_Bool bTemporaryShape)
:	SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, bTemporaryShape )
{
}

//////////////////////////////////////////////////////////////////////////////

SdXMLCustomShapeContext::~SdXMLCustomShapeContext()
{
}

//////////////////////////////////////////////////////////////////////////////

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLCustomShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( XML_NAMESPACE_DRAW == nPrefix )
	{
		if( IsXMLToken( rLocalName, XML_ENGINE ) )
		{
			maCustomShapeEngine = rValue;
			return;
		}
		if ( IsXMLToken( rLocalName, XML_DATA ) )
		{
			maCustomShapeData = rValue;
			return;
		}
	}
	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

//////////////////////////////////////////////////////////////////////////////

void SdXMLCustomShapeContext::StartElement( const uno::Reference< xml::sax::XAttributeList >& xAttrList )
{
	// create rectangle shape
	AddShape("com.sun.star.drawing.CustomShape");
	if ( mxShape.is() )
	{
		// Add, set Style and properties from base shape
		SetStyle();
		SetLayer();

		// set pos, size, shear and rotate
		SetTransformation();

		try
		{
			uno::Reference< beans::XPropertySet > xPropSet( mxShape, uno::UNO_QUERY );
			if( xPropSet.is() )
			{
				if ( maCustomShapeEngine.getLength() )
				{
					uno::Any aAny;
					aAny <<= maCustomShapeEngine;
					xPropSet->setPropertyValue( EASGet( EAS_CustomShapeEngine ), aAny );
				}
				if ( maCustomShapeData.getLength() )
				{
					uno::Any aAny;
					aAny <<= maCustomShapeData;
					xPropSet->setPropertyValue( EASGet( EAS_CustomShapeData ), aAny );
				}
			}
		}
		catch( uno::Exception& )
		{
			DBG_ERROR( "could not set enhanced customshape geometry" );
		}
		SdXMLShapeContext::StartElement(xAttrList);
	}
}

void SdXMLCustomShapeContext::EndElement()
{
    // for backward compatibility, the above SetTransformation() may already have
    // applied a call to SetMirroredX/SetMirroredY. This is not yet added to the
    // beans::PropertyValues in maCustomShapeGeometry. When applying these now, this 
    // would be lost again.
    // TTTT: Remove again after aw080
    if(!maUsedTransformation.isIdentity())
    {
        basegfx::B2DVector aScale, aTranslate;
        double fRotate, fShearX;

        maUsedTransformation.decompose(aScale, aTranslate, fRotate, fShearX);

        bool bFlippedX(aScale.getX() < 0.0);
        bool bFlippedY(aScale.getY() < 0.0);

        if(bFlippedX && bFlippedY)
        {
            // when both are used it is the same as 180 degree rotation; reset
            bFlippedX = bFlippedY = false;
        }

        if(bFlippedX || bFlippedY)
        {
            beans::PropertyValue aNewPoroperty;

            if(bFlippedX)
            {
                aNewPoroperty.Name = rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("MirroredX"));
            }
            else
            {
                aNewPoroperty.Name = rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("MirroredY"));
            }

            aNewPoroperty.Handle = -1;
            aNewPoroperty.Value <<= sal_True;
            aNewPoroperty.State = beans::PropertyState_DIRECT_VALUE;

            maCustomShapeGeometry.push_back(aNewPoroperty);
        }
    }

	if ( !maCustomShapeGeometry.empty() )
	{
		const rtl::OUString	sCustomShapeGeometry	( RTL_CONSTASCII_USTRINGPARAM( "CustomShapeGeometry" ) );

		// converting the vector to a sequence
		uno::Sequence< beans::PropertyValue > aSeq( maCustomShapeGeometry.size() );
		beans::PropertyValue* pValues = aSeq.getArray();
		std::vector< beans::PropertyValue >::const_iterator aIter( maCustomShapeGeometry.begin() );
		std::vector< beans::PropertyValue >::const_iterator aEnd( maCustomShapeGeometry.end() );
		while ( aIter != aEnd )
			*pValues++ = *aIter++;

		try
		{
			uno::Reference< beans::XPropertySet > xPropSet( mxShape, uno::UNO_QUERY );
			if( xPropSet.is() )
			{
				uno::Any aAny;
				aAny <<= aSeq;
				xPropSet->setPropertyValue( sCustomShapeGeometry, aAny );
			}
		}
		catch( uno::Exception& )
		{
			DBG_ERROR( "could not set enhanced customshape geometry" );
		}

        sal_Int32 nUPD( 0 );
        sal_Int32 nBuild( 0 );
        GetImport().getBuildIds( nUPD, nBuild );
        if( ((nUPD >= 640 && nUPD <= 645) || (nUPD == 680)) && (nBuild <= 9221) )
		{
			Reference< drawing::XEnhancedCustomShapeDefaulter > xDefaulter( mxShape, UNO_QUERY );
			if( xDefaulter.is() )
			{
				rtl::OUString aEmptyType;
				xDefaulter->createCustomShapeDefaults( aEmptyType );
			}
		}
	}

	SdXMLShapeContext::EndElement();
}

//////////////////////////////////////////////////////////////////////////////

SvXMLImportContext* SdXMLCustomShapeContext::CreateChildContext(
	sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName,
	const uno::Reference<xml::sax::XAttributeList>& xAttrList )
{
	SvXMLImportContext* pContext = NULL;
	if ( XML_NAMESPACE_DRAW == nPrefix )
	{
		if ( IsXMLToken( rLocalName, XML_ENHANCED_GEOMETRY ) )
		{
			uno::Reference< beans::XPropertySet > xPropSet( mxShape,uno::UNO_QUERY );
			if ( xPropSet.is() )
				pContext = new XMLEnhancedCustomShapeContext( GetImport(), mxShape, nPrefix, rLocalName, maCustomShapeGeometry );
		}
	}
	// delegate to parent class if no context could be created
	if ( NULL == pContext )
		pContext = SdXMLShapeContext::CreateChildContext( nPrefix, rLocalName,
														 xAttrList);
	return pContext;
}

///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

TYPEINIT1( SdXMLTableShapeContext, SdXMLShapeContext );

SdXMLTableShapeContext::SdXMLTableShapeContext( SvXMLImport& rImport, sal_uInt16 nPrfx, const rtl::OUString& rLocalName, const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList>& xAttrList, com::sun::star::uno::Reference< com::sun::star::drawing::XShapes >& rShapes )
: SdXMLShapeContext( rImport, nPrfx, rLocalName, xAttrList, rShapes, sal_False )
{
	memset( &maTemplateStylesUsed, 0, sizeof( maTemplateStylesUsed ) );
}

SdXMLTableShapeContext::~SdXMLTableShapeContext()
{
}

void SdXMLTableShapeContext::StartElement( const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& xAttrList )
{
	const char* pService = "com.sun.star.drawing.TableShape";

	sal_Bool bIsPresShape = maPresentationClass.getLength() && GetImport().GetShapeImport()->IsPresentationShapesSupported();
	if( bIsPresShape )
	{
		if( IsXMLToken( maPresentationClass, XML_PRESENTATION_TABLE ) )
		{
			pService = "com.sun.star.presentation.TableShape";
		}
	}

	AddShape( pService );

	if( mxShape.is() )
	{
		SetLayer();

		uno::Reference< beans::XPropertySet > xProps(mxShape, uno::UNO_QUERY);

		if(bIsPresShape)
		{
			if(xProps.is())
			{
				uno::Reference< beans::XPropertySetInfo > xPropsInfo( xProps->getPropertySetInfo() );
				if( xPropsInfo.is() )
				{
					if( !mbIsPlaceholder && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsEmptyPresentationObject") ), ::cppu::bool2any( sal_False ) );

					if( mbIsUserTransformed && xPropsInfo->hasPropertyByName(OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") )))
						xProps->setPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("IsPlaceholderDependent") ), ::cppu::bool2any( sal_False ) );
				}
			}
		}

		SetStyle();

		if( xProps.is() )
		{
			if( msTemplateStyleName.getLength() ) try
			{
				Reference< XStyleFamiliesSupplier > xFamiliesSupp( GetImport().GetModel(), UNO_QUERY_THROW );
				Reference< XNameAccess > xFamilies( xFamiliesSupp->getStyleFamilies() );
				const OUString sFamilyName( RTL_CONSTASCII_USTRINGPARAM("table" ) );
				Reference< XNameAccess > xTableFamily( xFamilies->getByName( sFamilyName ), UNO_QUERY_THROW );
				Reference< XStyle > xTableStyle( xTableFamily->getByName( msTemplateStyleName ), UNO_QUERY_THROW );
				xProps->setPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "TableTemplate" ) ), Any( xTableStyle ) );
			}
			catch( Exception& )
			{
				DBG_ERROR("SdXMLTableShapeContext::StartElement(), exception caught!");
			}

			const XMLPropertyMapEntry* pEntry = &aXMLTableShapeAttributes[0];
			for( int i = 0; pEntry->msApiName && (i < 6); i++, pEntry++ )
			{
			    try
			    {
				    const OUString sAPIPropertyName( OUString(pEntry->msApiName, pEntry->nApiNameLength, RTL_TEXTENCODING_ASCII_US ) );
				    xProps->setPropertyValue( sAPIPropertyName, Any( maTemplateStylesUsed[i] ) );
			    }
			    catch( Exception& )
			    {
				    DBG_ERROR("SdXMLTableShapeContext::StartElement(), exception caught!");
			    }			
            }
		}

		GetImport().GetShapeImport()->finishShape( mxShape, mxAttrList, mxShapes );

		const rtl::Reference< XMLTableImport >& xTableImport( GetImport().GetShapeImport()->GetShapeTableImport() );
		if( xTableImport.is() && xProps.is() )
		{
			uno::Reference< table::XColumnRowRange > xColumnRowRange(
				xProps->getPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("Model") ) ), uno::UNO_QUERY );

			if( xColumnRowRange.is() )
				mxTableImportContext = xTableImport->CreateTableContext( GetPrefix(), GetLocalName(), xColumnRowRange );

			if( mxTableImportContext.Is() )
				mxTableImportContext->StartElement( xAttrList );
		}
	}
}

void SdXMLTableShapeContext::EndElement()
{
	if( mxTableImportContext.Is() )
		mxTableImportContext->EndElement();

	SdXMLShapeContext::EndElement();

	if( mxShape.is() )
	{
		// set pos, size, shear and rotate
		SetTransformation();
	}
}

// this is called from the parent group for each unparsed attribute in the attribute list
void SdXMLTableShapeContext::processAttribute( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const ::rtl::OUString& rValue )
{
	if( nPrefix == XML_NAMESPACE_TABLE )
	{
		if( IsXMLToken( rLocalName, XML_TEMPLATE_NAME ) )
		{
			msTemplateStyleName = rValue;
		}
		else
		{
			int i = 0;
			const XMLPropertyMapEntry* pEntry = &aXMLTableShapeAttributes[0];
			while( pEntry->msApiName && (i < 6) )
			{
				if( IsXMLToken( rLocalName, pEntry->meXMLName ) )
				{
					if( IsXMLToken( rValue, XML_TRUE ) )
						maTemplateStylesUsed[i] = sal_True;
					break;
				}
				pEntry++;
				i++;
			}
		}
	}
	SdXMLShapeContext::processAttribute( nPrefix, rLocalName, rValue );
}

SvXMLImportContext* SdXMLTableShapeContext::CreateChildContext( sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName, const uno::Reference<xml::sax::XAttributeList>& xAttrList )
{
	if( mxTableImportContext.Is() && (nPrefix == XML_NAMESPACE_TABLE) )
		return mxTableImportContext->CreateChildContext(nPrefix, rLocalName, xAttrList);
	else
		return SdXMLShapeContext::CreateChildContext(nPrefix, rLocalName, xAttrList);
}

