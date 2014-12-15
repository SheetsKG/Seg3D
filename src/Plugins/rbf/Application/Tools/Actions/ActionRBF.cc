/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2014 Scientific Computing and Imaging Institute,
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

#include <Core/Action/ActionFactory.h>

//#include <Application/Provenance/Provenance.h>
//#include <Application/ToolManager/ToolManager.h>
#include <Application/Layer/LayerManager.h>
#include <Application/Filters/LayerFilter.h>
#include <Core/Utils/Log.h>

#include <rbf/Application/Tools/Actions/ActionRBF.h>

// RBF library includes
#include <rbf/Application/Tools/src/Surface.h>
#include <rbf/Application/Tools/src/RBF.h>
#include <rbf/Application/Tools/src/ScatteredData.h>

// test
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
// test

CORE_REGISTER_ACTION( Seg3D, RBF )

namespace Seg3D
{

// TODO really worth having a private class here?
class ActionRBFPrivate
{
public:
  std::string target_layer_id_;
  SandboxID sandbox_;
};

vec3 findNormal(ScatteredData *data, int n)
{
  int tot = data->x[0].size();
  int prev = (n-1)>=0?n-1:tot-1;
  int next = (n+1)<tot?n+1:0;
  
  while(data->x[2][prev]!=data->x[2][n])
  {
    prev = (prev-1)>=0?prev-1:tot-1;
  }
  
  while(data->x[2][next]!=data->x[2][n])
  {
    next = (next+1)<tot?next+1:0;
  }
  printf("%d %d %d %d\n", prev,n,next,tot); fflush(stdout);
  
  vec3 a(data->x[0][n], data->x[1][n], data->x[2][n]);
  vec3 b(data->x[0][prev], data->x[1][prev], data->x[2][prev]);
  vec3 c(data->x[0][next], data->x[1][next], data->x[2][next]);
  vec3 one = b-a;
  vec3 two = c-a;
  vec3 ret = one+two;
  return ret;
}

void augmentNormalData(ScatteredData *data)
{
  int n = data->x[0].size();
  for(int i=0; i<n; i++)
  {
    vec3 myNormal = findNormal(data, i);
    myNormal = normalize(myNormal);
    for(int j=0; j<3; j++)
    {
      data->x[j].push_back(data->x[j][i] + myNormal[j]);
    }
    data->fnc.push_back(10);
    
    for(int j=0; j<3; j++)
    {
      data->x[j].push_back(data->x[j][i] - myNormal[j]);
    }
    data->fnc.push_back(-10);
  }
}

class RBFAlgo : public LayerFilter
{
  
public:
  LayerHandle src_layer_;
  LayerHandle dst_layer_;

	RBFAlgo();
	virtual ~RBFAlgo();
  
