// ----------------------------------------------------------------------------------------------
// Geo_Prim.cpp    Jeffery Solheim    Illustrates various types of OpenGL geometric primitives.
// Command-line arguments determine: (1) name of file containing points,
//                                 & (2) primitive with which to draw those points.
// Sample invocation:  Geo_Prim.exe Point_Set_2.txt LINE_STRIP
// ----------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Angel.h"
using namespace std ;
// ----------------------------------------------------------------------------------------------
struct  DataArray
  {
  GLsizeiptr  size ;
  GLvoid *    data ;
  } ; // end DataArray struct
// ----------------------------------------------------------------------------------------------
// function prototypes ...
void       displayCallbackFunction () ;
GLuint     prepareProgram () ;
DataArray  flatten_2D ( const vector<vec2> & positionVector ) ;
DataArray  flatten_3D ( const vector<vec3> & colorVector ) ;
// ----------------------------------------------------------------------------------------------
vector<vec2> positionVector ;
vector<vec3> colorVector ;
GLenum       drawMode = GL_POINTS ;
// ----------------------------------------------------------------------------------------------
int main ( int argc, char ** argv )
  {
  // ------------------------------------------------------------------------------
  for ( int i = 0 ; i < argc ; i ++ )
    cout << i << '\t' << argv[i] << endl ;
  if ( argc < 3 )
    {
    cout << "Usage should be:  Geo_Prim.exe <point set file name> <draw mode>" << endl ;
    exit ( 0 ) ;
    }
  // -------------------------------------------------------------------------------
  string drawModeString ( argv[2] ) ;
  transform  (  drawModeString.begin(),  drawModeString.end(),
                drawModeString.begin(),  (int (*)(int))toupper  ) ;
  if ( drawModeString == string("POINTS") )
    drawMode = GL_POINTS ;
  else if ( drawModeString == string("LINE_STRIP") )
    drawMode = GL_LINE_STRIP ;
  else if ( drawModeString == string("LINE_LOOP") )
    drawMode = GL_LINE_LOOP ;
  else if ( drawModeString == string("LINES") )
    drawMode = GL_LINES ;
  else if ( drawModeString == string("TRIANGLE_STRIP") )
    drawMode = GL_TRIANGLE_STRIP ;
  else if ( drawModeString == string("TRIANGLE_FAN") )
    drawMode = GL_TRIANGLE_FAN ;
  else if ( drawModeString == string("TRIANGLES") )
    drawMode = GL_TRIANGLES ;
  // -------------------------------------------------------------------------------
  glutInit ( &argc, argv ) ;
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ) ;
  glutInitWindowSize ( 350, 350 ) ;
  glutInitWindowPosition ( 800, 20 ) ;
  glutCreateWindow ( "Geometric Primitives via C++ & OpenGL" ) ;
  glewInit () ;
  // ------------------------------------------------------------------------------
  // x- and y-axes ...
  positionVector.push_back ( vec2(-0.9,+0.0) ) ;
  positionVector.push_back ( vec2(+0.9,+0.0) ) ;
  positionVector.push_back ( vec2(+0.0,-0.9) ) ;
  positionVector.push_back ( vec2(+0.0,+0.9) ) ;

  colorVector.push_back ( vec3(1.0,1.0,1.0) ) ;
  colorVector.push_back ( vec3(1.0,1.0,1.0) ) ;
  colorVector.push_back ( vec3(1.0,1.0,1.0) ) ;
  colorVector.push_back ( vec3(1.0,1.0,1.0) ) ;
  // ------------------------------------------------------------------------------
  // read additional positions from file ...
  ifstream inStream ;
  inStream.open ( argv[1] ) ;
  string sansWhite = "" ;
  double xCoord(0.0), yCoord(0.0) ;
  while ( inStream >> xCoord )
    {
    inStream >> yCoord ;
    positionVector.push_back ( vec2(xCoord,yCoord) ) ;
    switch ( positionVector.size() % 3 )
      {
      case 0 : colorVector.push_back(vec3(0,1,0) );  break;
      case 1 : colorVector.push_back(vec3(0,0,1) );  break;
      case 2 : colorVector.push_back(vec3(1,0,0) );  break;
      } // end switch
    } // end while loop
  inStream.close() ;
  // ------------------------------------------------------------------------------
  GLuint position_vbo = 0 ;
  glGenBuffers ( 1, & position_vbo ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  const DataArray position = flatten_2D ( positionVector ) ;
  glBufferData ( GL_ARRAY_BUFFER, position.size, position.data, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint color_vbo = 0 ;
  glGenBuffers ( 1, & color_vbo ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, color_vbo ) ;
  const DataArray color = flatten_3D ( colorVector ) ;
  glBufferData ( GL_ARRAY_BUFFER, color.size, color.data, GL_STATIC_DRAW ) ;
  // ------------------------------------------------------------------------------
  GLuint vao = 0 ;
  glGenVertexArrays ( 1, & vao ) ;
  glBindVertexArray ( vao ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, position_vbo ) ;
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glBindBuffer ( GL_ARRAY_BUFFER, color_vbo ) ;
  glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL ) ;
  glEnableVertexAttribArray ( 0 ) ;
  glEnableVertexAttribArray ( 1 ) ;
  // ------------------------------------------------------------------------------
  prepareProgram () ;
  // ------------------------------------------------------------------------------
  glClearColor ( 0.4, 0.4, 0.4, 1.0 ) ;
  glPointSize ( 6.0 ) ;
  glutDisplayFunc ( displayCallbackFunction ) ;
  glutMainLoop () ;
  } // end main
