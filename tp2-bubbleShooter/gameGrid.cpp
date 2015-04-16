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

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la première ligne (index 0)
	{

		int newColor = getRandomValue(3);
		bubble[0][i].setSprite(newColor);
		bubble[0][i].update(screen);

	}

}

/* Décale le tableau d'une ligne avant d'en insérer une nouvelle
================================================================= */
void GameGrid::shiftGridLines(SDL_Surface *screen)
{

	//Décale les bulles de la première ligne à l'avant-dernière ligne.

	for (int i = (GRID_HEIGHT - 1); i > 0; i--)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			bool status = bubble[i - 1][j].getInGameStatus();

			//Si la bulle est affichée, la descendre d'un étage
			if (status)
			{
				int newColor = bubble[i-1][j].getColor();
				bubble[i][j].setSprite(newColor);

			}

			//Autrement, indiquer à l'étage plus bas que la bulle du dessus n'existe pas
			else
			{
				bubble[i - 1][j].setInGameStatus(false);
			}
		}
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
			if (bubble[i][j].getInGameStatus())
			{
				bubble[i][j].update(screen);
			}
			
		}
	}
}

/* S'occupe du moteur de collisions des bulles
================================================ */
bool GameGrid::manageCollision(Bubble* &activeBubble)
{
	int nbOfAdjBubbles = 0;		//nombre de bulles adjacentes trouvées par la grille

	if (checkActiveBubbleCollision(activeBubble))	//s'il y a effectivement collision
	{	
		//Remet la grille à zéro pour pouvoir faire la vérification des bulles adjacentes
		resetGridCheck();

		//Stoppe le mouvement de la bulle
		activeBubble->setVelocity(0, 0);
		
		//Insère la bulle au bon endroit dans la grille et vérifie les bulles adjacentes
		stickBubbleInGrid(activeBubble);

		////Retourne le nombre de bulles adjacentes
		//nbOfAdjBubbles = countAdjacentBubbles();

		////"Pop" les bulles adjacentes
		//if (nbOfAdjBubbles > REQUIRED_BUBBLES_TO_POP)	//si required_bubbles >= 1, ça plante...
		//{
		//	popBubbles();
		//}

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

	vector<PositionStructure> toPop;		//vecteur comprenant les positions dans la grille des bulles	
	
	double distance;						//distance entre les cases inoccupées de la grille et la bulle active
	double shortestDistance;				//stocke la valeur de la distance la plus courte
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

				//Si la distance entre les deux points est la plus courte, prendre l'index correspondant en note
				if (distance < shortestDistance)
				{
					shortestDistanceLine = i;
					shortestDistanceColumn = j;
					shortestDistance = distance;
				}
			}
		}
	}

	//Met la bulle active dans la grille au bon endroit
	newColor = activeBubble->getColor();
	bubble[shortestDistanceLine][shortestDistanceColumn].setSprite(newColor);

	//Met la bulle active placée dans la grille comme étant "vérifiée" afin de faire le check des bulles adjacentes
	int myColor = bubble[shortestDistanceLine][shortestDistanceColumn].getColor();

	//Vérifie s'il y a bien des bulles adjacentes
	addedValue(shortestDistanceLine, shortestDistanceColumn, myColor);
	
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

void GameGrid::resetGridCheck()
{
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{
		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{
			bubble[i][j].setIsAdjacent(false);
			bubble[i][j].setIsChecked(false);
		}
	}
}

void GameGrid::countAdjacent(vector<PositionStructure> &toPop, int x, int y, int myColor)
{
	PositionStructure position;

	for (int i = -1; i <= 1; i++)
	{
		position.x = x + i;
		for (int j = -1; j <= 1; j++)
		{
			position.y = y + j;

			if (position.x < 0 || position.x >= GRID_HEIGHT)
			{
				continue;
			}

			if (position.y < 0 || position.y >= GRID_WIDTH)
			{
				continue;
			}

			if (bubble[position.x][position.y].getColor() != myColor)
			{
				continue;
			}

			if (!contains(toPop, position))
			{
				toPop.push_back(position);
				countAdjacent(toPop, position.x, position.y, myColor);
			}
		}
	}
}

bool GameGrid::contains(vector<PositionStructure> toPop, PositionStructure position)
{
	bool isContained = false;
	for (PositionStructure currentPosition : toPop)
	{
		if (currentPosition.x == position.x && currentPosition.y == position.y)
		{
			isContained = true;
			break;
		}
	}

	return isContained;
}

void GameGrid::addedValue(int x, int y, int myColor)
{
	vector<PositionStructure> toPop;
	countAdjacent(toPop, x, y, myColor);
		
	if (toPop.size() >= REQUIRED_BUBBLES_TO_POP)
	{
		for (PositionStructure position : toPop)
		{
			bubble[position.x][position.y].setInGameStatus(false);
		}
	}
}

