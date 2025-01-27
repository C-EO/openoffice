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



#ifndef SC_XEPAGE_HXX
#define SC_XEPAGE_HXX

#include "xerecord.hxx"
#include "xlpage.hxx"
#include "xeroot.hxx"

// Page settings records ======================================================

// Header/footer --------------------------------------------------------------

/** Represents a HEADER or FOOTER record. */
class XclExpHeaderFooter : public XclExpRecord
{
public:
    explicit            XclExpHeaderFooter( sal_uInt16 nRecId, const String& rHdrString );

    virtual void        SaveXml( XclExpXmlStream& rStrm );
private:
    /** Writes the header or footer string. Writes an empty record, if no header/footer present. */
    virtual void        WriteBody( XclExpStream& rStrm );

private:
    String              maHdrString;        /// Header or footer contents.
};

// General page settings ------------------------------------------------------

/** Represents a SETUP record that contains common page settings. */
class XclExpSetup : public XclExpRecord
{
public:
    explicit            XclExpSetup( const XclPageData& rPageData );

    virtual void        SaveXml( XclExpXmlStream& rStrm );
private:
    /** Writes the contents of the SETUP record. */
    virtual void        WriteBody( XclExpStream& rStrm );

private:
    const XclPageData&  mrData;             /// Page settings data of current sheet.
};

// Manual page breaks ---------------------------------------------------------

/** Stores an array of manual page breaks for columns or rows. */
class XclExpPageBreaks : public XclExpRecord
{
public:
    explicit            XclExpPageBreaks(
                            sal_uInt16 nRecId,
                            const ScfUInt16Vec& rPageBreaks,
                            sal_uInt16 nMaxPos );

    /** Writes the record, if the list is not empty. */
    virtual void        Save( XclExpStream& rStrm );
    virtual void        SaveXml( XclExpXmlStream& rStrm );

private:
    /** Writes the page break list. */
    virtual void        WriteBody( XclExpStream& rStrm );

private:
    const ScfUInt16Vec& mrPageBreaks;       /// Page settings data of current sheet.
    sal_uInt16          mnMaxPos;           /// Maximum row/column for BIFF8 page breaks.
};

// Page settings ==============================================================

/** Contains all page (print) settings records for a single sheet. */
class XclExpPageSettings : public XclExpRecordBase, protected XclExpRoot
{
public:
    /** Creates all records containing the current page settings. */
    explicit            XclExpPageSettings( const XclExpRoot& rRoot );

    /** Returns read-only access to the page data. */
    inline const XclPageData& GetPageData() const { return maData; }

    /** Writes all page settings records to the stream. */
    virtual void        Save( XclExpStream& rStrm );
    virtual void        SaveXml( XclExpXmlStream& rStrm );

private:
    XclPageData         maData;         /// Page settings data.
};

// ----------------------------------------------------------------------------

/** Contains all page (print) settings records for a chart object. */
class XclExpChartPageSettings : public XclExpRecordBase, protected XclExpRoot
{
public:
    /** Creates all records containing the current page settings. */
    explicit            XclExpChartPageSettings( const XclExpRoot& rRoot );

    /** Returns read-only access to the page data. */
    inline const XclPageData& GetPageData() const { return maData; }

    /** Writes all page settings records to the stream. */
    virtual void        Save( XclExpStream& rStrm );

private:
    XclPageData         maData;         /// Page settings data.
};

// ============================================================================

#endif
