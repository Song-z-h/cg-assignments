#include <iostream>

#include "ShaderMaker.h"
#include "Lib.h"
#include "geometria.h"
#include "gestioneTexture.h"
#include "gestione_menu.h"
#include "materiali_base.h"
#include "load_meshes_assimp.hpp"
#include "gestione_mouse.h"
#include "gestione_telecamera.h"
#include "Strutture.h"
#include "enum.h"
#include "Mesh3D.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

string Meshdir = "Meshes/";
string Imagedir = "Textures/";
string SkyboxDir = "Skybox/";

bool visualizzaAncora = false;
int width = 1440;
int height = 1000;

// Gestione Mouse

int last_mouse_pos_Y;
int last_mouse_pos_X;
bool moving_trackball = 0;
bool firstMouse = true;
float lastX = (float)width / 2;
float lastY = (float)height / 2;
float timer = 0;
float angolo = 0.0;
// Varibili per il reshape
int w_up = width;
int h_up = height;

// Inizializzazione dei modelli
static vector<MeshObj> Model3D;
string stringa_asse;
float cameraSpeed = 0.1;
bool moveCamera = false;

/*map settings*/
float mapGeneratorFrequency = 2;
float octaves = 3;
Mesh3D tree(GL_TRIANGLES);
vector<vec3> treePos;
const int numTrees = 1000;
int randValue[numTrees];
const int MAP_SCALE = 10000;

float skyRotationSpeed = 0.001f;
float skyRotation = 0;

// variabili per la comunicazione delle variabili uniformi con gli shader
static unsigned int programId, programId_text, programId1, MatrixProj, MatModel, MatView;
static unsigned int lsceltaFS, lsceltaVS, loc_texture, MatViewS, MatrixProjS;
static unsigned int loc_view_pos, MatModelR, MatViewR, MatrixProjR, loc_view_posR, loc_cubemapR;

unsigned int idTex, texture, texture1, cubemapTexture, programIdr, loctime, texture2;

float raggio_sfera = 2.5;

string Operazione;
vec3 asse = vec3(0.0, 1.0, 0.0);

float Theta = -90.0f;
float Phi = 0.0f;

vector<std::string> faces{
	/*"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"*/
	SkyboxDir + "right.png",
	SkyboxDir + "left.png",
	SkyboxDir + "top.jpg",
	SkyboxDir + "bottom.png",
	SkyboxDir + "back.png",
	SkyboxDir + "front.png"};

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
//

mat4 Projection, Model, View;

point_light light;
vector<Material> materials;
vector<Shader> shaders;

vec3 playerMov = vec3(0, 0, 0);
quat playerRotationQuat = quat(0, vec3(0, 0, 0));

LightShaderUniform light_unif = {};

void INIT_SHADER(void)

{
	GLenum ErrorCheckValue = glGetError();

	char *vertexShader = (char *)"vertexShader_C.glsl";
	char *fragmentShader = (char *)"fragmentShader_C.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

	vertexShader = (char *)"vertexShader_CubeMap.glsl";
	fragmentShader = (char *)"fragmentShader_CubeMap.glsl";

	programId1 = ShaderMaker::createProgram(vertexShader, fragmentShader);

	vertexShader = (char *)"vertexShader_riflessione.glsl";
	fragmentShader = (char *)"fragmentShader_riflessione.glsl";

	programIdr = ShaderMaker::createProgram(vertexShader, fragmentShader);
}

