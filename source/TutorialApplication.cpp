//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/10/28
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN 1
#define mMoveDirection_UP   2
#define mMoveDirection_LEFT 3
#define mMoveDirection_RIGHT 4


BasicTutorial_00::BasicTutorial_00(void) : mMoveDirection(mMoveDirection_NONE) {}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{

}

// reset positions of all small spheres
void BasicTutorial_00::reset()
{
	mSceneMgr->clearScene();
	createScene();
}

void BasicTutorial_00::createSpace()
{
    String name_en;
    String name_sn;

	int edge = 10;
	int gap = 40;
	int temp[10][10] = {
		{Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle},
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Obstacle,	Empty,		Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Empty,		Obstacle},
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Empty,		Obstacle,	Empty,		Robot,		Obstacle,	Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Obstacle,	Empty,		Empty,		Obstacle,	Empty},
		{Obstacle,	Empty,		Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Empty,		Obstacle},
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle},
	};
	
    for (int i = 0; i < edge; ++i ) {
		for (int j = 0; j < edge; ++j ) {
			int index = 10 * i + j;
			map[i][j] = temp[i][j];
			int objectType = map[i][j];
			int x = (j - edge / 2) * gap;
			int z = (i - edge / 2) * gap;

			switch(objectType) {
				case Obstacle:
					genNameUsingIndex("E_", index, name_en);
					genNameUsingIndex("S_", index, name_sn);
					mObstacleEntity[index] = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
					mObstacle[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn, Vector3(x, 0, z));
					mObstacle[index]->attachObject(mObstacleEntity[index]);
					mObstacle[index]->scale(6.0, 6.0, 6.0);
					break;
				case Robot:
					mRobotEntity = mSceneMgr->createEntity("robot_entity", "robot.mesh" );
					mRobot = mSceneMgr->getRootSceneNode()->createChildSceneNode("robot_scene_node", Vector3(x, 0, z));
					mRobot->attachObject(mRobotEntity);
					break;
			}
		}
    }
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.3, 0.3, 0.3);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.25, 0.25, 0.25);		
	light->setSpecularColour(0.35, 0.35, 0.35);	

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");


	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

    ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);

    createSpace();
}

void BasicTutorial_00::createScene_01(void) 
{

}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	//mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyPressed ***\n";
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

	mMoveDirection = mMoveDirection_NONE;

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    Vector3 mdir = Vector3(0.0, 0.0, 0.0);
    if (mMoveDirection == mMoveDirection_UP ) {
        mdir += Vector3(0.0, 0.0, -5.0);
    }
	
    if (mMoveDirection == mMoveDirection_DOWN ) {
        mdir += Vector3(0.0, 0.0, 5.0);
    }
	
    if (mMoveDirection == mMoveDirection_LEFT ) {
        mdir += Vector3(-5.0, 0.0, 0.0);
    }
	
    if (mMoveDirection == mMoveDirection_RIGHT ) {
        mdir += Vector3(5.0, 0.0, 0.0);
    }
    //
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
