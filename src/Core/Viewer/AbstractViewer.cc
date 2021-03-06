/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2009 Scientific Computing and Imaging Institute,
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

// Core includes
#include <Core/Utils/Exception.h>
#include <Core/Viewer/AbstractViewer.h>

namespace Core
{

//////////////////////////////////////////////////////////////////////////
// class AbstractViewerPrivate
//////////////////////////////////////////////////////////////////////////

class AbstractViewerPrivate
{
public:
  AbstractViewerPrivate() : cursor_( CursorShape::CROSS_E ) {}

  // Handle with the renderer
  AbstractRendererHandle renderer_;
  
  // Width and height of the viewer window
  int width_;
  int height_;

  // Cursor shape
  CursorShape cursor_;

  // Last rendered Texture and Overlay Texture generated by the renderer
  Texture2DHandle texture_;
  Texture2DHandle overlay_texture_;

  AbstractViewer* viewer_;

  // SET_TEXTURE:
  // Connected to the redraw_completed_signal_ of the renderer.
  void set_texture( Texture2DHandle texture, bool delay_update );

  // SET_OVERLAY_TEXTURE:
  // Connected to the redraw_overlay_completed_signal_ of the renderer.
  void set_overlay_texture( Texture2DHandle texture, bool delay_update );

  // HANDLE_VISIBILITY_CHANGED:
  // Called when the viewer_visible_state_ has changed.
  void handle_visibility_changed( bool visible );

};

void AbstractViewerPrivate::set_texture( Texture2DHandle texture, bool delay_update )
{
  {
    AbstractViewer::lock_type lock( this->viewer_->get_mutex() );
    this->texture_ = texture;
  }

  if ( !delay_update )
  {
    this->viewer_->update_display_signal_();
  }
}

void AbstractViewerPrivate::set_overlay_texture( Texture2DHandle texture, bool delay_update )
{
  {
    AbstractViewer::lock_type lock( this->viewer_->get_mutex() );
    this->overlay_texture_ = texture;
  }

  if ( !delay_update )
  {
    this->viewer_->update_display_signal_();
  }
}

void AbstractViewerPrivate::handle_visibility_changed( bool visible )
{
  if ( visible )
  {
    this->renderer_->activate();
    this->renderer_->redraw_all();
  }
  else
  {
    this->renderer_->deactivate();
  }
}

//////////////////////////////////////////////////////////////////////////
// Class AbstractViewer
//////////////////////////////////////////////////////////////////////////

AbstractViewer::AbstractViewer( size_t viewer_id ) :
  StateHandler( std::string( "viewer" ) + Core::ExportToString( viewer_id ), false ),
  private_( new AbstractViewerPrivate )
{
  // ensure these are initialized
  this->private_->width_ = 0;
  this->private_->height_ = 0;
  this->private_->viewer_ = this;
  
  this->add_state( "viewer_visible", this->viewer_visible_state_, false );
  this->viewer_visible_state_->set_session_priority( StateBase::DO_NOT_LOAD_E );
}

AbstractViewer::~AbstractViewer()
{
  this->disconnect_all();
}

size_t AbstractViewer::get_viewer_id() const
{
  return this->get_statehandler_id_number();
}

void AbstractViewer::resize( int width, int height )
{
  this->private_->width_ = width;
  this->private_->height_ = height;
  if ( this->private_->renderer_ )
  {
    this->private_->renderer_->resize( width, height );
  }
}

void AbstractViewer::install_renderer( AbstractRendererHandle renderer )
{
  {
    lock_type lock( this->get_mutex() );

    if ( !renderer )
    {
      CORE_THROW_LOGICERROR( "Invalid renderer given" );
    }

    if ( this->private_->renderer_ )
    {
      CORE_THROW_LOGICERROR( "Cannot install renderer twice into viewer" );
    }

    this->private_->renderer_ = renderer;
  }

  {
    Core::StateEngine::lock_type lock( Core::StateEngine::GetMutex() );

    this->private_->handle_visibility_changed( this->viewer_visible_state_->get() );
    this->add_connection( this->viewer_visible_state_->value_changed_signal_.connect(
      boost::bind( &AbstractViewerPrivate::handle_visibility_changed, this->private_, _1 ) ) );
  }

  this->add_connection( this->private_->renderer_->redraw_completed_signal_.connect(
    boost::bind( &AbstractViewerPrivate::set_texture, this->private_, _1, _2 ) ) );

  this->add_connection( this->private_->renderer_->redraw_overlay_completed_signal_.connect(
    boost::bind( &AbstractViewerPrivate::set_overlay_texture, this->private_, _1, _2 ) ) );
}

void AbstractViewer::mouse_move_event( const MouseHistory& mouse_history, 
                    int button, int buttons, int modifiers )
{
  // Do nothing
}

void AbstractViewer::mouse_press_event( const MouseHistory& mouse_history, 
                     int button, int buttons, int modifiers )
{
  // Do nothing
}

void AbstractViewer::mouse_release_event( const MouseHistory& mouse_history, 
                     int button, int buttons, int modifiers )
{
  // Do nothing
}

void AbstractViewer::mouse_enter_event( int x, int y )
{
  // Do nothing
}

void AbstractViewer::mouse_leave_event()
{
  // Do nothing
}

bool AbstractViewer::wheel_event( int delta, int x, int y, int buttons, int modifiers )
{
  // Do nothing
  return false;
}

bool AbstractViewer::key_press_event( int key, int modifiers, int x, int y )
{
  // do nothing
  return false;
}

bool AbstractViewer::key_release_event( int key, int modifiers, int x, int y )
{
  // do nothing
  return false;
}

Texture2DHandle AbstractViewer::get_texture()
{
  lock_type lock( this->get_mutex() );
  return this->private_->texture_;
}

Texture2DHandle AbstractViewer::get_overlay_texture()
{
  lock_type lock( this->get_mutex() );
  return this->private_->overlay_texture_;
}

int AbstractViewer::get_width() const
{
  lock_type lock( this->get_mutex() );
  return this->private_->width_;
}

int AbstractViewer::get_height() const
{
  lock_type lock( this->get_mutex() );
  return this->private_->height_;
}

void AbstractViewer::set_cursor( CursorShape cursor )
{
  lock_type lock( this->get_mutex() );
  if ( this->private_->cursor_ != cursor )
  {
    this->private_->cursor_ = cursor;
    lock.unlock();
    this->update_display_signal_();
  }
}

CursorShape AbstractViewer::get_cursor() const
{
  lock_type lock( this->get_mutex() );
  return this->private_->cursor_;
}

} // end namespace Core

