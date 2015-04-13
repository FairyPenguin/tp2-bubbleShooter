#include "canon.h"

/* Constructeur du canon
========================= */
Canon::Canon() 
{
	originalSprite = loadBitmap("canon.bmp");
	displayedSprite = originalSprite;
	angle = 0;
}

/* Destructeur du canon
========================= */
Canon::~Canon()
{
	//Lib�rer l'image du canon de la m�moire
	SDL_FreeSurface(displayedSprite);
	SDL_FreeSurface(originalSprite);
}

/* Affiche le canon � l'�cran
============================================ */
void Canon::update(SDL_Surface *screen) 
{
	setPosition(displayedSprite, position, (SCREEN_WIDTH - displayedSprite->w) / 2, (SCREEN_HEIGHT - displayedSprite->h / 2));
	setTransparency(displayedSprite, 0, 0, 0);
	updateScreen(screen, displayedSprite, position);
}

/* Retourne la largeur du sprite du canon
============================================ */
int Canon::getWidth()
{
	return displayedSprite->w;
}

/* Retourne la hauteur du sprite du canon
============================================ */
int Canon::getHeight()
{
	return displayedSprite->h;
}

/* Tourne l'image du canon selon la direction
=============================================== */
void Canon::rotate(Orientation dir) 
{
	switch (dir)
	{
	case Canon::RIGHT:

		if (angle >= MIN_CANON_ANGLE)		//si l'angle du canon est plus grand que l'angle minimal, tourner le canon � droite
		{
			angle -= 3;
			displayedSprite = rotozoomSurface(originalSprite, angle, 1.0, 0);
		}
		break;

	case Canon::LEFT:						//si l'angle du canon est plus petit que l'angle maximal, tourner le canon � gauche

		if (angle <= MAX_CANON_ANGLE)
		{
			angle += 3;
			displayedSprite = rotozoomSurface(originalSprite, angle, 1.0, 0);
		}
		break;

	default:
		break;
	}
}

/* Retourne la position du canon
============================================ */
SDL_Rect Canon::getPosition()
{
	return position;
}

/* Retourne l'angle de rotation du canon
============================================ */
int Canon::getRotationAngle()
{
	return angle;
}