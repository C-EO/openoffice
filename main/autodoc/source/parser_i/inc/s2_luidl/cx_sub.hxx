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



#ifndef ADC_LUIDL_CX_SUB_HXX
#define ADC_LUIDL_CX_SUB_HXX

// USED SERVICES
	// BASE CLASSES
#include <tokens/tkpcont2.hxx>
	// COMPONENTS
	// PARAMETERS

#include "uidl_tok.hxx"

namespace csi
{
namespace uidl
{

class Token_Receiver;
class Token;


class Cx_Base : public ::TkpContext
{
  public:
	virtual bool		PassNewToken();
	virtual TkpContext &
						FollowUpContext();
  protected:
	// LIFECYCLE
						Cx_Base(
							Token_Receiver &	o_rReceiver,
							TkpContext &		i_rFollowUpContext )
												: 	rReceiver(o_rReceiver),
												    pFollowUpContext(&i_rFollowUpContext),
												    pNewToken()
													{}
  protected:
	void				SetToken(
							DYN Token *			let_dpToken )
												{ pNewToken = let_dpToken; }
    Token_Receiver &	Receiver()              { return rReceiver; }

  private:
	// DATA
	Token_Receiver &	rReceiver;
	TkpContext *		pFollowUpContext;
	Dyn<Token>			pNewToken;
};



/**
@descr
*/

class Context_MLComment : public Cx_Base
{
  public:
						Context_MLComment(
							Token_Receiver &	o_rReceiver,
							TkpContext &		i_rFollowUpContext )
												: 	Cx_Base(o_rReceiver, i_rFollowUpContext) {}
	virtual void		ReadCharChain(
							CharacterSource &	io_rText );
};

class Context_SLComment : public Cx_Base
{
  public:
						Context_SLComment(
							Token_Receiver &	o_rReceiver,
							TkpContext &		i_rFollowUpContext )
												: 	Cx_Base(o_rReceiver, i_rFollowUpContext) {}
	virtual void		ReadCharChain(
							CharacterSource &	io_rText );
};

class Context_Praeprocessor : public Cx_Base
{
  public:
						Context_Praeprocessor(
							Token_Receiver &	o_rReceiver,
							TkpContext &		i_rFollowUpContext )
												: 	Cx_Base(o_rReceiver, i_rFollowUpContext) {}
	virtual void		ReadCharChain(
							CharacterSource &	io_rText );
};

class Context_Assignment : public Cx_Base
{
  public:
						Context_Assignment(
							Token_Receiver &	o_rReceiver,
							TkpContext &		i_rFollowUpContext )
												: 	Cx_Base(o_rReceiver, i_rFollowUpContext) {}
	virtual void		ReadCharChain(
							CharacterSource &	io_rText );
};


}   // namespace uidl
}   // namespace csi

#endif
