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

#ifndef STYLES_H_INCLUDED
#define STYLES_H_INCLUDED

#include "NetVR/NetVR"
#include "GLUtils/GLUtils.h"


class SphereNode : public NetVR::ShapeAction
{
	public:
	SphereNode(float radius);
	SphereNode(const NetVR::Node* node, float radius);

	// Action interface
	SphereNode* link(const void* thing) const;
	void operator()() const;

	// Shape interface
	NetVR::Point getCenter() const;
	bool contains(const NetVR::Point& point) const;
	NetVR::HitResult intersectionsWithRay(const NetVR::Ray& ray) const;

	// member variables
	private:
	const NetVR::Node* node;
	float radius;
	float sqrRadius;
};

class CubeNode : public NetVR::ShapeAction
{
 public:
  CubeNode(float size);
  CubeNode(const NetVR::Node* node, float size);

  // Action interface
  CubeNode* link(const void* thing) const;
  void operator()() const;

  // Shape interface
  NetVR::Point getCenter() const;
  bool contains(const NetVR::Point& point) const;
  NetVR::HitResult intersectionsWithRay(const NetVR::Ray& ray) const;

  // member variables
 private:
  const NetVR::Node* node;
  float size; 
};

class CylinderEdge : public NetVR::ShapeAction
{
	public:
	CylinderEdge();
	CylinderEdge(const NetVR::Edge* edge, GLUquadricObj* quadric);
	~CylinderEdge();

	// Action interface
	CylinderEdge* link(const void *thing) const;
	void operator()() const;

	// Shape interface
	NetVR::Point getCenter() const;
	bool contains(const NetVR::Point &point) const;
	NetVR::HitResult intersectionsWithRay(const NetVR::Ray& ray) const;

	private:
	const NetVR::Edge* edge;
	const std::pair<NetVR::Node*, NetVR::Node*> nodes;
	GLUquadricObj* quadric;
};

class ArrowCylinderEdge : public NetVR::ShapeAction
{
	public:
	ArrowCylinderEdge();
	ArrowCylinderEdge(const NetVR::Edge* edge, GLUquadricObj* quadric);
	~ArrowCylinderEdge();

	// Action interface
	ArrowCylinderEdge* link(const void *thing) const;
	void operator()() const;

	// Shape interface
	NetVR::Point getCenter() const;
	bool contains(const NetVR::Point &point) const;
	NetVR::HitResult intersectionsWithRay(const NetVR::Ray& ray) const;
	
	private:
	const NetVR::Edge* edge;
	const std::pair<NetVR::Node*, NetVR::Node*> nodes;
	GLUquadricObj* quadric;
};

class Material : public NetVR::Action 
{
	public:
	Material(GLMaterial::Color ambientDiffuse, GLMaterial::Color specular, float shininess);

	// Action interface
	Material* link(const void* thing) const;
	void operator()() const;

	private:
	GLMaterial mat;
};

#endif
