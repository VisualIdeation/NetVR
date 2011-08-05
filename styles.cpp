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

#include "styles.h"

#include "NetVR/NetVR"

//#include <GL/glu.h>
//#include <GL/gl.h>
//#include <GL/GLContextData.h>
//#include <GL/GLModels.h>
//#include <GL/GLMaterial.h>

#include <GL/GLModels.h>

/****************************
 * CubeNode
 */
CubeNode::CubeNode(float size) : size(size) { }

CubeNode::CubeNode(const NetVR::Node* node, float size)
  : node(node), size(size) 
{ }


/*
 * Action interface
 */
CubeNode* CubeNode::link(const void* thing) const
{
  return new CubeNode(static_cast<const NetVR::Node*>(thing), size);
}

void CubeNode::operator()() const
{
  glPushMatrix();
  glTranslatef(node->getPosition()[0], node->getPosition()[1], node->getPosition()[2]);
  glDrawCube(size);
  glPopMatrix();
}

NetVR::Point CubeNode::getCenter() const
{
  return node->getPosition();
}
bool CubeNode::contains(const NetVR::Point& point) const
{
  NetVR::Point center = node->getPosition();
  if (fabs(point[0] - center[0]) > size/2.0) return false;
  if (fabs(point[1] - center[1]) > size/2.0) return false;
  if (fabs(point[2] - center[2]) > size/2.0) return false;
  return true;
}
NetVR::HitResult CubeNode::intersectionsWithRay(const NetVR::Ray& ray) const
{
	/* Initialize the result interval to contain everything: */
	float lambda1=Math::Constants<float>::min;
	float lambda2=Math::Constants<float>::max;
	
        NetVR::Point max(getCenter());
	NetVR::Point min(getCenter());
	for (int i = 0; i < 3; ++i) {
	  max[i] += size / 2.0;
	  min[i] -= size / 2.0;
	}

	/* Intersect the ray with each side of the box: */
	for(int i=0 ; i<3 ; ++i)
		{
		/* Calculate ray intersection interval along this direction: */
		float ro=ray.getOrigin()[i];
		float rd=ray.getDirection()[i];
		if(rd < 0)
			{
			/* Ray intersects "max" face first: */
			float l1=(max[i]-ro)/rd;
			float l2=(min[i]-ro)/rd;
			
			/* Intersect computed interval with result interval: */
			if(lambda1<l1)
				lambda1=l1;
			if(lambda2>l2)
				lambda2=l2;
			}
		else if(rd > 0)
			{
			/* Ray intersects "min" face first: */
			float l1=(min[i]-ro)/rd;
			float l2=(max[i]-ro)/rd;
			
			/* Intersect computed interval with result interval: */
			if(lambda1<l1)
				lambda1=l1;
			if(lambda2>l2)
				lambda2=l2;
			}
		else if(ro<min[i]||max[i]<ro)
			{
			/* Ray is entirely outside the faces: */
			lambda1=Math::Constants<float>::max;
			lambda2=Math::Constants<float>::min;
			break;
			}
		}
	
	/* Return the first intersection: */
	if(lambda1<lambda2)
		{
		if(lambda1 >= 0)
			return NetVR::HitResult(lambda1,NetVR::HitResult::ENTRY);
		else if(lambda2 >= 0)
			return NetVR::HitResult(lambda2,NetVR::HitResult::EXIT);
		else
			return NetVR::HitResult();
		}
	else
		return NetVR::HitResult();
}

/****************************
 * SphereNode
 */

SphereNode::SphereNode(float radius) : radius(radius) {}

SphereNode::SphereNode(const NetVR::Node* node, float radius)
	: node(node), radius(radius), sqrRadius(Math::sqr(radius))
{ }

/*
 * Action interface
 */
SphereNode* SphereNode::link(const void* thing) const
{
	return new SphereNode( static_cast<const NetVR::Node*>(thing), radius);
}
void SphereNode::operator()() const
{		 
	glPushMatrix();
	glTranslatef(node->getPosition()[0], node->getPosition()[1], node->getPosition()[2]);
	glDrawSphereIcosahedron(radius, 8);
	glPopMatrix();
}	

/*
 * Shape interface
 */
NetVR::Point SphereNode::getCenter() const 
{
	return node->getPosition();
}

bool SphereNode::contains(const NetVR::Point& point) const
{
	return Geometry::sqrDist(point, getCenter()) <= sqrRadius; 
}

NetVR::HitResult SphereNode::intersectionsWithRay(const NetVR::Ray& ray) const 
{
	float  d2        = Geometry::sqr(ray.getDirection());
	NetVR::Vector oc = ray.getOrigin() - getCenter();
	float  ph        = (oc * ray.getDirection());
	float  det       = Math::sqr(ph) - (Geometry::sqr(oc) - sqrRadius) * d2;
	if (det < 0)
		return NetVR::HitResult();

	det = Math::sqrt(det);
	float lambda = (-ph-det)/d2; // First intersection
	if (lambda >= 0)
		return NetVR::HitResult(lambda, NetVR::HitResult::ENTRY);

	lambda = (-ph + det) / d2; // Second intersection
	if (lambda >= 0)
		return NetVR::HitResult(lambda, NetVR::HitResult::EXIT);
	
	return NetVR::HitResult();
}


/****************************
 * CylinderEdge
 */

CylinderEdge::CylinderEdge()
	: edge(NULL), nodes(NULL, NULL), quadric(gluNewQuadric())
{
	gluQuadricDrawStyle(quadric, GLU_FILL);
}

