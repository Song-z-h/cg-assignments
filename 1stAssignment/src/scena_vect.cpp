
#include <iostream>
#include "ShaderMaker.h"
#include "CreateFigure.h"
static unsigned int programId, programId1;// scene and characters

// for programId
mat4 Projection;
GLuint MatProj, MatModel, loctime, locres, colorLerp, playerPos, playerHP, originalResolution;

// for programId1
GLuint MatProj1, MatModel1;

int nv_P;
// viewport size
int w_update = 1400, h_update = 800;
int width = 1400;
int height = 800;

// parametri della palla
float distacco_da_terra = 31; // distacco da terra
double delta = 1.5;			  // forza di gravita
double dy = 20;				  // velocita verticale (pixel per frame)
double dx = 0;				  // velocita orizzontale (pixel per frame)

double accelerazione = 1;	   // forza di accelerazione data dalla tastiera
double decelerazione = 1;	   // decelerazione in assenza di input
float posx = float(width) / 2; // coordinate sul piano della posizione iniziale della palla
float posy = float(height) * 0.2;

// Booleani posti a true se si usa il tasto a (spostamento a sinistra) e b (spostamento a destra)
bool pressing_left = false;
bool pressing_right = false;
bool jumping = false;

Figura Cielo = {};
Figura Prato = {};
Figura Sole = {};
Figura Palla = {};
Figura nose = {};
Figura head = {};
Figura eye1 = {};
Figura eye2 = {};
Figura body = {};
Figura wings = {};
Figura catCloth = {};

// objects for rendering the scene
vector<Figura> Scena;

// complex objects consistutes the player and enemy using hermite curve
Mesh character(GL_TRIANGLE_FAN);
Mesh player(GL_TRIANGLE_FAN);

// some parameters to interact with the game
float angolo = 0.0;
const int nEnemy = 5; // the number of fairies

vector<vec3> posProjectiles;				 // posiiton used to update bullets
vector<BoundingBox> 	boundingBoxesProjectile; // bounding boxes for bullets
BoundingBox boundingBoxPlayer;				 // bunding box to update player
BoundingBox projectiles[nEnemy];			 // single boundingBoxes of bullets
float playerHp = 1;							 // player hp bar ranging from -1 to 1
float bulletDamage = 0.1;					 // the dmg
float projectileSpeed = 10;
vector<vec3> scaleEnemies;	

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char *vertexShader = (char *)"vertexShader_M.glsl";
	char *fragmentShader = (char *)"fragmentShader_M_new.glsl";

	//char *vertexShader1 = (char *)"vertexShader_M.glsl";
	char *fragmentShader1 = (char *)"fragmentShader_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	programId1 = ShaderMaker::createProgram(vertexShader, fragmentShader1);
}

