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
			//créé une nouvelle bulle hypothétique dans la grille
			bubble[i][j].setPosition(bubbleDiameter * j + offsetX, bubbleDiameter * i);
	
		}

	}
}

/* Insére une ligne de bulles dans la grille
============================================== */
void GameGrid::insertLine(SDL_Surface *screen)
{

	int startingPattern[GRID_WIDTH] = {0, 2, 1, 2, 0, 0, 1, 0};

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la première ligne (index 0)
	{

		bubble[0][i].setSprite(startingPattern[i]);
		bubble[0][i].update(screen);

	}

}

/* Décale le tableau d'une ligne avant d'en insérer une nouvelle
================================================================= */
void GameGrid::shiftGridLines(SDL_Surface *screen)
{

	//Décale les bulles de la première ligne à l'avant-dernière ligne.

	for (int i = (GRID_HEIGHT - 2); i > 0 ; i--)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			bool status = bubble[i][j].getInGameStatus();

			//Si la bulle est affichée, la descendre d'un étage
			if (status)
			{
				int newColor = bubble[i][j].getColor();
				bubble[i - 1][j].setSprite(newColor);

			}

			//Autrement, indiquer à l'étage plus bas que la bulle du dessus n'existe pas
			else
			{
				bubble[i - 1][j].setInGameStatus(false);
			}
		}
	}

	//Retirer toutes les images des bulles existantes dans l'avant-dernière ligne.
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		bubble[8][i].setInGameStatus(false);
	}

	//Insère une ligne dans la première ligne
	insertLine(screen);

}

/* Met à jour l'affichage des bulles dans la grille de jeu
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
		
		//insère la bulle au bon endroit dans la grille
		stickBubbleInGrid(activeBubble);

		return true;
	}

	return false;
}

/* Vérifie si la bulle active entre en collision avec une autre bulle ou bien le mur du haut
============================================================================================= */
bool GameGrid::checkActiveBubbleCollision(Bubble* activeBubble)
{
	bool hasCollided;		//flag indiquant la collision
	hasCollided = false;
	
	SDL_Rect activeBubblePosition = activeBubble->getPosition();

	//Si la bulle est en haut de l'écran, il y a collision
	if (activeBubblePosition.y <= 0)
	{
		hasCollided = true;
	}
	else
	{
		//Vérifier si la bulle en touche une autre dans la grille de jeu
		hasCollided = checkBubbleCollisions(activeBubble);
	}

	return hasCollided;
}

/* Vérifie s'il y a collision pour chaque bulle de la grille de jeu
==================================================================== */
bool GameGrid::checkBubbleCollisions(Bubble* activeBubble)
{
	
	bool hasCollided;		//flag déterminant s'il y a bien eu collision 

	//initialisation des variables
	hasCollided = false;
	
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			if (bubble[i][j].checkCollisions(activeBubble))
			{
				hasCollided = true;
				break;
			}

		}
		if (hasCollided)
		{
			break;
		}

	}
	return hasCollided;
}

/* Libère les surfaces contenues dans la grille de jeu
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

	double distance;						//distance entre les cases inoccupées de la grille et la bulle active
	double shortestDistance;		//stocke la valeur de la distance la plus courte
	int shortestDistanceLine;				//ligne où se trouve la distance la plus courte
	int shortestDistanceColumn;				//colonne où se trouve la distance la plus courte
	int newColor;							//nouvelle couleur de la bulle active

	SDL_Rect activeBubblePos;				//position courante de la bulle active
	SDL_Rect otherBubblePos;				//position courante de la bulle à comparer

	//initialisation des variables
	activeBubblePos = activeBubble->getPosition();
	shortestDistance = DBL_MAX;

	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			otherBubblePos = bubble[i][j].getPosition();
			bool status = bubble[i][j].getInGameStatus();

			//Si la case est libre, vérifier la distance entre la bulle active et la case vide
			if (!status)
			{
				distance = calculateDistance(activeBubblePos.x, otherBubblePos.x, activeBubblePos.y, otherBubblePos.y);

				//Si la distance entre les deuxx points est la plus courte, prendre l'index correspondant en note
				if (distance < shortestDistance)
				{
					shortestDistanceLine = i;
					shortestDistanceColumn = j;
					shortestDistance = distance;
				}
			}
		}
	}

	//Met la bulle active dnas la grille au bon endroit
	newColor = activeBubble->getColor();
	bubble[shortestDistanceLine][shortestDistanceColumn].setSprite(newColor);
	
}

/* Vérifie si la grille contient quelque chose dans la 10e ligne
================================================================= */
bool GameGrid::checkLastLine()
{
	bool status;

	for (int i = 0; i < GRID_WIDTH; i++)	//pour chaque case de la 10e ligne	
	{
		status = bubble[9][i].getInGameStatus();

		//s'il y a de quoi à la 10e ligne, renvoyer le flag correspondant
		if (status)
		{
			return true;
		}
	}

	return false;
}

