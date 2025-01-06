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



package mod._fwk;

import java.io.PrintWriter;

import lib.Status;
import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.SOfficeFactory;

import com.sun.star.frame.XDesktop;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XTextDocument;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
* Test for object which is represented by service
* <code>com.sun.star.frame.Desktop</code>. <p>
* Object implements the following interfaces :
* <ul>
*  <li><code>com::sun::star::beans::XPropertySet</code></li>
*  <li><code>com::sun::star::frame::XComponentLoader</code></li>
*  <li><code>com::sun::star::frame::XDesktop</code></li>
*  <li><code>com::sun::star::frame::XDispatchProvider</code></li>
*  <li><code>com::sun::star::frame::XFrame</code></li>
*  <li><code>com::sun::star::frame::XFramesSupplier</code></li>
*  <li><code>com::sun::star::frame::XTasksSupplier</code></li>
*  <li><code>com::sun::star::lang::XComponent</code></li>
*  <li><code>com::sun::star::task::XStatusIndicatorFactory</code></li>
* </ul><p>
* @see com.sun.star.beans.XPropertySet
* @see com.sun.star.frame.XComponentLoader
* @see com.sun.star.frame.XDesktop
* @see com.sun.star.frame.XDispatchProvider
* @see com.sun.star.frame.XFrame
* @see com.sun.star.frame.XFramesSupplier
* @see com.sun.star.frame.XTasksSupplier
* @see com.sun.star.lang.XComponent
* @see com.sun.star.task.XStatusIndicatorFactory
* @see ifc.beans._XPropertySet
* @see ifc.frame._XComponentLoader
* @see ifc.frame._XDesktop
* @see ifc.frame._XDispatchProvider
* @see ifc.frame._XFrame
* @see ifc.frame._XFramesSupplier
* @see ifc.frame._XTasksSupplier
* @see ifc.lang._XComponent
* @see ifc.task._XStatusIndicatorFactory
*/
public class Desktop extends TestCase {

    XTextDocument xTextDoc;

    /**
     * Disposes the document, if exists, created in
     * <code>createTestEnvironment</code> method.
     */
    protected void cleanup( TestParameters Param, PrintWriter log) {

        log.println("disposing xTextDoc");

        if (xTextDoc != null) {
            try {
                xTextDoc.dispose();
            } catch (com.sun.star.lang.DisposedException de) {}
        }
    }

    /**
    * Creating a Testenvironment for the interfaces to be tested.
    * Creates service <code>com.sun.star.frame.Desktop</code>.
    */
    protected TestEnvironment createTestEnvironment(TestParameters Param, PrintWriter log) {

        // get a soffice factory object
        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)Param.getMSF());

        try {
            log.println( "creating a text document" );
            xTextDoc = SOF.createTextDoc(null);
        } catch ( com.sun.star.uno.Exception e ) {
            // Some exception occurred. FAILED
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }

        XInterface oObj = null;

        try {
            oObj = (XInterface)((XMultiServiceFactory)Param.getMSF()).createInstance(
                "com.sun.star.comp.framework.Desktop");
        } catch(com.sun.star.uno.Exception e) {
            e.printStackTrace(log);
            throw new StatusException(
                Status.failed("Couldn't create instance"));
        }

        TestEnvironment tEnv = new TestEnvironment( oObj );

        tEnv.addObjRelation("XDispatchProvider.URL", ".uno:Open");

        tEnv.addObjRelation("Desktop",(XDesktop)
                                UnoRuntime.queryInterface(XDesktop.class,oObj));

        return tEnv;
    } // finish method getTestEnvironment

}
