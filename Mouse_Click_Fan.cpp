// ----------------------------------------------------------------------------------------------
// Mouse_Click_Fan.cpp    Jeffery Solheim    Illustrates mouse callback functions.
// Each mouse click within window adds one point; 3rd & subsequent points build triangle fan.
// Coloring of fragments (even vs. odd fragments) done in fragment shader
// using shader input gl_PrimitiveID
// Developed using two-button mouse having wheel, push on wheel for middle button down/up.
// Register callback function for mouse wheel rotations via glutMouseWheelFunc.
// Mouse motion can be detected by registering callback functions
// via glutMotionFunc (mouse moves within window while one or more mouse buttons pressed)
// and glutPassiveMotionFunc (mouse moves within window while no mouse button is pressed).
// ----------------------------------------------------------------------------------------------
#include <GL/glew.h>         // OpenGL Extension Wrangler Library
#include <GL/freeglut.h>     // GL Utility Toolkit (GLUT) Header
#include <iostream>
using namespace std ;
// ----------------------------------------------------------------------------------------------
GLuint position_vbo  =  0 ;
// ----------------------------------------------------------------------------------------------
GLfloat position [2000] // up to 1000 two-dimensional positions
    = { -0.9, +0.0,
        +0.9, +0.0,
        +0.0, -0.9,
        +0.0, +0.9 } ;
// ----------------------------------------------------------------------------------------------
unsigned int numPts = 0 ;
// ----------------------------------------------------------------------------------------------
// Mouse motion can be detected by registering callback functions
// via glutMotionFunc and glutPassiveMotionFunc.
// ----------------------------------------------------------------------------------------------
void mouseWheelCallbackFunction ( int wheel, int direction, int x, int y )
  {
  cout << "wheel = " << wheel << ",  direction = " << direction << ",  ( x, y ) = ( " << x << ", " << y << " )\n" ;
  } // end mouseWheelCallbackFunction
// ----------------------------------------------------------------------------------------------
void motionCallbackFunction ( int x, int y )
  {
  cout << "mouse MOTION at ( x, y ) = ( " << x << ", " << y << " )\n" ;
  } // end motionCallbackFunction
// ----------------------------------------------------------------------------------------------
void passiveMotionCallbackFunction ( int x, int y )
  {
  cout << "mouse PASSIVE MOTION at ( x, y ) = ( " << x << ", " << y << " )\n" ;
  } // end motionCallbackFunction
// ----------------------------------------------------------------------------------------------
void mouseCallbackFunction ( int button, int state, int x, int y )
  {
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
    numPts ++ ;
    // map x & y to [ -1, +1 ]
    int width = glutGet(GLUT_WINDOW_WIDTH) ;
    int height = glutGet(GLUT_WINDOW_HEIGHT) ;
    GLfloat xCoord = ( 2 * static_cast<GLfloat>(x) / width - 1 ) ;
    GLfloat yCoord = ( 2 * static_cast<GLfloat>(height - y) / height - 1 ) ;
    // calculate indices into position array ...
    int     xIndex = 2 * numPts + 6 ;
    int     yIndex = 2 * numPts + 7 ;
    position [xIndex]   =  xCoord ;
    position [yIndex]   =  yCoord ;
    // copy data from application's array to buffer of GPU ...
    glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
    glBufferData ( GL_ARRAY_BUFFER, (8 + 2 * numPts) * sizeof(GLfloat), position, GL_STATIC_DRAW ) ;
    } // end if
  switch ( button )
    {
    case GLUT_LEFT_BUTTON   :  cout << "GLUT_LEFT_BUTTON      " ;  break ;
    case GLUT_MIDDLE_BUTTON :  cout << "GLUT_MIDDLE_BUTTON    " ;  break ;
    case GLUT_RIGHT_BUTTON  :  cout << "GLUT_RIGHT_BUTTON     " ;  break ;
    } // end switch
  switch ( state )
    {
    case GLUT_UP    :  cout << "GLUT_UP      at    ( x, y ) = ( " ;  break ;
    case GLUT_DOWN  :  cout << "GLUT_DOWN    at    ( x, y ) = ( " ;  break ;
    } // end switch
  cout << x << ", " << y << " )    " ;
  int modifiers = glutGetModifiers() ;
  switch ( modifiers )
    {
    case GLUT_ACTIVE_SHIFT    :  cout << "GLUT_ACTIVE_SHIFT" ;  break ;
    case GLUT_ACTIVE_CTRL     :  cout << "GLUT_ACTIVE_CTRL " ;  break ;
    case GLUT_ACTIVE_ALT      :  cout << "GLUT_ACTIVE_ALT  " ;  break ;
    } // end switch
  cout << endl ;
  glutPostRedisplay() ; // ask that scene be redrawn (to include new point)
  } // end mouseCallbackFunction
