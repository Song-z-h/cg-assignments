#ifndef _CREATE_FIGURE_H_
#define _CREATE_FIGURE_H_
#include "Mesh.h"
#include "Lib.h"

#define  PI   3.14159265358979323846


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


#endif