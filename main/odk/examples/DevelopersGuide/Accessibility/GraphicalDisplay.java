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



import javax.swing.*;
import java.awt.Dimension;
import java.awt.*;
import java.awt.geom.*;

import com.sun.star.accessibility.XAccessible;
import com.sun.star.accessibility.XAccessibleContext;
import com.sun.star.accessibility.XAccessibleComponent;

import com.sun.star.awt.Point;
import com.sun.star.awt.Size;
import com.sun.star.uno.UnoRuntime;


/** Display the currently focused accessible object graphically.
*/
public class GraphicalDisplay
    extends JPanel
    implements IAccessibleObjectDisplay
{
    /** Create a new graphical widget that displays some of the geometrical
        information available from accessible objects.
    */
    public GraphicalDisplay ()
    {
        setPreferredSize (new Dimension (300,200));
    }


    /** Paint some or all of the area of this widget with the outlines of
        the currently focused object and its ancestors.
     */
    public synchronized void paintComponent (Graphics g)
    {
        super.paintComponent (g);

        setupTransformation ();

        // Draw the screen representation to give a hint of the location of the
        // accessible object on the screen.
        Dimension aScreenSize = Toolkit.getDefaultToolkit().getScreenSize();
        // Fill the screen rectangle.
        g.setColor (new Color (250,240,230));
        g.fillRect (
            (int)(mnHOffset+0.5),
            (int)(mnVOffset+0.5),
            (int)(mnScale*aScreenSize.getWidth()),
            (int)(mnScale*aScreenSize.getHeight()));
        // Draw a frame around the screen rectangle to increase its visibility.
        g.setColor (Color.BLACK);
        g.drawRect (
            (int)(mnHOffset+0.5),
            (int)(mnVOffset+0.5),
            (int)(mnScale*aScreenSize.getWidth()),
            (int)(mnScale*aScreenSize.getHeight()));

        // Now do the actual display of the accessible object.
        drawAccessibleObject (g, mxContext, Color.GREEN);
    }
    public synchronized void paintChildren (Graphics g)
    {
    }
    public synchronized void paintBorder (Graphics g)
    {
    }




    /** Draw a simple representation of the given accessible object in the
        specified color.
    */
    public void drawAccessibleObject (Graphics g, XAccessibleContext xContext, Color aColor)
    {
        if (xContext != null)
        {
            // First draw our parent.
            XAccessible xParent = xContext.getAccessibleParent();
            if (xParent != null)
                drawAccessibleObject (g, xParent.getAccessibleContext(), Color.GRAY);
            
            // When the context supports the XAccessibleComponent interface
            // then draw its outline.
            XAccessibleComponent xComponent = 
                (XAccessibleComponent)UnoRuntime.queryInterface(
                    XAccessibleComponent.class, xContext);
            if (xComponent != null)
            {
                // Get size and location on screen and transform them to fit
                // everything inside this widget.
                Point aLocation = xComponent.getLocationOnScreen();
                Size aSize = xComponent.getSize();
                g.setColor (aColor);
                g.drawRect (
                    (int)(mnHOffset + mnScale*aLocation.X+0.5),
                    (int)(mnVOffset + mnScale*aLocation.Y+0.5),
                    (int)(mnScale*aSize.Width),
                    (int)(mnScale*aSize.Height));
            }
        }
    }


    public synchronized void setAccessibleObject (XAccessibleContext xContext)
    {
        mxContext = xContext;
        repaint ();
    }

    public synchronized void updateAccessibleObject (XAccessibleContext xContext)
    {
        repaint ();
    }


    /** Set up the transformation so that the graphical display can show a
        centered representation of the whole screen.
    */
    private void setupTransformation ()
    {
        Dimension aScreenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension aWidgetSize = getSize();
        if ((aScreenSize.getWidth() > 0) && (aScreenSize.getHeight() > 0))
        {
            // Calculate the scales that would map the screen onto the
            // widget in both of the coordinate axes and select the smaller
            // of the two: it maps the screen onto the widget in both axes
            // at the same time.
            double nHScale = (aWidgetSize.getWidth() - 10) / aScreenSize.getWidth();
            double nVScale = (aWidgetSize.getHeight() - 10) / aScreenSize.getHeight();
            if (nHScale < nVScale)
                mnScale = nHScale;
            else
                mnScale = nVScale;

            // Calculate offsets that center the scaled screen inside the widget.
            mnHOffset = (aWidgetSize.getWidth() - mnScale*aScreenSize.getWidth()) / 2.0;
            mnVOffset = (aWidgetSize.getHeight() - mnScale*aScreenSize.getHeight()) / 2.0;
        }
        else
        {
            // In case of a degenerate (not yet initialized?) screen size
            // use some meaningless default values.
            mnScale = 1;
            mnHOffset = 0;
            mnVOffset = 0;
        }
    }


    private XAccessibleContext mxContext;
    private double mnScale;
    private double mnHOffset;
    private double mnVOffset;
}
