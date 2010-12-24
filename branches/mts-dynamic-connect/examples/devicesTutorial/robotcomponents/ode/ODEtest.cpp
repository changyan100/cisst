

// Need to include these first otherwise there's a mess with
//  #defines in cisstConfig.h
#include <osgGA/TrackballManipulator>

#include <cisstDevices/robotcomponents/osg/devOSGCamera.h>
#include <cisstDevices/robotcomponents/ode/devODEWorld.h>
#include <cisstDevices/robotcomponents/ode/devODEBody.h>

#include <cisstVector/vctAxisAngleRotation3.h>

#include <cisstMultiTask/mtsTaskManager.h>

#include <cisstCommon/cmnGetChar.h>

int main(){

  mtsTaskManager* taskManager = mtsTaskManager::GetInstance();

  // Create the world
  devODEWorld* world = new devODEWorld( 0.001, OSA_CPU1 );
  taskManager->AddComponent( world );

  // Create a rigid body. Make up some mass + com + moit
  vctFixedSizeVector<double,3> u( 0.780004, 0.620257, 0.082920 );
  u.NormalizedSelf();
  vctFrame4x4<double> Rt( vctAxisAngleRotation3<double>( u, 0.7391 ),
			  vctFixedSizeVector<double,3>( 0.0, 0.0, 1.0 ) );
  devODEBody* hubble;
  hubble = new devODEBody( "hubble",                              // name
			   Rt,                                    // pos+ori
			   "libs/etc/cisstRobot/objects/hst.3ds", // model
			   world,
			   1.0,                                   // mass
			   vctFixedSizeVector<double,3>( 0.0 ),   // com
			   vctFixedSizeMatrix<double,3,3>::Eye(), // moit
			   world->GetSpaceID() );

  // Add a background top
  devODEBody* background;
  background = new devODEBody( "background", 
			       vctFrame4x4<double>(),
			       "libs/etc/cisstRobot/objects/background.3ds",
			       world,
			       world->GetSpaceID() );

  // Add a camera
  int width = 640, height = 480;
  devOSGCamera* camera = new devOSGCamera( "camera",
					   world,
					   0, 0, width, height,
					   55, ((double)width)/((double)height),
					   0.01, 10.0 );
  // Add+configure the trackball of the camera
  camera->setCameraManipulator( new osgGA::TrackballManipulator );
  camera->getCameraManipulator()->setHomePosition( osg::Vec3d( 1,0,1 ),
						   osg::Vec3d( 0,0,0 ),
						   osg::Vec3d( 0,0,1 ) );
  camera->home();

  // add a bit more light
  osg::ref_ptr<osg::Light> light = new osg::Light;
  light->setAmbient( osg::Vec4( 0.9, 0.9, 0.9, 0.0 ) );
  camera->setLight( light );
  
  // Add the camera component
  taskManager->AddComponent( camera );

  taskManager->CreateAll();
  taskManager->StartAll();

  cmnGetChar();

  taskManager->KillAll();

  return 0;

}
