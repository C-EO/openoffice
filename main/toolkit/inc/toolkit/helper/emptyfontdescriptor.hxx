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



#ifndef _TOOLKIT_HELPER_EMPTYFONTDESCRIPTOR_HXX_
#define _TOOLKIT_HELPER_EMPTYFONTDESCRIPTOR_HXX_

#include <com/sun/star/awt/FontDescriptor.hpp>
#include <com/sun/star/awt/FontSlant.hpp>
#include <com/sun/star/awt/FontUnderline.hpp>
#include <com/sun/star/awt/FontStrikeout.hpp>

//	----------------------------------------------------
//	class EmptyFontDescriptor
//	----------------------------------------------------
class EmptyFontDescriptor : public ::com::sun::star::awt::FontDescriptor 
{
public:	
	EmptyFontDescriptor()
	{
		// Not all enums are initialized correctly in FontDescriptor-CTOR because 
		// they are set to the first enum value, this is not always the default value.
		Slant = ::com::sun::star::awt::FontSlant_DONTKNOW;
		Underline = ::com::sun::star::awt::FontUnderline::DONTKNOW;
		Strikeout = ::com::sun::star::awt::FontStrikeout::DONTKNOW;
	}
};




#endif // _TOOLKIT_HELPER_EMPTYFONTDESCRIPTOR_HXX_

