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


#ifndef DBAUI_DRIVERSETTINGS_HXX
#define DBAUI_DRIVERSETTINGS_HXX

#ifndef _DBAUI_DSNTYPES_HXX_
#include "dsntypes.hxx"
#endif
#include <svl/poolitem.hxx>
#include <vector>

class SfxTabPage;
class Window;
namespace dbaui
{
	/// a collection class for all details a driver needs
	class ODriversSettings
	{
	public:

		/** filles the IDs of the settings which are reflected in indirect data source properties
            (aka properties in the css.sdb.DataSource.Info sequence)

            @param	_eType
				The Type of the data source.
			@param	_out_rDetailsIds
				Will be filled.
		*/
		static void getSupportedIndirectSettings( const ::rtl::OUString& _sURLPrefix,const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory >& _xFactory,::std::vector< sal_Int32>& _out_rDetailsIds );

		/** Creates the detail page for Dbase
		*/
		static	SfxTabPage*	CreateDbase2( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for ado
		*/
        static	SfxTabPage*	CreateDbase( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for ado
		*/
		static	SfxTabPage*	CreateAdo( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for ODBC
		*/
		static	SfxTabPage*	CreateODBC( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for user
		*/
		static	SfxTabPage*	CreateUser( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for MySQLODBC
		*/
		static	SfxTabPage*	CreateMySQLODBC( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for MySQLJDBC
		*/
		static	SfxTabPage*	CreateMySQLJDBC( Window* _pParent, const SfxItemSet& _rAttrSet );

        /** Creates the detail page for MySQLNATIVE
		*/
		static	SfxTabPage*	CreateMySQLNATIVE( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for Oracle JDBC
		*/
		static SfxTabPage*	CreateOracleJDBC( Window* pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for Adabas
		*/
		static	SfxTabPage*	CreateAdabas( Window* _pParent, const SfxItemSet& _rAttrSet );

		/** Creates the detail page for LDAP
		*/
		static	SfxTabPage*	CreateLDAP( Window* _pParent, const SfxItemSet& _rAttrSet );

		/// Creates the detail page for Text
		static	SfxTabPage*	CreateText( Window* _pParent, const SfxItemSet& _rAttrSet );


		/// creates the GeneratedValues page
		static	SfxTabPage*	CreateGeneratedValuesPage( Window* _pParent, const SfxItemSet& _rAttrSet );

		/// creates the "Special Settings" page of the "Advanced Settings" dialog
		static	SfxTabPage*	CreateSpecialSettingsPage( Window* _pParent, const SfxItemSet& _rAttrSet );
	};
}

#endif // DBAUI_DRIVERSETTINGS_HXX
