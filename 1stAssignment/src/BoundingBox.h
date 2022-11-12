#ifndef boundingBox_H
#define boundingBox_H

#include <list>

#include <iostream>
#include "ShaderMaker.h"
#include "Lib.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

class BoundingBox
{
private:
	//mat4 Model = mat4(1.0);
	vec4 top;
	vec4 bot;
	vec4 corner_b;
	vec4 corner_t;
	bool alive = true;
public:

    BoundingBox(){

	}
    ~BoundingBox(){

	}
	vec4 getTopCorner(){
		return vec4(corner_t);
	};
	vec4 getBottomCorner(){
		return vec4(corner_b);
	};

	bool isAlive(){
		return alive;
	}

	void setAlive(bool alive){
		this->alive = alive;
	}
    bool checkCollision(BoundingBox box){

		bool collisionX = getBottomCorner().x <= box.getTopCorner().x &&
			getTopCorner().x >= box.getBottomCorner().x;
			//cout << box.getTopCorner().x << endl;
		
		// guardo collisioni su asse y
		bool collisionY = getBottomCorner().y <= box.getTopCorner().y &&
			getTopCorner().y >= box.getBottomCorner().y;
		//Si ha collisione se c'� collisione sia nella direzione x che nella direzione y

		return (collisionX && collisionY);
	}
	void updateModel(mat4 matModel){
		//Model = matModel;
		corner_b = matModel * bot;
		corner_t = matModel * top;
	}

void addVertices(vector<vec3> vertici){
    float xmin,xmax,ymin,ymax;
	xmin =vertici[0].x;
	xmax = vertici[0].x;
	ymax = vertici[0].y;
	ymin = vertici[0].y;
	for(int i=1;i<vertici.size();i++){
		//cout << vertici[i].x << " " << vertici[i].y << endl;
		if(vertici[i].x < xmin)
			xmin = vertici[i].x;
		if(vertici[i].x > xmax)
			xmax = vertici[i].x;
		if(vertici[i].y < ymin)
			ymin = vertici[i].y;
		if(vertici[i].y > ymax)
			ymax = vertici[i].y;
	}
	bot = vec4(xmin, ymin, 0.0, 1.0);
	top = vec4(xmax, ymax, 0.0, 1.0);
}


};
	
#endif