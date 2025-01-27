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



#ifndef INCLUDED_PDFI_XMLEMITTER_HXX
#define INCLUDED_PDFI_XMLEMITTER_HXX

#include "pdfihelper.hxx"
#include <boost/shared_ptr.hpp>

namespace pdfi
{
    /** Output interface to ODF

        Should be easy to implement using either SAX events or plain ODF
     */
    class XmlEmitter
    {
    public:
        virtual ~XmlEmitter() {}
        
        /** Open up a tag with the given properties
         */
        virtual void beginTag( const char* pTag, const PropertyMap& rProperties ) = 0;
        /** Write PCTEXT as-is to output
         */
        virtual void write( const rtl::OUString& rString ) = 0;
        /** Close previously opened tag
         */
        virtual void endTag( const char* pTag ) = 0;
    };

    typedef boost::shared_ptr<XmlEmitter> XmlEmitterSharedPtr;
}

#endif