CylinderEdge::CylinderEdge(const NetVR::Edge* edge, GLUquadricObj* quadric)
	: edge(edge),
	  nodes(static_cast<NetVR::Node*>((edge->getNodes().first )->getMonitor(edge->getNetworkMonitor())),
	        static_cast<NetVR::Node*>((edge->getNodes().second)->getMonitor(edge->getNetworkMonitor())) ),
	  quadric(quadric)
{ }

CylinderEdge::~CylinderEdge()
{
	if (!edge && quadric)
		gluDeleteQuadric(quadric);  
}

/*
 * Action interface
 */
CylinderEdge* CylinderEdge::link(const void *thing) const 
{ 
	return new CylinderEdge( static_cast<const NetVR::Edge*>(thing), quadric );
}

void CylinderEdge::operator()() const 
{
	GLUtils::drawCylinder(quadric, nodes.first->getPosition(), nodes.second->getPosition());
}

/*
 * Shape interface
 */
NetVR::Point CylinderEdge::getCenter() const 
{
	return NetVR::Point();
}

bool CylinderEdge::contains(const NetVR::Point &point) const 
{
	return false;
}

NetVR::HitResult CylinderEdge::intersectionsWithRay(const NetVR::Ray& ray) const 
{
	return NetVR::HitResult();
}


/****************************
 * ArrowCylinderEdge
 */

ArrowCylinderEdge::ArrowCylinderEdge()
	: edge(NULL), nodes(NULL, NULL), quadric(gluNewQuadric())
{
	gluQuadricDrawStyle(quadric, GLU_FILL);
}

ArrowCylinderEdge::ArrowCylinderEdge(const NetVR::Edge* edge, GLUquadricObj* quadric)
	: edge(edge),
	  nodes(static_cast<NetVR::Node*>((edge->getNodes().first )->getMonitor(edge->getNetworkMonitor())),
	        static_cast<NetVR::Node*>((edge->getNodes().second)->getMonitor(edge->getNetworkMonitor()))),
	        quadric(quadric)
{
	gluQuadricDrawStyle(quadric, GLU_FILL);
}

ArrowCylinderEdge::~ArrowCylinderEdge()
{
	if (!edge && quadric)
		gluDeleteQuadric(quadric);
}

/*
 * Action interface
 */

ArrowCylinderEdge* ArrowCylinderEdge::link(const void *thing) const 
{ 
	return new ArrowCylinderEdge(static_cast<const NetVR::Edge*>(thing), quadric);
}

void ArrowCylinderEdge::operator()() const 
{
	if (edge->isDirected()) {
		if (edge->comesFromNode(nodes.first->getNode())) {
			NetVR::Ray ray(nodes.first->getPosition(), nodes.second->getPosition());
			NetVR::Point pos(ray(nodes.second->intersectionsWithRay(ray).getParameter()));
			GLUtils::drawArrow(quadric, nodes.first->getPosition(), pos);
		} else {
			NetVR::Ray ray(nodes.first->getPosition(), nodes.second->getPosition());
			NetVR::Point pos(ray(nodes.first->intersectionsWithRay(ray).getParameter()));
			GLUtils::drawArrow(quadric, pos, nodes.second->getPosition());
		}
	} else {
		NetVR::Ray ray(nodes.first->getPosition(), nodes.second->getPosition());
		NetVR::Point pos1(ray(nodes.first->intersectionsWithRay(ray).getParameter()));
		NetVR::Point pos2(ray(nodes.second->intersectionsWithRay(ray).getParameter()));
		GLUtils::drawDoubleArrow(quadric, pos1, pos2);
	}

/*
	Vrui::Point pos;
	Vrui::Point dest;
	if (edge->comesFromNode(nodes.first->getNode())) {
		pos  = nodes.first->getPosition();
		dest = nodes.second->getPosition();
	} else {
		dest = nodes.first->getPosition();
		pos  = nodes.second->getPosition();
	}

	Vrui::Vector v(dest-pos);	
	double length = GLUtils::mag(v);
	
	Vrui::Vector norm = Geometry::cross(v, Vrui::Vector(0.0,0.0,1.0));
	double theta = acos(GLUtils::dot(v, Vrui::Vector(0.0,0.0,1.0))/GLUtils::mag(v));
	glPushMatrix();
	
	float sphere = 0.25;
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(-theta*180.0/M_PI, norm[0], norm[1], norm[2]);
	glTranslatef(0.0, 0.0, sphere);

	if(edge->isDirected())
		GLUtils::drawArrow(quadric, length-2.0*sphere);
	else
		GLUtils::drawDoubleArrow(quadric, length-2.0*sphere);
	glPopMatrix();	
*/
}

/*
 * Shape interface
 */

NetVR::Point ArrowCylinderEdge::getCenter() const 
{ 
	return NetVR::Point();
}

bool ArrowCylinderEdge::contains(const NetVR::Point &point) const 
{ 
	return false;
}

NetVR::HitResult ArrowCylinderEdge::intersectionsWithRay(const NetVR::Ray& ray) const 
{
	return NetVR::HitResult();
}


/****************************
 * Material
 */

Material::Material(GLMaterial::Color ambientDiffuse, GLMaterial::Color specular, float shininess) 
{ 
	mat.ambient = ambientDiffuse;
	mat.diffuse = ambientDiffuse;
	mat.specular = specular;
	mat.shininess = shininess;
}

Material* Material::link(const void* thing) const 
{
	return new Material( *this );
}

void Material::operator()() const
{
	glMaterial(GLMaterialEnums::FRONT_AND_BACK, mat);
}
