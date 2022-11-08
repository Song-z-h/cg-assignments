#version 330 core

#ifdef GL_ES
precision highp float;
#endif

in vec4 ourColor;
out vec4 FragColor;  
uniform float time;
uniform vec2 resolution;
 
#define PI 3.14
//vec3 color;
mat2 rotate3d(float angle)
{
    return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}


float random(vec2 pos) {
	return fract(1.0 * sin(pos.y + fract(100.0 * sin(pos.x))));
}

float noise(vec2 pos) {
	vec2 i = floor(pos);
	vec2 f = fract(pos);
	float a = random(i + vec2(0.0, 0.0));
	float b = random(i + vec2(1.0, 0.0));
	float c = random(i + vec2(0.0, 1.0));
	float d = random(i + vec2(1.0, 1.0));
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 pos) {
	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100.0);
	mat2 rot = mat2(cos(0.15), sin(0.15), -sin(0.25), cos(0.5));
	for (int i=0; i < 12; i++) {
		v += a * noise(pos);
		pos = rot * pos * 2.0 + shift;
		a *= 0.55;
	}
	return v;
}



void main()
{
// Convert screen coordinates to normalized device coordinates (NDC)
 
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
   
   if(ndc.y> 0 && ndc.y< 1)
     {
       vec2 pos = (gl_FragCoord.xy - resolution) / min(resolution.x, resolution.y);

      float f = fbm(pos * 2.0 * vec2(fbm(pos - (time / 8.0)), fbm(pos / 2.0 - (time / 8.0))));
      
      vec3 colour = vec3(0.00, 0.2, 0.8);
      
      colour = (f * 6.5) * colour;
      
      FragColor = vec4(colour, 1.0);
      
    }
    //Costruisce un cerchio con centro in (-0.8,-0.8) e raggio 0.1
    else if (distance(ndc.xy, vec2(-0.8,-0.8)) < 0.1){
        
    }
		  
    else{ 
       FragColor = ourColor;
    }
}