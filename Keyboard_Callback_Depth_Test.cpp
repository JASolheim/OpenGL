// ----------------------------------------------------------------------------------------------
// Keyboard_Callback_Depth_Test.cpp    Jeffery Solheim
// OpenGL example to illustrate (1) keyboard event handling and (2) depth testing.
// Press R to select red circle, G to select green triangle, & B to select blue rectangle.
// The press < to decrease, > to increase, currently selected figure's z-plane.
// ----------------------------------------------------------------------------------------------
#include <GL/glew.h>         // OpenGL Extension Wrangler Library
#include <GL/freeglut.h>     // GL Utility Toolkit (GLUT) Header
#include "Angel.h"           // available at https://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/CODE/
#include <iostream>
using namespace std ;
// ----------------------------------------------------------------------------------------------
// Currently selected figure is ...
// 'G' for green triangle, 'B' for blue rectangle; 'R' for red circle
GLubyte  currentColor  =  'G' ;
// ----------------------------------------------------------------------------------------------
GLint  redCircleZ      =  +3 ;
GLint  greenTriangleZ  =  +2 ;
GLint  blueRectangleZ  =  +1 ;
// ----------------------------------------------------------------------------------------------
GLfloat position [2199]  //  allocating space also to store coordinates of red circle
  =  {   -0.9, +0.0, -1.0,   +0.9, +0.0, -1.0,                      //  x-axis (2 points)
         +0.0, -0.9, -1.0,   +0.0, +0.9, -1.0,                      //  y-axis (2 points)
         -0.3, -0.3, +0.2,   +0.7, -0.3, +0.2,   +0.2, +0.7, +0.2,  //  green triangle (3 points)
         -0.6, +0.4, +0.1,   -0.6, -0.6, +0.1,   //  blue rectangle (first 2 points)
         +0.2, -0.6, +0.1,   +0.2, +0.4, +0.1    //  blue rectangle (second 2 points)
     } ;
// ----------------------------------------------------------------------------------------------
GLfloat color [2199]
  =  {   1.0, 1.0, 1.0,   1.0, 1.0, 1.0,                   //  x-axis (2 colors)
         1.0, 1.0, 1.0,   1.0, 1.0, 1.0,                   //  y-axis (2 colors)
		 0.6, 0.9, 0.6,   0.6, 0.9, 0.6,   0.6, 0.9, 0.6,  //  green triangle (3 colors)
		 0.4, 0.7, 1.0,   0.4, 0.7, 1.0,                   //  blue rectangle (first 2 colors)
		 0.4, 0.7, 1.0,   0.4, 0.7, 1.0                    //  blue rectangle (second 2 colors)
     } ;
// ----------------------------------------------------------------------------------------------
GLuint position_vbo = 0 ;
GLuint color_vbo    = 0 ;
// ----------------------------------------------------------------------------------------------
void keyboardCallbackFunction ( unsigned char key, int x, int y )
  {
  // When glutKeyboardFunc is used to register callback function,
  // the following keys are ignored & do not generate keyboard events:
  //   Function keys, numeric keypad keys, up & down arrows
  //   Home, End, PageUp, PageDown, Insert
  // Use glutSpecialFunc if desire to recognize the keys listed above.
  cout << "key is " << key << ", x = " << x << ", y = " << y << endl ;

  switch ( key )
    {
    case 'r' : 
    case 'R' : currentColor = 'R';  break;
    case 'g' : 
    case 'G' : currentColor = 'G';  break;
    case 'b' : 
    case 'B' : currentColor = 'B';  break;
    }

  if ( (key == '<') && (currentColor == 'R') )
    redCircleZ -- ;
  else if ( (key == '<') && (currentColor == 'G') )
    greenTriangleZ -- ;
  else if ( (key == '<') && (currentColor == 'B') )
    blueRectangleZ -- ;

  if ( (key == '>') && (currentColor == 'R') )
    redCircleZ ++ ;
  else if ( (key == '>') && (currentColor == 'G') )
    greenTriangleZ ++ ;
  else if ( (key == '>') && (currentColor == 'B') )
    blueRectangleZ ++ ;

  GLfloat zValue = 1.0f ;
  if ( redCircleZ == +10 )       zValue = +0.9999f ;
  else if ( redCircleZ == -10 )  zValue = -0.9999f ;
  else                           zValue = 0.1 * redCircleZ ;
  for ( int i = 0 ; i < 722 ; i ++ )
    position[3*i + 35] = zValue ;
  cout << "red circle zValue is " << zValue << endl ;

  zValue = 1.0f ;
  if ( greenTriangleZ == +10 )       zValue = +0.9999f ;
  else if ( greenTriangleZ == -10 )  zValue = -0.9999f ;
  else                               zValue = 0.1 * greenTriangleZ ;
  position[14] = position[17] = position[20] = zValue ;
  cout << "green triangle zValue is " << zValue << endl ;

  if ( blueRectangleZ == +10 )       zValue = +0.9999f ;
  else if ( blueRectangleZ == -10 )  zValue = -0.9999f ;
  else                               zValue = 0.1 * blueRectangleZ ;
  position[23] = position[26] = position[29]  = position[32] = zValue ;
  cout << "blue rectangle zValue is " << zValue << endl ;

  glutPostRedisplay() ;
  } // end keyboardCallbackFunction
