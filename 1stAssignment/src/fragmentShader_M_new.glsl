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


const int complexity = 100;
const float offset = 100.0;
const float fluid_speed = 10.0;
const float color_intensity = 0.3;



float noise(float x, float amplitude) {
	float n = sin(x*8.0 + time) * 0.05 +
		sin(x*27.3 + time*0.5) * 0.005 + 
		sin(time) * 0.01;
	return n * amplitude;
}

vec3 getColor(){
   vec2 p = (2.0 * gl_FragCoord.xy - resolution) / max(resolution.x,resolution.y);
        
        for(int i = 1;i < complexity; i++) {
            vec2 newp = p;
            newp.x += 0.5/float(i) * sin(float(i) * p.y + time / fluid_speed + 0.9 * float(i)) + offset / 100.;
            newp.y += 0.5/float(i) * cos(float(i) * p.x + time / fluid_speed + 0.5 * float(i + 10)) - offset / 150.;
            p = newp;
        }

        vec3 col = vec3(
      color_intensity * sin(3.0 * p.x) + color_intensity + 0.3, 
      color_intensity * sin(2.0 * p.y) + color_intensity + 0.1,
      color_intensity * sin(2.0 * p.y) + color_intensity + 0.1);
    return col;
}


void main()
{
// Convert screen coorzates to normalized device coordinates (NDC)
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
    vec2 PlayerPosNdc = vec2((playerPos.x / resolution.x - 0.5) * 2, ((playerPos.y + 135)/ resolution.y - 0.5) * 2);
    float n0 = noise(ndc.x * 1.18, 0.7);
    float playerDis = distance(ndc.xy, PlayerPosNdc.xy);
    if(abs(playerDis) < 0.06 && abs(playerDis) > 0.02 ){
      FragColor = vec4(1.0, 1.0, 0, 1);
    }
 // hp bar
    else if(ndc.y > -0.95-n0 && ndc.y < -0.75 -n0 && ndc.x > -1 && ndc.x < playerHp){
      FragColor = vec4(getColor(), 1.0);
    }
  //luce 
   else if(ndc.y > -1 && ndc.y < 1)
     {
		  FragColor = vec4(ourColor.x * colorLerp, ourColor.y * colorLerp, ourColor.z * colorLerp, ourColor.a);
      
    }
   
    else{ 
       FragColor = ourColor;
    }
}