// ----------------------------------------------------------------------------------------------
void displayCallbackFunction ()
  {
  glClear ( GL_COLOR_BUFFER_BIT ) ;
  glDrawArrays ( GL_LINES, 0, 4 ) ;
  glDrawArrays ( GL_POINTS, 4, positionVector.size() - 4 ) ;
  glDrawArrays ( drawMode, 4, positionVector.size() - 4 ) ;
  glutSwapBuffers() ;
  glFlush() ;
  } // end displayCallbackFunction
// ----------------------------------------------------------------------------------------------
GLuint prepareProgram ()
  {
  const char * shaderProgram [ 2 ] ;
  shaderProgram[0] = "#version 400\nlayout(location = 0) in vec2 vertexPosition;\nlayout(location = 1) in vec3 vertex_color;\nout vec3 color;\nvoid main () { gl_Position = vec4(vertexPosition,0.0,1.0); color = vertex_color; }\n\0" ;
  shaderProgram[1] = "#version 400\nin vec3 color;\nvoid main () { gl_FragColor = vec4(color,1.0); }\n\0" ;
  GLuint program = glCreateProgram ( ) ;
  GLuint myVertexShader   = glCreateShader ( GL_VERTEX_SHADER   ) ;
  GLuint myFragmentShader = glCreateShader ( GL_FRAGMENT_SHADER ) ;
  glShaderSource ( myVertexShader, 1, (const GLchar**) &(shaderProgram[0]), NULL ) ;
  glShaderSource ( myFragmentShader, 1, (const GLchar**) &(shaderProgram[1]), NULL ) ;
  glCompileShader ( myVertexShader ) ;
  glCompileShader ( myFragmentShader ) ;
  glAttachShader ( program, myFragmentShader ) ;
  glAttachShader ( program, myVertexShader ) ;
  glLinkProgram ( program ) ;
  glUseProgram ( program ) ;
  return program ;
  } // end prepareProgram function
// ----------------------------------------------------------------------------------------------
DataArray flatten_2D ( const vector<vec2> & positionVector )
  {
  DataArray pos ;
  pos.size = positionVector.size() * 2 * sizeof(GLfloat) ;
  GLfloat * floatPtr = new GLfloat [pos.size] ;
  for ( unsigned int i = 0 ; i < positionVector.size() ; i ++ )
    {
	floatPtr[2*i  ] = positionVector.at(i).x ;
	floatPtr[2*i+1] = positionVector.at(i).y ;
    }
  pos.data = static_cast<GLvoid *>(floatPtr) ;
  return pos ;
  } // end flatten_2D function
// ----------------------------------------------------------------------------------------------
DataArray flatten_3D ( const vector<vec3> & colorVector )
  {
  DataArray pos ;
  pos.size = colorVector.size() * 3 * sizeof(GLfloat) ;
  GLfloat * floatPtr = new GLfloat [pos.size] ;
  for ( unsigned int i = 0 ; i < colorVector.size() ; i ++ )
    {
	floatPtr[3*i  ] = colorVector.at(i).x ;
	floatPtr[3*i+1] = colorVector.at(i).y ;
	floatPtr[3*i+2] = colorVector.at(i).z ;
    }
  pos.data = static_cast<GLvoid *>(floatPtr) ;
  return pos ;
  } // end flatten_3D function
// ----------------------------------------------------------------------------------------------
