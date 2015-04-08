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
	extern const int NB_COLORS;				//nombre de couleurs maximales
	extern const int BUBBLE_RADIUS;			//rayon du cercle en pixels

	/* Prototypes des fonctions
	============================= */
	void initVideo();
	SDL_Surface* setupScreen(char* title, char* img);
	SDL_Surface* loadBitmap(char* imgName);
	void setPosition(SDL_Surface *picture, SDL_Rect &position, int posX, int posY);
	void setTransparency(SDL_Surface *picture, int red, int green, int blue);
	void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
	void updateScreenWithSpriteSheet(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position);
	void initCharset(SDL_Rect charset[], int taille);
	int setRandomValue(int maxValue);
}