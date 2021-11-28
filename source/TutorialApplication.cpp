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
#include "math.h"
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <array>

using namespace Ogre;
using namespace std;

const float PI = 3.141592654;

#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN 1
#define mMoveDirection_UP   2
#define mMoveDirection_LEFT 3
#define mMoveDirection_RIGHT 4


BasicTutorial_00::BasicTutorial_00(void) : mMoveDirection(mMoveDirection_NONE), edge(10) {}

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
	mCamera->setPosition(Ogre::Vector3(0,300,0));
	mCamera->lookAt(Ogre::Vector3(0,0,0.001));
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

#define COL 10
#define ROW 10

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;
 
// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
 
// A structure to hold the necessary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};
 
// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool BasicTutorial_00::isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}
 
// A Utility Function to check whether the given cell is
// blocked or not
bool BasicTutorial_00::isUnBlocked(int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (map[row][col] != Obstacle)
        return (true);
    else
        return (false);
}
 
// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}
 
// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula
	return ((double)Ogre::Math::Sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}
 
// A Utility Function to trace the path from the source
// to destination
void tracePath(cell cellDetails[][COL], Pair dest)
{
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;
 
    stack<Pair> Path;
 
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }
 
    return;
}
 
// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void BasicTutorial_00::aStarSearch()
{
	Vector2 goal = posToGrid(targetPos);
	Vector2 start = posToGrid(mRobot->getPosition());
	Pair src = make_pair(start.x, start.y);
	Pair dest = make_pair(goal.x, goal.y);
	std::cout << "start = (" << start.x << ", " << start.y << ") = " << map[(int) start.x][(int) start.y] << std::endl;
	std::cout << "goal = (" << goal.x << ", " << goal.y << ") = " << map[(int) goal.x][(int) goal.y] << std::endl;

    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }
 
    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }
 
    // Either the source or the destination is blocked
    if (isUnBlocked(src.first, src.second) == false
        || isUnBlocked(dest.first, dest.second)
               == false) {
        printf("Source or the destination is blocked\n");
        return;
    }
 
    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }
 
    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
 
    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[ROW][COL];
 
    int i, j;
 
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
 
    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
 
    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair of
     pair.*/
    std::set<pPair> openList;
 
    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
 
    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;
 
    while (!openList.empty()) {
        pPair p = *openList.begin();
 
        // Remove this vertex from the open list
        openList.erase(openList.begin());
 
        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
 
        /*
         Generating all the 8 successor of this cell
 
             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \
                /   |  \
             S.W    S   S.E
 
         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West           (i, j-1)*/
 
        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;
 
        //----------- 1st Successor (North) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i - 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j] == false
                     && isUnBlocked(i - 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn・t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));
 
                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 2nd Successor (South) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i + 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j] == false
                     && isUnBlocked(i + 1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn・t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 3rd Successor (East) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j + 1] == false
                     && isUnBlocked(i, j + 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;
 
                // If it isn・t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));
 
                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 4th Successor (West) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j - 1] == false
                     && isUnBlocked(i, j - 1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;
 
                // If it isn・t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));
 
                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
    }
 
    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");
 
    return;
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
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Obstacle,	Empty,		Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Empty,		Obstacle},
		{Obstacle,	Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Empty,		Obstacle},
		{Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle,	Obstacle},
	};
	
    for (int i = 0; i < edge; ++i ) {
		for (int j = 0; j < edge; ++j ) {
			int index = edge * i + j;
			map[i][j] = static_cast<int>(temp[i][j]);
			int objectType = map[i][j];
			int x = (j - edge / 2) * gap;
			int z = (i - edge / 2) * gap;

			switch(objectType) {
				case Obstacle:
					genNameUsingIndex("E_", index, name_en);
					genNameUsingIndex("S_", index, name_sn);
					mObstacleEntity[index] = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
					mObstacle[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn, Vector3(x + gap / 2, 0, z + gap / 2));
					mObstacle[index]->attachObject(mObstacleEntity[index]);
					mObstacle[index]->scale(6.0, 6.0, 6.0);
					break;
				case Robot:
					mRobotEntity = mSceneMgr->createEntity("robot_entity", "robot.mesh" );
					mRobot = mSceneMgr->getRootSceneNode()->createChildSceneNode("robot_scene_node", Vector3(x + gap / 2, 0, z + gap / 2));
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
			"plane",
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

void BasicTutorial_00::printMap(void) {
	int edge = 10;
	for (int i = 0; i < edge; i++) {
		for (int j = 0; j < edge; j++) {
			std::cout << std::setw(8) << map[i][j];
		}
		std::cout << std::endl;
	}
}

/*void updateMap(void) {
	Vector2 goal = posToGrid(targetPos);
	Vector2 start = posToGrid(mRobot->getPosition());
	int edge = 10;
	std::cout << "goal = (" << goal.x << ", " << goal.y << ")" << std::endl;
	std::cout << (int) goal.x << ", " << (int) goal.y << ", " << map[(int) goal.x][(int) goal.y] << std::endl;
	// If target map is not empty, return
	if (map[(int) goal.x][(int) goal.y] != Empty) {
		return;
	}

	// Update the weight of all empty map from destination
	for (int i = 0; i < edge; i++) {
		for (int j = 0; j < edge; j++) {
			if (map[i][j] == Empty) {
				map[i][j] = abs(goal.x - i) + abs(goal.y - j);
			}
		}
	}
	printMap();
	// Update the weight of all empty map from start with BFS
	int currentMinCost = 64;
	std::vector<Vector2> dir;
	dir.push_back(Vector2(1, 0));
	dir.push_back(Vector2(0, 1));
	dir.push_back(Vector2(-1, 0));
	dir.push_back(Vector2(0, -1));

	std::queue<std::pair<int, Vector2>> path;
	std::set<int> visited;
	path.push(std::pair<int, Vector2>(0, Vector2(start.x, start.y)));
	int iter = 1;
	while (!path.empty()) {
		std::pair<int, Vector2> current = path.front();
		path.pop();
		int curCost = current.first;
		Vector2 curPath = current.second;
		
		int x = static_cast<int>(curPath.x);
		int y = static_cast<int>(curPath.y);

		std::cout << "iter = " << iter << ", x=" << x << ",y =" << y << std::endl;
		iter++;

		map[x][y] += curCost;

		visited.insert(curPath.x * edge + curPath.y);
		for (int i = 0; i < dir.size(); i++) {
			int nextX = curPath.x + dir[i].x;
			int nextY = curPath.y + dir[i].y;
			if (map[nextX][nextY] == Empty && visited.count(nextX * edge + nextY) != 1) {
				path.push(std::pair<int, Vector2>(curCost + 1, Vector2(nextX, nextY)));
			}
		}
	}
	printMap();
}*/

Vector2 BasicTutorial_00::posToGrid(Vector3 pos) {
	int gap = 40;
	int edge = 10;
	return Vector2(floor(pos.x / gap) + edge / 2, floor(pos.z / gap) + edge / 2);
}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (id == OIS::MB_Left) {
		Ray mRay = mTrayMgr->getCursorRay(mCamera);
		std::pair<bool,Real> result = mRay.intersects(Plane(Vector3::UNIT_Y, 0));
		if (result.first) {
			targetPos = mRay.getPoint(result.second);
			isAnimation = true;
			std::cout << "target = " << targetPos.x << ", " << targetPos.z << ", " << std::endl;
			printMap();
			aStarSearch();
		}
	}

    return BaseApplication::mousePressed( arg, id );
}

bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg ) {
	if (mTrayMgr->injectMouseMove(arg)) return true;
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
    std::stringstream ss;
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
    std::stringstream ss;
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
    
	AnimationState* mAnimationState;
	if (isAnimation) {
		int stepUnit = 20;
		Entity* mEntity = static_cast<Entity*>(mRobot->getAttachedObject(0));
		mAnimationState = mEntity->getAnimationState("Walk");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(0.5 * evt.timeSinceLastFrame);

		Vector3 pos = mRobot->getPosition();
		Vector3 vec = targetPos - pos;
		Real length = vec.length();
		if (length > 1.0) {
			vec.normalise();
			mRobot->lookAt(targetPos, Node::TransformSpace::TS_WORLD);
			mRobot->yaw(Radian(90));
			mRobot->translate(evt.timeSinceLastFrame * stepUnit * vec);
		} else {
			isAnimation = false;
		}
    /*Vector3 mdir = Vector3(0.0, 0.0, 0.0);
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
    }*/
	} else {
		Entity* mEntity = static_cast<Entity*>(mRobot->getAttachedObject(0));
		mAnimationState = mEntity->getAnimationState("Idle");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
	}
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
