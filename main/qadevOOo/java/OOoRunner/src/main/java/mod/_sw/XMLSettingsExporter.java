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



package mod._sw;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.SOfficeFactory;
import util.XMLTools;

import com.sun.star.beans.XPropertySet;
import com.sun.star.document.XExporter;
import com.sun.star.frame.XController;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XTextDocument;
import com.sun.star.uno.Any;
import com.sun.star.uno.Type;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;
import com.sun.star.view.XViewSettingsSupplier;
import com.sun.star.xml.sax.XDocumentHandler;

/**
 * Test for object which is represented by service
 * <code>com.sun.star.comp.Calc.XMLSettingsExporter</code>. <p>
 * Object implements the following interfaces :
 * <ul>
 *  <li><code>com::sun::star::lang::XInitialization</code></li>
 *  <li><code>com::sun::star::document::ExportFilter</code></li>
 *  <li><code>com::sun::star::document::XFilter</code></li>
 *  <li><code>com::sun::star::document::XExporter</code></li>
 *  <li><code>com::sun::star::beans::XPropertySet</code></li>
 * </ul>
 * @see com.sun.star.lang.XInitialization
 * @see com.sun.star.document.ExportFilter
 * @see com.sun.star.document.XFilter
 * @see com.sun.star.document.XExporter
 * @see com.sun.star.beans.XPropertySet
 * @see ifc.lang._XInitialization
 * @see ifc.document._ExportFilter
 * @see ifc.document._XFilter
 * @see ifc.document._XExporter
 * @see ifc.beans._XPropertySet
 */
public class XMLSettingsExporter extends TestCase {

    XTextDocument xTextDoc;
    SettingsFilterChecker Filter;

    /**
     * New text document created.
     */
    protected void initialize( TestParameters tParam, PrintWriter log ) {
        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)tParam.getMSF() );

        try {
            log.println( "creating a textdocument" );
            xTextDoc = SOF.createTextDoc( null );

        } catch ( com.sun.star.uno.Exception e ) {
            // Some exception occurred. FAILED
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }

    }

    /**
     * Document disposed here.
     */
    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println( "    disposing xTextDoc " );
        util.DesktopTools.closeDoc(xTextDoc);
    }

    /**
    * Creating a Testenvironment for the interfaces to be tested.
    * Creates an instance of the service
    * <code>com.sun.star.comp.Calc.XMLSettingsExporter</code> with
    * argument which is an implementation of <code>XDocumentHandler</code>
    * and which can check if required tags and character data is
    * exported. <p>
    * The text document is set as a source document for exporter
    * created. New document zoom is set as one of the 'View' settings. This made
    * for checking if this setting is successfully exported within
    * the document settings.
    *     Object relations created :
    * <ul>
    *  <li> <code>'MediaDescriptor'</code> for
    *      {@link ifc.document._XFilter} interface </li>
    *  <li> <code>'XFilter.Checker'</code> for
    *      {@link ifc.document._XFilter} interface </li>
    *  <li> <code>'SourceDocument'</code> for
    *      {@link ifc.document._XExporter} interface </li>
    * </ul>
    */
    public synchronized TestEnvironment createTestEnvironment
            (TestParameters tParam, PrintWriter log) {

        final short ZOOM = 50;

        XMultiServiceFactory xMSF = (XMultiServiceFactory)tParam.getMSF() ;
        XInterface oObj = null;

        Filter = new SettingsFilterChecker(log);
        Any arg = new Any(new Type(XDocumentHandler.class), Filter);
        try {
            oObj = (XInterface) xMSF.createInstanceWithArguments(
                "com.sun.star.comp.Writer.XMLSettingsExporter",
                new Object[] {arg});
            XExporter xEx = (XExporter) UnoRuntime.queryInterface
                (XExporter.class,oObj);
            xEx.setSourceDocument(xTextDoc);

            //set some settings
            XController xController = xTextDoc.getCurrentController();
            XViewSettingsSupplier xViewSetSup = (XViewSettingsSupplier)
                UnoRuntime.queryInterface(XViewSettingsSupplier.class,
                xController);
            XPropertySet xPropSet = xViewSetSup.getViewSettings();
            xPropSet.setPropertyValue("ZoomValue", new Short(ZOOM));

        } catch (com.sun.star.uno.Exception e) {
            e.printStackTrace(log) ;
            throw new StatusException("Can't create component.", e) ;
        }

        // adding tags which must be contained in XML output
        Filter.addTag( new XMLTools.Tag("office:document-settings") );
        Filter.addTagEnclosed(
            new XMLTools.Tag("office:settings"),
            new XMLTools.Tag("office:document-settings") );
        Filter.addCharactersEnclosed(
            String.valueOf(ZOOM),
            new XMLTools.Tag("config:config-item",
                "config:name", "ZoomFactor") );

        // create testobject here
        log.println( "creating a new environment" );
        TestEnvironment tEnv = new TestEnvironment( oObj );

        tEnv.addObjRelation("MediaDescriptor", XMLTools.createMediaDescriptor(
            new String[] {"FilterName"},
            new Object[] {"swriter: StarOffice XML (Writer)"}));
        tEnv.addObjRelation("SourceDocument",xTextDoc);
        tEnv.addObjRelation("XFilter.Checker", Filter);
        return tEnv;

    }

    /**
     * This class checks the XML for tags and data required and returns
     * checking result to <code>XFilter</code> interface test. All
     * the information about errors occurred in XML data is written
     * to log specified.
     * @see ifc.document._XFilter
     */
    protected class SettingsFilterChecker extends XMLTools.XMLChecker
      implements ifc.document._XFilter.FilterChecker {

        /**
         * Creates a class which will write information
         * into log specified.
         */
        public SettingsFilterChecker(PrintWriter log) {
            super(log, false) ;
        }

        /**
         * <code>_XFilter.FilterChecker</code> interface method
         * which returns the result of XML checking.
         * @return <code>true</code> if the XML data exported was
         * valid (i.e. all necessary tags and character data exists),
         * <code>false</code> if some errors occurred.
         */
        public boolean checkFilter() {
            return check();
        }
    }
}

