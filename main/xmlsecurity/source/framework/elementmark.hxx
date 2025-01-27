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



#ifndef _ELEMENTMARK_HXX
#define _ELEMENTMARK_HXX

#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/xml/crypto/sax/ElementMarkType.hpp>

class BufferNode;

class ElementMark
/****** elementmark.hxx/CLASS ElementMark *************************************
 *
 *   NAME
 *	ElementMark -- Class to manipulate an element mark
 *
 *   FUNCTION
 *	This class maintains the security id, buffer id and its type for a
 *	buffer node.
 *
 *   HISTORY
 *	05.01.2004 -	implemented
 *
 *   AUTHOR
 *	Michael Mi
 *	Email: michael.mi@sun.com
 ******************************************************************************/
{
protected:
	/* the BufferNode maintained by this object */
	BufferNode* m_pBufferNode;
	
	/* the security Id */
	sal_Int32 m_nSecurityId;
	
	/* the buffer Id */
	sal_Int32 m_nBufferId;
	
	/*
	 * the type value, is one of following values:
	 * TYPEOFELEMENTMARK - the default value, represents an blocker if 
	 *                     not changed
	 * TYPEOFELEMENTCOLLECTOR - represents an ElementCollector
	 */
	com::sun::star::xml::crypto::sax::ElementMarkType m_type;
	
public:
	ElementMark(sal_Int32 nSecurityId, sal_Int32 nBufferId);
	virtual ~ElementMark() {};

	BufferNode* getBufferNode() const;
	void setBufferNode(const BufferNode* pBufferNode);
	sal_Int32 getSecurityId() const;
	void setSecurityId(sal_Int32 nSecurityId);
	com::sun::star::xml::crypto::sax::ElementMarkType getType() const;
	sal_Int32 getBufferId() const;
};

#endif