void INIT_Illuminazione()
{
	// Setup della luce
	light.position = {0.0, 15.0, -30.0};
	light.color = {1.0, 1.0, 1.0};
	light.power = 2.f;

	// Setup dei materiali
	//  Materials setup
	materials.resize(8);
	materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
	materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
	materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
	materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
	materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

	materials[MaterialType::EMERALD].name = "Emerald";
	materials[MaterialType::EMERALD].ambient = emerald_ambient;
	materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
	materials[MaterialType::EMERALD].specular = emerald_specular;
	materials[MaterialType::EMERALD].shininess = emerald_shininess;

	materials[MaterialType::BRASS].name = "Brass";
	materials[MaterialType::BRASS].ambient = brass_ambient;
	materials[MaterialType::BRASS].diffuse = brass_diffuse;
	materials[MaterialType::BRASS].specular = brass_specular;
	materials[MaterialType::BRASS].shininess = brass_shininess;

	materials[MaterialType::SNOW_WHITE].name = "Snow_White";
	materials[MaterialType::SNOW_WHITE].ambient = snow_white_ambient;
	materials[MaterialType::SNOW_WHITE].diffuse = snow_white_diffuse;
	materials[MaterialType::SNOW_WHITE].specular = snow_white_specular;
	materials[MaterialType::SNOW_WHITE].shininess = snow_white_shininess;

	materials[MaterialType::YELLOW].name = "Yellow";
	materials[MaterialType::YELLOW].ambient = yellow_ambient;
	materials[MaterialType::YELLOW].diffuse = yellow_diffuse;
	materials[MaterialType::YELLOW].specular = yellow_specular;
	materials[MaterialType::YELLOW].shininess = yellow_shininess;

	materials[MaterialType::ROSA].name = "ROSA";
	materials[MaterialType::ROSA].ambient = rosa_ambient;
	materials[MaterialType::ROSA].diffuse = rosa_diffuse;
	materials[MaterialType::ROSA].specular = rosa_specular;
	materials[MaterialType::ROSA].shininess = rosa_shininess;

	materials[MaterialType::MARRONE].name = "MARRONE";
	materials[MaterialType::MARRONE].ambient = marrone_ambient;
	materials[MaterialType::MARRONE].diffuse = marrone_diffuse;
	materials[MaterialType::MARRONE].specular = marrone_specular;
	materials[MaterialType::MARRONE].shininess = marrone_shininess;
	materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
	materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
	materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].shininess = 1.f;

	// Setup degli shader
	shaders.resize(5);
	shaders[ShaderOption::NONE].value = 0;
	shaders[ShaderOption::NONE].name = "NONE";
	shaders[ShaderOption::GOURAD_SHADING].value = 1;
	shaders[ShaderOption::GOURAD_SHADING].name = "GOURAD SHADING";
	shaders[ShaderOption::PHONG_SHADING].value = 2;
	shaders[ShaderOption::PHONG_SHADING].name = "PHONG SHADING";
	shaders[ShaderOption::ONDE_SHADING].value = 3;
	shaders[ShaderOption::ONDE_SHADING].name = "ONDE SHADING";
	shaders[ShaderOption::BANDIERA_SHADING].value = 4;
	shaders[ShaderOption::BANDIERA_SHADING].name = "BANDIERA SHADING";
}

void crea_VAO_Vector_MeshObj(MeshObj *mesh)
{

	glGenVertexArrays(1, &mesh->VAO);
	glBindVertexArray(mesh->VAO);
	// Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &mesh->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertici.size() * sizeof(vec3), mesh->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &mesh->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, mesh->colori.size() * sizeof(vec4), mesh->colori.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);

	// Genero , rendo attivo, riempio il VBO delle normali
	glGenBuffers(1, &mesh->VBO_normali);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_normali);
	glBufferData(GL_ARRAY_BUFFER, mesh->normali.size() * sizeof(vec3), mesh->normali.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO delle NORMALI nel layer 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(2);

	// EBO di tipo indici
	glGenBuffers(1, &mesh->EBO_indici);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO_indici);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indici.size() * sizeof(GLuint), mesh->indici.data(), GL_STATIC_DRAW);
}

