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



#ifndef INCLUDED_PDFI_CONTENTSINK_HXX
#define INCLUDED_PDFI_CONTENTSINK_HXX

#include <com/sun/star/uno/Reference.hxx>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/rendering/ARGBColor.hpp>
#include <boost/shared_ptr.hpp>

namespace rtl { class OUString; }
namespace com { namespace sun { namespace star { 
namespace rendering
{
    class  XPolyPolygon2D;
}
namespace geometry
{
    struct Matrix2D;
    struct AffineMatrix2D;
    struct RealRectangle2D;
    struct RealPoint2D;
    struct RealSize2D;
} 
namespace beans
{
    struct PropertyValue;
} } } }

namespace pdfi
{
    struct FontAttributes
    {
        FontAttributes( const rtl::OUString& familyName_,
                        bool                 isBold_,
                        bool                 isItalic_,
                        bool                 isUnderline_,
                        bool                 isOutline_,
                        double               size_ ) :
            familyName(familyName_),
            isBold(isBold_),
            isItalic(isItalic_),
            isUnderline(isUnderline_),
            isOutline(isOutline_),
            size(size_)
        {}

        FontAttributes() :
            familyName(),
            isBold(false),
            isItalic(false),
            isUnderline(false),
            isOutline(false),
            size(0.0)
        {}

        ::rtl::OUString     familyName;
        bool                isBold;
        bool                isItalic;
        bool                isUnderline;
        bool                isOutline;
        double              size; // device pixel
            
        bool operator==(const FontAttributes& rFont) const
        {
            return familyName == rFont.familyName &&
                !isBold == !rFont.isBold &&
                !isItalic == !rFont.isItalic &&
                !isUnderline == !rFont.isUnderline &&
                !isOutline == !rFont.isOutline &&
                size == rFont.size;
        }
    };

    /** (preliminary) API wrapper around xpdf

        Wraps the functionality currently used from xpdf's OutputDev
        interface. Subject to change.
     */
    struct ContentSink
    {
        virtual ~ContentSink() {}
        
        /// Total number of pages for upcoming document
        virtual void setPageNum( sal_Int32 nNumPages ) = 0;
        virtual void startPage( const ::com::sun::star::geometry::RealSize2D& rSize ) = 0;
        virtual void endPage() = 0;
        
        virtual void hyperLink( const ::com::sun::star::geometry::RealRectangle2D& rBounds,
                                const ::rtl::OUString&                             rURI ) = 0;
        
        virtual void pushState() = 0;
        virtual void popState() = 0;
        
        virtual void setFlatness( double ) = 0;
        virtual void setTransformation( const ::com::sun::star::geometry::AffineMatrix2D& rMatrix ) = 0;
        virtual void setLineDash( const ::com::sun::star::uno::Sequence<double>& dashes,
                                  double                                         start ) = 0;
        virtual void setLineJoin( sal_Int8 lineJoin ) = 0;
        virtual void setLineCap( sal_Int8 lineCap ) = 0;
        virtual void setMiterLimit(double) = 0;
        virtual void setLineWidth(double) = 0;
        virtual void setFillColor( const ::com::sun::star::rendering::ARGBColor& rColor ) = 0;
        virtual void setStrokeColor( const ::com::sun::star::rendering::ARGBColor& rColor ) = 0;
        virtual void setBlendMode( sal_Int8 blendMode ) = 0;
        virtual void setFont( const FontAttributes& rFont ) = 0;
        virtual void setTextRenderMode( sal_Int32 ) = 0;
        

        virtual void strokePath( const ::com::sun::star::uno::Reference< 
                                       ::com::sun::star::rendering::XPolyPolygon2D >& rPath ) = 0;
        virtual void fillPath( const ::com::sun::star::uno::Reference< 
                                     ::com::sun::star::rendering::XPolyPolygon2D >& rPath ) = 0;
        virtual void eoFillPath( const ::com::sun::star::uno::Reference< 
                                       ::com::sun::star::rendering::XPolyPolygon2D >& rPath ) = 0;
        
        virtual void intersectClip(const ::com::sun::star::uno::Reference< 
                                         ::com::sun::star::rendering::XPolyPolygon2D >& rPath) = 0;
        virtual void intersectEoClip(const ::com::sun::star::uno::Reference< 
                                           ::com::sun::star::rendering::XPolyPolygon2D >& rPath) = 0;
        
        virtual void drawGlyphs( const rtl::OUString&                               rGlyphs,
                                 const ::com::sun::star::geometry::RealRectangle2D& rRect,
                                 const ::com::sun::star::geometry::Matrix2D&        rFontMatrix ) = 0;

        /// issued when a sequence of associated glyphs is drawn
        virtual void endText() = 0;
        
        /// draws given bitmap as a mask (using current fill color)
        virtual void drawMask(const ::com::sun::star::uno::Sequence<
                                    ::com::sun::star::beans::PropertyValue>& xBitmap,
                              bool                                           bInvert ) = 0;
        /// Given image must already be color-mapped and normalized to sRGB.
        virtual void drawImage(const ::com::sun::star::uno::Sequence<
                                     ::com::sun::star::beans::PropertyValue>& xBitmap ) = 0;
        /** Given image must already be color-mapped and normalized to sRGB. 

            maskColors must contain two sequences of color components
         */
        virtual void drawColorMaskedImage(const ::com::sun::star::uno::Sequence<
                                                ::com::sun::star::beans::PropertyValue>& xBitmap,
                                          const ::com::sun::star::uno::Sequence< 
                                                ::com::sun::star::uno::Any>&             xMaskColors ) = 0;
        virtual void drawMaskedImage(const ::com::sun::star::uno::Sequence<
                                           ::com::sun::star::beans::PropertyValue>& xBitmap,
                                     const ::com::sun::star::uno::Sequence<
                                           ::com::sun::star::beans::PropertyValue>& xMask,
                                     bool                                             bInvertMask) = 0;
        virtual void drawAlphaMaskedImage(const ::com::sun::star::uno::Sequence<
                                                ::com::sun::star::beans::PropertyValue>& xImage,
                                          const ::com::sun::star::uno::Sequence<
                                                ::com::sun::star::beans::PropertyValue>& xMask) = 0;
    };

    typedef boost::shared_ptr<ContentSink> ContentSinkSharedPtr;
}

#endif /* INCLUDED_PDFI_CONTENTSINK_HXX */
