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

package org.apache.openoffice.ooxml.schema.model.complex;

import org.apache.openoffice.ooxml.schema.model.base.INodeVisitor;
import org.apache.openoffice.ooxml.schema.model.base.Location;
import org.apache.openoffice.ooxml.schema.model.base.Node;
import org.apache.openoffice.ooxml.schema.model.base.NodeType;
import org.apache.openoffice.ooxml.schema.model.base.QualifiedName;

/** Representation of a 'sequence' XML schema element.
 *  It defines an order on its children.
 */
 public class Sequence
    extends Node
{
    public Sequence (
        final Node aParent,
        final QualifiedName aName,
        final Location aLocation)
    {
        super(aParent, aName, aLocation);
    }
    
    
    
    
    @Override
    public NodeType GetNodeType ()
    {
        return NodeType.Sequence;
    }




    @Override
    public void AcceptVisitor (final INodeVisitor aVisitor)
    {
        aVisitor.Visit(this);
    }
    
    
    
    
    @Override
    public String toString ()
    {
        if (GetName() != null)
            return "sequence "+GetName().GetDisplayName();
        else
            return "sequence";
    }
}
