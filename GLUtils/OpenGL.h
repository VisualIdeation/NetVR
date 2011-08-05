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

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/GLGeometryWrappers.h>
#include <GL/GLColorMap.h>
#include <GL/GLFont.h>
#include <GL/GLScalarConverter.h>
#include <GL/GLLightModelEnums.h>
#include <GL/GLValueCoders.h>
#include <GL/GLMaterial.h>
#include <GL/GLWindow.h>
#include <GL/GLMatrixEnums.h>
#include <GL/GLModels.h>
#include <GL/GLFogEnums.h>
#include <GL/GLVertex.h>
#include <GL/GLScalarLimits.h>
#include <GL/GLVertexArrayParts.h>
#include <GL/GLTexEnvEnums.h>
#include <GL/GLExtensions.h>
#include <GL/GLLightEnums.h>
#include <GL/GLVector.h>
#include <GL/GLTransformationWrappers.h>
#include <GL/GLExtensionManager.h>
#include <GL/GLColorOperations.h>
#include <GL/GLLineIlluminator.h>
#include <GL/GLLight.h>
#include <GL/GLColor.h>
#include <GL/GLContextData.h>
#include <GL/GLMaterialEnums.h>
#include <GL/GLFog.h>
#include <GL/GLObject.h>
#include <GL/GLBox.h>

// makes errors:
// #include <GL/GLGetTemplates.h>
// #include <GL/GLPrimitiveTemplates.h>
// #include <GL/GLIndexTemplates.h>
// #include <GL/GLMultiTexCoordTemplates.h>
// #include <GL/GLNormalTemplates.h>
// #include <GL/GLVertexTemplates.h>
// #include <GL/GLColorTemplates.h>
// #include <GL/GLTexCoordTemplates.h>
// #include <GL/GLVertexArrayTemplates.h>
// #include <GL/GLLightTemplates.h>
// #include <GL/GLGetTexEnvTemplates.h>
// #include <GL/GLGetMiscTemplates.h>
// #include <GL/GLFogTemplates.h>
// #include <GL/GLGetLightTemplates.h>
// #include <GL/GLGetMaterialTemplates.h>
// #include <GL/GLGetFogTemplates.h>
// #include <GL/GLMaterialTemplates.h>
// #include <GL/GLGetMatrixTemplates.h>
// #include <GL/GLLightModelTemplates.h>
// #include <GL/GLMatrixTemplates.h>
// #include <GL/GLGetPrimitiveTemplates.h>
// #include <GL/GLFogCoordTemplates.h>
// #include <GL/GLTexEnvTemplates.h>
// #include <GL/GLSecondaryColorTemplates.h>
// #include <GL/GLMiscTemplates.h>

