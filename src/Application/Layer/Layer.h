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

#ifndef APPLICATION_LAYER_LAYER_H
#define APPLICATION_LAYER_LAYER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// STL includes
#include <string>
#include <vector>

// Boost includes 
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

// Core includes
#include <Core/Action/Action.h>
#include <Core/Application/Application.h>
#include <Core/Interface/Interface.h>
#include <Core/Resource/ResourceLock.h>
#include <Core/State/State.h>
#include <Core/Volume/Volume.h>

// Application includes
#include <Application/Layer/LayerFWD.h>

namespace Seg3D
{

// CLASS Layer
// This is the main class for collecting state information on a layer


// Class definition
class Layer : public Core::StateHandler
{

  // -- Constructor/destructor --
protected:
  // NOTE: Use the specific class to build the layer
  Layer( const std::string& name );
  Layer( const std::string& name, const std::string& state_id );
  virtual ~Layer();

private:
  // INITIALIZE_STATES:
  // This function sets the initial states of the layer, and returns true if successful
  void initialize_states( const std::string& name );

  // -- Layer properties --
public:

  // TYPE
  // Get the type of the layer
  virtual Core::VolumeType type() const = 0;

  // GRID_TRANSFORM
  // Get the transform of the layer
  virtual const Core::GridTransform& get_grid_transform() const = 0;

  // -- layer progress signals --
public:
  // NOTE: Layers can be locked and unlock to use as a source 
  // (No alterations are made) or layers can be locked for filtering
  // in which case progress on the new layer is reported as well
  
  typedef boost::signals2::signal< void () > lock_signal_type;
  typedef boost::signals2::signal< void () > unlock_signal_type;
  
  typedef boost::signals2::signal< void () > start_progress_signal_type;
  typedef boost::signals2::signal< void () > finish_progress_signal_type;
  typedef boost::signals2::signal< void (double) > update_progress_signal_type;

  // LOCK_SIGNAL:
  // The layer is being locked
  lock_signal_type lock_signal_;
  
  // UNLOCK_SIGNAL:
  // The layer is being unlocked
  unlock_signal_type unlock_signal_;
  

  // START_PROGRESS_SIGNAL:
  // This signal is raised when a filter starts updating the layer
  start_progress_signal_type start_progress_signal_;
  
  // FINISH_PROGRESS_SIGNAL:
  // This signal is raised when a filter finishes updating a layer
  finish_progress_signal_type finish_progress_signal_;
  
  // UPDATE_PROGRESS:
  // When new information on progress is available this signal is triggered.
  update_progress_signal_type update_progress_signal_;
  
  // -- layer filter abort support --
public:
  // RAISE_ABORT:
  // Abort the filter that is working on this layer
  void raise_abort();
  
  // CHECK_ABORT:
  // Check whether the abort flag was raised, it was reset by start_progress
  bool check_abort();
  
private:
  bool abort_;

  // -- Layer Locking system --
public:
  
  // GET_RESOURCE_LOCK:
  // This function returns the resource lock that ensures that an action
  // cannot be executed on a resource that is being computed.
  Core::ResourceLockHandle get_resource_lock();
    
private:
  Core::ResourceLockHandle resource_lock_;
  
  // -- State variables --
public:
  
  // The name of the layer
  Core::StateNameHandle name_state_;

  // Per viewer state of whether this layer is visible
  std::vector< Core::StateBoolHandle > visible_state_;

  // State indicating whether the layer is locked
  Core::StateBoolHandle lock_state_;

  // State that describes the opacity with which the layer is displayed
  Core::StateRangedDoubleHandle opacity_state_;

  // State of the checkbox that records which layer needs to be processed in
  // the group
  Core::StateBoolHandle selected_state_;

  // State that describes which menu is currently shown
  Core::StateOptionHandle edit_mode_state_;

protected:
  // State that stores the generation of its datablock
  Core::StateIntHandle generation_state_;


  // -- Accessors --
public:

  // GET_LAYER_GROUP:
  // Get a handle to the group this layer belongs to
  LayerGroupHandle get_layer_group();
  
  // SET_LAYER_GROUP:
  // Set the group this layer is contained within
  void set_layer_group( LayerGroupWeakHandle layer_group );
  
  // TODO: Need to check whether we really need to store whether layer is active here,
  // as it duplicates state information and allows for synchronization problems
  // --JS
  
  bool get_active();
  void set_active( bool active );
  
  // GET_LAYER_ID:
  // Get the id of this layer
  std::string get_layer_id() const;

  // GET_LAYER_NAME:
  // Get the name of the layer
  std::string get_layer_name() const;
  
private:  
  // The unique ID of the layer

  LayerGroupWeakHandle layer_group_;

  // TODO: At some point we should move this one
  bool active_;

  // -- Locking system --
public:
  // This code just aligns the layer locking with the StateEngine locking
  typedef Core::StateEngine::mutex_type mutex_type;
  typedef Core::StateEngine::lock_type lock_type;
  
  // GETMUTEX:
  // Get the mutex of the state engine
  static mutex_type& GetMutex();
  
};

} // end namespace Seg3D

#endif
