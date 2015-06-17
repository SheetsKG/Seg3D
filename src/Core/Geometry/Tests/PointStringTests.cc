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

#include <gtest/gtest.h>

#include <Core/Geometry/Point.h>

using namespace Core;

class PointStringTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    sample.x(5.0); sample.y(-3.14); sample.z(0.879);
    sampleF.z(5.0f); sampleF.y(-3.14f); sampleF.z(0.879f);

    pointsVector.push_back( Point(1.0, 0, 0) );
    pointsVector.push_back( Point(0, 1.0, 0) );
    pointsVector.push_back( Point(0, 0, 1.0) );

    pointsVector2D_1.push_back( pointsVector );
    pointsVector2D_2.push_back( pointsVector );
    std::vector<Point> pointsVector2;
    pointsVector2.push_back( Point(2.0, 0, 0) );
    pointsVector2.push_back( Point(0, 2.0, 0) );
    pointsVector2.push_back( Point(0, 0, 2.0) );
    pointsVector2D_2.push_back( pointsVector2 );

    pointsFVector.push_back( PointF(1.0f, 0, 0) );
    pointsFVector.push_back( PointF(0, 1.0f, 0) );
    pointsFVector.push_back( PointF(0, 0, 1.0f) );

    pointsFVector2D_1.push_back( pointsFVector );
    pointsFVector2D_2.push_back( pointsFVector );
    std::vector<PointF> pointsFVector2;
    pointsFVector2.push_back( PointF(2.0f, 0, 0) );
    pointsFVector2.push_back( PointF(0, 2.0f, 0) );
    pointsFVector2.push_back( PointF(0, 0, 2.0f) );
    pointsFVector2D_2.push_back( pointsFVector2 );

    pointsString2D_2 = "[[[1.0,0,0],[0,1.0,0],[0,0,1.0]]|[[2.0,0,0],[0,2.0,0],[0,0,2.0]]]";
  }

  Point zero;
  PointF zeroF;

  Point sample;
  PointF sampleF;

  std::vector<Point> pointsVector;
  std::vector< std::vector<Point> > pointsVector2D_1;
  std::vector< std::vector<Point> > pointsVector2D_2;

  std::vector<PointF> pointsFVector;
  std::vector< std::vector<PointF> > pointsFVector2D_1;
  std::vector< std::vector<PointF> > pointsFVector2D_2;
  std::string pointsString2D_2;
};

TEST_F(PointStringTests, exportZeroPoint)
{
  std::string result = ExportToString( zero );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportZeroPointF)
{
  std::string result = ExportToString( zeroF );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportBasicPoint)
{
  std::string result = ExportToString( sample );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportBasicPointF)
{
  std::string result = ExportToString( sampleF );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportBasicPointNoDecimals)
{
  std::string result = ExportToString( Point(5, -3, 1) );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportBasicPointFNoDecimals)
{
  std::string result = ExportToString( PointF(5, -3, 1) );
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportPointsVector)
{
  std::string result = ExportToString(pointsVector);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportPointsFVector)
{
  std::string result = ExportToString(pointsFVector);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportPointsVector2DOneEntry)
{
  std::string result = ExportToString(pointsVector2D_1);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportPointsFVectorOneEntry)
{
  std::string result = ExportToString(pointsFVector2D_1);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}


TEST_F(PointStringTests, exportPointsVector2DTwoEntries)
{
  std::string result = ExportToString(pointsVector2D_2);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, exportPointsFVector2DTwoEntries)
{
  std::string result = ExportToString(pointsFVector2D_2);
  ASSERT_FALSE( result.empty() );
  std::cerr << result << std::endl;
}

TEST_F(PointStringTests, importPointsVector2DTwoEntries)
{
  std::vector< std::vector<double> > values;
  ImportFromString(pointsString2D_2, values);
  ASSERT_EQ( values.size(), 2 );
  for( auto v: values )
  {
    std::ostringstream oss;
    oss << "[ ";
    std::for_each(v.begin(), v.end(), [&oss](double x){ oss << x << " "; });
    oss << "]";
    std::cerr << oss.str() << std::endl;
  }
}


TEST_F(PointStringTests, importPointsFVector2DTwoEntries)
{
  std::vector< std::vector<float> > values;
  ImportFromString(pointsString2D_2, values);
  ASSERT_EQ( values.size(), 2 );
  for( auto v: values )
  {
    std::ostringstream oss;
    oss << "[ ";
    std::for_each(v.begin(), v.end(), [&oss](float x){ oss << x << " "; });
    oss << "]";
    std::cerr << oss.str() << std::endl;
  }
}