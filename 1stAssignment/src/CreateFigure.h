#ifndef _CREATE_FIGURE_H_
#define _CREATE_FIGURE_H_
#include "Mesh.h"
#include "Lib.h"

#define  PI   3.14159265358979323846
extern int width;
extern int height;

void crea_VAO_Vector(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3),fig->vertici.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_DYNAMIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}

void costruisci_palla(vec4 color_top, vec4 color_bot, vec4 color_top_ombra, vec4 color_bot_ombra, Figura* palla)
{
	int i;
	float stepA = (2 * PI) / palla->nTriangles;
	float t;


	palla->vertici.push_back(vec3(0.0, 0.0, 0.0));
	palla->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	for (i = 0; i <= palla->nTriangles; i++)
	{
		t = (float)i * stepA;

		palla->vertici.push_back(vec3(cos(t), sin(t), 0.0));
		//Colore 
		palla->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	}
}

void costruisci_personaggio(string name, vec4 color_top, vec4 color_bot, Figura* persona)
{
	int i;

	vector<float> points;
	string myText;
	ifstream MyReadFile(name);
	float x = 0, y = 0, z = 0;

	persona->vertici.push_back(vec3(0.0, 0.0, 0.0));
	persona->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	while (getline(MyReadFile, myText)) {
  	// Output the text from the file
		stringstream ss(myText);
		ss >> x;
		ss >> y;
		ss >> z;
		persona->vertici.push_back(vec3(x, y, z));
		persona->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	}
	MyReadFile.close();

}


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



#endif