///* Vérifie le nombre de bulles adjacentes de même couleur 
//========================================================== */
//void GameGrid::checkAdjacentBubbles(Bubble bubbleToCheck)
//{
//	SDL_Rect checkedPosition;
//	
//	int nbAdjBubbles = 0;		//nombre de bulles de même couleur adjacentes à la bulle vérifiée
//	int lineOfCheckedBubble;
//	int columnOfCheckedBubble;
//	int colorToCompare;
//
//	int lineCheckMaxValue = 0;
//	int lineCheckMinValue = 0;
//	int columnCheckMaxValue = 0;
//	int columnCheckMinValue = 0;
//
//	//Vérifier quelle couleur il faut comparer
//	colorToCompare = bubbleToCheck.getColor();
//
//	//Demander à la bulle à vérifier sa position dans l'espace
//	checkedPosition = bubbleToCheck.getPosition();
//
//	//Identifier la position dans la grille de la bulle à partir de sa position dans l'espace
//	for (int i = 0; i < GRID_HEIGHT; i++)		
//	{
//		for (int j = 0; j < GRID_WIDTH; j++)	
//		{
//			//Si les positions en X et en Y correspondent, prendre l'index de la bulle à vérifier en note
//			if (bubble[i][j].getPosition().x == checkedPosition.x && bubble[i][j].getPosition().y == checkedPosition.y)
//			{
//				lineOfCheckedBubble = i;
//				columnOfCheckedBubble = j;
//				break;
//			}
//
//		}
//	}
//
//	//Assigne les valeurs maximales et minimales de vérification
//	lineCheckMinValue = lineOfCheckedBubble - 1;
//	lineCheckMaxValue = lineOfCheckedBubble + 1;
//
//	columnCheckMinValue = columnOfCheckedBubble - 1;
//	columnCheckMaxValue = columnOfCheckedBubble + 1;
//
//	//Ajuste les limites du check pour s'assurer que l'on ne tombe pas "out of bounds"
//	if (lineOfCheckedBubble <= 0)
//	{
//		lineCheckMinValue = 1;
//	}
//	else if (lineOfCheckedBubble >= GRID_HEIGHT - 1)
//	{
//		lineCheckMaxValue = GRID_HEIGHT - 2;
//	}
//
//	if (columnOfCheckedBubble <= 0)
//	{
//		columnCheckMinValue = 1;
//	}
//	else if (columnOfCheckedBubble >= GRID_WIDTH - 1)
//	{
//		columnCheckMaxValue = GRID_WIDTH - 2;
//	}
//
//	//Vérifie la couleur des bulles se trouvant autour de la bulle vérifiée
//	for (int i = lineCheckMinValue; i <= lineCheckMaxValue; i++)
//	{
//		for (int j = columnCheckMinValue; j <= columnCheckMaxValue; j++)
//		{
//			//Ne vérifier la bulle que si elle est bien en jeu
//			if (bubble[i][j].getInGameStatus())
//			{
//				//Si la bulle n'a pas été vérifiée auparavant et que la bulle adjacente est de la même couleur
//				//appeler de façon récursive la fonction pour vérifier les paramètres de la bulle adjacente à celle-ci
//				if (bubble[i][j].getIsChecked() == false && bubble[i][j].getColor() == colorToCompare)
//				{
//					bubble[i][j].setIsAdjacent(true);
//					bubble[i][j].setIsChecked(true);
//					/******* SECTION À PROBLÈME!************/
//					/*****Passer le débogueur dessus de façon détaillée*****/
//					checkAdjacentBubbles(bubble[i][j]);
//				}
//			}
//
//			bubble[i][j].setIsChecked(true);
//
//		}
//	}
//
//}
//
///* Remet la grille à zéro pour faire la vérification des bulles adjacentes 
//=========================================================================== */
//void GameGrid::resetGridCheck()
//{
//	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
//	{
//		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
//		{
//			bubble[i][j].setIsAdjacent(false);
//			bubble[i][j].setIsChecked(false);
//		}
//	}
//}
//
///* Compte et retourne le nombre de bulles adjacentes 
//====================================================== */
//int GameGrid::countAdjacentBubbles() 
//{
//	int nbAdjBubbles = 0;	//nombre de bulles adjacentes
//
//	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
//	{
//		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
//		{
//			if (bubble[i][j].getIsAdjacent())
//			{
//				nbAdjBubbles++;
//			}
//		}
//	}
//
//	return nbAdjBubbles;
//}
//
///* Retire les bulles devant exploser dans le jeu
//================================================== */
//void GameGrid::popBubbles()
//{
//	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
//	{
//		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
//		{
//
//			//Toutes les bulles ayant été déterminées adjacentes n'existent plus en jeu
//			if (bubble[i][j].getIsAdjacent())
//			{
//				bubble[i][j].setInGameStatus(false);
//			}
//		}
//	}
//}