void crea_VAO_Vector(Mesh *mesh)
{

	glGenVertexArrays(1, &mesh->VAO);
	glBindVertexArray(mesh->VAO);
	// Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &mesh->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertici.size() * sizeof(vec3), mesh->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &mesh->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, mesh->colori.size() * sizeof(vec4), mesh->colori.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);

	// Genero , rendo attivo, riempio il VBO delle normali
	glGenBuffers(1, &mesh->VBO_normali);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_normali);
	glBufferData(GL_ARRAY_BUFFER, mesh->normali.size() * sizeof(vec3), mesh->normali.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO delle NORMALI nel layer 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &mesh->VBO_coord_texture);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_coord_texture);
	glBufferData(GL_ARRAY_BUFFER, mesh->texCoords.size() * sizeof(vec2), mesh->texCoords.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO delle NORMALI nel layer 2
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(3);

	// EBO di tipo indici
	glGenBuffers(1, &mesh->EBO_indici);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO_indici);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indici.size() * sizeof(GLuint), mesh->indici.data(), GL_STATIC_DRAW);
}

void crea_Tree(Mesh3D *mesh)
{
	Mesh Cono, Cilindro, Toro;

	// cono
	crea_cono(&Cono, vec4(0.0, 0.0, 0.0, 1.0));
	crea_VAO_Vector(&Cono);
	Cono.nome = "Cono";
	Cono.ModelM = mat4(1.0);
	// Cono.ModelM = translate(Cono.ModelM, vec3(-12.5, 5.2, 12.0));
	Cono.ModelM = scale(Cono.ModelM, vec3(1.2f, 1.5f, 1.2f));
	// Cono.ModelM = rotate(Cono.ModelM, radians(180.0f), vec3(1.0, 0.0, 0.0));
	Cono.sceltaVS = 1;
	Cono.sceltaFS = 1;
	Cono.material = MaterialType::RED_PLASTIC;
	Cono.ancora_obj = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// Scena.push_back(Cono);

	// CILIDNRO
	crea_cilindro(&Cilindro, vec4(0.5, 0.0, 0.0, 1.0));
	crea_VAO_Vector(&Cilindro);
	Cilindro.nome = "Cilindro";
	Cilindro.ModelM = mat4(1.0);
	// Cilindro.ModelM = translate(Cilindro.ModelM, vec3(7.0, -4.0, 7.0));
	Cilindro.ModelM = scale(Cilindro.ModelM, vec3(1.0, 1.5, 1.0));
	Cilindro.sceltaVS = 1;
	Cilindro.sceltaFS = 1;
	Cilindro.material = MaterialType::MARRONE;
	Cilindro.ancora_obj = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// Scena.push_back(Cilindro);

	// TORO
	crea_toro(&Toro, vec4(0.0, 1.0, 0.0, 1.0));
	crea_VAO_Vector(&Toro);
	Toro.nome = "Toro";
	Toro.ModelM = mat4(1.0);
	// Toro.ModelM = translate(Toro.ModelM, vec3(7.0, -1.0, 7.0));
	Toro.ModelM = scale(Toro.ModelM, vec3(1.0, 0.5, 1.0));
	Toro.sceltaVS = 1;
	Toro.sceltaFS = 1;
	Toro.material = MaterialType::BRASS;
	Toro.ancora_obj = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// Scena.push_back(Toro);
	// tree.addBodypart(Cilindro, 0, 0, 0, 0.001);
	tree.addBodypart(Cilindro, 0, 0, 0, 1, 1, 1);
	tree.addBodypart(Cono, 0, 1, 0, 5, 1, 5);
	tree.addBodypart(Cono, 0, 1.5, 0, 4, 1, 4);
	tree.addBodypart(Cono, 0, 2, 0, 3, 1, 3);
	tree.addBodypart(Cono, 0, 2.5, 0, 2, 1, 2);
}

