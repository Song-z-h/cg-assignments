
#include <iostream>
#include "ShaderMaker.h"
#include "CreateFigure.h"
static unsigned int programId, programId1;

mat4 Projection;
GLuint MatProj, MatModel, loctime, locres, colorLerp, playerPos, playerHP;
int nv_P;
// viewport size
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
vector<BoundingBox> boundingBoxesProjectile; // bounding boxes for bullets
BoundingBox boundingBoxPlayer;				 // bunding box to update player
BoundingBox projectiles[nEnemy];			 // single boundingBoxes of bullets
float playerHp = 1;							 // player hp bar ranging from -1 to 1
float bulletDamage = 0.2;					 // the dmg
float projectileSpeed = 10;

void costruisci_Sole(vec4 color_top, vec4 color_bot, vec4 color_top_alone, vec4 color_bot_alone, Figura *Sole)
{

	int i;
	float stepA = (2 * PI) / Sole->nTriangles;
	float t;

	int x = 1, y = 1;

	Sole->vertici.push_back(vec3(0.0, 0.0, 0.0));
	Sole->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	vec4 colorOffset = vec4(0.3);

	for (i = 0; i <= Sole->nTriangles; i++)
	{
		t = (float)i * stepA;
		Sole->vertici.push_back(vec3(cos(t), sin(t), 0.0));

		if (i > Sole->nTriangles / 2)
		{
			x = 5;
			y = 2;
			color_bot = colorOffset;
			color_top = colorOffset;
			Sole->vertici.push_back(vec3(cos(t) * 5 * x, sin(t) * 5 * y, 0.0));
			Sole->vertici.push_back(vec3(cos(t) * 2 * x, sin(t) * 2 * y, 0.0));
		}
		else
		{
			Sole->vertici.push_back(vec3(cos(t) * 2 * x, sin(t) * 2 * y, 0.0));
			Sole->vertici.push_back(vec3(cos(t) * 5 * x, sin(t) * 5 * y, 0.0));
		}

		// Sole->vertici.push_back(vec3(cos(t)* 2 * x, sin(t)* 2 * y,0.0));
		// Sole->vertici.push_back(vec3(cos(t)* i, sin(t)* i,0.0));
		// Colore
		Sole->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
		Sole->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
		Sole->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	}

	// Costruzione matrice di Moellazione Sole, che rimane la stessa(non si aggiorna durante l'animazione)
	Sole->nv = Sole->vertici.size();
	// Sole->Model = mat4(1.0);
	// Sole->Model = translate(Sole->Model, vec3(float(width) * 0.5, float(height) * 0.5, 0.0));
	// Sole->Model = scale(Sole->Model, vec3(30.0, 30.0, 1.0));
}

void costruisci_montagne(int numero_di_montagne, vec4 color_top, vec4 color_bot, Figura *v_montagna)
{
	int i = 0;

	float step = 1 / (float)v_montagna->nTriangles;
	float frequenza = PI * numero_di_montagne;
	float t;
	int n_vertici = 0;

	for (i = 0; i < v_montagna->nTriangles; i += 1)
	{
		t = i / (float)v_montagna->nTriangles * frequenza;
		v_montagna->vertici.push_back(vec3(i * step, 0, 0));
		v_montagna->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
		v_montagna->vertici.push_back(vec3(i * step, abs(sin(t)), 0.0));

		v_montagna->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	}
	v_montagna->nv = v_montagna->vertici.size();

	// Costruzione matrice di Modellazione Montagne, che rimane la stessa(non si aggiorna)
	v_montagna->Model = mat4(1.0);
	v_montagna->Model = translate(v_montagna->Model, vec3(0.0, float(height) / 2, 0.0));
	v_montagna->Model = scale(v_montagna->Model, vec3(float(width), float(height) / 8, 1.0));
}

void costruisci_cielo(vec4 color_top, vec4 color_bot, Figura *cielo)
{

	cielo->vertici.push_back(vec3(0.0, 0.0, 0.0));
	cielo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_top.a));
	cielo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	cielo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
	cielo->vertici.push_back(vec3(0.0, 1.0, 0.0));
	cielo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	cielo->vertici.push_back(vec3(1.0, 1.0, 0.0));
	cielo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

	cielo->nv = cielo->vertici.size();

	// Costruzione matrice di Modellazione Sole, che rimane la stessa(non si aggiorna)
	cielo->Model = mat4(1.0);
	cielo->Model = translate(cielo->Model, vec3(0.0, float(height) / 2, 0.0));
	cielo->Model = scale(cielo->Model, vec3(float(width), float(height) / 2, 1.0));
}
void costruisci_prato(vec4 color_top, vec4 color_bot, Figura *prato)
{

	prato->vertici.push_back(vec3(0.0, 0.0, 0.0));
	prato->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	prato->vertici.push_back(vec3(0.0, 1.0, 0.0));
	prato->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	prato->vertici.push_back(vec3(1.0, 0.0, 0.0));
	prato->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	prato->vertici.push_back(vec3(1.0, 1.0, 0.0));
	prato->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	prato->nv = prato->vertici.size();

	// Costruzione matrice di Modellazione Prato, che rimane la stessa(non si aggiorna)
	prato->Model = mat4(1.0);
	prato->Model = scale(prato->Model, vec3(float(width), float(height) / 2, 1.0));
}

