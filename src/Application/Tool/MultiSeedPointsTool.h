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

#ifndef APPLICATION_TOOL_MULTISEEDPOINTSTOOL_H
#define APPLICATION_TOOL_MULTISEEDPOINTSTOOL_H

#include <Application/Tool/SingleTargetTool.h>
#include <Application/Tool/SeedPointsInterface.h>

namespace Seg3D
{

class MultiSeedPointsToolPrivate;
typedef boost::shared_ptr< MultiSeedPointsToolPrivate > MultiSeedPointsToolPrivateHandle;

class MultiSeedPointsTool : public SingleTargetTool, public SeedPointsInterface
{
public:
  MultiSeedPointsTool( Core::VolumeType target_volume_type, const std::string& toolid );
  virtual ~MultiSeedPointsTool() = 0;

public:
  /// HANDLE_MOUSE_PRESS:
  /// Called when a mouse button has been pressed.
  virtual bool handle_mouse_press( ViewerHandle viewer,
                                   const Core::MouseHistory& mouse_history,
                                   int button, int buttons, int modifiers ) override;

  /// HANDLE_MOUSE_MOVE:
  /// Called when the mouse moves  in a viewer.
  virtual bool handle_mouse_move( ViewerHandle viewer,
                                  const Core::MouseHistory& mouse_history,
                                  int button, int buttons, int modifiers ) override;

  /// HANDLE_KEY_PRESS:
  /// Called when a key is pressed.
  virtual bool handle_key_press( ViewerHandle viewer, int key, int modifiers ) override;

  /// REDRAW:
  /// Draw seed points in the specified viewer.
  /// The function should only be called by the renderer, which has a valid GL context.
  virtual void redraw( size_t viewer_id, const Core::Matrix& proj_mat,
                       int viewer_width, int viewer_height ) override;

  /// HAS_2D_VISUAL:
  /// Returns true if the tool draws itself in the 2D view, otherwise false.
  /// The default implementation returns false.
  virtual bool has_2d_visual() override;

  /// HAS_3D_VISUAL:
  /// Returns true if the tool draws itself in the volume view, otherwise false.
  /// The default implementation returns false.
  virtual bool has_3d_visual() override;

public:
  /// HANDLE_SEED_POINTS_CHANGED:
   /// Called when the seed points have changed.
  /// The default implementation will cause all the 2D viewers to redraw overlay.
  virtual void handle_seed_points_changed() override;

  /// CLEAR:
  /// Remove all the seed points.
  virtual void clear( Core::ActionContextHandle context ) override;

  /// FIND_POINT:
  /// Find seed point in viewer and return coordinates.
  virtual bool find_point( ViewerHandle viewer, double world_x, double world_y,
                           Core::VolumeSliceHandle vol_slice, Core::Point& pt ) override;

public:
  Core::StatePoint2DVectorHandle seed_points_state_;

private:
  MultiSeedPointsToolPrivateHandle private_;

};

} // end namespace Seg3D

#endif
