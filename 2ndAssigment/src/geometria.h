#include "Lib.h"
#include "Strutture.h"
void crea_cubo(Mesh* mesh);
void crea_piano(Mesh* mesh,  vec4 colore);
void crea_piano_suddiviso(Mesh* mesh, vec4 colore, int height, int N);
void crea_piramide(Mesh* piramide);
void crea_sfera(Mesh* mesh, vec4 colore);
void crea_cilindro(Mesh* mesh, vec4 colore);
void crea_toro(Mesh* mesh, vec4 colore);
void crea_cono(Mesh* mesh, vec4 colore);
void crea_casa(Mesh* mesh, vec4 coloret, vec4 coloreb);
float getNoise(int x, int z, int seed = 10);
float getSmoothNoise(int x, int z, int seed = 10);
float interpolate(float a, float b, float blend);
float getInterpolatedNoise(float x, float z);

 
