/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/** \author Ioan Sucan */

#include "planning_environment/util/construct_object.h"
#include <geometric_shapes/shapes.h>
#include <geometric_shapes/shape_operations.h>
#include <tf_conversions/tf_eigen.h>
#include <tf/transform_datatypes.h>
#include <ros/console.h>
#include <vector>


std::vector<shapes::Shape*> planning_environment::cloneShapeVector(const std::vector<shapes::Shape*>& shapes) {
   std::vector<shapes::Shape*> ret;
   for(unsigned int i = 0; i < shapes.size(); i++) {
     ret.push_back(shapes[i]->clone());
   }
   return ret;
 }
 
void planning_environment::deleteShapeVector(std::vector<shapes::Shape*>& shapes) {
   for(unsigned int i = 0; i < shapes.size(); i++) {
     delete shapes[i];
   }
   shapes.clear();
 }
 
const EigenSTL::vector_Affine3d planning_environment::vectorTFToAffine(const std::vector<tf::Transform>& tf) {
	EigenSTL::vector_Affine3d ret;
	for(unsigned int i = 0; i < tf.size(); i++) {
		Eigen::Affine3d a;
		tf::transformTFToEigen(tf[i],a);
		ret.push_back(a);
	}
	return ret;
}

const EigenSTL::vector_Vector3d planning_environment::vectorTFToVector3d(std::vector<tf::Vector3>& tf) {
	EigenSTL::vector_Vector3d ret;
	for(unsigned int i = 0; i < tf.size(); i++) {
		Eigen::Vector3d a;
		tf::vectorTFToEigen(tf[i],a);
		ret.push_back(a);
	}
	return ret;
}



void planning_environment::maskPosesInsideBodyVectors(const std::vector<tf::Transform>& poses,
                                         const std::vector<bodies::BodyVector*>& bvs,
                                         std::vector<bool>& mask,
                                         bool use_padded) {
   mask.resize(poses.size(), false);
   for(unsigned int i = 0; i < poses.size(); i++) {
     bool inside = false;
     tf::Vector3 pt = poses[i].getOrigin();
     for(unsigned int j = 0; !inside && j < bvs.size(); j++) {
       for(unsigned int k = 0;!inside && k < bvs[j]->getCount(); k++) {
         if(!use_padded) {
				//QUICKFIX
				bodies::BoundingSphere tmp;
				bvs[j]->getBody(k)->computeBoundingSphere(tmp);
				Eigen::Vector3d tmp2;
				tf::vectorTFToEigen(pt,tmp2);
				bodies::BoundingSphere tmp3;
				bvs[j]->getBody(k)->computeBoundingSphere(tmp3);
				double squareRad = tmp3.radius * tmp3.radius;
				double dist = (tmp.center - tmp2).norm();
			 
           //if(bvs[j]->getBoundingSphere(k).center.distance2(pt) < bvs[j]->getBoundingSphereRadiusSquared(k)) {
           if(dist < squareRad) {
             if(bvs[j]->getBody(k)->containsPoint(tmp2)) {
               inside = true;
             }
           }
         } else {
				//QUICKFIX
				bodies::BoundingSphere tmp;
				bvs[j]->getBody(k)->computeBoundingSphere(tmp);
				Eigen::Vector3d tmp2;
				tf::vectorTFToEigen(pt,tmp2);
				bodies::BoundingSphere tmp3;
				bvs[j]->getBody(k)->computeBoundingSphere(tmp3);
				double squareRad = tmp3.radius * tmp3.radius;
				double dist = (tmp.center - tmp2).norm();
			// Padding is no longer considered by ROS but leaving this dummy branch
           //if(bvs[j]->getPaddedBoundingSphere(k).center.distance2(pt) < bvs[j]->getPaddedBoundingSphereRadiusSquared(k)) {
           if(dist < squareRad) {
             //if(bvs[j]->getPaddedBody(k)->containsPoint(tmp2)) {
             if(bvs[j]->getBody(k)->containsPoint(tmp2)) {
               inside = true;
             }
           }
         }
       }
     }
     mask[i] = !inside;
   }
}




