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

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "GLUtils/Geometry.h"
#include "GLUtils/OpenGL.h"


namespace GLUtils {

//
// Base Level
//

void drawCylinder(GLUquadricObj *quadric);

void drawArrow(GLUquadricObj *quadric);

void drawArrow(GLUquadricObj *quadric, GLfloat length, 
               GLfloat head=0.5, GLfloat radius=0.05);

void drawDoubleArrow(GLUquadricObj *quadric);

void drawDoubleArrow(GLUquadricObj *quadric, GLfloat length,
                     GLfloat head=0.5, GLfloat radius=0.05);

void drawAxes(GLfloat length);

void drawAxes(GLUquadricObj *quadric, GLfloat length);

void drawGrid(GLfloat length, int div);

void drawXYGrid(GLfloat length, int div);

void drawYZGrid(GLfloat length, int div);

void drawXZGrid(GLfloat length, int div);

//
// Middle Level
//

void drawArrow(GLUquadricObj *quadric, Point a, Point b);

void drawDoubleArrow(GLUquadricObj *quadric, Point a, Point b);

void drawCylinder(GLUquadricObj *quadric, Point a, Point b,
                  GLfloat radius=0.05);


void drawBox(Point min, Point max);

void drawWireframeBox(GLfloat length, GLfloat width, GLfloat height);

void drawBox(GLfloat length, GLfloat width, GLfloat height);

void drawBox(Point a, Point b, GLfloat width, GLfloat height);

} // namespace GLUtils

#endif
