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

/* Met en place la hitbox de la bulle
================================================= */
void setHitbox(SDL_Rect &position, int radius) 
{
	//see how to set hitbox
}

/* Affiche la bulle � une position donn�e dans l'�cran
======================================================== */
void Bubble::update(SDL_Surface *screen, int x, int y) 
 {
	setPosition(spriteSheet, position, x, y);
	setTransparency(spriteSheet, 255, 255, 255);
	updateScreenWithSpriteSheet(screen, spriteSheet, sprite[color], position);
}