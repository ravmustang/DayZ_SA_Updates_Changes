#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D TexDiffuse;


varying vec2 v_texcoord;
varying vec2 v_worldcoord;

//! [0]
void main()
{
	const float d   = 12.67;

    // Set fragment color from texture
    	vec4	outc	= texture2D(TexDiffuse, v_texcoord);
	
	// stripes
	float 	x	= abs(mod(v_worldcoord.x, d) - mod(v_worldcoord.y, d) );
	float 	y	= abs(mod(v_worldcoord.x, d) - d + mod(v_worldcoord.y, d) );
	float 	k	= 1.0 - min(x,y);

	float 	oa 	= mod(outc.a, 0.1);

	if (oa > 0.01 && oa < 0.03)
	{
		outc.a = 1.0;
	}
	else
	{	
		outc.a	*= k;
	}
	

	gl_FragColor	= outc;

	

	

}
//! [0]

