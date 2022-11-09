#version 330 core

#ifdef GL_ES
precision highp float;
#endif

in vec4 ourColor;
out vec4 FragColor;  
uniform float time;
uniform vec2 resolution;
uniform float colorLerp;
uniform vec2 playerPos;
 
#define PI 3.14
//vec3 color;

float timer = 0;
float colorLerp2 = 0;
void main()
{
// Convert screen coorzates to normalized device coordinates (NDC)
 
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
    vec2 PlayerPosNdc = vec2((playerPos.x / resolution.x - 0.5) * 2, (playerPos.y / resolution.y - 0.5) * 2);

    //Costruisce un cerchio con centro in (-0.8,-0.8) e raggio 0.1
    if (distance(ndc.xy, PlayerPosNdc) < 0.2){
         FragColor = ourColor;
    }
   else if(ndc.y > 0 && ndc.y < 1)
     {
		  FragColor = vec4(ourColor.x * colorLerp, ourColor.y * colorLerp, ourColor.z * colorLerp, ourColor.a);
      
    }else if(ndc.y > -1 && ndc.y < 0){
      colorLerp2 = colorLerp;

      FragColor = vec4(ourColor.x * colorLerp2, ourColor.y * colorLerp2, ourColor.z * colorLerp2, ourColor.a);

    }
   

    else{ 
       FragColor = ourColor;
    }
}