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


float rangeConverter(float value, float a1, float b1, float a2, float b2){
    return (value - a1) / (b1-a1) * (b2 - a2) + a2;
}

float timer = 0;
void main()
{
// Convert screen coorzates to normalized device coordinates (NDC)
 
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
   
   if(ndc.y > -1 && ndc.y < 1 && false)
     {
		//timer = sin(time * 1);
		//timer = rangeConverter(timer, -1.0, 1.0, 0.0, 0.3);
		//FragColor = vec4(ourColor.x - timer, ourColor.y - timer, ourColor.z - timer, ourColor.a);
      
    }
    //Costruisce un cerchio con centro in (-0.8,-0.8) e raggio 0.1
    else if (distance(ndc.xy, vec2(-0.8,-0.8)) < 0.1){
        
    }
		  
    else{ 
       FragColor = ourColor;
    }
}