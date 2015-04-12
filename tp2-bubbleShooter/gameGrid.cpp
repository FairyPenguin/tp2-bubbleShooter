#include "gameGrid.h"

/* Constructeur de la classe "GameGrid"
======================================== */
GameGrid::GameGrid(int bubbleDiameter)
{
	offsetX = 10;
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			//créé une nouvelle bulle hypothétique dans la grille
			bubble[GRID_HEIGHT][GRID_WIDTH].setPosition(bubbleDiameter * j + offsetX, bubbleDiameter * i);// = new Bubble(bubbleDiameter * j + offsetX, bubbleDiameter * i);
	
		}

	}
}

/* Destructeur de la classe "GameGrid"
====================================== */
GameGrid::~GameGrid()
{
	//Libérer la surface des bulles de la mémoire
	/*SDL_FreeSurface(spriteSheet);*/
}

/* Insére une ligne de bulles dans la grille
============================================== */
void GameGrid::insertLine(SDL_Surface *screen)
{

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la première ligne (index 0)
	{
		int randColor = setRandomValue(NB_COLORS);
		bubble[0][i].setSprite(randColor);
		bubble[0][i].update(screen);

	}

}

void GameGrid::update(SDL_Surface *screen) 
{
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			bubble[i][j].update(screen);
		}
	}
}

void GameGrid::checkBubbleCollisions(Bubble* activeBubble)
{
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			bubble[i][j].checkCollisions(activeBubble);
		}

	}
}

void GameGrid::freeSurfaces() 
{
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			delete &bubble[i][j];
		}
	}
}