void INIT_VAO(void)
{
	vec4 col_top = {0.0, 0.4980, 1.0, 1.0};
	vec4 col_bottom = {0.8471, 0.4020, 0.20, 1.0};
	costruisci_cielo(col_top, col_bottom, &Cielo);
	crea_VAO_Vector(&Cielo);

	col_top = vec4{0.1333, 0.5451, 0.1333, 1.0000};
	col_bottom = vec4{0.6784, 1.0, 0.1843, 1.0000};
	costruisci_prato(col_top, col_bottom, &Prato);
	crea_VAO_Vector(&Prato);

	Sole.nTriangles = 30;
	col_top = vec4{1.0, 0.9, 0.9, 0.8};
	col_bottom = vec4{1.0, 0.8627, 0.0, 1.0000};
	vec4 col_top_alone = {1.0, 1.0, 1.0, 0.0};
	vec4 col_bottom_alone = {1.0, 0.8627, 0.0, 1.0000};
	costruisci_Sole(col_top, col_bottom, col_top_alone, col_bottom_alone, &Sole);
	crea_VAO_Vector(&Sole);

	Palla.nTriangles = 50;
	col_top = {1.0, 0.0, 0.0, 1.0};
	col_bottom = {1.0, 0.8, 0.0, 1.0};
	vec4 col_top_ombra = {0.49, 0.49, 0.49, 0.0};
	vec4 col_bottom_ombra = {0.0, 0.0, 0.0, 0.6};
	costruisci_palla(col_top, col_bottom, col_top_ombra, col_bottom_ombra, &Palla);
	crea_VAO_Vector(&Palla);

	// order of scene objects
	Scena.push_back(Cielo); // 0
	Scena.push_back(Prato); // 1
	Scena.push_back(Sole);	// 2
	Scena.push_back(Palla); // 3

	wings.nTriangles = 80;
	col_top = {0.2, 0.01, 0.05, 1.0};
	col_bottom = {1.0, 0.0, 0.0, 1.0};
	costruisci_farfalla(0, 0, 1, 1, col_top, col_bottom, &wings);
	crea_VAO_Vector(&wings);

	catCloth.nTriangles = 5;
	col_top = {0.2, 0.1, 0.7, 1.0};
	col_bottom = {0.0, 0.0, 1.0, 0.3};
	costruisci_farfalla(0, 0, 1, 1, col_top, col_bottom, &catCloth);
	crea_VAO_Vector(&catCloth);

	// create my character's body
	col_top = {1.0, 1.0, 1.0, 1.0};
	col_bottom = {0.3, 0.0, 0.0, 1.0};
	costruisci_personaggio("res/nose.txt", col_top, col_bottom, &nose);
	crea_VAO_Vector(&nose);

	// create head
	col_top = {0.7, 0.6, 0.3, 1.0};
	col_bottom = {1, 0.8, 0.0, 1.0};
	costruisci_personaggio("res/head.txt", col_top, col_bottom, &head);
	crea_VAO_Vector(&head);
	costruisci_personaggio("res/body.txt", col_top, col_bottom, &body);
	crea_VAO_Vector(&body);

	col_top = {0.0, 0.2, .2, .2f};
	col_bottom = {0.2, .5, 16.0, 0.1};
	costruisci_personaggio("res/eyes.txt", col_top, col_bottom, &eye1);
	crea_VAO_Vector(&eye1);
	costruisci_personaggio("res/eyes.txt", col_top, col_bottom, &eye2);
	crea_VAO_Vector(&eye2);

	character.addBodypart(body, 0, 0, 0, 0.001);
	character.addBodypart(wings, 0, 0, 0, 3.5);
	character.addBodypart(body);
	character.addBodypart(head, -0.1, 1, 0);
	character.addBodypart(nose, 0, 4.2, 0, 0.2);
	character.addBodypart(eye1, 1, 6, 0, 0.2);
	character.addBodypart(eye2, -1, 6, 0, 0.2);
	character.addBodypart(catCloth, 0, 0.2, 0, 1.5);

	col_top = {1.0, 0.6, 0.0, 1.0};
	col_bottom = {1.0, 0.2, 0.0, 1.0};
	costruisci_personaggio("res/head.txt", col_top, col_bottom, &head);
	crea_VAO_Vector(&head);
	costruisci_personaggio("res/body.txt", col_top, col_bottom, &body);
	crea_VAO_Vector(&body);

	player.addBodypart(body, 0, 0, 0, 0.001);
	// player.addBodypart(wings, 0, 0, 0, 3);
	player.addBodypart(body, 0, 0, 0, 1.3);
	player.addBodypart(head, -0.1, 1, 0);
	player.addBodypart(nose, 0, 4.2, 0, 0.2);
	player.addBodypart(eye1, 1, 6, 0, 0.2);
	player.addBodypart(eye2, -1, 6, 0, 0.2);
	// player.addBodypart(catCloth, 0, 0.2, 0, 1.5);

	// Costruzione della matrice di Proiezione per scena
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	loctime = glGetUniformLocation(programId, "time");
	locres = glGetUniformLocation(programId, "resolution");
	originalResolution =  glGetUniformLocation(programId, "originalResolution");
	colorLerp = glGetUniformLocation(programId, "colorLerp");
	playerPos = glGetUniformLocation(programId, "playerPos");
	playerHP = glGetUniformLocation(programId, "playerHp");

	// creation of matrices for objects
	MatProj1 = glGetUniformLocation(programId1, "Projection");
	MatModel1 = glGetUniformLocation(programId1, "Model");

	// add bounding boxes to objects
	for (int i = 0; i < nEnemy; i++)
	{
		posProjectiles.push_back(vec3(0, 0, 0));
		scaleEnemies.push_back(vec3(1, 1, 1));
		projectiles[i].addVertices(Palla.vertici);
		boundingBoxesProjectile.push_back(projectiles[i]);
	}
	boundingBoxPlayer.addVertices(player.getAllVertices());

	
}
void drawScene(void)
{

	int k;
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	

	// Disegno Cielo

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	vec2 resolution = vec2((float)w_update, (float)h_update);

	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glUniform1f(loctime, time);
	glUniform2f(locres, resolution.x, resolution.y);
	glUniform1f(playerHP, playerHp);
	glUniform2f(originalResolution, width, height);

	float distacco_da_terra_n = -distacco_da_terra;
	float sun_scale = lerp(0.1, 0.8, (float)distacco_da_terra_n / 255);

	float bwidth = distacco_da_terra_n < 0 ? lerp(80, 100, (float)abs(distacco_da_terra_n) / 20) : 80; // larghezza effettiva in pixel della palla
	float bheight = distacco_da_terra_n < 0 ? 80 + distacco_da_terra_n : 80;						   // altezza effettiva in pixel della palla

	// glUseProgram(programId1);
	glUniform1f(colorLerp, (float)lerp(0.58, 1.0, (float)distacco_da_terra_n / 255));
	glUniform2f(playerPos, posx - bwidth / 2, posy + bheight / 2 + distacco_da_terra_n);

	for (k = 0; k < Scena.size(); k++)
	{
		if (k == 2)
		{
			// sole
			Scena[k].Model = mat4(1.0);
			Scena[k].Model = translate(Scena[k].Model, vec3(float(width) * 0.5, float(height) * 0.5, 0.0));
			Scena[k].Model = scale(Scena[k].Model, vec3(30.0, 30.0, 1.0));
			Scena[k].Model = scale(Scena[k].Model, vec3(1, sun_scale, 1.0));
		}

		glUniformMatrix4fv(MatModel1, 1, GL_FALSE, value_ptr(Scena[k].Model));
		glBindVertexArray(Scena[k].VAO);

		if (k == 2)
		{ // sole
			glDrawArrays(GL_TRIANGLES, 0, Scena[k].nv);
		}
		else
			glDrawArrays(GL_TRIANGLE_STRIP, 0, Scena[k].nv);
		glBindVertexArray(0);
	}

	glUseProgram(programId1);
	glUniformMatrix4fv(MatProj1, 1, GL_FALSE, value_ptr(Projection));
	// draw enemies
	for (int i = 0; i < nEnemy; i++)
	{
		if (posProjectiles[i].x < 0)
		{
			posProjectiles[i].x = getXFromButterfly(time / 2 + 100 * i) * 70 + 0.8 * width;
			posProjectiles[i].y = 300 + getYFromButterfly(time / 2 + 100 * i) * 70;
			boundingBoxesProjectile[i].setAlive(true);
		}

		character.translateMainBody(getXFromButterfly(time / 2 + 100 * i) * 70 + 0.8 * width, 300 + getYFromButterfly(time / 2 + 100 * i) * 70);
		character.translateBodyPart();
		character.animation(1, sin(time * 3 + 10 * i), 1);
		character.scaleOne(1 ,scaleEnemies[i].x, scaleEnemies[i].y);
		//character.animation(5, 1, abs(sin(time*3 + 10 * i)));
		//character.animation(6, 1, abs(sin(time*3 + 10 * i)));
		character.scaleAll(40, 40);
		character.draw(MatModel1);
	}

	// draw player
	player.translateMainBody(posx - bwidth / 2, posy + bheight / 2 + distacco_da_terra_n, 0.0f);
	player.translateBodyPart();
	player.animation(4, 1, abs(sin(time*3)));
	player.animation(5, 1, abs(sin(time*3)));
	player.scaleAll(60, 60);
	player.draw(MatModel1);
	boundingBoxPlayer.updateModel(player.getBodyPartsModel(1));

	glBindVertexArray(0);
	int indexK = 3;
	glBindVertexArray(Scena[indexK].VAO);
	// matrice di Trasformazione della Palla
	for (int i = 0; i < nEnemy; i++)
	{
		Scena[indexK].Model = mat4(1.0);
		Scena[indexK].Model = translate(Scena[indexK].Model, vec3(posProjectiles[i].x, posProjectiles[i].y, 0.0f));
		Scena[indexK].Model = scale(Scena[indexK].Model, vec3(10 + time, 10 + time, 1.0));

		glUniformMatrix4fv(MatModel1, 1, GL_FALSE, value_ptr(Scena[indexK].Model));
		if (boundingBoxesProjectile[i].isAlive())
			glDrawArrays(GL_TRIANGLE_FAN, 0, (Scena[indexK].vertici.size()));
		boundingBoxesProjectile[i].updateModel(Scena[indexK].Model);
	}

	glutSwapBuffers();
}

void resizeWindow(int w, int h){
	Projection = ortho(0.0f, float(width), 0.0f, float(height));	
	float windowRatio = (float)width/ (float)height;

	if(windowRatio > w /h){
		//width is less than original aspect ratio, then we reduce the scrren with width
		glViewport(0, 0, w, w / windowRatio);
		w_update = (float)w;
		h_update = w / windowRatio;
	}else{
		//width is more, then we resize the window with height
		glViewport(0, 0, h * windowRatio, h);
		w_update = h * windowRatio;
		h_update = (float)h;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("first assignment song zhaohui");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboardPressedEvent);
	// Evento tastiera tasto rilasciato

	glutKeyboardUpFunc(keyboardReleasedEvent);

	// gestione animazione
	glutTimerFunc(66, update, 0);
	glutTimerFunc(66, update_pala, 0);
	glutTimerFunc(66, update_projectiles, 0);

	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
}
