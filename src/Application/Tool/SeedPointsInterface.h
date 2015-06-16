/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2015 Scientific Computing and Imaging Institute,
 University of Utah.


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
*/

#ifndef APPLICATION_TOOL_SEEDPOINTSINTERFACE_H
#define APPLICATION_TOOL_SEEDPOINTSINTERFACE_H

namespace Seg3D
{

class SeedPointsInterface
{
public:
  SeedPointsInterface() {}
  virtual ~SeedPointsInterface() {}

public:
  /// HANDLE_SEED_POINTS_CHANGED:
  /// Called when the seed points have changed.
  virtual void handle_seed_points_changed() = 0;

  /// CLEAR:
  /// Remove all the seed points.
  virtual void clear( Core::ActionContextHandle context ) = 0;

  /// FIND_POINT:
  /// Find seed point in viewer and return coordinates.
  virtual bool find_point( ViewerHandle viewer, double world_x, double world_y,
                           Core::VolumeSliceHandle vol_slice, Core::Point& pt ) = 0;
};

} // end namespace Seg3D
#endif