/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: bubbleShooterUtil
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 12/04/2015
Description :  Fonctions utilitaires du jeu Bubble Shooter */

/* Directives au pr�-processeur
================================= */
#include <iostream>
#include <ctime>
#include "../SDL/SDL.h"

namespace BubbleShooterUtil 
{

	/* Constantes du programme
	=========================== */
	extern const int SCREEN_HEIGHT;
	extern const int SCREEN_WIDTH;

	/* Prototypes des fonctions
	============================= */
	void initVideo();
	SDL_Surface* setupScreen(char* title, char* img);
	SDL_Surface* loadBitmap(char* imgName);
	void setPosition(SDL_Surface *picture, SDL_Rect &position, int posX, int posY);
	void setTransparency(SDL_Surface *picture, int red, int green, int blue);
	void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
	void updateScreenWithSpriteSheet(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position);
	int getRandomValue(int maxValue);
	double calculateDistance(int x1, int x2, int y1, int y2);
	void setOutOfBoundsValues(int &maxLineNbr, int &minLineMbr, int &maxColumnNbr, int &minColumnNbr);

}