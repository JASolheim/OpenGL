/*
 *  Orbiting_Point.cpp    Jeffery Solheim    Illustrates use of arrow keys to control animation.
 *  A point travels in a circular orbit.  The Up and Down arrows can be used
 *  to change the point's size from a minimum of 1 to a maximum of 50 (see glPointSize).
 *  The Right and Left arrows can be used to cause the point to increase and decrease
 *  the point's angular velocity in the clockwise and counterclockwise directions.
 */

// ==========================================================================

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <chrono>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std ;
using namespace std::chrono ;

// ==========================================================================

// global variables ...
unsigned long int  previousMsCount(0) ;
double             angle( atan(1) * 2 ) ;
GLint              angleLoc(0) ;
GLfloat            pointSize(10.0) ;
double             timeMultiplier(0.0) ;

// ==========================================================================

GLuint initShaders ( ) ; // function prototype

// ==========================================================================
void idleCallbackFunction ()
  {
  milliseconds ms ;
  unsigned long int  currentMsCount, deltaMsCount ;
  ms = duration_cast<milliseconds>
         ( high_resolution_clock::now().time_since_epoch() ) ;
  currentMsCount = ms.count() ;
  deltaMsCount = currentMsCount - previousMsCount ;
  previousMsCount = currentMsCount ;
  angle += ( deltaMsCount * timeMultiplier ) ;
  glUniform1f ( angleLoc, angle ) ;
  glutPostRedisplay() ;
  }

// ==========================================================================

void specialCallbackFunction ( int key, int x, int y )
  {
  const int UP    = 101 ;
  const int DOWN  = 103 ;
  const int LEFT  = 102 ;
  const int RIGHT = 100 ;
  if ( key == UP )              pointSize += 0.5 ;
  else if ( key == DOWN )       pointSize -= 0.5 ;
  if ( pointSize > 50.0 )       pointSize = 50.0 ;
  else if ( pointSize < 1.0 )   pointSize = 1.0 ;
  if ( key == RIGHT )           timeMultiplier += 0.0001 ;
  else if ( key == LEFT )       timeMultiplier -= 0.0001 ;
  }

// ==========================================================================
void displayCallbackFunction ()
  {
  glPointSize ( pointSize ) ;
  glClear ( GL_COLOR_BUFFER_BIT ) ;
  glDrawArrays ( GL_LINES, 0, 4 ) ;
  glDrawArrays ( GL_POINTS, 4, 1 ) ;
  glutSwapBuffers() ;
  glFlush() ;
  }

// ==========================================================================

int main ( int argc, char ** argv )
  {
  GLfloat  position [10]
    =  {   -0.9, +0.0,  +0.9, +0.0,  +0.0, -0.9,   +0.0, +0.9,   +0.0, +0.7   } ;
  // LightSkyBlue has the following RGB color components ...
  GLfloat R = 0x87 / 255.0 ;
  GLfloat G = 0xCE / 255.0 ;
  GLfloat B = 0xFA / 255.0 ;
  GLfloat  color [15]
    =  {  1, 1, 1,  1, 1, 1,  1, 1, 1,  1, 1, 1,  R, G, B  } ;

  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ) ;
  glutInitWindowSize ( 400, 400 ) ;
  glutInitWindowPosition ( 820, 10 ) ;
  glutCreateWindow ( "Orbiting Point" ) ;
  glewInit () ;

  GLuint positionVBO ;
  glGenBuffers ( 1, & positionVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, positionVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW ) ;
  GLuint colorVBO ;
  glGenBuffers ( 1, & colorVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW ) ;

  GLuint vao ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, positionVBO ) ;
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
  glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glEnableVertexAttribArray ( 0 ) ;
  glEnableVertexAttribArray ( 1 ) ;

  // initialize shaders ...
  GLuint program = initShaders ( ) ;
  glUseProgram ( program ) ;

  angleLoc = glGetUniformLocation ( program, "angle" ) ;
  glUniform1f ( angleLoc, angle ) ;

  glClearColor ( 0.4, 0.4, 0.4, 1.0 ) ;
  glutIdleFunc ( idleCallbackFunction ) ;
  glutSpecialFunc ( specialCallbackFunction ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  milliseconds ms ;
  ms = duration_cast<milliseconds>
         ( high_resolution_clock::now().time_since_epoch() ) ;
  previousMsCount = ms.count() ;
  glutMainLoop () ;
  } // end main function

// ==========================================================================

GLuint initShaders ( )
  {
  string vertexShaderAsString
    = "#version 400\n"
      "layout(location = 0) in vec2 vertexPosition;\n"
      "layout(location = 1) in vec3 vertex_color;\n"
      "out vec3 color;\n"
      "uniform float angle;\n"
      "void main ()\n"
      "  {\n"
      "  if(gl_VertexID < 4)\n"
      "    gl_Position = vec4(vertexPosition,0.0,1.0);\n"
      "  else\n"
      "    gl_Position = vec4(0.7*cos(angle),0.7*sin(angle),0.0,1.0);\n"
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
