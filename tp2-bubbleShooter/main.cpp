/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 09/04/2015
Description :  */

/* TODO: am�liorations � apporter au programme
================================================= */

/* ------ Am�lie ---------- */
//Conflit WaitEvent/PollEvent � r�gler. Conflit entre actions du canon et actions de la bulle. Je ne peux ni mettre la canon en PollEvent ou la bulle en WaitEvent... =/
//Mon random y'est tout croche. Il redonne la m�me couleur presque tout le temps.
//Calculer la v�locit� correctement. On peut toutefois tester le reste avec des valeurs hardcod�es en attendant
//R�gler changement v�locit� en X quand on touche les coins. Il stick sur le bord.
//Collision des bulles � d�boguer. �a overlap un peu... 
//Check Bulles Adjacentes quand le moteur des collisions sera termin�. Ressortir le D�mineur des boules � mites. LOL.
//Continuer � cleaner le main parce que le main est vraiment trop long.

/* ------ Marc-Antoine ---------- */
//Int�grer scoring
//Int�grer SDL_ttf de fa�on portative


/* Directives au pr�-processeur
================================= */
#include <math.h>
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"
#include "bubbleShooterUtil.h"
#include "canon.h"
#include "bubble.h"

using namespace BubbleShooterUtil;

/* Constantes du projet 
======================== */
const int GRID_HEIGHT = 10;
const int GRID_WIDTH = 8;
const int REFRESH_TIME = 600;

/* Structures du projet
======================== */
struct GameGrid
{
	SDL_Rect position;				//position dans la grille
	Bubble bubble;					//bulle contenue dans la grille
};

/* Prototype des fonctions
=========================== */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive);
void initGameGrid(GameGrid grid[GRID_HEIGHT][GRID_WIDTH]);
void insertLine(SDL_Surface *screen, GameGrid grid[GRID_HEIGHT][GRID_WIDTH]);
bool checkBubbleCollisions(Bubble *thisBubble, GameGrid grid[GRID_HEIGHT][GRID_WIDTH]);


/* M�thode principale
======================= */
int main(int argc, char *argv[])
{

	SDL_Surface *screen = NULL;			//fen�tre du jeu
	SDL_Surface *background = NULL;		//image de fond du jeu
	SDL_Surface *menu_rect = NULL;		//rectangle du menu
	SDL_Surface *game_logo = NULL;		//logo (nom) du jeu)

	SDL_Event e;						//�v�nement capt� par SDL_PollEvent

	SDL_Rect bgPosition;				//position du fond d'�cran dans l'espace
	SDL_Rect menuPosition;				//position du menu dans l'espace
	SDL_Rect logoPosition;				//position du logo dans l'espace

	bool isActive = true;				//flag d�terminant si la fen�tre est active


	//Initialisation de la SDL
	initVideo();

	//Param�tres de l'�cran
	screen = setupScreen("Bubble Shooter", "greenBubble.bmp");
	
	//Param�tres du fond d'�cran
	background = loadBitmap("menu_bg.bmp");
	setPosition(background, bgPosition, 0, 0);
	
	//Param�tres du rectangle du menu
	menu_rect = loadBitmap("rectangle_menu.bmp");
	setPosition(menu_rect, menuPosition, 39, 256);

	//Param�tres du logo
	game_logo = loadBitmap("game_logo2.bmp");
	setPosition(game_logo, logoPosition, 50, 56);

	//Mettre � jour l'�cran une premi�re fois
	updateScreen(screen, background, bgPosition);
	updateScreen(screen, game_logo, logoPosition);
	updateScreen(screen, menu_rect, menuPosition);

	while (isActive)	//tant que l'application est active
	{
		SDL_Flip(screen);

		SDL_WaitEvent(&e);
		switch (e.type)
		{

		case SDL_QUIT:		//si le joueur clique sur le X de la fen�tre, on ferme l'application
			isActive = 0;
			break;

		case SDL_KEYDOWN:	//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_q:	//si on appuie sur Q, fermer le programme
				isActive = 0;
				break;


			case SDLK_r:	//si on appuie sur R, montrer les r�gles du jeu
				showRules(screen, menu_rect, menuPosition, isActive);
				break;


			case SDLK_SPACE:	//si on appuie sur Espace, appeler la fonction playGame();
				playGame(screen, background, bgPosition, isActive);
				break;

			}

		}

	}

	//Lib�re les surfaces de la m�moire	
	SDL_FreeSurface(menu_rect);
	SDL_FreeSurface(game_logo);
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);

	//Arr�t de la SDL (vider la m�moire)
	SDL_Quit();
	return 0;

}


