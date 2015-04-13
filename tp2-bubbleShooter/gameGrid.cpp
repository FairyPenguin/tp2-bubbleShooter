#include "gameGrid.h"

/* Constructeur de la classe "GameGrid"
======================================== */
GameGrid::GameGrid(int bubbleDiameter)
{
	offsetX = 35;
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			//cr�� une nouvelle bulle hypoth�tique dans la grille
			bubble[i][j].setPosition(bubbleDiameter * j + offsetX, bubbleDiameter * i);
	
		}

	}
}

/* Ins�re une ligne de bulles dans la grille
============================================== */
void GameGrid::insertLine(SDL_Surface *screen)
{

	int startingPattern[GRID_WIDTH] = {0, 2, 1, 2, 0, 0, 1, 0};

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la premi�re ligne (index 0)
	{

		bubble[0][i].setSprite(startingPattern[i]);
		bubble[0][i].update(screen);

	}

}

/* Met � jour l'affichage des bulles dans la grille de jeu
============================================================ */
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

/* S'occupe du moteur de collisions des bulles
================================================ */
bool GameGrid::manageCollision(Bubble* &activeBubble)
{

	if (checkActiveBubbleCollision(activeBubble))	//s'il y a effectivement collision
	{
		//Stoppe le mouvement de la bulle
		activeBubble->setVelocity(0, 0);
		
		//ins�re la bulle au bon endroit dans la grille
		/*stickBubbleInGrid(activeBubble);*/

		return true;
	}

	return false;
}

/* V�rifie si la bulle active entre en collision avec une autre bulle ou bien le mur du haut
============================================================================================= */
bool GameGrid::checkActiveBubbleCollision(Bubble* activeBubble)
{
	bool hasCollided;		//flag indiquant la collision
	hasCollided = false;
	
	SDL_Rect activeBubblePosition = activeBubble->getPosition();

	//Si la bulle est en haut de l'�cran, il y a collision
	if (activeBubblePosition.y <= 0)
	{
		hasCollided = true;
	}

	//V�rifier si la bulle en touche une autre dans la grille de jeu
	hasCollided = checkBubbleCollisions(activeBubble);

	return hasCollided;
}

/* V�rifie s'il y a collision pour chaque bulle de la grille de jeu
==================================================================== */
bool GameGrid::checkBubbleCollisions(Bubble* activeBubble)
{
	
	bool hasCollided;		//flag d�terminant s'il y a bien eu collision 

	//initialisation des variables
	hasCollided = false;
	
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			//v�rifier seulement s'il n'y a pas eu collision encore
			if (!hasCollided)
			{
				if (bubble[i][j].checkCollisions(activeBubble))
				{
					hasCollided = true;
				}
			}
			
		}
	}

	return hasCollided;
}

/* Lib�re les surfaces contenues dans la grille de jeu
======================================================== */
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

/* Place la bulle dans la grille de jeu, selon la position dans laquelle elle se trouve
========================================================================================= */
void GameGrid::stickBubbleInGrid(Bubble* &activeBubble) 
{

	double distance;						//distance entre les cases inoccup�es de la grille et la bulle active
	double shortestDistance = DBL_MAX;		//stocke la valeur de la distance la plus courte
	int shortestDistanceLine;				//ligne o� se trouve la distance la plus courte
	int shortestDistanceColumn;				//colonne o� se trouve la distance la plus courte
	int newColor;							//nouvelle couleur de la bulle active

	SDL_Rect activeBubblePos;				//position courante de la bulle active
	SDL_Rect otherBubblePos;				//position courante de la bulle � comparer

	activeBubblePos = activeBubble->getPosition();

	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			otherBubblePos = bubble[i][j].getPosition();
			bool status = bubble[i][j].getInGameStatus();

			//Si la case est libre, v�rifier la distance entre la bulle active et la case vide
			if (!status)
			{
				distance = calculateDistance(activeBubblePos.x, otherBubblePos.x, activeBubblePos.y, otherBubblePos.y);

				//Si la distance entre les deuxx points est la plus courte, prendre l'index correspondant en note
				if (distance < shortestDistance)
				{
					shortestDistanceLine = i;
					shortestDistanceColumn = j;
				}
			}
		}
	}

	//Met la bulle active dnas la grille au bon endroit
	newColor = activeBubble->getColor();
	bubble[shortestDistanceLine][shortestDistanceColumn].setSprite(newColor);

	
}

