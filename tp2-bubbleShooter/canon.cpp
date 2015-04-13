#include "canon.h"

Canon::Canon() 
{
	originalSprite = loadBitmap("canon.bmp");
	displayedSprite = originalSprite;
	angle = 0;
}

Canon::~Canon()
{
	//Libérer l'image du canon de la mémoire
	SDL_FreeSurface(displayedSprite);
	SDL_FreeSurface(originalSprite);
}

void Canon::update(SDL_Surface *screen) 
{
	setPosition(displayedSprite, position, (SCREEN_WIDTH - displayedSprite->w) / 2, (SCREEN_HEIGHT - displayedSprite->h / 2));
	setTransparency(displayedSprite, 0, 0, 0);
	updateScreen(screen, displayedSprite, position);
}

int Canon::getWidth()
{
	return displayedSprite->w;
}

int Canon::getHeight()
{
	return displayedSprite->h;
}

void Canon::rotate(Orientation dir) 
{
	switch (dir)
	{
	case Canon::RIGHT:

		if (angle >= MIN_CANON_ANGLE)
		{
			angle -= 3;
			displayedSprite = rotozoomSurface(originalSprite, angle, 1.0, 0);
		}
		break;

	case Canon::LEFT:

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