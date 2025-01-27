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



#ifndef _SDR_CONTACT_VIEWCONTACTOFSDROBJ_HXX
#define _SDR_CONTACT_VIEWCONTACTOFSDROBJ_HXX

#include <sal/types.h>
#include <svx/sdr/contact/viewcontact.hxx>
#include <svx/sdtakitm.hxx>
#include "svx/svxdllapi.h"

//////////////////////////////////////////////////////////////////////////////
// predeclarations

class SdrObject;
class GeoStat;
class Bitmap;

//////////////////////////////////////////////////////////////////////////////

namespace sdr
{
	namespace contact
	{
		class SVX_DLLPUBLIC ViewContactOfSdrObj : public ViewContact
		{
		protected:
			// the owner of this ViewContact. Set from constructor and not
			// to be changed in any way.
			SdrObject&										mrObject;

			// Remember AnimationKind of object. Used to find out if that kind
			// has changed in ActionChanged()
			SdrTextAniKind									meRememberedAnimationKind;

			// Create a Object-Specific ViewObjectContact, set ViewContact and
			// ObjectContact. Always needs to return something.
			virtual ViewObjectContact& CreateObjectSpecificViewObjectContact(ObjectContact& rObjectContact);

		public:
			// access to SdrObject
			SdrObject& GetSdrObject() const
			{
				return mrObject;
			}

			// basic constructor, used from SdrObject.
			ViewContactOfSdrObj(SdrObject& rObj);
			virtual ~ViewContactOfSdrObj();

			// Access to possible sub-hierarchy
			virtual sal_uInt32 GetObjectCount() const;
			virtual ViewContact& GetViewContact(sal_uInt32 nIndex) const;
			virtual ViewContact* GetParentContact() const;

			// React on changes of the object of this ViewContact
			virtual void ActionChanged();

			// overload for accessing the SdrObject
			virtual SdrObject* TryToGetSdrObject() const;

			//////////////////////////////////////////////////////////////////////////////
			// primitive stuff

			// add Gluepoints (if available)
			virtual drawinglayer::primitive2d::Primitive2DSequence createGluePointPrimitive2DSequence() const;
            
            // allow embedding if needed (e.g. for SdrObjects, evtl. Name, Title and description get added). This
            // is a helper normally used from getViewIndependentPrimitive2DSequence(), but there is one exception
            // for 3D scenes
            virtual drawinglayer::primitive2d::Primitive2DSequence embedToObjectSpecificInformation(const drawinglayer::primitive2d::Primitive2DSequence& rSource) const;
		};
	} // end of namespace contact
} // end of namespace sdr

//////////////////////////////////////////////////////////////////////////////

#endif //_SDR_CONTACT_VIEWCONTACTOFSDROBJ_HXX

// eof
