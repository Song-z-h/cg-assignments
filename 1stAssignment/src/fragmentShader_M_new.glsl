#version 330 core

#ifdef GL_ES
precision highp float;
#endif

in vec4 ourColor;
out vec4 FragColor;  
uniform float time;
uniform vec2 resolution;
uniform float colorLerp;
 
#define PI 3.14
//vec3 color;

float timer = 0;
float colorLerp2 = 0;
void main()
{
// Convert screen coorzates to normalized device coordinates (NDC)
 
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
   
   if(ndc.y > 0 && ndc.y < 1)
     {
		  FragColor = vec4(ourColor.x * colorLerp, ourColor.y * colorLerp, ourColor.z * colorLerp, ourColor.a);
      
    }else if(ndc.y > -1 && ndc.y < 0){
      colorLerp2 = colorLerp+0.2;

      FragColor = vec4(ourColor.x * colorLerp2, ourColor.y * colorLerp2, ourColor.z * colorLerp2, ourColor.a);

    }
    //Costruisce un cerchio con centro in (-0.8,-0.8) e raggio 0.1
    else if (distance(ndc.xy, vec2(-0.8,-0.8)) < 0.1){
        
    }
		  
    else{ 
       FragColor = ourColor;
    }
}