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

#include <Application/Tool/MultiSeedPointsTool.h>

#include <Core/State/Actions/ActionAdd.h>
#include <Core/State/Actions/ActionClear.h>
#include <Core/State/Actions/ActionRemove.h>
#include <Application/ViewerManager/ViewerManager.h>

namespace Seg3D
{

//class MultiSeedPointsToolPrivate
//{
//public:
//};

bool MultiSeedPointsTool::find_point( ViewerHandle viewer, double world_x, double world_y,
                                 Core::VolumeSliceHandle vol_slice, Core::Point& pt )
{
//  // Step 1. Compute the size of a pixel in world space
//  double x0, y0, x1, y1;
//  viewer->window_to_world( 0, 0, x0, y0 );
//  viewer->window_to_world( 1, 1, x1, y1 );
//  double pixel_width = Core::Abs( x1 - x0 );
//  double pixel_height = Core::Abs( y1 - y0 );
//
//  // Step 2: Search for the first seed point that's within 4 pixels in each direction
//  // from the given search position (world_x, world_y)
//  double range_x = pixel_width * 4;
//  double range_y = pixel_height * 4;
//  std::vector< Core::Point > seed_points;
//  {
//    Core::StateEngine::lock_type lock( Core::StateEngine::GetMutex() );
//    seed_points = this->seed_points_state_->get();
//  }
//  size_t num_of_pts = seed_points.size();
//  for ( size_t i = 0; i < num_of_pts; i++ )
//  {
//    double pt_x, pt_y;
//    vol_slice->project_onto_slice( seed_points[ i ], pt_x, pt_y );
//    if ( Core::Abs( pt_x - world_x ) <= range_x &&
//        Core::Abs( pt_y - world_y ) <= range_y )
//    {
//      pt = seed_points[ i ];
//      return true;
//    }
//  }

  return false;
}

void MultiSeedPointsTool::clear( Core::ActionContextHandle context )
{
  Core::ActionClear::Dispatch( context, this->seed_points_state_ );
}

void MultiSeedPointsTool::handle_seed_points_changed()
{
  ViewerManager::Instance()->update_2d_viewers_overlay();
}

void MultiSeedPointsTool::handle_new_points_vector( Core::ActionContextHandle context )
{
}

}