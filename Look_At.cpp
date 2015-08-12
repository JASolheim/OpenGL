// ----------------------------------------------------------------------------------------------
// Look_At.cpp    Jeffery Solheim
// Allows user to select between Orthographic (O) and Perspective (P) projections,
// ans also allows user to re-position camera using the mouse.
// ----------------------------------------------------------------------------------------------
#include <iostream>
#include <GL/glew.h>         // OpenGL Extension Wrangler Library
#include <GL/freeglut.h>     // GL Utility Toolkit (GLUT) Header
#include "Angel.h"
using namespace std ;
// ----------------------------------------------------------------------------------------------
typedef vec3 point3 ;
typedef vec4 point4 ;
// ----------------------------------------------------------------------------------------------
point3  cylindricalToRectangular ( point3 cylCoord )  //  cylCoord = ( rho, phi, y )
  {
  GLfloat rho  =  cylCoord [0] ;
  GLfloat phi  =  cylCoord [1] ;
  GLfloat y    =  cylCoord [2] ;
  GLfloat x    =  rho * cos ( phi ) ;
  GLfloat z    =  rho * sin ( phi ) ;
  point3 P ( x, y, z ) ;
  return P ;
  } // end cylindricalToRectangular function
// ----------------------------------------------------------------------------------------------
GLint   modelViewMatrixParamLoc ;
GLint   projectionMatrixParamLoc ;
mat4    lookAtMatrix ;
mat4    projectionMatrix ;
point3  eyeCylindrical ( 10, M_PI / 2, 0 ) ; // ( rho, phi, y )
// rho = cylinder's radius increased / decreased via mouse wheel
// phi = angle with positive x-axis increased / decreased via horizontal mouse left down drag
// y increased / decreased via vertical mouse left down drag
point4  eye ( 0, 0, 10, 1 ) ; // ( x, y, z, 1 )
point4  at  ( 0, 0, 0, 1 ) ;
vec4    up  ( 0, 1, 0, 0 ) ;
bool    leftDown = false ;
GLfloat currentX(0.0),  currentY(0.0) ;
unsigned char  viewType = 'O' ; // 'O' = Orthogonal, 'P' = Perspective
// ----------------------------------------------------------------------------------------------
mat4  perpectiveProjectionMatrix = Frustum( -0.91, +0.91, -0.91, +0.91, 9, 100 ) ; // Perspective view
mat4  orthographicProjectionMatrix = Ortho( -1, +1, -1, +1, 0.7, 100 ) ; // Orthographic parallel view
// ----------------------------------------------------------------------------------------------
GLfloat  position [33] // 11 points, each point being 3-D (x, y, & z)
            =  {  -0.9,  0.0, 0.0,  +0.9,  0.0, 0.0,  //  x axis
                   0.0, -0.9, 0.0,   0.0, +0.9, 0.0,  //  y axis
                  -0.2, -0.2, 0.1,  +0.6, -0.2, 0.1,  +0.2, 0.4, 0.1,  //  green triangle
                  -0.4, -0.4, 0.2,  +0.2, -0.4, 0.2,      //  blue square
                  +0.2, +0.2, 0.2,  -0.4, +0.2, 0.2   } ; //  blue square
GLfloat  color [33] // 11 colors, each color being 3-D (RGB)
            =  {  1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  //  x-axis
                  1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  //  y-axis
                  0.5, 0.9, 0.5,  0.5, 0.9, 0.5,  0.5, 0.9, 0.5,  //  green triangle
                  0.4, 0.4, 1.0,  0.4, 0.4, 1.0,       //  blue square
                  0.4, 0.4, 1.0,  0.4, 0.4, 1.0   } ;  //  blue square
// ----------------------------------------------------------------------------------------------
void mouseCallbackFunction ( int button, int state, int x, int y )
  {
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    leftDown = true ;
  else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    leftDown = false ;
  } // end mouseCallbackFunction
// ----------------------------------------------------------------------------------------------
void mouseMotionCallbackFunction ( int x, int y )
  {
  if ( leftDown )
    {
    cout << "mouse motion at ( " << x << ", " << y << " )\n" ;
    GLfloat previousX = currentX;
    GLfloat previousY = currentY;
    int width = glutGet(GLUT_WINDOW_WIDTH) ;
    int height = glutGet(GLUT_WINDOW_HEIGHT) ;
    currentX = ( 2 * static_cast<GLfloat>(x) / width - 1 ) ;
    currentY = ( 2 * static_cast<GLfloat>(height - y) / height - 1 ) ;
    GLfloat deltaX = currentX - previousX ;
    GLfloat deltaY = currentY - previousY ;
    if ( fabs(deltaX) > 0.10 || fabs(deltaY) > 0.10 )
      return ;
    GLfloat rho  =  eyeCylindrical [0] ;
    GLfloat phi  =  eyeCylindrical [1] ;
    GLfloat y    =  eyeCylindrical [2] ;
    phi -= 4 * deltaX ;
	y   += 20 * deltaY ;
    eyeCylindrical = point3 ( rho, phi, y ) ;
    point3 P = cylindricalToRectangular ( eyeCylindrical ) ;
    eye.x = P.x;  eye.y = P.y;  eye.z = P.z;
    switch ( viewType )
      {
      case  'P'  :  cout << "PERSPECTIVE  " ;  break ;
      case  'O'  :  cout << "ORTHOGONAL   " ;  break ;
      } // end switch
    cout << "EYE at " << eye << endl ;
    // ------------------------------------------------------------------------------
    lookAtMatrix = LookAt ( eye, at, up ) ;
    glUniformMatrix4fv (  modelViewMatrixParamLoc,  1,  GL_TRUE,  lookAtMatrix  ) ;
    // ------------------------------------------------------------------------------
    glutPostRedisplay() ;
    }
  } // end mouseMotionCallbackFunction