shapes::Shape* planning_environment::constructObject(const arm_navigation_msgs::Shape &obj)
{
    shapes::Shape *shape = NULL;
    if (obj.type == arm_navigation_msgs::Shape::SPHERE)
    {
	if (obj.dimensions.size() != 1)
	    ROS_ERROR("Unexpected number of dimensions in sphere definition");
	else
	    shape = new shapes::Sphere(obj.dimensions[0]);
    }
    else
    if (obj.type == arm_navigation_msgs::Shape::BOX)
    {
	if (obj.dimensions.size() != 3)
	    ROS_ERROR("Unexpected number of dimensions in box definition");
	else
	    shape = new shapes::Box(obj.dimensions[0], obj.dimensions[1], obj.dimensions[2]);
    }
    else
    if (obj.type == arm_navigation_msgs::Shape::CYLINDER)
    {
	if (obj.dimensions.size() != 2)
	    ROS_ERROR("Unexpected number of dimensions in cylinder definition");
	else
	    shape = new shapes::Cylinder(obj.dimensions[0], obj.dimensions[1]);
    }   
    else
    if (obj.type == arm_navigation_msgs::Shape::MESH)
    {
	if (obj.dimensions.size() != 0)
	    ROS_ERROR("Unexpected number of dimensions in mesh definition");
	else
	{
	    if (obj.triangles.size() % 3 != 0)
		ROS_ERROR("Number of triangle indices is not divisible by 3");
	    else
	    {
		if (obj.triangles.empty() || obj.vertices.empty())
		    ROS_ERROR("Mesh definition is empty");
		else
		{
		    std::vector<tf::Vector3>    vertices(obj.vertices.size());
		    std::vector<unsigned int> triangles(obj.triangles.size());
		    for (unsigned int i = 0 ; i < obj.vertices.size() ; ++i)
			vertices[i].setValue(obj.vertices[i].x, obj.vertices[i].y, obj.vertices[i].z);
		    for (unsigned int i = 0 ; i < obj.triangles.size() ; ++i)
			triangles[i] = obj.triangles[i];
			
		    shape = shapes::createMeshFromVertices(planning_environment::vectorTFToVector3d(vertices), triangles);
		}
	    }
	}
    }
    
    if (shape == NULL)
	ROS_ERROR("Unable to construct shape corresponding to object of type %d", (int)obj.type);
    
    return shape;
}


bool planning_environment::constructObjectMsg(const shapes::Shape* shape, arm_navigation_msgs::Shape &obj, double padding)
{
  obj.dimensions.clear();
  obj.vertices.clear();
  obj.triangles.clear();
  if (shape->type == shapes::SPHERE)
  {
    obj.type = arm_navigation_msgs::Shape::SPHERE;
    obj.dimensions.push_back(static_cast<const shapes::Sphere*>(shape)->radius+padding);
  }
  else
    if (shape->type == shapes::BOX)
    {
      obj.type = arm_navigation_msgs::Shape::BOX;
      const double* sz = static_cast<const shapes::Box*>(shape)->size;	
      obj.dimensions.push_back(sz[0]+padding*2.0);
      obj.dimensions.push_back(sz[1]+padding*2.0);
      obj.dimensions.push_back(sz[2]+padding*2.0);
    }
    else
      if (shape->type == shapes::CYLINDER)
      {	
	obj.type = arm_navigation_msgs::Shape::CYLINDER;
	obj.dimensions.push_back(static_cast<const shapes::Cylinder*>(shape)->radius+padding);
	obj.dimensions.push_back(static_cast<const shapes::Cylinder*>(shape)->length+padding*2.0);
      }
      else
        if (shape->type == shapes::MESH)
        {
          obj.type = arm_navigation_msgs::Shape::MESH;

          const shapes::Mesh *mesh = static_cast<const shapes::Mesh*>(shape);
          const unsigned int t3 = mesh->triangle_count * 3;

          obj.vertices.resize(mesh->vertex_count);
          obj.triangles.resize(t3);
	
          double sx = 0.0, sy = 0.0, sz = 0.0;
          for (unsigned int i = 0 ; i < mesh->vertex_count ; ++i)
          {
            unsigned int i3 = i * 3;
            obj.vertices[i].x = mesh->vertices[i3];
            obj.vertices[i].y = mesh->vertices[i3 + 1];
            obj.vertices[i].z = mesh->vertices[i3 + 2];
            sx += obj.vertices[i].x;
            sy += obj.vertices[i].y;
            sz += obj.vertices[i].z;
          }
          // the center of the mesh
          sx /= (double)mesh->vertex_count;
          sy /= (double)mesh->vertex_count;
          sz /= (double)mesh->vertex_count;

          // scale the mesh
          for (unsigned int i = 0 ; i < mesh->vertex_count ; ++i)
          {
            // vector from center to the vertex
            double dx = obj.vertices[i].x - sx;
            double dy = obj.vertices[i].y - sy;
            double dz = obj.vertices[i].z - sz;
		
            //double theta_xy = atan2(dy,dx);
            //double theta_xz = atan2(dz,dx);

            double ndx = ((dx > 0) ? dx+padding : dx-padding);
            double ndy = ((dy > 0) ? dy+padding : dy-padding);
            double ndz = ((dz > 0) ? dz+padding : dz-padding);

            obj.vertices[i].x = sx + ndx;
            obj.vertices[i].y = sy + ndy;
            obj.vertices[i].z = sz + ndz;
          }

          //for (unsigned int i = 0 ; i < mesh->vertexCount ; ++i)
          //{
          //    obj.vertices[i].x = mesh->vertices[3 * i    ];
          //    obj.vertices[i].y = mesh->vertices[3 * i + 1];
          //    obj.vertices[i].z = mesh->vertices[3 * i + 2];
          //}
	
          for (unsigned int i = 0 ; i < t3  ; ++i)
	    obj.triangles[i] = mesh->triangles[i];
        }
        else
        {
          ROS_ERROR("Unable to construct object message for shape of type %d", (int)shape->type);
          return false;
        }
    
  return true;
}


