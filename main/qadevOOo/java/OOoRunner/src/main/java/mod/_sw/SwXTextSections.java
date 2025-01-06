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

import com.sun.star.container.XNameAccess;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XText;
import com.sun.star.text.XTextContent;
import com.sun.star.text.XTextCursor;
import com.sun.star.text.XTextDocument;
import com.sun.star.text.XTextSectionsSupplier;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
 *
 * initial description
 * @see com.sun.star.text.XText
 *
 */
public class SwXTextSections extends TestCase {
    XTextDocument xTextDoc;

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

    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println( "    disposing xTextDoc " );
        util.DesktopTools.closeDoc(xTextDoc);
    }

    /**
     *    creating a Testenvironment for the interfaces to be tested
     */
    public synchronized TestEnvironment createTestEnvironment
        (TestParameters Param, PrintWriter log ) throws StatusException {

        XInterface oObj = null;
        XInterface oTS = null;
        XNameAccess oTSSuppName = null;

        XText oText = null;

        // creation of testobject here
        // first we write what we are intend to do to log file
        log.println( "creating a test environment" );


        oText = xTextDoc.getText();
        XTextCursor oCursor = oText.createTextCursor();


        log.println( "inserting TextSections" );

        XMultiServiceFactory oDocMSF = (XMultiServiceFactory)
            UnoRuntime.queryInterface(XMultiServiceFactory.class, xTextDoc);

        // First TextSection
        try {
            oTS = (XInterface) oDocMSF.createInstance
                ("com.sun.star.text.TextSection");
            XTextContent oTSC = (XTextContent)
                UnoRuntime.queryInterface(XTextContent.class, oTS);
            oText.insertTextContent(oCursor, oTSC, false);
        }
        catch(Exception e){
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }

        // Second TextSection
        try {
            oTS = (XInterface) oDocMSF.createInstance
                ("com.sun.star.text.TextSection");
            XTextContent oTSC = (XTextContent)
                UnoRuntime.queryInterface(XTextContent.class, oTS);
            oText.insertTextContent(oCursor, oTSC, false);
        }
        catch(Exception e){
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }


        log.println( "try to get a TextSection with the XTextSectionSupplier()" );

        try{
            XTextSectionsSupplier oTSSupp = (XTextSectionsSupplier)
                UnoRuntime.queryInterface( XTextSectionsSupplier.class,
                xTextDoc );
            oTSSuppName = oTSSupp.getTextSections();
        }
        catch(Exception e){
            System.out.println("Couldn't get Textsection " + e);
        }


        oObj = oTSSuppName;

        log.println( "creating a new environment for TextSections object" );
        TestEnvironment tEnv = new TestEnvironment( oObj );

        log.println( "adding TextDocument as mod relation to environment" );
        tEnv.addObjRelation("TEXTDOC", xTextDoc);

        return tEnv;
    } // finish method getTestEnvironment

}    // finish class SwXTextSection
