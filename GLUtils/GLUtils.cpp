/*******************************************************************
Copyright (c) 2006-2008 Braden Pellett and Jordan Van Aalsburg

This file is part of NetVR.

NetVR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NetVR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NetVR.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

#include "GLUtils.h"

namespace GLUtils {

void drawCylinder(GLUquadricObj *quadric)
{
    // Draw a cylinder with caps on both ends
    GLfloat radius = 0.05f;

    glPushMatrix();
    gluCylinder(quadric, radius, radius, 1.0f, 12, 12);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluDisk(quadric, 0.0f, radius, 12, 3); 
    gluQuadricOrientation(quadric, GLU_OUTSIDE);   
    glTranslatef(0.0f, 0.0f, 1.0f);
    gluDisk(quadric, 0.0f, radius, 12, 3); 
    glPopMatrix();
}

void drawArrow(GLUquadricObj *quadric)
{
    // Draw an arrow of length l=1 along z-axis
    GLfloat radius = 0.05f;
  
    glPushMatrix();
    gluCylinder(quadric, radius, radius, 0.8f, 12, 12);    
    glTranslatef(0.0f, 0.0f, 0.8f);
    gluCylinder(quadric, 2.5*radius, 0.0f, 0.2f, 12, 12);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluDisk(quadric, 0.0f, 2.5*radius, 12, 3);       
    glPopMatrix();
    gluDisk(quadric, 0.0f, radius, 12, 3);     
    gluQuadricOrientation(quadric, GLU_OUTSIDE);   
}

void drawArrow(GLUquadricObj *quadric, GLfloat length, 
               GLfloat head, GLfloat radius) {
    
    // Draw an arrow of specified length.
    // Avoids distortion due to glScalef() calls.    
    glPushMatrix();
    gluCylinder(quadric, radius, radius, (length-head), 12, 12);
    glTranslatef(0.0f, 0.0f, (length-head));
    gluCylinder(quadric, 2.5f*radius, 0.0f, head, 12, 12);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluDisk(quadric, 0.0f, 2.5f*radius, 12, 3);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    glPopMatrix();    
}

void drawDoubleArrow(GLUquadricObj *quadric)
{
    GLfloat radius=0.05;
    
    glPushMatrix();
    gluCylinder(quadric, 0.0f, 2.5*radius, 0.2f, 12, 12);
    glTranslatef(0.0f, 0.0f, 0.2f);
    gluDisk(quadric, 0.0f, 2.5*radius, 12, 3);
    gluCylinder(quadric, radius, radius, 0.6f, 12, 12);
    glTranslatef(0.0f, 0.0f, 0.6f);
    gluCylinder(quadric, 2.5*radius, 0.0f, 0.2f, 12, 12);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluDisk(quadric, 0.0f, 2.5*radius, 12, 3);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);   
    glPopMatrix();
}

void drawDoubleArrow(GLUquadricObj *quadric, GLfloat length,
                     GLfloat head, GLfloat radius)
{
    glPushMatrix();
    gluCylinder(quadric, 0.0f, 2.5*radius, head, 12, 12);
    glTranslatef(0.0f, 0.0f, head);
    gluDisk(quadric, 0.0f, 2.5*radius, 12, 3);
    gluCylinder(quadric, radius, radius, (length-2.0*head), 12, 12);
    glTranslatef(0.0f, 0.0f, (length-2.0*head));
    gluCylinder(quadric, 2.5*radius, 0.0f, head, 12, 12);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluDisk(quadric, 0.0f, 2.5*radius, 12, 3);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);   
    glPopMatrix();
}

void drawAxes(GLfloat length)
{
    // Draw axes as lines
    GLfloat origin[3] = { 0.0f, 0.0f, 0.0f };

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3fv(origin);
    glVertex3f(length, 0.0f, 0.0f);
    glVertex3fv(origin);
    glVertex3f(0.0f, length, 0.0f);
    glVertex3fv(origin);
    glVertex3f(0.0f, 0.0f, length);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawAxes(GLUquadricObj *quadric, GLfloat length)
{
    // Draw axes as 3D arrows
    glPushMatrix();
    glScalef(1.0f, 1.0f, length);
    drawArrow(quadric);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-90.f, 1.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, length);
    drawArrow(quadric);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.f, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, length);
    drawArrow(quadric);
    glPopMatrix();
}

void drawGrid(GLfloat length, int div)
{
    // Draw a grid in the X-Y plane
    //
    //  length = size of grid
    //  div    = number of divisions
    glDisable(GL_LIGHTING);
    GLfloat spacing = length/div;
    glBegin(GL_LINES);
    for(GLfloat x=0.0; x<=length; x+=spacing) {
        glVertex3f(x, 0.0f,   0.0f);
        glVertex3f(x, length, 0.0f);        
    }
    for(GLfloat y=0.0; y<=length; y+=spacing) {
        glVertex3f(0.0f,   y, 0.0f);
        glVertex3f(length, y, 0.0f);        
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawXYGrid(GLfloat length, int div)
{
    // Convenience function
    drawGrid(length, div);   
}

void drawYZGrid(GLfloat length, int div)
{
    // Convenience function
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    drawGrid(length, div);
    glPopMatrix();      
}

void drawXZGrid(GLfloat length, int div)
{
    // Convenience function
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawGrid(length, div);
    glPopMatrix();    
}

//
// Middle Level
//

void drawArrow(GLUquadricObj *quadric, Point a, Point b)
{
    Vector v(b-a);       
    double length = Geometry::mag(v);
   
    Vector norm = Geometry::cross(v, Vector(0.0,0.0,1.0));
    double theta = acos(Geometry::dot(v, Vector(0.0,0.0,1.0))/Geometry::mag(v));
    
    glPushMatrix();
    glTranslatef(a[0], a[1], a[2]);  
    glRotatef(-theta*180.0/M_PI, norm[0], norm[1], norm[2]);
    //glScalef(1.0f, 1.0f, length);
    //drawArrow(quadric);    
    drawArrow(quadric, length);    
    glPopMatrix();    
}

void drawDoubleArrow(GLUquadricObj *quadric, Point a, Point b)
{
    Vector v(b-a);    
    double length = Geometry::mag(v);
  
    Vector norm = Geometry::cross(v, Vector(0.0,0.0,1.0));
    double theta = acos(Geometry::dot(v, Vector(0.0,0.0,1.0))/Geometry::mag(v));
    
    glPushMatrix();
    glTranslatef(a[0], a[1], a[2]);
    glRotatef(-theta*180.0/M_PI, norm[0], norm[1], norm[2]);
    //glScalef(1.0f, 1.0f, length);
    //drawDoubleArrow(quadric);
    drawDoubleArrow(quadric, length);
    glPopMatrix();    
}

void drawCylinder(GLUquadricObj *quadric, Point a, Point b,
                  GLfloat radius) 
{
    Vector v(b-a);    
    double length = Geometry::mag(v);   

    Vector norm = Geometry::cross(v, Vector(0.0,0.0,1.0));
    double theta = acos(Geometry::dot(v, Vector(0.0,0.0,1.0))/Geometry::mag(v));
    
    glPushMatrix();
    glTranslatef(a[0], a[1], a[2]);
    glRotatef(-theta*180.0/M_PI, norm[0], norm[1], norm[2]);
    glScalef(1.0f, 1.0f, length);  
    drawCylinder(quadric);
    glPopMatrix();    
}

void drawBox(Point min, Point max)
{
    glDisable(GL_LIGHTING);
   
    glBegin(GL_LINE_LOOP);    
    glVertex3f(min[0], min[1], min[2]);
    glVertex3f(min[0], max[1], min[2]);
    glVertex3f(max[0], max[1], min[2]);
    glVertex3f(max[0], min[1], min[2]);
    glEnd();

    glBegin(GL_LINE_LOOP);   
    glVertex3f(min[0], min[1], max[2]);
    glVertex3f(min[0], max[1], max[2]);
    glVertex3f(max[0], max[1], max[2]);
    glVertex3f(max[0], min[1], max[2]);
    glEnd();
    
    glBegin(GL_LINES);    
    glVertex3f(min[0], min[1], min[2]);
    glVertex3f(min[0], min[1], max[2]);

    glVertex3f(min[0], max[1], min[2]);
    glVertex3f(min[0], max[1], max[2]);

    glVertex3f(max[0], max[1], min[2]);
    glVertex3f(max[0], max[1], max[2]);

    glVertex3f(max[0], min[1], min[2]);
    glVertex3f(max[0], min[1], max[2]);    
    glEnd();
    
    glEnable(GL_LIGHTING);
}

void drawWireframeBox(GLfloat length, GLfloat width, GLfloat height)
{
    glDisable(GL_LIGHTING);
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0f,   0.0f,  0.0f);
    glVertex3f(length, 0.0f,  0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(0.0f,   width, 0.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0f,   0.0f,  height);
    glVertex3f(length, 0.0f,  height);
    glVertex3f(length, width, height);
    glVertex3f(0.0f,   width, height);
    glEnd();
    
    glBegin(GL_LINES);  
    glVertex3f(0.0f,   0.0f,  0.0f);
    glVertex3f(0.0f,   0.0f,  height);
    
    glVertex3f(length, 0.0f,  0.0f);
    glVertex3f(length, 0.0f,  height);
     
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, width, height);
    
    glVertex3f(0.0f,   width, 0.0f);
    glVertex3f(0.0f,   width, height);
    glEnd();

    glEnable(GL_LIGHTING);    
}

void drawBox(GLfloat length, GLfloat width, GLfloat height)
{   
    glPushMatrix();    
    glTranslatef(-length/2.0, -width/2.0, -height/2.0);
    
    glBegin(GL_QUADS);
    // Front
    glNormal3f(0.0f,  -1.0f,  0.0f);
    glVertex3f(0.0f,   0.0f,  0.0f);
    glVertex3f(length, 0.0f,  0.0f);
    glVertex3f(length, 0.0f,  height);
    glVertex3f(0.0f,   0.0f,  height);
    
    // Right
    glNormal3f(1.0f,   0.0f,  0.0f);
    glVertex3f(length, 0.0f,  0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, width, height);
    glVertex3f(length, 0.0f,  height);
    
    // Back
    glNormal3f(0.0f,   1.0f,  0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(0.0f,   width, 0.0f);
    glVertex3f(0.0f,   width, height);
    glVertex3f(length, width, height);

    // Left 
    glNormal3f(-1.0f, 0.0f,  0.0f);
    glVertex3f(0.0f,  width, 0.0f);
    glVertex3f(0.0f,  0.0f,  0.0f);
    glVertex3f(0.0f,  0.0f,  height);    
    glVertex3f(0.0f,  width, height);

    // Top
    glNormal3f(0.0f,   0.0f,  1.0f);
    glVertex3f(0.0f,   0.0f,  height);
    glVertex3f(length, 0.0f,  height );
    glVertex3f(length, width, height);
    glVertex3f(0.0f,   width, height);

    // Bottom    
    glNormal3f(0.0f,   0.0f, -1.0f);
    glVertex3f(0.0f,   width, 0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, 0.0f,  0.0f);
    glVertex3f(0.0f,   0.0f,  0.0f);
    
    glEnd();
    glPopMatrix();
}

void drawBox(Point a, Point b, GLfloat width, GLfloat height)
{
    Vector v(b-a);    
    double length = Geometry::mag(v);   
    
    Vector norm = Geometry::cross(v, Vector(0.0,0.0,1.0));
    double theta = acos(Geometry::dot(v, Vector(0.0,0.0,1.0))/Geometry::mag(v));
    
    glPushMatrix();
    glTranslatef(a[0], a[1], a[2]);
    glRotatef(-theta*180.0/M_PI, norm[0], norm[1], norm[2]);
    glTranslatef(0.0f, 0.0f, length/2.0);
    glScalef(width, height, length);    
    drawBox(1.0f, 1.0f, 1.0f);
    glPopMatrix();        
}

}
