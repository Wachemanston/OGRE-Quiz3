//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: Yu-Shu Li
\n
My ID: 310552024
\n
My Email: yushuli.cs10@nycu.edu.tw
\n Date: 2021/11/22

This is an assignment of 3D Game Programming

*/

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include <stdio.h>
#include "BaseApplication.h"

using namespace Ogre;

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	virtual void createViewports(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void chooseSceneManager(void);
    //
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
    //
protected:
	/*!
	\brief Create a viewport

	Create a viewport for the entire screen.

	\return The sum of two integers.
	*/
	void createViewport_00(void);
	void createViewport_01(void);
	//
	void createCamera_00();
	void createCamera_01();

	void createScene_00();
	void createScene_01();
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    void createSpace();

    void reset();
protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];

    int mMoveDirection;
    SceneNode *mRobot;
    Entity *mRobotEntity;

    SceneNode *mSphere[64];
    Entity *mSphereEntity[64];

    SceneNode *mObstacle[64];
    Entity *mObstacleEntity[64];

	enum ObjectType { Empty = 0, Obstacle = 1, Robot = 2, Sphere = 3 };
	int map[10][10];
};

#endif // #ifndef __BasicTutorial_00_h_