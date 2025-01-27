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



#ifndef _VCL_UNOHELP2_HXX
#define _VCL_UNOHELP2_HXX

#include <com/sun/star/datatransfer/XTransferable.hpp>
#include <cppuhelper/weak.hxx>
#include <tools/string.hxx>
#include <osl/mutex.hxx>

namespace com { namespace sun { namespace star { namespace datatransfer { namespace clipboard {
    class XClipboard;
} } } } }
#include <vcl/dllapi.h>

namespace vcl { namespace unohelper {

	class VCL_DLLPUBLIC TextDataObject :
                        	public ::com::sun::star::datatransfer::XTransferable,
							public ::cppu::OWeakObject
	{
	private:
		String			maText;

	public:	
						TextDataObject( const String& rText );
						~TextDataObject();

		String&			GetString()	{ return maText; }

		// ::com::sun::star::uno::XInterface
		::com::sun::star::uno::Any					SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException);
		void										SAL_CALL acquire() throw()	{ OWeakObject::acquire(); }
		void										SAL_CALL release() throw()	{ OWeakObject::release(); }

		// ::com::sun::star::datatransfer::XTransferable
		::com::sun::star::uno::Any SAL_CALL getTransferData( const ::com::sun::star::datatransfer::DataFlavor& aFlavor ) throw(::com::sun::star::datatransfer::UnsupportedFlavorException, ::com::sun::star::io::IOException, ::com::sun::star::uno::RuntimeException);
		::com::sun::star::uno::Sequence< ::com::sun::star::datatransfer::DataFlavor > SAL_CALL getTransferDataFlavors(  ) throw(::com::sun::star::uno::RuntimeException);
		sal_Bool SAL_CALL isDataFlavorSupported( const ::com::sun::star::datatransfer::DataFlavor& aFlavor ) throw(::com::sun::star::uno::RuntimeException);

        /// copies a given string to a given clipboard
        static  void    CopyStringTo(
            const String& rContent,
            const ::com::sun::star::uno::Reference< ::com::sun::star::datatransfer::clipboard::XClipboard >& rxClipboard
        );
	};

	struct MutexHelper
	{
	    private:
	        ::osl::Mutex maMutex;
	    public:
	        ::osl::Mutex& GetMutex() { return maMutex; }
	};

}}	// namespace vcl::unohelper

#endif  // _VCL_UNOHELP2_HXX
