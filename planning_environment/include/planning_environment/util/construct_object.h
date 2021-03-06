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

#ifndef PLANNING_ENVIRONMENT_UTIL_CONSTRUCT_OBJECT_
#define PLANNING_ENVIRONMENT_UTIL_CONSTRUCT_OBJECT_

#include <geometric_shapes/shapes.h>
#include <geometric_shapes/bodies.h>
#include <arm_navigation_msgs/Shape.h>
#include <tf_conversions/tf_eigen.h>
#include <eigen_stl_containers/eigen_stl_containers.h>
#include <vector>

namespace planning_environment
{
std::vector<shapes::Shape*> cloneShapeVector(const std::vector<shapes::Shape*>& shapes);
void deleteShapeVector(std::vector<shapes::Shape*>& shapes);
const EigenSTL::vector_Affine3d vectorTFToAffine(const std::vector<tf::Transform>& tf);
const EigenSTL::vector_Vector3d vectorTFToVector3d(std::vector<tf::Vector3>& tf);
void maskPosesInsideBodyVectors(const std::vector<tf::Transform>& poses, const std::vector<bodies::BodyVector*>& bvs, std::vector<bool>& mask, bool use_padded);
 
shapes::Shape* constructObject(const arm_navigation_msgs::Shape &obj);
bool constructObjectMsg(const shapes::Shape* shape, arm_navigation_msgs::Shape &obj, double padding = 0.0);

    
}

#endif