/* Montre les r�gles du jeu
============================= */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive)
{
	SDL_Surface *rules = NULL;			//r�glements du jeu
	SDL_Event e;						//�v�nement capt� par SDL_WaitEvent
	SDL_Rect rulesPos;					//position des r�gles dans l'espace

	bool rulesIsActive = true;			//fen�tre des r�gles du jeu est active

	//Param�tres du rectangle du menu
	rules = loadBitmap("rectangle_rules.bmp");
	setPosition(rules, rulesPos, 39, 256);

	//Mettre � jour l'�cran une premi�re fois
	updateScreen(screen, rules, rulesPos);

	while (rulesIsActive)	//tant que les r�glements sont actifs
	{

		SDL_Flip(screen);

		SDL_WaitEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:		//si le joueur clique sur le X de la fen�tre, on ferme l'application EN ENTIER
			rulesIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:	//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_q:	//si on appuie sur Q, fermer les r�gles et montrer le menu � nouveau

				updateScreen(screen, menu, menuPos);	//Redessiner le menu sur l'�cran	
				rulesIsActive = 0;
				break;
			}

		}

	}

	SDL_FreeSurface(rules);		//Lib�rer les r�gles de la m�moire

}

/* D�marre le jeu en tant que tel
===================================== */
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive)
{

	SDL_Event e;										//�v�nement capt� par SDL_WaitEvent
	SDL_Event moveE;									//�v�nement capt� par SDL_PollEvent

	SDL_Rect activeBubblePos;							//position de la bulle active lorsqu'elle est en mouvement
	SDL_Rect nextBubblePos;								//position de la prochaine bulle

	int currentTime;									//temps actuel
	int previousTime;									//temps pr�c�dent
	int bubbleCounter;									//compte le nombre de bulles en jeu

	bool gameIsActive;									//le jeu est actif
	bool hasCollided;									//gestion des collisions

	GameGrid bubbleGrid[GRID_HEIGHT][GRID_WIDTH];		//grille de jeu contenant les bulles

	Canon* canon = new Canon();							//le canon tirant les bulles 
	Bubble* activeBubble = new Bubble();				//la bulle active
	Bubble* nextBubble = new Bubble();					//la prochaine bulle en jeu

	//Initialisation des variables
	bubbleCounter = 0;
	previousTime = 0;
	currentTime = 0;
	gameIsActive = true;

	//Redessiner l'�cran avec le background pour faire dispara�tre le menu
	updateScreen(screen, bg, bgPos);

	//G�n�rer l'aire de jeu et ins�rer les bulles qui seront en jeu au d�part
	initGameGrid(bubbleGrid);
	insertLine(screen, bubbleGrid);

	//Dessiner une premi�re fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);

	activeBubble->setSprite(0);
	activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);

	nextBubble->setSprite(1);
	nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);

	SDL_EnableKeyRepeat(50, 50);

	while (gameIsActive)	//tant que le jeu est actif
	{
		SDL_Flip(screen);

		//SDL_WaitEvent(&e);		//�v�nements de type Wait
		//	switch (e.type)
		//	{
		//	case SDL_QUIT:			//si le joueur clique sur le X de la fen�tre, on ferme l'application EN ENTIER
		//		gameIsActive = 0;
		//		screenIsActive = 0;
		//		break;

		//	case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

		//		switch (e.key.keysym.sym)
		//		{

		//		case SDLK_LEFT:		//tourne le canon � gauche avec la fl�che de gauche
		//			canon->rotate(Canon::LEFT);
		//			break;

		//		case SDLK_RIGHT:   //tourne le canon � droite avec la fl�che de droite
		//			canon->rotate(Canon::RIGHT);
		//			break;
		//		}

		//		break;
		//	}


		SDL_PollEvent(&moveE);		//�v�nements de type Poll
		switch (moveE.type)
		{
		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (moveE.key.keysym.sym)
			{
			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace

				hasCollided = false;	//initialise le fait qu'il n'y a pas eu de collision

				if (!hasCollided)		//si la bulle n'a pas eu de collisions
				{

					currentTime = SDL_GetTicks();		//initialise le timer en prenant les valeurs r�elles

					if (currentTime - previousTime > REFRESH_TIME)		//� chaque rafraichissement de l'image
					{

						//V�rifier les collisions pour chaque bulle pr�sentement en jeu
						hasCollided = checkBubbleCollisions(activeBubble, bubbleGrid);
						if (!hasCollided)
						{
							activeBubble->move(hasCollided);
							currentTime = previousTime;
						}
						else
						{
							//on endort le programme pour ne pas qu'il prenne trop de CPU
							SDL_Delay(REFRESH_TIME - (previousTime - currentTime));
						}
					}
						
					//si la bulle ne bouge plus car elle est entr�e en contact avec quelque chose, 
					//changer la bulle active pour la prochaine bulle, et g�n�rer une nouvelle prochaine bulle
					if (hasCollided)
					{

						activeBubble = nextBubble;
						int randColor = setRandomValue(NB_COLORS);
						nextBubble = new Bubble();
						nextBubble->setSprite(randColor);
					}

				}
				/*checkSurroundingBubbles(activeBubble, bubbleArray[])*/
				break;
			}
			break;
		}

		//Mettre � jour l'�cran apr�s chaque it�ration de la boucle
		updateScreen(screen, bg, bgPos);
		
		//Mettre � jour les bulles pr�sentement en jeu

		for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
		{

			for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
			{

				//si la bulle est pr�sentement en jeu, lib�rer la surface de la bulle de la m�moire
				bool status = bubbleGrid[i][j].bubble.getInGameStatus();
				if (status)
				{
					//******************************** ERREUR: Access Violation ***************************************
					bubbleGrid[i][j].bubble.update(screen, bubbleGrid[i][j].position.x, bubbleGrid[i][j].position.y);
				}

			}

		}

		//Mettre � jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);

		activeBubblePos = activeBubble->getPosition();
		activeBubble->update(screen, activeBubblePos.x, activeBubblePos.y);
		
		nextBubblePos = nextBubble->getPosition();
		nextBubble->update(screen, nextBubblePos.x, nextBubblePos.y);
		
	}


	//Lib�rer les surfaces de la m�moire

	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			//si la bulle est pr�sentement en jeu, lib�rer la surface de la bulle de la m�moire
			bool status = bubbleGrid[i][j].bubble.getInGameStatus();
			if (status)
			{
				SDL_Surface* sprite = bubbleGrid[i][j].bubble.getSpriteSheet();
				SDL_FreeSurface(sprite);
			}

		}

	}

	delete canon;
	delete activeBubble;
	delete nextBubble;

}