// ----------------------------------------------------------------------------------------------
void mouseWheelCallbackFunction ( int wheel, int direction, int x, int y )
  {
  cout << "wheel = " << wheel << ",  direction = " << direction << ",  ( x, y ) = ( " << x << ", " << y << " )\n" ;
  GLfloat eyeRho  =  eyeCylindrical [0] ;
  GLfloat eyePhi  =  eyeCylindrical [1] ;
  GLfloat eyeY    =  eyeCylindrical [2] ;
  eyeRho -= (direction / 2.0) ;
  eyeCylindrical = point3 ( eyeRho, eyePhi, eyeY ) ;
  point3 P = cylindricalToRectangular ( eyeCylindrical ) ;
  eye.x = P.x;  eye.y = P.y;  eye.z = P.z;
  switch ( viewType )
    {
    case  'P'  :  cout << "PERSPECTIVE  " ;  break ;
    case  'O'  :  cout << "ORTHOGONAL   " ;  break ;
    } // end switch
  cout << "EYE at " << eye << endl ;
  // ------------------------------------------------------------------------------
  lookAtMatrix = LookAt ( eye, at, up ) ;
  glUniformMatrix4fv (  modelViewMatrixParamLoc,  1,  GL_TRUE,  lookAtMatrix  ) ;
  // ------------------------------------------------------------------------------
  glutPostRedisplay() ;
  } // end mouseWheelCallbackFunction
// ----------------------------------------------------------------------------------------------
void keyboardCallbackFunction ( unsigned char key, int x, int y )
  {
  cout << "key is " << key << ", x = " << x << ", y = " << y << endl ;
  if ( key == 'p' || key == 'P' )
    {
    viewType = 'P' ;
    projectionMatrix = perpectiveProjectionMatrix ; // Perspective view
    }
  else if ( key == 'o' || key == 'O' )
    {
    viewType = 'O' ;
    projectionMatrix = orthographicProjectionMatrix ; // oRthographic parallel view
    }
  cout << "projectionMatrix is " << endl ;
  cout << projectionMatrix << endl ;
  glUniformMatrix4fv (  projectionMatrixParamLoc,  1,  GL_TRUE,  projectionMatrix  ) ;
  // ------------------------------------------------------------------------------
  glutPostRedisplay() ;
  } // end keyboardCallbackFunction
// ----------------------------------------------------------------------------------------------
void displayCallbackFunction ()
  {
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
  glDrawArrays ( GL_LINES, 0, 4 ) ; // x- & y-axes
  glDrawArrays ( GL_TRIANGLES, 4, 3 ) ; // green triangle
  glDrawArrays ( GL_TRIANGLE_FAN, 7, 4 ) ; // blue square
  glutSwapBuffers() ;
  glFlush() ;
  } // end displayCallbackFunction
// ----------------------------------------------------------------------------------------------
int main ( int argc, char ** argv )
  {
  // -------------------------------------------------------------------------------
  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ) ;
  glutInitWindowSize ( 500, 500 ) ;
  glutInitWindowPosition ( 620, 20 ) ;
  glutCreateWindow ( "Look At" ) ;
  glewInit () ;
  // ------------------------------------------------------------------------------
  GLuint vertexVBO = 0 ;
  GLuint colorVBO  = 0 ;
  // ------------------------------------------------------------------------------
  glGenBuffers ( 1, & vertexVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, vertexVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, 33 * sizeof(GLfloat), position, GL_STATIC_DRAW ) ;
  glGenBuffers ( 1, & colorVBO ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, colorVBO ) ;
  glBufferData ( GL_ARRAY_BUFFER, 33 * sizeof(GLfloat), color, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint vao = 0 ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, vertexVBO ) ;
  glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
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
  lookAtMatrix = LookAt ( eye, at, up ) ;
  projectionMatrix = orthographicProjectionMatrix ; // initially, orthographic parallel view ...
  modelViewMatrixParamLoc = glGetUniformLocation ( program, "modelViewMatrix" ) ;
  projectionMatrixParamLoc = glGetUniformLocation ( program, "projectionMatrix" ) ;
  glUniformMatrix4fv (  modelViewMatrixParamLoc,  1,  GL_TRUE,  lookAtMatrix  ) ;
  glUniformMatrix4fv (  projectionMatrixParamLoc,  1,  GL_TRUE,  projectionMatrix  ) ;
  // ------------------------------------------------------------------------------
  glEnable ( GL_DEPTH_TEST ) ;
  glClearColor ( 0.2, 0.2, 0.2, 1.0 ) ;
  glutMouseFunc ( mouseCallbackFunction ) ;
  glutMotionFunc ( mouseMotionCallbackFunction ) ;
  glutMouseWheelFunc ( mouseWheelCallbackFunction ) ;
  glutKeyboardFunc ( keyboardCallbackFunction ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  glutMainLoop () ;
  } // end main
// ----------------------------------------------------------------------------------------------
