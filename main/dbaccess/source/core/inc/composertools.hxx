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



#ifndef DBACCESS_SOURCE_CORE_INC_COMPOSERTOOLS_HXX
#define DBACCESS_SOURCE_CORE_INC_COMPOSERTOOLS_HXX

/** === begin UNO includes === **/
/** === end UNO includes === **/

#ifndef _RTL_USTRBUF_HXX_
#include <rtl/ustrbuf.hxx>
#endif

#include <functional>

//........................................................................
namespace dbaccess
{
//........................................................................

	//====================================================================
	//= TokenComposer
	//====================================================================
	struct TokenComposer : public ::std::unary_function< ::rtl::OUString, void >
	{
    private:
        #ifdef DBG_UTIL
        bool                    m_bUsed;
        #endif

    protected:
		::rtl::OUStringBuffer   m_aBuffer;

    public:
        ::rtl::OUString getComposedAndClear()
        {
            #ifdef DBG_UTIL
            m_bUsed = true;
            #endif
            return m_aBuffer.makeStringAndClear();
        }

        void clear()
        {
            #ifdef DBG_UTIL
            m_bUsed = false;
            #endif
            m_aBuffer.makeStringAndClear();
        }

    public:
        TokenComposer()
        #ifdef DBG_UTIL
            :m_bUsed( false )
        #endif
        {
        }

        virtual ~TokenComposer()
        {
        }

		void operator() (const ::rtl::OUString& lhs)
		{
			append(lhs);
		}

        void append( const ::rtl::OUString& lhs )
        {
            #ifdef DBG_UTIL
            OSL_ENSURE( !m_bUsed, "FilterCreator::append: already used up!" );
            #endif
            if ( lhs.getLength() )
            {
                if ( m_aBuffer.getLength() )
                    appendNonEmptyToNonEmpty( lhs );
                else
                    m_aBuffer.append( lhs );
            }
        }

        /// append the given part. Only to be called when both the part and our buffer so far are not empty
		virtual void appendNonEmptyToNonEmpty( const ::rtl::OUString& lhs ) = 0;
	};

	//====================================================================
	//= FilterCreator
	//====================================================================
    struct FilterCreator : public TokenComposer
    {
		virtual void appendNonEmptyToNonEmpty( const ::rtl::OUString& lhs )
		{
            m_aBuffer.insert( 0, (sal_Unicode)' ' );
            m_aBuffer.insert( 0, (sal_Unicode)'(' );
            m_aBuffer.appendAscii( " ) AND ( " );
            m_aBuffer.append( lhs );
            m_aBuffer.appendAscii( " )" );
		}
    };

	//====================================================================
	//= FilterCreator
	//====================================================================
	struct OrderCreator : public TokenComposer
	{
		virtual void appendNonEmptyToNonEmpty( const ::rtl::OUString& lhs )
		{
            m_aBuffer.appendAscii( ", " );
            m_aBuffer.append( lhs );
		}
	};

//........................................................................
} // namespace dbaccess
//........................................................................

#endif // DBACCESS_SOURCE_CORE_INC_COMPOSERTOOLS_HXX
