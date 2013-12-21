/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2013 Scientific Computing and Imaging Institute,
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

#ifndef APPLICATION_IMAGEREGISTRATIONTOOLS_FFTFILTER_H
#define APPLICATION_IMAGEREGISTRATIONTOOLS_FFTFILTER_H

#include <Application/Tool/Tool.h>

// Application includes
#include <Application/Tool/SingleTargetTool.h>

namespace Seg3D
{

class FFTFilter : public SingleTargetTool
{
  
SEG3D_TOOL(
  SEG3D_TOOL_NAME( "FFTFilter", "" )
  SEG3D_TOOL_MENULABEL( "ir-fft" )
  SEG3D_TOOL_MENU( "Advanced Filters" )
//  SEG3D_TOOL_SHORTCUT_KEY( "CTRL+ALT+R" )
//  SEG3D_TOOL_URL( "" )
  SEG3D_TOOL_VERSION( "1" )
)
  
public:
  FFTFilter( const std::string& toolid );
  virtual ~FFTFilter();
  
  // -- state --
public:
  
  // -- execute --
public:
  /// Execute the tool and dispatch the action
  virtual void execute( Core::ActionContextHandle context );
};
  
} // end namespace

#endif
