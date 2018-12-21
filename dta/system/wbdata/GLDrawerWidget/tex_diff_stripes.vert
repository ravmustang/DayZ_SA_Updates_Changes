#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;

#endif


//***************************************
// attributes 
attribute vec3 position;
attribute vec2 uv0;


//***************************************
// uniform
uniform mat4 mvp_matrix;

//***************************************
// out
varying vec2 v_texcoord;
varying vec2 v_worldcoord;


//! [0]
void main()
{
    // Calculate vertex position in screen space
	vec4 p		= mvp_matrix * vec4(position, 1.0);
	gl_Position 	= p;
	v_worldcoord    = vec2(position.x, position.y);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = uv0;
}
//! [0]
