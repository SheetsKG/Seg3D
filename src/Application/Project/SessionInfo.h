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

#ifndef APPLICATION_PROJECT_SESSIONINFO_H
#define APPLICATION_PROJECT_SESSIONINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// STL includes
#include <string>
#include <vector>

// boost includes
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

namespace Seg3D
{

// Forward declaration
typedef long long SessionID;

class SessionInfo;
typedef std::vector< SessionInfo > SessionInfoList;
typedef boost::shared_ptr< SessionInfoList > SessionInfoListHandle;

/// CLASS SessionInfo
/// This helper class is for storing sessions in the database
class SessionInfo
{
public:
  typedef boost::posix_time::ptime timestamp_type;

  SessionInfo( SessionID session_id, const std::string& session_name, 
    const std::string& user_id, const timestamp_type& timestamp );
  SessionInfo();
  ~SessionInfo();
  
  const SessionID session_id() const
  {
    return this->session_id_;
  }

  const std::string& session_name() const
  {
    return this->session_name_;
  }

  const std::string& user_id() const
  {
    return this->user_id_;
  }

  const timestamp_type& timestamp() const
  {
    return this->timestamp_;
  }
  
private:
  /// ID of the session
  SessionID session_id_;
  /// Name of the session
  std::string session_name_;
  /// User that saved the session
  std::string user_id_;
  /// Timestamp of when the session was saved in UTC time
  timestamp_type timestamp_;
};  

} // end namespace Seg3D

#endif