// ----------------------------------------------------------------------------------------------
void displayCallbackFunction ()
  {
  glClear ( GL_COLOR_BUFFER_BIT ) ;
  glDrawArrays ( GL_LINES, 0, 2 ) ;        // x-axis
  glDrawArrays ( GL_LINES, 2, 2 ) ;        // y-axis
  if ( numPts == 1 )
    glDrawArrays ( GL_POINTS, 4, numPts ) ;  // points generated by mouse downs
  else if ( numPts == 2 )
    glDrawArrays ( GL_LINES, 4, numPts ) ;
  else if ( numPts >= 3 )
    glDrawArrays ( GL_TRIANGLE_FAN, 4, numPts ) ;
  glutSwapBuffers() ;
  glFlush() ;
  } // end displayCallbackFunction
// ----------------------------------------------------------------------------------------------
int main ( int argc, char ** argv )
  {
  // -------------------------------------------------------------------------------
  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ) ;
  glutInitWindowSize ( 800, 500 ) ;
  glutInitWindowPosition ( 400, 20 ) ;
  glutCreateWindow ( "Mouse Callback Function" ) ;
  glewInit () ;
  // ------------------------------------------------------------------------------
  glGenBuffers ( 1, & position_vbo ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glBufferData ( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), position, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint vao = 0 ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glEnableVertexAttribArray ( 0 ) ;
  // ------------------------------------------------------------------------------
  const char * shaderProgram [ 2 ] ;
  shaderProgram[0] = "#version 400\nlayout(location = 0) in vec2 vertexPosition;\nvoid main () { gl_Position = vec4(vertexPosition,0.0,1.0); }\n\0" ;
  shaderProgram[1] = "#version 400\nvoid main () { if(gl_PrimitiveID%2==0) gl_FragColor = vec4(1.0,0.7,0.8,1.0); else  gl_FragColor = vec4(0.6,0.9,0.6,1.0); }\n\0" ;
  // ------------------------------------------------------------------------------
  GLuint program = glCreateProgram ( ) ;
  GLuint myVertexShader   = glCreateShader ( GL_VERTEX_SHADER   ) ;
  GLuint myFragmentShader = glCreateShader ( GL_FRAGMENT_SHADER ) ;
  glShaderSource ( myVertexShader, 1, (const GLchar**) &(shaderProgram[0]), NULL ) ;
  glShaderSource ( myFragmentShader, 1, (const GLchar**) &(shaderProgram[1]), NULL ) ;
  glCompileShader ( myVertexShader ) ;
  glCompileShader ( myFragmentShader ) ;
  // ------------------------------------------------------------------------------
  GLint vertexShaderCompiled ;
  glGetShaderiv ( myVertexShader, GL_COMPILE_STATUS, & vertexShaderCompiled ) ;
  if ( vertexShaderCompiled )
    cout << "GL_VERTEX_SHADER    Compiled successfully!\n" ;
  else
    {
    cout << "GL_VERTEX_SHADER    Failed to compile!\n" ;
    exit ( 1 ) ;
    }
  // ------------------------------------------------------------------------------
  GLint fragmentShaderCompiled ;
  glGetShaderiv ( myFragmentShader, GL_COMPILE_STATUS, & fragmentShaderCompiled ) ;
  if ( fragmentShaderCompiled )
    cout << "GL_FRAGMENT_SHADER  Compiled successfully!\n" ;
  else
    {
    cout << "GL_FRAGMENT_SHADER  Failed to compile!\n" ;
    exit ( 1 ) ;
    }
  // ------------------------------------------------------------------------------
  glAttachShader ( program, myFragmentShader ) ;
  glAttachShader ( program, myVertexShader ) ;
  // ------------------------------------------------------------------------------
  glLinkProgram ( program ) ;
  GLint programLinked ;
  glGetProgramiv ( program, GL_LINK_STATUS, & programLinked ) ;
  if ( programLinked )
    cout << "GL_LINK_STATUS      Linked successfully!\n" ;
  else
    {
    cout << "GL_LINK_STATUS      Failed to link!\n" ;
    exit ( 1 ) ;
    }
  // ------------------------------------------------------------------------------
  glUseProgram ( program ) ;
  // ------------------------------------------------------------------------------
  glClearColor ( 0.3, 0.3, 0.3, 1.0 ) ;
  glPointSize ( 2.0 ) ;
  glutMouseFunc ( mouseCallbackFunction ) ;
  glutMouseWheelFunc ( mouseWheelCallbackFunction ) ;
  glutMotionFunc( motionCallbackFunction ) ;
  glutPassiveMotionFunc( passiveMotionCallbackFunction ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  glutMainLoop () ;
  } // end main
// ----------------------------------------------------------------------------------------------