// ----------------------------------------------------------------------------------------------
void displayCallbackFunction ()
  {
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glBufferSubData ( GL_ARRAY_BUFFER, 0, 2199 * sizeof(GLfloat), position ) ;
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
  glDrawArrays ( GL_LINES, 0, 4 ) ;            // draw axes
  glDrawArrays ( GL_TRIANGLES, 4, 3 ) ;        // draw green triangle
  glDrawArrays ( GL_TRIANGLE_FAN, 7, 4 ) ;     // draw blue rectangle
  glDrawArrays ( GL_TRIANGLE_FAN, 11, 722 ) ;  // draw red circle
  glutSwapBuffers() ;
  glFlush() ;
  } // end displayCallbackFunction
// ----------------------------------------------------------------------------------------------
int main ( int argc, char ** argv )
  {
  // -------------------------------------------------------------------------------
  // initialize x, y, z coordinates & color of circle's center ...
  position [33] = -0.4;  position [34] = +0.3;  position [35] = +0.3;
  color    [33] =  0.9;  color    [34] = 0.6;   color    [35] =  0.7;
  // initialize x, y, z coordinates & colors of points around the circle ...
  for ( int i = 0 ; i < 721 ; i ++ )
    {
    int j = 3*i + 36 ;
    int k = 3*i + 37 ;
    int l = 3*i + 38 ;
    double theta = i / 360.0 * M_PI ;
    position[j] = -0.4 + 0.5 * cos(theta);
    position[k] = +0.3 + 0.5 * sin(theta);
    position[l] = +0.3;
    color[j] = 0.9;  color[k] = 0.6;  color[l] = 0.7;
    }
  // -------------------------------------------------------------------------------
  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ) ;
  glutInitWindowSize ( 600, 500 ) ;
  glutInitWindowPosition ( 620, 20 ) ;
  glutCreateWindow ( "Keyboard Callback Function / Depth Testing" ) ;
  glewInit () ;
  // ------------------------------------------------------------------------------
  glGenBuffers ( 1, & position_vbo ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glBufferData ( GL_ARRAY_BUFFER, 2199 * sizeof(GLfloat), position, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  glGenBuffers ( 1, & color_vbo ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, color_vbo ) ;
  glBufferData ( GL_ARRAY_BUFFER, 2199 * sizeof(GLfloat), color, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint vao = 0 ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, color_vbo ) ;
  glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glEnableVertexAttribArray ( 0 ) ;
  glEnableVertexAttribArray ( 1 ) ;
  // ------------------------------------------------------------------------------
  GLuint program = InitShader( "./shaders/Vertex_Shader.glsl",
                               "./shaders/Fragment_Shader.glsl" ) ;
  glUseProgram ( program ) ;
  // ------------------------------------------------------------------------------
  glEnable ( GL_DEPTH_TEST ) ;
  glClearColor ( 0.4, 0.4, 0.4, 1.0 ) ;
  glutKeyboardFunc ( keyboardCallbackFunction ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  glutMainLoop () ;
  } // end main
// ----------------------------------------------------------------------------------------------
