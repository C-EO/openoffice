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



#ifndef SD_FU_GRID_HXX
#define SD_FU_GRID_HXX

#include "fupoor.hxx"

namespace sd {

class FuGrid 
    : public FuPoor
{
public:
	TYPEINFO();

	static FunctionReference Create( ViewShell* pViewSh, ::sd::Window* pWin, ::sd::View* pView, SdDrawDocument* pDoc, SfxRequest& rReq );

	virtual void Activate();
	virtual void Deactivate();

private:
	FuGrid (
        ViewShell* pViewSh, 
        ::sd::Window* pWin, 
        ::sd::View* pView,
        SdDrawDocument* pDoc, 
        SfxRequest& rReq);
};

} // end of namespace sd

#endif