/* Initialise les positions de la grille
========================================== */
void initGameGrid(GameGrid grid[GRID_HEIGHT][GRID_WIDTH]) 
{
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			grid[i][j].position.x = 47 * j + 10;		//il y a 10 pixels de d�calage en X par rapport � la position de d�part (0,0)
			grid[i][j].position.y = 47 * i;
			grid[i][j].position.h = 47;
			grid[i][j].position.w = 47;

		}

	}
}

/* Ins�re une ligne de bulles dans la grille
============================================== */
void insertLine(SDL_Surface *screen, GameGrid grid[GRID_HEIGHT][GRID_WIDTH])
{

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la premi�re ligne (index 0)
	{
		int randColor = setRandomValue(NB_COLORS);
		grid[0][i].bubble.setSprite(randColor);
		grid[0][i].bubble.update(screen, grid[0][i].position.x, grid[0][i].position.y);

	}

}															

/* V�rifie la collision de bulles dans l'aire de jeu
======================================================= */
bool checkBubbleCollisions(Bubble *thisBubble, GameGrid grid[GRID_HEIGHT][GRID_WIDTH]) 
{
	
	bool collision = false;		//flag indiquant s'il y a eu collision
	bool status;				//indique si la bulle v�rifi�e dans la grille est bien en-jeu et affich�e � l'�cran
	
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			//si la bulle est pr�sentement en jeu, 
			//v�rifier si la bulle courante entre en collision avec elle
			//si et seulement si on n'a pas d�j� trouv� de collision au pr�alable

			bool status = grid[i][j].bubble.getInGameStatus();
			if (status && !collision)
			{
				collision = thisBubble->checkCollisions(grid[i][j].bubble);
			}

		}

	}

	return collision;

}