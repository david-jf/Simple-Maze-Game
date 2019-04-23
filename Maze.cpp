#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#define M_PI 3.14159265358979323846 // Window doesn't seem to define this
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
#include <ctime> //clock
//#include <tgmath.h>
#include <fstream>
using namespace std;


/* 
 
 Notes for Mac Users
 
 windows.h is not needed
 conio.h is not needed (for Mac or Windows users)
 
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
 #include <math.h>
 
 Your program must use (also works with Windows)
 
 int main(int argc, char** argv)
 {
    glutInit( &argc, argv );
 
 
 Comment out the #define M_PI 3.14159265358979323846 since it is already present
 
 
 When compiling you need to include the appropriate frameworks:
 
 clang++ Circle.cpp -framework GLUT -framework OpenGL -o Circle -Wno-deprecated-declarations
 
 The -Wno-deprecation-declarations keep from displaying warnings that GLUT is outdated.
 
 
*/


///to compile: clang++ -lGL -lglut -lGLU Assignment2.cpp -o FILENAMEHERE

/*
David Fountain
ID: 140203
CMPT315
Assignment 2
*/

double angle = 0.0; //red target spin angle
double forward_back = 0.0;

double left_right = 0.0;
bool fill_wall = true; //maze begins with the walls filled in
double turnAngle = 0.0; //declares how many degrees to turn each time a side arrow is pushed
//double increment_x = 0.0;
//double increment_z = 0.0;
//double projCube_x = 0.0;
//double projCube_z = 0.0;
bool projectile = false; //indicator for when a projectile is thrown
bool wall_restrictions = true;
int time_to_live = 0;
int turn = 0;
double lastPosX = 0.0;
double lastPosZ = 0.0;

//initialized maze level
int wallArray[14][14]={
	 {0,0,0,0,0,0,0,0,0,1,1,1,1,1}
	,{0,0,0,1,1,0,0,0,0,0,0,0,2,1}
	,{0,0,0,0,0,0,0,0,0,0,0,0,0,1}
	,{0,0,0,0,0,0,0,0,0,1,2,0,0,0}
	,{0,0,0,1,1,1,1,0,0,1,1,1,1,0}
	,{0,0,0,1,0,0,0,0,0,0,0,2,1,0}
	,{0,0,0,1,0,0,0,0,0,0,0,0,1,0}
	,{0,0,0,0,0,0,1,1,1,1,0,0,1,0}
	,{0,0,0,0,0,0,1,2,0,1,0,0,1,0}
	,{0,0,0,0,0,0,1,0,0,1,0,0,1,2}
	,{0,0,0,0,0,0,1,0,0,1,0,0,1,1}
	,{0,0,0,0,0,0,0,0,0,0,0,0,0,1}
	,{0,0,0,0,0,0,0,0,0,0,0,0,0,1}
	,{0,0,1,1,1,1,1,1,1,1,1,1,1,1}};


//method for getting key presses from the keyboard
//uses ascii codes
void keyBoard(unsigned char key, int x, int y)
{
	//cout<<key<<endl;

	//listens for keypress
	//controls wireframe
	if(key == 'o' && fill_wall)
		fill_wall=false;
	else if(key == 'o' && !fill_wall)
		fill_wall=true;
	
	//controls wall passthrough
	if(key == 'w' && wall_restrictions)
		wall_restrictions=false;
	else if(key =='w' && !wall_restrictions)
		wall_restrictions=true;


	//controls projectile thrown
	if(key == 32)
	{
		//throwCube();
		projectile=true;
		time_to_live=30;

	}
}

//draws the walls of the maze
void walls()
{
	

  //double y_displace=0.8;
  //double x_displace=-0.9;
  for(int y=0; y<14; y++)
  {
    
    for(int x=0; x<14; x++)
    {

    	float wallColour[] = {1.0,1.0,0.0,1.0}; //green
    	float cubeColour[] = {1.0,0.0,0.0,1.0}; //red
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wallColour);
      glLoadIdentity();
      glRotatef(turnAngle, 0.0,1.0, 0.0);
      glTranslatef(left_right, 0, forward_back); //move direction
      

      //creates walls
      if(wallArray[y][x]==1)
      {  
       	glTranslatef(y,0.0,-x);
				glutSolidCube(1.0);
      }
      else if(wallArray[y][x]==2) //creates targets
      {
      	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cubeColour);
        //glLoadIdentity();
       	glTranslatef(y,0.0,-x);
       	glRotatef(angle, 0.0, 1.0, 0.0); //rotates the cube around
				glutSolidCube(0.5);
        //cout<<x_displace<<endl;
      }

      //if a cube has been thrown
      //will only be true when space bar is hit
      if(projectile)
		  {
			  float projectileColour[] = {0.0,0.0,1.0,1.0}; //blue
			  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, projectileColour);

			  //projCube_z+=cos(turnAngle/180*M_PI)*0.1;
    		//projCube_x+=-sin(turnAngle/180*M_PI)*0.1;

				//cout<<"testing"<<endl;
				
				
				//not working so placed and 0,0,0 in maze
				glTranslatef(0.0, 0.0, 0.0); //throws the cube in which ever way you're facing
				//cout<<projCube_x<<" "<<projCube_z<<endl;
				//cout<<left_right<<" "<<forward_back<<endl;
				glutSolidCube(0.2);
				//glTranslatef(-forward_back*3, 0.0, -left_right*3); //throws the cube in which ever way you're facing

				//determines how long the projectile will last for
				if(time_to_live<0)
					projectile=false; //resets the state
		  }
		  
    }
  }
}

