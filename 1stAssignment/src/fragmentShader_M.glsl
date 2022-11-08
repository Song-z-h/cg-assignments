#version 330 core
in vec4 ourColor;
out vec4 FragColor;  
uniform vec2 resolution;
  
void main()
{
    vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
    if(ndc.y> 0 && ndc.y< 1){
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }else
    FragColor = ourColor;

}