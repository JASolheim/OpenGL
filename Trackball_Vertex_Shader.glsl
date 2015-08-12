  #version 400
  layout(location = 0) in vec2 vertexPosition ;
  layout(location = 1) in vec3 vertexColor ;
  uniform float angle ;
  uniform float magnitude ;
  mat4 rotateZ ;
  vec2 vPos ;
  out vec3 color ;
  void main ()
    {
    rotateZ = mat4(1.0) ;
    rotateZ[0][0] = cos(angle) ;
    rotateZ[0][1] = - sin(angle) ;
    rotateZ[1][0] = sin(angle) ;
    rotateZ[1][1] = cos(angle) ;
    gl_Position = vec4 ( vertexPosition, 0.0, 1.0 ) ;
    if ( (gl_VertexID >= 4) && (gl_VertexID <= 8) )
      {
      vPos =  magnitude * vertexPosition ;
      gl_Position = rotateZ * vec4 ( vPos, 0.0, 1.0 ) ;
      }
    color = vertexColor;
    }