void costruisci_farfalla(float cx, float cy, float raggiox, float raggioy, vec4 color_top, vec4 color_bot, Figura *fig)
{

	int i;
	float stepA = (2 * PI) / fig->nTriangles;
	float t;

	fig->vertici.push_back(vec3(cx, cy, 0.0));

	fig->colors.push_back(color_top);

	for (i = 0; i <= fig->nTriangles; i++)
	{
		t = (float)i * stepA;
		fig->vertici.push_back(vec3(cx + raggiox * (sin(t) * (exp(cos(t)) - 2 * cos(4 * t)) + pow(sin(t / 12), 5)) / 4, cy + raggioy * (cos(t) * (exp(cos(t)) - 2 * cos(4 * t)) + pow(sin(t / 12), 5)) / 4, 0.0));
		// Colore
		fig->colors.push_back(color_bot);
	}
	fig->nv = fig->vertici.size();
}

double lerp(double a, double b, double amount)
{

	// Interpolazione lineare tra a e b secondo amount

	return (1 - amount) * a + amount * b;
}

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char *vertexShader = (char *)"vertexShader_M.glsl";
	char *fragmentShader = (char *)"fragmentShader_M_new.glsl";

	// char*  fragmentShader1 = (char*)"fragmentShader_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	// programId1 = ShaderMaker::createProgram(vertexShader, fragmentShader1);
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
	Scena.push_back(Cielo);
	Scena.push_back(Prato);
	Scena.push_back(Sole);
	Scena.push_back(Palla);

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
	character.addBodypart(wings, 0, 0, 0, 3);
	character.addBodypart(body);
	character.addBodypart(head, -0.1, 1, 0);
	character.addBodypart(nose, 0, 4.2, 0, 0.2);
	character.addBodypart(eye1, 1, 6, 0, 0.2);
	character.addBodypart(eye2, -1, 6, 0, 0.2);
	character.addBodypart(catCloth, 0, 0.2, 0, 1.5);

	player.addBodypart(body, 0, 0, 0, 0.001);
	// player.addBodypart(wings, 0, 0, 0, 3);
	player.addBodypart(body);
	player.addBodypart(head, -0.1, 1, 0);
	player.addBodypart(nose, 0, 4.2, 0, 0.2);
	player.addBodypart(eye1, 1, 6, 0, 0.2);
	player.addBodypart(eye2, -1, 6, 0, 0.2);
	// player.addBodypart(catCloth, 0, 0.2, 0, 1.5);

	// Costruzione della matrice di Proiezione
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	loctime = glGetUniformLocation(programId, "time");
	locres = glGetUniformLocation(programId, "resolution");
	colorLerp = glGetUniformLocation(programId, "colorLerp");
	playerPos = glGetUniformLocation(programId, "playerPos");
	playerHP = glGetUniformLocation(programId, "playerHp");
	for (int i = 0; i < nEnemy; i++)
	{
		posProjectiles.push_back(vec3(0, 0, 0));

		projectiles[i].addVertices(Palla.vertici);
		boundingBoxesProjectile.push_back(projectiles[i]);
	}
	boundingBoxPlayer.addVertices(body.vertici);
}
void drawScene(void)
{

	int k;
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));

	// Disegno Cielo

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	vec2 resolution = vec2((float)width, (float)height);

	glUseProgram(programId);
	glUniform1f(loctime, time);
	glUniform2f(locres, resolution.x, resolution.y);
	glUniform1f(playerHP, playerHp);
	float fadeAmount = 3;

	float distacco_da_terra_n = -distacco_da_terra;
	float sun_scale = lerp(0.1, 0.8, (float)distacco_da_terra_n / 255);

	float shadow_scale = lerp(1, 0, (float)distacco_da_terra_n / 255);
	if (shadow_scale < 0)
		shadow_scale = 0;

	float bwidth = distacco_da_terra_n < 0 ? lerp(80, 100, (float)abs(distacco_da_terra_n) / 20) : 80; // larghezza effettiva in pixel della palla
	float bheight = distacco_da_terra_n < 0 ? 80 + distacco_da_terra_n : 80;						   // altezza effettiva in pixel della palla

	// glUseProgram(programId1);
	glUniform1f(colorLerp, (float)lerp(0.68, 1.0, (float)distacco_da_terra_n / 255));

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

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[k].Model));
		glBindVertexArray(Scena[k].VAO);

		if (k == 2)
		{
			glDrawArrays(GL_TRIANGLES, 0, Scena[k].nv);
		}
		else
			glDrawArrays(GL_TRIANGLE_STRIP, 0, Scena[k].nv);
		glBindVertexArray(0);
	}

	// Scena[indexK].Model = scale(Scena[indexK].Model, vec3(float(bwidth) / 2, float(bheight) / 2, 1.0));
	// Disegna Corpo della palla

	/*
	 //matrice di Trasformazione della Palla
	 Scena[indexK].Model = mat4(1.0);
	 Scena[indexK].Model = translate(Scena[indexK].Model, vec3(posx - bwidth / 2 * shadow_scale, posy + 10 + 10 * (1 - shadow_scale), 0.0f));
	 Scena[indexK].Model = scale(Scena[indexK].Model, vec3(float(bwidth) * shadow_scale, (50.0 * shadow_scale), 1.0));
	 glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[indexK].Model));
	 //Disegna Ombra
	 glDrawArrays(GL_TRIANGLE_FAN, (Scena[indexK].nTriangles) + 2, (Scena[indexK].nTriangles) + 2);

	glBindVertexArray(0);
	*/

	// Disegno 6 PALE EOLICHE
	/*glBindVertexArray(Scena[4].VAO);

		//Disegno il sostegno
		//Definisco la matrice di Trasformazione per il sostegno che rimane fisso durante l'animazione

		Scena[4].Model = mat4(1.0);
		Scena[4].Model = translate(Scena[4].Model, vec3(posx - bwidth / 2, posy + bheight + distacco_da_terra_n, 0.0f));
		//Scena[4].Model = translate(Scena[4].Model, vec3(float(width) * 0.15, float(height) * 0.65, 0.0));
		Scena[4].Model = scale(Scena[4].Model, vec3(3.0, 100.0, 1.0));


		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[4].Model));


		glDrawArrays(GL_TRIANGLE_STRIP, Scena[4].nv - 4, 4);
	*/

	// Disegna la Pala
	// Definisco la matrice di trasformazione per la Pala che ruota
	/*
	int k = 3;
   glBindVertexArray(Scena[k].VAO);
   Scena[k].Model = mat4(1.0);
   Scena[k].Model = translate(Scena[k].Model, vec3(width/2, height/2, 0.0f));
   //Scena[4].Model = translate(Scena[4].Model, vec3(float(width) * 0.15, float(height) * 0.65, 0.0));
   Scena[k].Model = scale(Scena[k].Model, vec3(40.0, 40.0, 1.0));
   Scena[k].Model = rotate(Scena[k].Model, radians(angolo), vec3(0.0f, 0.0f, 1.0f));
   glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[4].Model));

   glDrawArrays(GL_TRIANGLES, 0, Scena);
   */

	// character.rotateAll(90, 0, 0, 1);
	for (int i = 0; i < nEnemy; i++)
	{
		if (posProjectiles[i].x < 0)
		{
			posProjectiles[i].x = getXFromButterfly(time / 2 + 100 * i) * 70 + 0.8 * width;
			posProjectiles[i].y = 300 + getYFromButterfly(time / 2 + 100 * i) * 70;
			boundingBoxesProjectile[i].setAlive(true);
		}

		character.translateMainBody(getXFromButterfly(time / 2 + 100 * i) * 70 + 0.8 * width, 300 + getYFromButterfly(time / 2 + 100 * i) * 70);
		character.animation(1, sin(time * 3 + 10 * i), 1);
		character.translateBodyPart();
		character.scaleAll(40, 40);
		character.draw(MatModel);
	}

	player.translateMainBody(posx - bwidth / 2, posy + bheight / 2 + distacco_da_terra_n, 0.0f);
	// splayer.animation(1, sin(time*3), 1);
	player.translateBodyPart();
	player.scaleAll(40, 40);
	player.draw(MatModel);
	boundingBoxPlayer.updateModel(player.getBodyPartsModel(1));

	glBindVertexArray(0);
	int indexK = 3;
	// Palla ed Ombra
	glBindVertexArray(Scena[indexK].VAO);
	// matrice di Trasformazione della Palla
	for (int i = 0; i < nEnemy; i++)
	{
		Scena[indexK].Model = mat4(1.0);
		Scena[indexK].Model = translate(Scena[indexK].Model, vec3(posProjectiles[i].x, posProjectiles[i].y, 0.0f));
		// Scena[indexK].Model = rotate(Scena[indexK].Model, radians(angolo), vec3(0.0f, 0.0f, 1.0f));
		Scena[indexK].Model = scale(Scena[indexK].Model, vec3(10 + time, 10 + time, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[indexK].Model));
		if (boundingBoxesProjectile[i].isAlive())
			glDrawArrays(GL_TRIANGLE_FAN, 0, (Scena[indexK].vertici.size()));
		boundingBoxesProjectile[i].updateModel(Scena[indexK].Model);
	}

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scena OpenGL");
	glutDisplayFunc(drawScene);

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
