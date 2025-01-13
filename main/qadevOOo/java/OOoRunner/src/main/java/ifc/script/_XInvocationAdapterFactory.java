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



package ifc.script;

import lib.MultiMethodTest;
import lib.StatusException;

import com.sun.star.io.XInputStream;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.lang.XSingleServiceFactory;
import com.sun.star.script.XInvocation;
import com.sun.star.script.XInvocationAdapterFactory;
import com.sun.star.uno.Type;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
* Testing <code>com.sun.star.script.XInvocationAdapterFactory</code>
* interface methods :
* <ul>
*  <li><code> createAdapter()</code></li>
* </ul> <p>
* Test is <b> NOT </b> multithread compilant. <p>
* @see com.sun.star.script.XInvocationAdapterFactory
*/
public class _XInvocationAdapterFactory extends MultiMethodTest {

    /**
     * oObj filled by MultiMethodTest
     */
    public XInvocationAdapterFactory oObj = null;

    /**
    * First an invocation object of <code>com.sun.star.io.Pipe</code>
    * instance is created using <code>com.sun.star.script.Invocation
    * </code> service. Then trying to create an adapter of this
    * invocation for <code>com.sun.star.io.XInputStream</code>
    * interface. <p>
    * Has <b>OK</b> status if the adapter returned is successfully
    * queried for <code>com.sun.star.io.XInputStream</code>
    * interface.
    * @see com.sun.star.script.Invocation
    * @see com.sun.star.script.XInvocation
    * @see com.sun.star.io.Pipe
    */
    public void _createAdapter() {
        XInvocation xInv = null ;
        XMultiServiceFactory xMSF = null;
        try {
            xMSF = (XMultiServiceFactory)tParam.getMSF();
            Object[] args = {xMSF.createInstance
                ("com.sun.star.io.Pipe")};

            Object oInvFac = xMSF.createInstance
                ("com.sun.star.script.Invocation") ;

            XSingleServiceFactory xInvFac = (XSingleServiceFactory) UnoRuntime.
                queryInterface(XSingleServiceFactory.class, oInvFac) ;

            Object oInv = xInvFac.createInstanceWithArguments(args) ;

            xInv = (XInvocation) UnoRuntime.queryInterface
                (XInvocation.class, oInv) ;

        } catch (com.sun.star.uno.Exception e) {
            e.printStackTrace(log) ;
            throw new StatusException("Can't create invocation for object", e) ;
        }

        XInterface xInStr = null ;

        Object adp = oObj.createAdapter(xInv,
                new Type(XInputStream.class)) ;

        xInStr = (XInterface) UnoRuntime.queryInterface
                (XInputStream.class, adp) ;


        if (xInStr != null)
            tRes.tested("createAdapter()", true) ;
        else {
            log.println("Adapter created doesn't implement required interface") ;
            tRes.tested("createAdapter()", false) ;
        }
    }
}

