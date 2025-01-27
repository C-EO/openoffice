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



#ifndef DSAPI_TK_ATAG2_HXX
#define DSAPI_TK_ATAG2_HXX

// USED SERVICES
	// BASE CLASSES
#include <s2_dsapi/dsapitok.hxx>
	// COMPONENTS
	// PARAMETERS
#include <luxenum.hxx>

namespace csi
{
namespace dsapi
{


class Tok_AtTag : public Token
{
  public:
	// TYPE
	enum E_TokenId
	{
		e_none = 0,
		author =  1,
		see = 2,
		param = 3,
		e_return = 4,
		e_throw = 5,
		example = 6,
		deprecated = 7,
		suspicious = 8,
		missing = 9,
		incomplete = 10,
		version = 11,
		guarantees = 12,
		exception = 13,
		since = 14
	};
	typedef lux::Enum<E_TokenId> EV_TokenId;

	// Spring and Fall
						Tok_AtTag(
							EV_TokenId			i_eTag )
												:	eTag(i_eTag) {}
	// OPERATIONS
	virtual void		Trigger(
							TokenInterpreter &	io_rInterpreter ) const;
	// INQUIRY
	virtual const char*	Text() const;
	E_TokenId			Id() const				{ return eTag; }

  private:
	EV_TokenId			eTag;
};

}   // namespace dsapi
}   // namespace csi


#endif
