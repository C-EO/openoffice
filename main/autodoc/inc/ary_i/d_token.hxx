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



#ifndef CSI_DSAPI_D_TOKEN_HXX
#define CSI_DSAPI_D_TOKEN_HXX

// BASE CLASSES
#include <ary_i/ci_text2.hxx>
#include <ary_i/ci_atag2.hxx>


namespace ary
{
namespace inf
{
    class DocumentationDisplay;
}
}



namespace csi
{
namespace dsapi
{

using ary::inf::DocumentationDisplay;


class DT_Dsapi : public ary::inf::DocuToken
{
  public:
	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const  = 0;
	virtual bool		IsWhiteOnly() const;
};



class DT_TextToken : public DT_Dsapi
{
  public:
	explicit		    DT_TextToken(
							const char *		i_sText )
												:	sText(i_sText) {}
	explicit		    DT_TextToken(
							const String &  	i_sText )
												:	sText(i_sText) {}
    virtual             ~DT_TextToken();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const char *		GetText() const			{ return sText; }
	const String &      GetTextStr() const		{ return sText; }

	String &            Access_Text()   		{ return sText; }

	virtual bool		IsWhiteOnly() const;

  private:
	String				sText;
};

class DT_White : public DT_Dsapi
{
  public:
                        DT_White() {}
    virtual             ~DT_White();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	virtual bool		IsWhiteOnly() const;
};


class DT_MLTag : public DT_Dsapi
{
  public:
	enum E_Kind
	{
		k_unknown = 0,
		k_begin,
		k_end,
		k_single
	};
};

class DT_MupType : public DT_MLTag
{
  public:
	explicit		    DT_MupType(             /// Constructor for End-Tag
							bool				)	/// Must be there, but is not evaluated.
                                                :	bIsBegin(false) {}
	explicit		    DT_MupType(             /// Constructor for Begin-Tag
							const String &		i_sScope )
												:	sScope(i_sScope), bIsBegin(true) {}
    virtual             ~DT_MupType();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const String  &		Scope() const			{ return sScope; }
	bool				IsBegin() const			{ return bIsBegin; }

  private:
	String 				sScope;
	bool				bIsBegin;
};

class DT_MupMember : public DT_MLTag
{
  public:
	explicit		    DT_MupMember(           /// Constructor for End-Tag
							bool				)	/// Must be there, but is not evaluated.
												:	bIsBegin(false) {}
						DT_MupMember(           /// Constructor for Begin-Tag
							const String &      i_sScope )
												:	sScope(i_sScope), bIsBegin(true) {}
    virtual             ~DT_MupMember();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const String  &		Scope() const			{ return sScope; }
	bool				IsBegin() const			{ return bIsBegin; }

  private:
	String 				sScope;
	bool				bIsBegin;
};

class DT_MupConst : public DT_Dsapi
{
  public:
						DT_MupConst(
							const char *		i_sConstText )
												:	sConstText(i_sConstText) {}
	virtual             ~DT_MupConst();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const char *		GetText() const			{ return sConstText; }

  private:
	String 				sConstText;				/// Without HTML.
};


class DT_Style : public DT_MLTag
{
  public:
						DT_Style(
							const char *		i_sPlainHtmlTag,
							bool                i_bNewLine )
												: sText(i_sPlainHtmlTag), bNewLine(i_bNewLine) {}
	virtual             ~DT_Style();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const char *		GetText() const			{ return sText; }
	bool				IsStartOfNewLine() const
												{ return bNewLine; }
  private:
	String 				sText;					/// With HTML.
	E_Kind              eKind;
	bool				bNewLine;
};

class DT_EOL : public DT_Dsapi
{
  public:
                        DT_EOL() {}
    virtual             ~DT_EOL();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	virtual bool		IsWhiteOnly() const;
};


class DT_AtTag : public ary::inf::AtTag2
{
  public:
	void            	AddToken(
							DYN ary::inf::DocuToken &
												let_drToken )
												{	aText.AddToken(let_drToken); }
	void				SetName(
							const char *		i_sName )
												{ sTitle = i_sName; }

  protected:
						DT_AtTag(
							const char *		i_sTitle )
												:	ary::inf::AtTag2(i_sTitle) {}
};

class DT_StdAtTag : public DT_AtTag
{
  public:
	explicit			DT_StdAtTag(
							const char *		i_sTitle )
												:	DT_AtTag(i_sTitle) {}
	virtual             ~DT_StdAtTag();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
};

class DT_SeeAlsoAtTag : public DT_AtTag
{
  public:
						DT_SeeAlsoAtTag()		:	DT_AtTag("") {}
    virtual             ~DT_SeeAlsoAtTag();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
	const String  &		LinkText() const		{ return sTitle; }	// Missbrauch von sTitle
};

class DT_ParameterAtTag : public DT_AtTag
{
  public:
						DT_ParameterAtTag()		:	DT_AtTag("") {}
    virtual             ~DT_ParameterAtTag();

	void				SetTitle(
		                	const char *		i_sTitle );
	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
};

class DT_SinceAtTag : public DT_AtTag
{
  public:
						DT_SinceAtTag()		:	DT_AtTag("Since version") {}
    virtual             ~DT_SinceAtTag();

	virtual void		DisplayAt(
							DocumentationDisplay &
												o_rDisplay ) const;
};


}   // namespace dsapi
}   // namespace csi

#endif
