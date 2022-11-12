#include "Lib.h"

extern bool pressing_left, pressing_right, moving, jumping;
extern double 	dx;
extern double 	dy; //velocita verticale (pixel per frame)
extern double velocitaMassima; // velocita di movimento orizzontale massima
extern double delta;
extern float distacco_da_terra; //negativo, perch� lo misuro nel sistema di riferimento locale della palla
extern double accelerazione ; // forza di accelerazione data dalla tastiera
extern double decelerazione; //decelerazione in assenza di input
extern float posx ; //coordinate sul piano della posizione iniziale della palla
extern float  posy;
extern int  width;
extern float angolo;
extern vector<vec3> posProjectiles;
extern float projectileSpeed;
extern vector<BoundingBox> boundingBoxesProjectile;
extern BoundingBox boundingBoxPlayer;
extern float playerHp;
extern float bulletDamage;

void keyboardPressedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = true;
		break;

	case 'd':
		pressing_right = true;
		break;

	case ' ':
		jumping = true;
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = false;
		break;

	case 'd':
		pressing_right = false;
		break;
	
	case ' ':
		break;

	default:
		break;
	}
}
void update(int a)
{
	bool moving = false;
	//Movimento della palla in orizzontale

	if (pressing_left)
	{
		dx -= accelerazione;
		moving = true;
	}

	if (pressing_right)
	{
		dx += accelerazione;
		moving = true;
	}



	if (!moving) {   //Se non mi sto muovendo con i tasti a e d decremento od incremento la velocita' iniziale fino a portarla
					 // a zero e la palla continua a rimbalzare sul posto

		if (dx > 0)
		{
			dx -= 1;
			if (dx < 0)
				dx = 0;
		}

		if (dx < 0)
		{
			dx += 1;
			if (dx > 0)
				dx = 0;
		}
	}

	//Aggioramento della posizione in x della pallina, che regola il movimento orizzontale

	posx += dx;



	//Se la pallina assume una posizione in x minore di 0 o maggiore di width dello schermo
	//facciamo rimbalzare la pallina ai bordi dello schermo

	if (posx < 0) {
		posx = 0;
		dx = -dx * 0.8;
	}

	if (posx > width) {
		posx = width;
		dx = -dx * 0.8;
	}

	// Gestione del rimbalzo e quindi dell'altezza da terra

	//Rimbalzo
	if(jumping){
		dy -= delta;
		distacco_da_terra -= dy;
	}

	//printf("dy %f  Distacca da terra %f \n", dy, distacco_da_terra);
	 
   if (distacco_da_terra > 30)
   {
		jumping = false;
	   distacco_da_terra = 30;
	   dy = 30;   //Una volta giunta a terra la pallina ottiene un impulso positivo che la ritornare su
   }



	
	glutPostRedisplay();
	glutTimerFunc(30, update, 0);
}

float getXFromButterfly(float t){
	return sin(t)*(exp(cos(t))-2*cos(4*t)); 
}
float getYFromButterfly(float t){
	return cos(t)*(exp(cos(t))-2*cos(4*t));
}

void update_pala(int a)
{
	
	angolo+=20;
	glutPostRedisplay();
	glutTimerFunc(24, update_pala, 0);
}

void update_projectiles(int a){



	for(int i = 0; i < posProjectiles.size(); i++){

		//don't do collisione check at begining
		if(boundingBoxPlayer.getBottomCorner().x == boundingBoxPlayer.getTopCorner().x){
			break;
		}
		//cout << i << " -> " <<  boundingBoxesProjectile[i].getBottomCorner().x << " " <<  boundingBoxesProjectile[i].getBottomCorner().y << " , " << boundingBoxesProjectile[i].getTopCorner().x << " " << boundingBoxesProjectile[i].getTopCorner().y  << endl;; 
		//cout << boundingBoxPlayer.getBottomCorner().x << " " <<  boundingBoxPlayer.getBottomCorner().y << " , " << boundingBoxPlayer.getTopCorner().x << " " << boundingBoxPlayer.getTopCorner().y  << endl;; 
		if(boundingBoxesProjectile[i].checkCollision(boundingBoxPlayer) && boundingBoxesProjectile[i].isAlive()){
			//cout << " hit " << endl;
			//exit(0);
			playerHp -= bulletDamage;
			boundingBoxesProjectile[i].setAlive(false);
		}
		if(playerHp <= -0.99){
			exit(0);
		}
		posProjectiles[i].x -= projectileSpeed;
	}

	glutPostRedisplay();
	glutTimerFunc(24, update_projectiles, 0);
}