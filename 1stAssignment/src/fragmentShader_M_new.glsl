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
    //playerHP is from 0% to 100%, i need the x to be from -1 to 0.95 
uniform float playerHp;
 
#define PI 3.14
//vec3 color;

float timer = 0;

void main()
{
// Convert screen coorzates to normalized device coordinates (NDC)
 
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
    vec2 PlayerPosNdc = vec2((playerPos.x / resolution.x - 0.5) * 2, (playerPos.y / resolution.y - 0.5) * 2);
    float playerDis = distance(ndc.xy, PlayerPosNdc.xy);
    
   if(ndc.y > 0 && ndc.y < 1)
     {
		  FragColor = vec4(ourColor.x * colorLerp, ourColor.y * colorLerp, ourColor.z * colorLerp, ourColor.a);
      
    }
    else if(ndc.y > -0.95 && ndc.y < - 0.8 && ndc.x > -0.95 && ndc.x < playerHp){
      FragColor = vec4(1.0, 0.2, 0.2, 1.0);
    }
    else{ 
       FragColor = ourColor;
    }
}