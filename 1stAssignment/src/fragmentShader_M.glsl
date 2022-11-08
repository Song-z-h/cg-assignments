#version 330 core
in vec4 ourColor;
out vec4 FragColor;  
//uniform vec2 resolution;
  
void main()
{
    //vec2 ndc = vec2((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);
    
    FragColor = ourColor;

}