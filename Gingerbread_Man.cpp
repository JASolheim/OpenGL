/*
 *  Gingerbread_Man.cpp    Jeffery Solheim
 *  Illustrates animation of a "Gingerbread Man" formed via the iterated function system
 *  described by Hill and Kelley in Computer Graphics Using OpenGL, 3rd edition.
 */

// ==========================================================================

#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <chrono>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std ;
using namespace std::chrono ;

// ==========================================================================

void displayCallbackFunction ( void ) ;
void idleCallbackFunction ( void ) ;
void mouseCallbackFunction ( int button, int state, int x, int y ) ;
milliseconds ms ;
unsigned long int  currentMsCount, previousMsCount, deltaMsCount ;
GLuint initShaders ( void ) ;
void   recalculatePositions ( void ) ;
void   setColors ( void ) ;
const int NUM_PTS = 8192 ;
GLdouble  M = 0.030 ; // could try different values of M & L
GLdouble  L = 0.0017 ;
GLfloat   currentX = 0.0 ;
GLfloat   currentY = 0.0 ;
GLfloat   targetX = 0.0 ;
GLfloat   targetY = 0.0 ;
GLfloat   position [NUM_PTS * 2 + 4] ;
GLfloat   color [NUM_PTS * 3 + 6] ;
GLuint    positionVBO ;
int windowWidth  ;
int windowHeight  ;

// ==========================================================================

int main ( int argc, char ** argv )
	{
	glutInit ( &argc, argv ) ;
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ) ;
	glutInitWindowSize ( 400, 400 ) ;
	glutInitWindowPosition ( 820, 10 ) ;
	glutCreateWindow ( "Gingerbread Man" ) ;
	glewInit () ;

	// a red dot travels around to where the user clicks the mouse
	position [NUM_PTS * 2 + 0] = targetX ;
	position [NUM_PTS * 2 + 1] = targetY ;
	position [NUM_PTS * 2 + 2] = currentX ;
	position [NUM_PTS * 2 + 3] = currentY ;
	color    [NUM_PTS * 3 + 0] = 1.0 ;
	color    [NUM_PTS * 3 + 1] = 0.0 ;
	color    [NUM_PTS * 3 + 2] = 0.0 ;
	color    [NUM_PTS * 3 + 3] = 0.0 ;
	color    [NUM_PTS * 3 + 4] = 1.0 ;
	color    [NUM_PTS * 3 + 5] = 0.0 ;

	recalculatePositions ( ) ;
	setColors ( ) ;

	glGenBuffers ( 1, & positionVBO ) ;
	glBindBuffer ( GL_ARRAY_BUFFER, positionVBO ) ;
	glBufferData ( GL_ARRAY_BUFFER, sizeof(position), position, GL_DYNAMIC_DRAW ) ;
	GLuint colorVBO ;
	glGenBuffers ( 1, & colorVBO ) ;
	glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
	glBufferData ( GL_ARRAY_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW ) ;

	GLuint vao ;
	glGenVertexArrays ( 1, & vao ) ;
	glBindVertexArray ( vao ) ;
	glBindBuffer ( GL_ARRAY_BUFFER, positionVBO ) ;
	glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL ) ;
	glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
	glEnableVertexAttribArray ( 0 ) ;
	glEnableVertexAttribArray ( 1 ) ;

	GLuint program = initShaders ( ) ;
	glUseProgram ( program ) ;

    windowWidth = glutGet(GLUT_WINDOW_WIDTH) ;
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT) ;
	ms = duration_cast<milliseconds>
		( high_resolution_clock::now().time_since_epoch() ) ;
	currentMsCount = previousMsCount = ms.count() ;
	glClearColor ( 0.3, 0.3, 0.3, 1.0 ) ;
	glutDisplayFunc ( displayCallbackFunction ) ;
	glutIdleFunc ( idleCallbackFunction ) ;
	glutMouseFunc ( mouseCallbackFunction ) ;
	glutMainLoop () ;
	} // end main

// ==========================================================================

void recalculatePositions ( void )
	{
	GLfloat px = currentX ;
	GLfloat py = currentY ;
	GLfloat qx ;
	GLfloat qy ;
	for ( int i = 0 ; i < NUM_PTS ; i ++ )
		{
		position[2*i  ] = px ;
		position[2*i+1] = py ;
		qx = M * (1+2*L) - py + fabs(px-L*M) ;
		qy = px ;
		px = qx ;
		py = qy ;
		} // end for
	}

// ==========================================================================

