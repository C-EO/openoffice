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


package mod._toolkit;

import com.sun.star.awt.Rectangle;
import com.sun.star.awt.XControl;
import com.sun.star.awt.XControlModel;
import com.sun.star.awt.XDevice;
import com.sun.star.awt.XGraphics;
import com.sun.star.awt.XToolkit;
import com.sun.star.awt.XWindow;
import com.sun.star.awt.XWindowPeer;
import com.sun.star.drawing.XControlShape;
import com.sun.star.drawing.XShape;
import com.sun.star.frame.XFrame;
import com.sun.star.frame.XModel;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XTextDocument;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;
import com.sun.star.view.XControlAccess;
import java.awt.Dimension;
import java.awt.Toolkit;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;

import util.FormTools;
import util.SOfficeFactory;
import util.WriterTools;
import util.utils;


public class UnoScrollBarControl extends TestCase {
    private static XTextDocument xTextDoc;

    protected void initialize(TestParameters Param, PrintWriter log) {
        SOfficeFactory SOF = SOfficeFactory.getFactory(
                                     (XMultiServiceFactory) Param.getMSF());

        try {
            log.println("creating a textdocument");
            xTextDoc = SOF.createTextDoc(null);
            
            log.println("maximize the window size");
            XModel xModel = (XModel) UnoRuntime.queryInterface(XModel.class, xTextDoc);
            XFrame xFrame = xModel.getCurrentController().getFrame();
            XWindow xWin = xFrame.getContainerWindow();
            
            Toolkit tk = Toolkit.getDefaultToolkit();
            Dimension dim = tk.getScreenSize();
            
            Rectangle newPosSize = xWin.getPosSize();
            newPosSize.Width = new Double(dim.getWidth()).intValue();
            newPosSize.Height = new Double(dim.getHeight()).intValue();
            newPosSize.X = 0;
            newPosSize.Y = 0;
            
            xWin.setPosSize(newPosSize.X, newPosSize.Y, newPosSize.Width,
                            newPosSize.Height, com.sun.star.awt.PosSize.POSSIZE);            
            
        } catch (com.sun.star.uno.Exception e) {
            // Some exception occurred. FAILED
            e.printStackTrace(log);
            throw new StatusException("Couldn't create document", e);
        }
    }

    protected void cleanup(TestParameters tParam, PrintWriter log) {
        log.println("    disposing xTextDoc ");

        util.DesktopTools.closeDoc(xTextDoc);
    }

    protected TestEnvironment createTestEnvironment(TestParameters Param, 
                                                    PrintWriter log) {
        XInterface oObj = null;
        XWindowPeer the_win = null;
        XToolkit the_kit = null;
        XDevice aDevice = null;
        XGraphics aGraphic = null;
        XControl aControl = null;

        //Insert a ControlShape and get the ControlModel
        XControlShape aShape = FormTools.createUnoControlShape(xTextDoc, 3000, 
                                                               4500, 15000, 
                                                               10000, 
                                                               "ScrollBar", 
                                                               "UnoControlScrollBar");

        WriterTools.getDrawPage(xTextDoc).add((XShape) aShape);

        XControlModel the_Model = aShape.getControl();

        XControlShape aShape2 = FormTools.createControlShape(xTextDoc, 3000, 
                                                             4500, 5000, 10000, 
                                                             "TextField");

        WriterTools.getDrawPage(xTextDoc).add((XShape) aShape2);

        XControlModel the_Model2 = aShape2.getControl();

        //Try to query XControlAccess
        XControlAccess the_access = (XControlAccess) UnoRuntime.queryInterface(
                                            XControlAccess.class, 
                                            xTextDoc.getCurrentController());

        //get the ScrollBarControl for the needed Object relations
        try {
            oObj = the_access.getControl(the_Model);
            aControl = the_access.getControl(the_Model2);
            the_win = the_access.getControl(the_Model).getPeer();
            the_kit = the_win.getToolkit();
            aDevice = the_kit.createScreenCompatibleDevice(200, 200);
            aGraphic = aDevice.createGraphics();
        } catch (Exception e) {
            log.println("Couldn't get ScrollBarControl");
            e.printStackTrace(log);
            throw new StatusException("Couldn't get ScrollBarControl", e);
        }

        log.println(
                "creating a new environment for UnoControlScrollBar object");

        TestEnvironment tEnv = new TestEnvironment(oObj);


        //adding Object-Relation for XScrollBar
        tEnv.addObjRelation("Document", xTextDoc);


        //Adding ObjRelation for XView
        tEnv.addObjRelation("GRAPHICS", aGraphic);


        //Adding ObjRelation for XControl
        tEnv.addObjRelation("CONTEXT", xTextDoc);
        tEnv.addObjRelation("WINPEER", the_win);
        tEnv.addObjRelation("TOOLKIT", the_kit);
        tEnv.addObjRelation("MODEL", the_Model);

        XWindow forObjRel = (XWindow) UnoRuntime.queryInterface(XWindow.class, 
                                                                aControl);

        tEnv.addObjRelation("XWindow.AnotherWindow", forObjRel);

        System.out.println("ImplementationName: " + utils.getImplName(oObj));

        return tEnv;
    } // finish method getTestEnvironment
}
