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



#ifndef _FOREIGN_WINDOW_SYSTEM_HXX
#define _FOREIGN_WINDOW_SYSTEM_HXX

#include <X11/Xlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Initialize our atoms and determine if the current window manager is
 * providing FWS extension support.
 */

Bool 
WMSupportsFWS (Display *display, int screen);

/* Send a client message to the FWS_COMM_WINDOW indicating the existence
 * of a new FWS client window.  Be careful to avoid BadWindow errors on
 * the XSendEvent in case the FWS_COMM_WINDOW root window property had
 * old/obsolete junk in it.
 */

Bool 
RegisterFwsWindow (Display *display, Window window);

/* Add the FWS protocol atoms to the WMProtocols property for the window.
 */

void 
AddFwsProtocols (Display *display, Window window);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _FOREIGN_WINDOW_SYSTEM_HXX
