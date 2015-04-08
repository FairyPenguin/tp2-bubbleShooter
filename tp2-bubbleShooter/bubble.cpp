#include "bubble.h"

/* Constructeur de la classe "Bubble"
====================================== */
Bubble::Bubble(int colorValue)
{

	spriteSheet = loadBitmap("bubble.bmp");
	color = colorValue;
	initCharset(sprite, NB_COLORS);

}

/* Destructeur de la classe "Bubble" 
====================================== */
Bubble::~Bubble()
{
	//Libérer la surface des bulles de la mémoire
	SDL_FreeSurface(spriteSheet);

}

/* Retourne la couleur d'une bulle donnée 
========================================== */
int Bubble::getColor() 
{
	return color;
}

/* Affiche la bulle à une position donnée dans l'écran
======================================================== */
void Bubble::update(SDL_Surface *screen, int x, int y) 
 {
	setPosition(spriteSheet, position, x, y);
	setTransparency(spriteSheet, 255, 255, 255);
	updateScreenWithSpriteSheet(screen, spriteSheet, sprite[color], position);
}