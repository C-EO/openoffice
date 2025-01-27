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


#ifndef _IDLC_ASTENUM_HXX_
#define _IDLC_ASTENUM_HXX_

#include <idlc/asttype.hxx>
#include <idlc/astscope.hxx>
#include <idlc/astconstant.hxx>

class AstEnum : public AstType
			  , public AstScope
{
public:
	AstEnum(const ::rtl::OString& name, AstScope* pScope);

	virtual ~AstEnum();

	void setEnumValueCount(sal_Int32 count)
		{ m_enumValueCount = count; }
	sal_Int32 getEnumValueCount()
		{ return m_enumValueCount++; }

	AstConstant* checkValue(AstExpression* pExpr);

	virtual sal_Bool dump(RegistryKey& rKey);

	virtual AstDeclaration* addDeclaration(AstDeclaration* pDecl);
private:
	sal_Int32 	m_enumValueCount;
};

#endif // _IDLC_ASTENUM_HXX_
