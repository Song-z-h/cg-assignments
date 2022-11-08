#extension GL_OES_standard_derivatives : enable

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2  resolution;
uniform float zoom;

#define PI 3.14

mat2 rotate3d(float angle)
{
    return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    p = rotate3d((time * 10.0) * PI) * p;
    float t;
    if (sin(time) == 1.0)
        t = 0.075 / abs(1.0 - length(p));
    else
        t = 0.075 / abs(1.0/*sin(time)*/ - length(p));
    gl_FragColor = vec4(vec3(t)  * vec3(0.25*(sin(time)+6.0), p.y*0.5, 0.0), 1.0);
}