void INIT_VAO(void)
{

	Mesh Pannello, Sfondo, Sfera, Sky;
	string name = "leaves.png";
	string path = Imagedir + name;
	texture = loadTexture(path.c_str(), 0);
	name = "grass.jpg";
	path = Imagedir + name;
	texture1 = loadTexture(path.c_str(), 0);
	name = "tree.png";
	path = Imagedir + name;
	texture2 = loadTexture(path.c_str(), 0);

	cubemapTexture = loadCubemap(faces, 0);
	{
		// Sky
		crea_cubo(&Sky);
		crea_VAO_Vector(&Sky);
		Scena.push_back(Sky);
		// Sfera
		crea_sfera(&Sfera, vec4(1.0, 0.0, 0.0, 1.0));
		crea_VAO_Vector(&Sfera);
		Sfera.ModelM = mat4(1.0);
		Sfera.ModelM = translate(Sfera.ModelM, vec3(5.5, 2.8, 12.0));
		Sfera.ModelM = scale(Sfera.ModelM, vec3(1.0f, 1.0f, 1.0f));
		Sfera.sceltaVS = 1;
		Sfera.sceltaFS = 1;
		Sfera.nome = "Sfera";
		Sfera.material = MaterialType::EMERALD;
		Sfera.ancora_obj = vec4(0, 0, 0, 1);
		Scena.push_back(Sfera);
	}

	crea_piano_suddiviso(&Pannello, vec4(0.0, 0.0, 0.0, 1.0), 220, 100);
	crea_VAO_Vector(&Pannello);
	Pannello.nome = "Pannello";
	Pannello.ModelM = mat4(1.0);
	// Pannello.ModelM = translate(Pannello.ModelM, vec3(-7.0, 0, -2.0));
	Pannello.ModelM = scale(Pannello.ModelM, vec3(MAP_SCALE, 1, MAP_SCALE));
	// Pannello.ModelM = rotate(Pannello.ModelM, radians(90.0f), vec3(1.0, 0.0, 0.0));
	Pannello.sceltaVS = 1;
	Pannello.sceltaFS = 10;
	Pannello.material = MaterialType::EMERALD;
	Scena.push_back(Pannello);

	crea_Tree(&tree);
	for (int i = 0; i < numTrees; i++)
	{
		//treePos.push_back(vec3(rand() % 30 - 15, 0, rand() % 30 - 15));
		treePos[i] = Pannello.ModelM * vec4(treePos[i], 1.0);
		randValue[i] = rand() % 100;
	}

	bool obj;
	{
		int nmeshes = 0;

		name = "piper_pa18.obj";
		path = Meshdir + name;
		obj = loadAssImp(path.c_str(), Model3D); // OK ombrellone.obj, divano.obj, low_poly_house,man

		nmeshes = Model3D.size();

		// playerPos = vec4(0.0);
		// ViewSetup.target = playerPos;
		for (int i = 0; i < nmeshes; i++)
		{

			crea_VAO_Vector_MeshObj(&Model3D[i]);
			Model3D[i].ModelM = mat4(1.0);
			// Model3D[i].ModelM = translate(Model3D[i].ModelM, vec3(playerPos.x, playerPos.y, playerPos.z));
			// Model3D[i].ModelM = scale(Model3D[i].ModelM, vec3(2.5, 2.5, 2.5));
			// Model3D[i].ModelM = rotate(Model3D[i].ModelM, radians(180.0f), vec3(0, 1, 0));
			Model3D[i].nome = "Piper";
			Model3D[i].ancora_obj = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			Model3D[i].sceltaVS = 1;
			Model3D[i].sceltaFS = 5;
		}
		ScenaObj.push_back(Model3D);

		Model3D.clear();

		name = "Shine_Sprite.obj";
		path = Meshdir + name;
		obj = loadAssImp(path.c_str(), Model3D); // OK ombrellone.obj, divano.obj, low_poly_house,man

		nmeshes = Model3D.size();

		for (int i = 0; i < nmeshes; i++)
		{

			crea_VAO_Vector_MeshObj(&Model3D[i]);
			Model3D[i].ModelM = mat4(1.0);
			Model3D[i].ModelM = translate(Model3D[i].ModelM, vec3(-16.0, 6.0, 3.0));
			Model3D[i].ModelM = scale(Model3D[i].ModelM, vec3(2.5, 2.5, 2.5));
			Model3D[i].ModelM = rotate(Model3D[i].ModelM, radians(180.0f), vec3(0, 1, 0));
			Model3D[i].nome = "Shine";
			Model3D[i].ancora_obj = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			Model3D[i].sceltaVS = 1;
			Model3D[i].sceltaFS = 5;
		}
		ScenaObj.push_back(Model3D);

		Model3D.clear();
	}
	// player movement
	playerMov.z = 1;
	// playerMov.y = 0.5;
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
	{
		switch (key)
		{
		case 'v':
			visualizzaAncora = false;
			break;
		case 'b':
			moveCamera = !moveCamera;
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}

void INIT_CAMERA_PROJECTION(void)
{
	// Imposto la telecamera

	ViewSetup = {};
	ViewSetup.position = glm::vec4(0.0, 0.5, -70.0, 1.0);
	ViewSetup.target = glm::vec4(0.0, 0.0, 0.0, 1.0);
	ViewSetup.direction = ViewSetup.target - ViewSetup.position;
	ViewSetup.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);

	// Imposto la proiezione prospettica
	PerspectiveSetup = {};
	PerspectiveSetup.aspect = (GLfloat)width / (GLfloat)height;
	PerspectiveSetup.fovY = 45.0f;
	PerspectiveSetup.far_plane = 6000.0f;
	PerspectiveSetup.near_plane = 0.1f;
}

bool isCloseTO(vec3 target, vec3 center, float meters){
	return target.x > center.x - meters && target.x < center.x + meters &&
	   target.z > center.z - meters && target.z < center.z + meters;
	   
}

void resize(int w, int h)
{
	// Imposto la matrice di Proiezione per il rendering del testo

	// Imposto la matrice di proiezione per la scena da diegnare
	Projection = perspective(radians(PerspectiveSetup.fovY), PerspectiveSetup.aspect, PerspectiveSetup.near_plane, PerspectiveSetup.far_plane);

	float AspectRatio_mondo = (float)(width) / (float)(height); // Rapporto larghezza altezza di tutto ci� che � nel mondo
																// Se l'aspect ratio del mondo � diversa da quella della finestra devo mappare in modo diverso
																// per evitare distorsioni del disegno
	if (AspectRatio_mondo > w / h)								// Se ridimensioniamo la larghezza della Viewport
	{
		glViewport(0, 0, w, w / AspectRatio_mondo);
		w_up = (float)w;
		h_up = w / AspectRatio_mondo;
	}
	else
	{ // Se ridimensioniamo la larghezza della viewport oppure se l'aspect ratio tra la finestra del mondo
	  // e la finestra sullo schermo sono uguali
		glViewport(0, 0, h * AspectRatio_mondo, h);
		w_up = h * AspectRatio_mondo;
		h_up = (float)h;
	}
}

void drawScene(void)
{
	glUniformMatrix4fv(MatrixProj, 1, GL_FALSE, value_ptr(Projection));
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	timer = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	View = lookAt(vec3(ViewSetup.position), vec3(ViewSetup.target), vec3(ViewSetup.upVector));

	// cout << playerPos.x << " " << playerPos.y << " " << playerPos.z << " " << playerPos.w << endl;
	//  Disegno Sky box
	glDepthMask(GL_FALSE);
	glUseProgram(programId1);
	glUniform1f(loctime, timer);
	glUniform1i(glGetUniformLocation(programId1, "skybox"), 0);
	glUniformMatrix4fv(MatrixProjS, 1, GL_FALSE, value_ptr(Projection));
	skyRotation += skyRotationSpeed * timer;
	mat4 skyview = rotate(View, radians(skyRotation), vec3(0, 1, 0));
	glUniformMatrix4fv(MatViewS, 1, GL_FALSE, value_ptr(skyview));
	// skybox cube
	glBindVertexArray(Scena[0].VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, Scena[0].indici.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	// Costruisco la matrice di Vista che applicata ai vertici in coordinate del mondo li trasforma nel sistema di riferimento della camera.

	// Passo al Vertex Shader il puntatore alla matrice View, che sar� associata alla variabile Uniform mat4 Projection
	// all'interno del Vertex shader. Uso l'identificatio MatView
	glPointSize(10.0);

	// Uso il program shader per gestire la riflessione della sfera (oggetto in posizione 1 di Scena)
	glUseProgram(programIdr);

	glUniformMatrix4fv(MatrixProjR, 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(MatModelR, 1, GL_FALSE, value_ptr(Scena[1].ModelM));
	glUniformMatrix4fv(MatViewR, 1, GL_FALSE, value_ptr(View));
	glUniform3f(loc_view_posR, ViewSetup.position.x, ViewSetup.position.y, ViewSetup.position.z);
	glBindVertexArray(Scena[1].VAO);
	Scena[1].ancora_world = Scena[1].ancora_obj;
	Scena[1].ancora_world = Scena[1].ModelM * Scena[1].ancora_world;
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, (Scena[1].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	// Cambio program per renderizzare tutto il resto della scena

	glUseProgram(programId);
	glUniformMatrix4fv(MatView, 1, GL_FALSE, value_ptr(View));

	// Passo allo shader il puntatore a  colore luce, posizione ed intensit�

	glUniform3f(light_unif.light_position_pointer, light.position.x + 10 * cos(radians(angolo)), light.position.y, light.position.z + 10 * sin(radians(angolo)));
	//glUniform3f(light_unif.light_position_pointer, light.position.x, light.position.y, light.position.z);

	glUniform3f(light_unif.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_unif.light_power_pointer, light.power);

	// Passo allo shader il puntatore alla posizione della camera
	glUniform3f(loc_view_pos, ViewSetup.position.x, ViewSetup.position.y, ViewSetup.position.z);

	for (int k = 2; k < Scena.size(); k++)
	{
		Scena[k].ancora_world = Scena[k].ancora_obj;
		Scena[k].ancora_world = Scena[k].ModelM * Scena[k].ancora_world;

		// Trasformazione delle coordinate dell'ancora dal sistema di riferimento dell'oggetto in sistema
		// di riferimento del mondo premoltiplicando per la matrice di Modellazione.
		// cout << Scena[k].ancora_world.r << " " << Scena[k].ancora_world.b << " " << Scena[k].ancora_world.g << endl;
		//  Passo al Vertex Shader il puntatore alla matrice Model dell'oggetto k-esimo della Scena, che sar� associata alla variabile Uniform mat4 Projection
		//  all'interno del Vertex shader. Uso l'identificatio MatModel

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k].ModelM));
		glUniform1i(lsceltaVS, Scena[k].sceltaVS);
		glUniform1i(lsceltaFS, Scena[k].sceltaFS);
		// Passo allo shader il puntatore ai materiali
		glUniform3fv(light_unif.material_ambient, 1, glm::value_ptr(materials[Scena[k].material].ambient));
		glUniform3fv(light_unif.material_diffuse, 1, glm::value_ptr(materials[Scena[k].material].diffuse));
		glUniform3fv(light_unif.material_specular, 1, glm::value_ptr(materials[Scena[k].material].specular));
		glUniform1f(light_unif.material_shininess, materials[Scena[k].material].shininess);

		glBindVertexArray(Scena[k].VAO);

		if (visualizzaAncora == true)
		{
			// Visualizzo l'ancora dell'oggetto
			int ind = Scena[k].indici.size() - 1;
			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));
		}
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//  glDrawElements(GL_TRIANGLES, (Scena[k].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

		if (k < Scena.size() - 1)
		{
			glUniform1i(loc_texture, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		else
		{
			glUniform1i(loc_texture, 0);
			glBindTexture(GL_TEXTURE_2D, texture1);
		}

		glDrawElements(GL_TRIANGLES, (Scena[k].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
	// Visualizzo gli oggetti di tipo Mesh Obj caricati dall'esterno:
	// la j-esima Mesh � costituita da ScenaObj[j].size() mesh.
	for (int j = 0; j < ScenaObj.size(); j++)
	{

		for (int k = 0; k < ScenaObj[j].size(); k++)
		{

			// Trasformazione delle coordinate dell'ancora dal sistema di riferimento dell'oggetto in sistema
			// di riferimento del mondo premoltiplicando per la matrice di Modellazione.

			// Passo al Vertex Shader il puntatore alla matrice Model dell'oggetto k-esimo della Scena, che sar� associata alla variabile Uniform mat4 Projection
			// all'interno del Vertex shader. Uso l'identificatio MatModel
			if (ScenaObj[j][k].nome == "Piper")
			{
				ScenaObj[j][k].ModelM = translate(ScenaObj[j][k].ModelM, playerMov);
				if (playerRotationQuat.w > 0)
				{
					ScenaObj[j][k].ModelM = ScenaObj[j][k].ModelM * toMat4(playerRotationQuat);
					if (k == ScenaObj[j].size() - 1)
						playerRotationQuat.w = -1;
				}
			}
			ScenaObj[j][k].ancora_world = ScenaObj[j][k].ancora_obj;
			ScenaObj[j][k].ancora_world = ScenaObj[j][k].ModelM * ScenaObj[j][k].ancora_world;

			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(ScenaObj[j][k].ModelM));
			glUniform1i(lsceltaVS, ScenaObj[j][k].sceltaVS);
			glUniform1i(lsceltaFS, ScenaObj[j][k].sceltaFS);
			// Passo allo shader il puntatore ai materiali
			glUniform3fv(light_unif.material_ambient, 1, value_ptr(ScenaObj[j][k].materiale.ambient));
			glUniform3fv(light_unif.material_diffuse, 1, value_ptr(ScenaObj[j][k].materiale.diffuse));
			glUniform3fv(light_unif.material_specular, 1, value_ptr(ScenaObj[j][k].materiale.specular));
			glUniform1f(light_unif.material_shininess, ScenaObj[j][k].materiale.shininess);

			glBindVertexArray(ScenaObj[j][k].VAO);

			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, (ScenaObj[j][k].indici.size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}

	ViewSetup.target += vec4(playerMov, 0);
	/**I need to define the airplane's motion with a direction, so i can apply the same rotation to the camera.*/
	if (!moving_trackball)
		ViewSetup.direction = ViewSetup.target - ViewSetup.position;
	// vec3 tempPos = ViewSetup.target;
	
	if (moveCamera)
		ViewSetup.position = ViewSetup.target - normalize(ViewSetup.direction) * 40.0f;
	else
		ViewSetup.position = vec4(0, 0, -30, 0);
  
	
	int randIndex = 0;
	for (int i = 0; i < 10000; i += randValue[randIndex])
	{
		randIndex = i % 1000;
		float x = treePos[i].x * MAP_SCALE;
		float z = treePos[i].z * MAP_SCALE;
		float y = treePos[i].y;
		tree.resetModelMat();
		tree.translateMainBody(x, y + 30, z);
		tree.translateBodyPart();
		tree.scaleAll(20, 60, 20);
		// animation of eyes only for some timer
		tree.rotateAll(180, 1, 0, 0);
		//cout << x << " " << z << endl;
		//if(isCloseTO(treePos[i], vec3(0, 0, 0), 10))
		if(randValue[randIndex] < 10 ||randValue[randIndex] > 90)
			tree.draw(MatModel, lsceltaVS, lsceltaFS, light_unif, loc_texture, texture, texture2);
	}
	//cout << playerMov.x << " " << playerMov.z << endl;
	glutSwapBuffers();
}
void update(int a)
{
	angolo += 1;
	glutTimerFunc(10, update, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	// glutInitContextVersion(4, 6);
	// glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	// Inizializzo finestra per il rendering della scena 3d con tutti i suoi eventi le sue inizializzazioni e le sue impostazioni

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scena 3D");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboardPressedEvent);
	glutKeyboardUpFunc(keyboardReleasedEvent);
	//  glutPassiveMotionFunc(my_passive_mouse);

	glutMotionFunc(mouseActiveMotion); // Evento tasto premuto
	glutTimerFunc(10, update, 0);
	glewExperimental = GL_TRUE;
	glewInit();

	// Inizializzazione setup illuminazione, materiali
	INIT_Illuminazione();
	// Inizializzazione setup Shader
	INIT_SHADER();
	// Inizializzazione VAO
	INIT_VAO();
	// Inizializzazione setup telecamera
	INIT_CAMERA_PROJECTION();

	// Menu collegato al tasto centrale
	buildOpenGLMenu();

	// Abilita l'uso del Buffer di Profondit� per la gestione dell'eliminazione dlele superifici nascoste
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Chiedo che mi venga restituito l'identificativo della variabile uniform mat4 Projection (in vertex shader).
	// QUesto identificativo sar� poi utilizzato per il trasferimento della matrice Projection al Vertex Shader
	MatrixProj = glGetUniformLocation(programId, "Projection");
	// Chiedo che mi venga restituito l'identificativo della variabile uniform mat4 Model (in vertex shader)
	// QUesto identificativo sar� poi utilizzato per il trasferimento della matrice Model al Vertex Shader
	MatModel = glGetUniformLocation(programId, "Model");
	// Chiedo che mi venga restituito l'identificativo della variabile uniform mat4 View (in vertex shader)
	// QUesto identificativo sar� poi utilizzato per il trasferimento della matrice View al Vertex Shader
	MatView = glGetUniformLocation(programId, "View");
	lsceltaVS = glGetUniformLocation(programId, "sceltaVS");
	lsceltaFS = glGetUniformLocation(programId, "sceltaFS");
	loc_view_pos = glGetUniformLocation(programId, "ViewPos");
	loc_texture = glGetUniformLocation(programId, "id_tex");
	// location delle variabili uniformi per la gestione della luce
	light_unif.light_position_pointer = glGetUniformLocation(programId, "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(programId, "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(programId, "light.power");

	// location delle variabili uniformi per la gestione dei materiali
	light_unif.material_ambient = glGetUniformLocation(programId, "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(programId, "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(programId, "material.specular");
	light_unif.material_shininess = glGetUniformLocation(programId, "material.shininess");

	// location variabili uniformi per lo shader della gestione della cubemap
	MatrixProjS = glGetUniformLocation(programId1, "Projection");
	// Chiedo che mi venga restituito l'identificativo della variabile uniform mat4 Model (in vertex shader)
	// QUesto identificativo sar� poi utilizzato per il trasferimento della matrice Model al Vertex Shader

	// Chiedo che mi venga restituito l'identificativo della variabile uniform mat4 Model (in vertex shader)
	// QUesto identificativo sar� poi utilizzato per il trasferimento della matrice Model al Vertex Shader
	MatViewS = glGetUniformLocation(programId1, "View");
	loctime = glGetUniformLocation(programId1, "time");

	MatModelR = glGetUniformLocation(programIdr, "Model");
	MatViewR = glGetUniformLocation(programIdr, "View");
	MatrixProjR = glGetUniformLocation(programIdr, "Projection");
	loc_view_posR = glGetUniformLocation(programIdr, "ViewPos");
	loc_cubemapR = glGetUniformLocation(programIdr, "cubemap");
	glutMainLoop();
}
