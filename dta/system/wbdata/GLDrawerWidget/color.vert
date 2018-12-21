#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;

#endif


//***************************************
// attributes 
attribute vec3 position;
attribute vec4 color;


//***************************************
// uniform
uniform mat4 mvp_matrix;

//***************************************
// out
varying vec4 v_color;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    	gl_Position = mvp_matrix * vec4(position, 1.0);
	v_color	    = color; 
}
//! [0]
