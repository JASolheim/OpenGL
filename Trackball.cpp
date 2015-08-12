// ----------------------------------------------------------------------------------------------
// Trackball.cpp    Jeffery Solheim    Illustrates possible simulation of trackball via a mouse.
// Each mouse down of the left mouse button dictates magnitude & direction of vector.
// Press ESCAPE key to set magnitude of vector to zero.
// Vector's direction is to represent direction trackball is rotating,
// and vector's magnitude is to represent speed with which trackball is rotating.
// ----------------------------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <GL/glew.h>         // OpenGL Extension Wrangler Library
#include <GL/freeglut.h>     // GL Utility Toolkit (GLUT) Header
#include "Angel.h"
using namespace std ;
// ----------------------------------------------------------------------------------------------
GLfloat  position [742] // 371 points, each point being 2-D (x & y)
            =  {  -0.9, 0.0,  +0.9, 0.0,  0.0, -0.9,  0.0, +0.9, // x- & y-axes
                   0.0, 0.0,  0.95,  0.0,  1.0, 0.0,  0.9, 0.1,  0.9, -0.1  } ;
GLfloat  color [1113] // 371 colors, each color being 3-D (RGB)
            =  {  1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, 1.0, 1.0,
                  0.9, 0.5, 0.5,  0.9, 0.5, 0.5,  0.9, 0.5, 0.5,  0.9, 0.5, 0.5,  0.9, 0.5, 0.5  } ;
// ----------------------------------------------------------------------------------------------
GLfloat  currentAngle   =  0.0 ;
GLfloat  targetAngle    =  currentAngle ;
GLint    angleParamLoc ;
GLfloat  currentMagnitude  =  0.0 ;
GLfloat  targetMagnitude   =  currentMagnitude ;
GLint    magnitudeParamLoc ;
// ----------------------------------------------------------------------------------------------
void idleCallbackFunction ()
  {
  if (    (fabs(targetAngle - currentAngle) > 0.01)
       || (fabs(targetMagnitude - currentMagnitude) > 0.01)  )
    {
    currentAngle += (0.005 * (targetAngle - currentAngle)) ;
    glUniform1f (  angleParamLoc,  - currentAngle  ) ;
    currentMagnitude += (0.005 * (targetMagnitude - currentMagnitude)) ;
    glUniform1f (  magnitudeParamLoc,  currentMagnitude  ) ;
    glutPostRedisplay() ;
    }
  else
    {
    if ( targetAngle > M_PI )           targetAngle -= (2 * M_PI) ;
    else if ( targetAngle < (- M_PI) )  targetAngle += (2 * M_PI) ;
    currentAngle = targetAngle ;
    currentMagnitude = targetMagnitude ;
    }
  } // end idleCallbackFunction
// ----------------------------------------------------------------------------------------------
void mouseCallbackFunction ( int button, int state, int x, int y )
  {
  GLfloat  leftDownX          =  0.0 ;
  GLfloat  leftDownY          =  0.0 ;
  GLfloat  proposedMagnitude  =  0.0 ;
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
    int width = glutGet(GLUT_WINDOW_WIDTH) ;
    int height = glutGet(GLUT_WINDOW_HEIGHT) ;
    leftDownX = ( 2 * static_cast<GLfloat>(x) / width - 1 ) ;
    leftDownY = ( 2 * static_cast<GLfloat>(height - y) / height - 1 ) ;
    proposedMagnitude = sqrt ( (leftDownX * leftDownX) + (leftDownY * leftDownY) ) ;
    if ( proposedMagnitude > 1 )
      return ;
	targetMagnitude = proposedMagnitude ;
    targetAngle = atan2 ( leftDownY, leftDownX ) ;
    GLfloat deltaAngle = fabs(targetAngle - currentAngle) ;
    if ( (deltaAngle > M_PI) && (targetAngle < currentAngle) )
      targetAngle += (2 * M_PI) ;
    else if ( (deltaAngle > M_PI) && (targetAngle > currentAngle) )
      targetAngle -= (2 * M_PI) ;
    }
  } // end mouseCallbackFunction
// ----------------------------------------------------------------------------------------------
void keyboardCallbackFunction ( unsigned char key, int x, int y )
  {
  if ( key == 27 ) // ESCAPE key
    targetMagnitude  =  0.0 ;
  glutPostRedisplay() ;
  } // end keyboardCallbackFunction
// ----------------------------------------------------------------------------------------------
void displayCallbackFunction ()
  {
  glClear ( GL_COLOR_BUFFER_BIT ) ;
  glDrawArrays ( GL_TRIANGLE_FAN, 9, 362 ) ; // unit circle
  glLineWidth ( 1.0 ) ;
  glDrawArrays ( GL_LINES, 0, 4 ) ; // x- & y-axes
  glLineWidth ( 4.0 ) ;
  glDrawArrays ( GL_LINES, 4, 2 ) ; // vector having tail at origin
  glLineWidth ( 1.0 ) ;
  glDrawArrays ( GL_TRIANGLES, 6, 3 ) ; // head of vector
  glutSwapBuffers() ;
  glFlush() ;
  } // end displayCallbackFunction
// ----------------------------------------------------------------------------------------------
int main ( int argc, char ** argv )
  {
  // -------------------------------------------------------------------------------
  // create circle ...
  // first, the position & color of the circle's center ...
  position[18] = 0.0 ;  position[19] = 0.0 ;
  color[27] = 0.2 ;     color[28] = 0.3 ;      color[29] = 0.2 ;
  // second, positions & colors around the circle's periphery ...
  for ( unsigned int i = 10 ; i < 371 ; i ++ )
    {
    GLfloat  radians = (M_PI * (i - 10) / 180) ;
    position [2*i + 0] = cos(radians) ;
    position [2*i + 1] = sin(radians) ;
    color    [3*i + 0] = 0.2 ;
    color    [3*i + 1] = 0.3 ;
    color    [3*i + 2] = 0.2 ;
    }
  // -------------------------------------------------------------------------------
  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ) ;
  glutInitWindowSize ( 500, 500 ) ;
  glutInitWindowPosition ( 620, 20 ) ;
  glutCreateWindow ( "Trackball" ) ;
  glewInit () ;
  // ------------------------------------------------------------------------------
  GLuint vertexVBO = 0 ;
  GLuint colorVBO  = 0 ;
  // ------------------------------------------------------------------------------
  glGenBuffers ( 1, & vertexVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, vertexVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, 742 * sizeof(GLfloat), position, GL_STATIC_DRAW ) ;
  glGenBuffers ( 1, & colorVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, 1113 * sizeof(GLfloat), color, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint vao = 0 ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, vertexVBO ) ;
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
  glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glEnableVertexAttribArray ( 0 ) ;
  glEnableVertexAttribArray ( 1 ) ;
  // ------------------------------------------------------------------------------
  // vertex shader handles both rotation & scaling of vector by way of uniform parameters
  GLuint program = InitShader( "./shaders/Vertex_Shader.glsl",
                               "./shaders/Fragment_Shader.glsl" ) ;
  glUseProgram ( program ) ;
  // ------------------------------------------------------------------------------
  glClearColor ( 0.2, 0.2, 0.2, 1.0 ) ;
  angleParamLoc = glGetUniformLocation ( program, "angle" ) ;
  magnitudeParamLoc = glGetUniformLocation ( program, "magnitude" ) ;
  glutIdleFunc ( idleCallbackFunction ) ;
  glutMouseFunc ( mouseCallbackFunction ) ;
  glutKeyboardFunc ( keyboardCallbackFunction ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  glutMainLoop () ;
  } // end main
// ----------------------------------------------------------------------------------------------