//method that checks for collision with a wall if wall_restrictions is turned on
bool wallCollision()
{
	int x = 0;
	int z = 0;
	//if wall restricions is turned on
	if(wall_restrictions)
	{

    x = floor(abs(left_right)+0.6);
    z = floor(abs(forward_back+0.6));

    if(wallArray[x][z] == 1) //if the position hits a wall
    {
    	//cout<<"hit"<<endl;
    	//forward_back = lastPosZ;
    	//left_right = lastPosX;
    	return true;
    }
	}
	else{return false;}
	return false;

}

//method will get input from arrow keys
void directionKey(int key, int x, int y)
{
	
  if(key == GLUT_KEY_LEFT)
  {
    //cout<<"left"<<endl;
    
    turnAngle-=0.5; //changes the turnAngle by -0.5
    //turn=turn-1;
  	//cout<<turn<<endl;
  }
  else if(key ==GLUT_KEY_UP)
  {
    //cout<<"up"<<endl;
    forward_back+=cos(turnAngle/180*M_PI)*0.1; //not sure what this is doing but it works
    left_right+=-sin(turnAngle/180*M_PI)*0.1;

    //if a wall was hit
    if(wallCollision())
    {
    	//goes back in the ooposite direction
    	lastPosZ = forward_back-=cos(turnAngle/180*M_PI)*0.1;
  		lastPosX = left_right-=-sin(turnAngle/180*M_PI)*0.1;
    }
    else{}
    //int g = floor(abs(left_right));
    //cout<<"floor "<< g<<endl;
    //cout<<floor(abs(left_right))<<" "<<floor(abs(forward_back))<<endl;
    //cout<<cos(turnAngle/180*M_PI)*0.1<<",  "<<-sin(turnAngle/180*M_PI)*0.1<<endl;
  }
  else if(key == GLUT_KEY_RIGHT)
  {
  	//cout<<"right"<<endl;
  	//turn=turn+1;
  	//cout<<turn<<endl;
  	turnAngle+=0.5; //changes the turnAngle by 0.5

  }
  else if(key == GLUT_KEY_DOWN)
  {
    //cout<<"down"<<endl;
    forward_back-=cos(turnAngle/180*M_PI)*0.1;
    left_right-=-sin(turnAngle/180*M_PI)*0.1;

    //if a wall was hit
    if(wallCollision())
    {
    	//goes back one spot in the opposite direction
	    lastPosZ = forward_back+=cos(turnAngle/180*M_PI)*0.1;
	  	lastPosX = left_right+=-sin(turnAngle/180*M_PI)*0.1;
  	}
  	else{}

  }
  
}

//creates the lighting of the world and the different modes
void display(void)
{
	//controls the wireframe
	if(fill_wall)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //fill colour
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //outline

	//////making world view////////
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f); // background colour
	glClear(GL_COLOR_BUFFER_BIT); // clear the background
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1,0.1,-0.1,0.1,0.1,20); //uses frustum instead of orthographic
	glMatrixMode(GL_MODELVIEW); //going back into modelview

	///////lighting////////
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float position[] = {0.9,0.9,0.9,1.0};
	float value[] = {1.0,0.0,0.0,1.0};
	float light_colour[] = {1.0, 1.0, 1.0, 0.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_colour);//type and colour of light
	glLightfv(GL_LIGHT0, GL_POSITION, position); //position of the light source
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, value);

	walls(); //lays out the walls for the maze level

	glFlush(); // make sure the polygon is pushed to the graphics card
    
  glutSwapBuffers(); // flip the buffers so the polygon I just drew is now on the front buffer
}




//timer to rotate the cube
void timer(int value)
{
	angle+=8; //incrementation for red cube rotation

	//projCube_z+=cos(turnAngle/180*M_PI)*0.01;
  //projCube_x+=-sin(turnAngle/180*M_PI)*0.01;
  //cout<<projCube_x<<" "<<projCube_z<<endl;
	//increment_x += -sin(turnAngle/180*M_PI)*0.01;
	//increment_z += cos(turnAngle/180*M_PI)*0.01;
	time_to_live--;
	directionKey(0,0,0);
	//cout<<angle<<endl;
	glutTimerFunc(60, timer, 42);
	glutPostRedisplay();
}

//main method
int main(int argc, char** argv)
{
    // First set up GLUT
    glutInit( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Make the window
    int windowHandle = glutCreateWindow("Maze Game");
    glutSetWindow(windowHandle);

    // Place and size the window
    glutPositionWindow ( 100, 100 );
    glutReshapeWindow( 500, 500 );
    glEnable(GL_DEPTH_TEST);
 
    // set the drawing and mouse callback functions
    // notice the names are the functions defined above
    glutDisplayFunc(display);
    glutTimerFunc(1000, timer, 42);
    glutSpecialFunc(directionKey);
    glutKeyboardFunc(keyBoard);
    //glutMouseFunc( mouseF );
    
    // Go into an infinite loop waiting for messages
    glutMainLoop();
    return 0;
}