  SCI_BEGIN_RUN()
  {
    //Take the input
    ScatteredData *mySurfaceData = new ScatteredData();
    
    //	readSurfaceDataFile("./segmentation/sample_points_DEMRI.txt", mySurfaceData);
    std::vector<Surface*> mySurface;
    // file contents (sample data)
    double myData[][3] =
    { {-0.009749965466282262, -9.912165437127271, 87.26},
      {20.374263742141693, -22.382373895334435, 87.26},
      {15.817837148676354, -36.531264261377174, 87.26},
      {5.745736257858331, -42.28674508824203, 87.26},
      {-6.484671966706447, -39.88862807704833, 87.26},
      {-14.158653597805905, -29.81653663003486, 87.26},
      {-12.00034626405919, -20.463880286379492, 87.26},
      {-7.923543522537589, -13.029717551679063, 84.76},
      {7.184607813689474, -10.391788839366003, 84.76},
      {15.338213296732652, -14.22877605727591, 84.76},
      {19.894639890197954, -27.658231319960535, 84.76},
      {11.021598629239204, -39.88862807704833, 84.76},
      {-1.2088095953255547, -41.80712168600328, 84.76},
      {-12.240158190031039, -36.051640859138445, 84.76},
      {-15.117901301693347, -26.21936111324434, 84.76},
      {-11.280910486143618, -16.147269666230855, 84.76} };
    
    for (int j = 0; j < 16; j++)
    {
      for(int i=0; i<3; i++)
        mySurfaceData->x[i].push_back(myData[j][i]);
      mySurfaceData->fnc.push_back(0);
    }
    // ???
    for(int i=0; i<3; i++)
      mySurfaceData->x[i].pop_back();
    mySurfaceData->fnc.pop_back();
    std::cout<<"Done"<<std::endl;
    std::cout<<"Augmenting data"<<std::endl;
    augmentNormalData(mySurfaceData);
    std::cout<<"Done"<<std::endl;
    
    RBF *mySurfaceRBF;
    Kernel myKernel = ThinPlate;
    
    mySurfaceRBF = new RBF(mySurfaceData, myKernel);
    mySurfaceRBF->setDataReduction(Random);
    
    mySurface.push_back(new Surface(mySurfaceData, mySurfaceRBF));
    
    
    //Construct RBFs
    mySurface[0]->computeRBF();
    
    
    //Compute material
    std::vector<std::vector<std::vector<double> > > value;
    value.resize(100);
    vec3 myOrigin(-30, -50, 80);
    for(int i=0; i<100; i++)
    {
      if(i%10==0)
        printf("%d/100 done\n", i); fflush(stdout);
      value[i].resize(100);
      for(int j=0; j<100; j++)
      {
        //if(j%10==0)
        //	printf("\t%d/100 done\n", j); fflush(stdout);
        value[i][j].resize(100);
        for(int k=0; k<100; k++)
        {
          //if(k%10==0)
          //	printf("\t\t%d/100 done\n", k); fflush(stdout);
          vec3 location = myOrigin + 0.6*i*vec3::unitX + 0.5*j*vec3::unitY + 0.1*k*vec3::unitZ;
          //std::cout<<"Computing Val ... "<<std::endl;
          double myVal = mySurface[0]->computeValue(location);
          //printf("Interpolant: %lf %lf %lf %lf\n", location[0], location[1], location[2], myVal); fflush(stdout);
          value[i][j][k]=myVal;
        }
      }
    }
    
    vec3 dim(100,100,100);
    vec3 spacing(0.6,0.5,0.1);
    std::string filename = "surface.nrrd";
    //	writeNrrdFile(filename, value, dim, spacing);
    
    std::cout << "Writing file '" << filename << "'" << std::endl;
    std::ofstream nrrd_file(filename.c_str(), std::ofstream::binary);
    
    if(nrrd_file.is_open())
    {
      nrrd_file << "NRRD0001" << std::endl;
      nrrd_file << "# Complete NRRD file format specification at:" << std::endl;
      nrrd_file << "# http://teem.sourceforge.net/nrrd/format.html" << std::endl;
      nrrd_file << "type: float" << std::endl;
      nrrd_file << "dimension: 3" << std::endl;
      nrrd_file << "sizes: " << dim[0] << " " << dim[1] << " " << dim[2] << std::endl;
      nrrd_file << "axis mins: " << 0 << ", " << 0 << ", " << 0 << std::endl;
      nrrd_file << "spacings: " << spacing[0] << " " << spacing[1] << " " << spacing[2] << std::endl;
      nrrd_file << "centerings: cell cell cell" << std::endl;
      nrrd_file << "endian: little" << std::endl;
      nrrd_file << "encoding: raw" << std::endl;
      nrrd_file << std::endl;
      
      // write data portion
      for(int k=0; k < dim[2]; k++)
      {
        for(int j=0; j < dim[1]; j++)
        {
          for(int i=0; i < dim[0]; i++)
          {
            double val = value[i][j][k];
            nrrd_file.write((char*)&val, sizeof(float));
          }
        }
      }
      
      nrrd_file.close();
    }
  }
  SCI_END_RUN()
  
  // GET_FITLER_NAME:
  // The name of the filter, this information is used for generating new layer labels.
  virtual std::string get_filter_name() const
  {
    return "RBF Tool";
  }
  
  // GET_LAYER_PREFIX:
  // This function returns the name of the filter. The latter is prepended to the new layer name,
  // when a new layer is generated.
  virtual std::string get_layer_prefix() const
  {
    return "RBF";	
  }	
};

RBFAlgo::RBFAlgo()
{
}

RBFAlgo::~RBFAlgo()
{
}

ActionRBF::ActionRBF() :
  private_( new ActionRBFPrivate )
{
  this->add_parameter( this->private_->sandbox_ );
}

bool ActionRBF::validate( Core::ActionContextHandle& context )
{
  return true;
}

bool ActionRBF::run( Core::ActionContextHandle& context, Core::ActionResultHandle& result )
{
	boost::shared_ptr< RBFAlgo > algo( new RBFAlgo() );
  
	// Set up parameters
	algo->set_sandbox( this->private_->sandbox_ );
	
	// Build the undo-redo record
	algo->create_undo_redo_and_provenance_record( context, this->shared_from_this(), true );
  
	// Start the filter.
	Core::Runnable::Start( algo );
  
	return true;
}

void ActionRBF::Dispatch( Core::ActionContextHandle context)
{
  ActionRBF* action = new ActionRBF;
  
  Core::ActionDispatcher::PostAction( Core::ActionHandle( action ), context );
}

} // end namespace Seg3D
