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



#ifndef _CONNECTIVITY_ADO_VIEW_HXX_
#define _CONNECTIVITY_ADO_VIEW_HXX_

#include "connectivity/sdbcx/VView.hxx"
#include "ado/Awrapadox.hxx"

namespace connectivity
{
	namespace ado
	{

		typedef sdbcx::OView OView_ADO;

		class OAdoView :	 public OView_ADO
		{
			WpADOView		m_aView;

		protected:
			// OPropertySetHelper
			virtual void SAL_CALL getFastPropertyValue(
                                ::com::sun::star::uno::Any& rValue,
									sal_Int32 nHandle
										 ) const;
		public:
			OAdoView(sal_Bool _bCase, ADOView* _pView=NULL);

			// com::sun::star::lang::XUnoTunnel
            virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& aIdentifier ) throw(::com::sun::star::uno::RuntimeException);
            static ::com::sun::star::uno::Sequence< sal_Int8 > getUnoTunnelImplementationId();
			virtual void SAL_CALL acquire() throw();
            virtual void SAL_CALL release() throw();

			WpADOView getImpl() const { return m_aView;}
		};
	}
}

#endif // _CONNECTIVITY_ADO_VIEW_HXX_
