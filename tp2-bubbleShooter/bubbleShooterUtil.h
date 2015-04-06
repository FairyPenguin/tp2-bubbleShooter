/* Directives au pré-processeur
================================= */
#include <iostream>
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
}