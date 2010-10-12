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

#ifndef APPLICATION_FILTERS_ACTIONS_ACTIONDILATEFILTER_H
#define APPLICATION_FILTERS_ACTIONS_ACTIONDILATEFILTER_H

#include <Core/Action/Actions.h>
#include <Core/Interface/Interface.h>
#include <Application/Layer/Layer.h>

namespace Seg3D
{

class ActionDilateFilter : public Core::Action
{

CORE_ACTION( 
	CORE_ACTION_TYPE( "DilateFilter", "Filter that dilates a segmentation by a certain pixel radius." )
	CORE_ACTION_ARGUMENT( "layerid", "The layerid on which this filter needs to be run." )
	CORE_ACTION_KEY( "replace", "true", "Replace the old layer (true), or add an new mask layer (false)." )
	CORE_ACTION_KEY( "radius", "2", "The distance over which the filter dilates a mask." )
)
	
	// -- Constructor/Destructor --
public:
	ActionDilateFilter()
	{
		// Action arguments
		this->add_argument( this->target_layer_ );
		
		// Action options
		this->add_key( this->replace_ );		
		this->add_key( this->radius_ );
	}
	
	virtual ~ActionDilateFilter()
	{
	}
	
	// -- Functions that describe action --
public:
	virtual bool validate( Core::ActionContextHandle& context );
	virtual bool run( Core::ActionContextHandle& context, Core::ActionResultHandle& result );
	
	// -- Action parameters --
private:

	Core::ActionParameter< std::string > target_layer_;
	Core::ActionParameter< bool > replace_;
	Core::ActionParameter< int > radius_;
	
	// -- Dispatch this action from the interface --
public:

	// DISPATCH:
	// Create and dispatch action that inserts the new layer 
	static void Dispatch( Core::ActionContextHandle context, 
		std::string target_layer, bool replace, int radius );
					
};
	
} // end namespace Seg3D

#endif