void setColors ( void )
	{
	for ( int i = 0 ; i < NUM_PTS ; i ++ )
		{
		color   [3*i  ] = 0.53 ;
		color   [3*i+1] = 0.81 ;
		color   [3*i+2] = 0.98 ;
		} // end for
	}

// ==========================================================================

void displayCallbackFunction ( void )
	{
	glClear ( GL_COLOR_BUFFER_BIT ) ;
	glPointSize ( 1.0 ) ;
	glDrawArrays ( GL_POINTS, 0, NUM_PTS ) ;
	glPointSize ( 5.0 ) ;
	glDrawArrays ( GL_POINTS, NUM_PTS, 2 ) ;
	glutSwapBuffers() ;
	}

// ==========================================================================

void idleCallbackFunction ( void )
	{
	// -----  possibly omit time stuff  -----
	ms = duration_cast<milliseconds>
		( high_resolution_clock::now().time_since_epoch() ) ;
	currentMsCount = ms.count() ;
	deltaMsCount = (currentMsCount - previousMsCount) ;
	previousMsCount = currentMsCount ;
	// --------------------------------------
	currentX += (targetX - currentX) / 2048 ;
	currentY += (targetY - currentY) / 2048 ;
	position [NUM_PTS * 2 + 2] = currentX ;
	position [NUM_PTS * 2 + 3] = currentY ;
	recalculatePositions ( ) ;
	glBindBuffer ( GL_ARRAY_BUFFER, positionVBO ) ;
	glBufferData ( GL_ARRAY_BUFFER, sizeof(position), position, GL_DYNAMIC_DRAW ) ;
	glutPostRedisplay() ;
	} // end idleCallbackFunction

// ==========================================================================

void mouseCallbackFunction ( int button, int state, int x, int y )
  {
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
	targetX = ( 2 * static_cast<GLfloat>(x) / windowWidth - 1 ) ;
	targetY = ( 2 * static_cast<GLfloat>(windowHeight - y) / windowHeight - 1 ) ;
	position [NUM_PTS * 2 + 0] = targetX ;
	position [NUM_PTS * 2 + 1] = targetY ;
    }
  }

// ==========================================================================

GLuint initShaders ( )
  {
  string vertexShaderAsString
    = "#version 400\n"
      "layout(location = 0) in vec2 vertexPosition;\n"
      "layout(location = 1) in vec3 vertex_color;\n"
      "out vec3 color;\n"
      "mat4 rotationMatrix = mat4(1.0);\n"
      "float theta = 3.0 * atan(1.0);\n"
      "void main ()\n"
      "  {\n"
      "  rotationMatrix[0][0] = cos(theta);\n"
      "  rotationMatrix[0][1] = -sin(theta);\n"
      "  rotationMatrix[1][0] = sin(theta);\n"
      "  rotationMatrix[1][1] = cos(theta);\n"
      "  if(gl_VertexID < 8192)\n"
      "    gl_Position = rotationMatrix * vec4 ( vertexPosition, 0.0, 1.0 );\n"
      "  else\n"
      "    gl_Position = vec4 ( vertexPosition, 0.0, 1.0 );\n"
      "  color = vertex_color;\n"
      "  }\n" ;
  string fragmentShaderAsString
    = "#version 400\n"
      "in vec3 color;\n"
      "void main ()\n"
      " { gl_FragColor = vec4(color,1.0); }\n" ;
  GLuint program = glCreateProgram ( ) ;
  // ------------------------ prepare vertex shader ------------------------
  GLuint vertexShader = glCreateShader ( GL_VERTEX_SHADER ) ;
  char * vertex_cstr = new char [ vertexShaderAsString.length() + 1 ] ;
  std::strcpy ( vertex_cstr, vertexShaderAsString.c_str() ) ;
  glShaderSource ( vertexShader, 1, & vertex_cstr, NULL ) ;
  delete [] vertex_cstr ;
  glCompileShader ( vertexShader ) ;
  glAttachShader ( program, vertexShader ) ;
  // ----------------------- prepare fragment shader -----------------------
  GLuint fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER ) ;
  char * fragment_cstr = new char [ fragmentShaderAsString.length() + 1 ] ;
  std::strcpy ( fragment_cstr, fragmentShaderAsString.c_str() ) ;
  glShaderSource ( fragmentShader, 1, & fragment_cstr, NULL ) ;
  delete [] fragment_cstr ;
  glCompileShader ( fragmentShader ) ;
  glAttachShader ( program, fragmentShader ) ;
  // ------------------------- link shader program -------------------------
  glLinkProgram ( program ) ;
  return program ;
  } // end initShaders function

// ==========================================================================
