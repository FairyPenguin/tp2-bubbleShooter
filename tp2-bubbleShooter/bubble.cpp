#include "bubble.h"

/* Constructeur de la classe "Bubble"
====================================== */
Bubble::Bubble(int colorValue)
{

	spriteSheet = loadBitmap("bubble.bmp");
	color = colorValue;
	initCharset(sprite, NB_COLORS);
	inGameStatus = true;

}

/* Destructeur de la classe "Bubble" 
====================================== */
Bubble::~Bubble()
{
	//Lib�rer la surface des bulles de la m�moire
	SDL_FreeSurface(spriteSheet);
}

/* Retourne la position d'une bulle donn�e
============================================ */
SDL_Rect Bubble::getPosition() 
{
	return position;
}

/* Retourne la couleur d'une bulle donn�e 
========================================== */
int Bubble::getColor() 
{
	return color;
}

/* Retourne la veleur indiquant si une bulle donn�e a explos� (et est donc inaccessible)
========================================================================================= */
bool Bubble::getInGameStatus()
{
	return inGameStatus;
}

/* Retourne la vitesse en X d'une bulle donn�e
=============================================== */
int Bubble::getVelocityX() 
{
	return velocityX;
}

/* Retourne la vitesse en Y d'une bulle donn�e
=============================================== */
int Bubble::getVelocityY()
{
	return velocityY;
}

/* Retourne la hitbox d'une bulle donn�e
=============================================== */
Bubble::Circle Bubble::getHitbox() 
{
	return hitbox;
}

/* Calcule et assigne � la bulle une vitesse en X et en Y
============================================================= */
void Bubble::setVelocity(int velX, int velY) 
{
	//TODO: Calculs dans cette section pour trouver la bonne v�locit�. Va falloir faire un peu de maths.
	velocityX = velX;
	velocityY = velY;
}

/* Met en place la hitbox de la bulle
================================================= */
void Bubble::setHitbox(SDL_Rect &position, int radius) 
{
	hitbox.radius = BUBBLE_RADIUS;	//le rayon du cercle
	
	//le centre du cercle en pixels
	hitbox.centerX = position.x + BUBBLE_RADIUS;
	hitbox.centerY = position.y + BUBBLE_RADIUS;
}

/* Bouge une bulle dans l'espace et v�rifie s'il y a des collisions avec le bord de l'�cran
============================================================================================= */
void Bubble::move() 
{
	//Si la position en X touche les murs de l'�cran,  la v�locit� en X s'inverse (multipli�e par -1) et la bulle recule pour �tre coll�e au bord
	//Puisque la bulle monte toujours, la v�locit� en Y demeure inchang�e ici. Elle sera plut�t v�rifi�e dans la fonction checkCollisions().

	if (position.x + 47 > SCREEN_WIDTH)		//si la balle touche le mur de droite
	{
		int newVelocity = velocityX * (-1);
		velocityX = newVelocity;

	}
	else if (position.x < 0)				//si la balle touche le mur de gauche
	{
		int newVelocity = velocityX * (-1);
		velocityX = newVelocity;

	}
	
	//Bouge la bulle dans l'espace
	position.x += velocityX;
	position.y += velocityY;
}

/* Affiche la bulle � une position donn�e dans l'�cran
======================================================== */
void Bubble::update(SDL_Surface *screen, int x, int y) 
 {
	 if (inGameStatus)		//si les bulles sont bien en jeu
	 {
		setPosition(spriteSheet, position, x, y);
		setHitbox(position, BUBBLE_RADIUS);
		setTransparency(spriteSheet, 255, 255, 255);
		updateScreenWithSpriteSheet(screen, spriteSheet, sprite[color], position);
	 }
	
}