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



#ifndef _EMBEDTRANSFER_HXX
#define _EMBEDTRANSFER_HXX

#include "svtools/svtdllapi.h"
#include <com/sun/star/embed/XEmbeddedObject.hpp>
#include <svtools/transfer.hxx>

class Graphic;
class SVT_DLLPUBLIC SvEmbedTransferHelper : public TransferableHelper
{
private:

	::com::sun::star::uno::Reference< ::com::sun::star::embed::XEmbeddedObject > m_xObj;
	Graphic* m_pGraphic;
	sal_Int64 m_nAspect;

protected:

	virtual void		AddSupportedFormats();
	virtual sal_Bool	GetData( const ::com::sun::star::datatransfer::DataFlavor& rFlavor );
	virtual void		ObjectReleased();

public:
	// object, replacement image, and the aspect
    SvEmbedTransferHelper( const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XEmbeddedObject >& xObj,
							Graphic* pGraphic,
							sal_Int64 nAspect );
    ~SvEmbedTransferHelper();

    static void         FillTransferableObjectDescriptor( TransferableObjectDescriptor& rDesc,
                            const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XEmbeddedObject >& xObj,
							Graphic* pGraphic,
							sal_Int64 nAspect );
};

#endif
