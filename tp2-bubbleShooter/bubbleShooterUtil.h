/* Directives au pré-processeur
================================= */
#include <iostream>
#include <ctime>
#include "../SDL/SDL.h"

namespace BubbleShooterUtil 
{

	///* Structures du projet
	//======================== */
	//struct GameGrid
	//{
	//	SDL_Rect position;				//position dans la grille
	//	Bubble bubble;					//bulle contenue dans la grille
	//};

	/* Constantes du programme
	=========================== */
	extern const int SCREEN_HEIGHT;
	extern const int SCREEN_WIDTH;
	extern const int NB_COLORS;				//nombre de couleurs maximales
	//extern const int GRID_HEIGHT;
	//extern const int GRID_WIDTH;

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
	double calculateDistance(int x1, int x2, int y1, int y2);

}