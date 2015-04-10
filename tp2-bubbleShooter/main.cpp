/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 09/04/2015
Description :  */

/* TODO: améliorations à apporter au programme
================================================= */

/* ------ Amélie ---------- */
//Conflit WaitEvent/PollEvent à règler. Conflit entre actions du canon et actions de la bulle. Je ne peux ni mettre la canon en PollEvent ou la bulle en WaitEvent... =/
//Mon random y'est tout croche. Il redonne la même couleur presque tout le temps.
//Calculer la vélocité correctement. On peut toutefois tester le reste avec des valeurs hardcodées en attendant
//Règler changement vélocité en X quand on touche les coins. Il stick sur le bord.
//Collision des bulles à déboguer. Ça overlap un peu... 
//Check Bulles Adjacentes quand le moteur des collisions sera terminé. Ressortir le Démineur des boules à mites. LOL.
//Continuer à cleaner le main parce que le main est vraiment trop long.

/* ------ Marc-Antoine ---------- */
//Intégrer scoring
//Intégrer SDL_ttf de façon portative


/* Directives au pré-processeur
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


/* Méthode principale
======================= */
int main(int argc, char *argv[])
{

	SDL_Surface *screen = NULL;			//fenêtre du jeu
	SDL_Surface *background = NULL;		//image de fond du jeu
	SDL_Surface *menu_rect = NULL;		//rectangle du menu
	SDL_Surface *game_logo = NULL;		//logo (nom) du jeu)

	SDL_Event e;						//événement capté par SDL_PollEvent

	SDL_Rect bgPosition;				//position du fond d'écran dans l'espace
	SDL_Rect menuPosition;				//position du menu dans l'espace
	SDL_Rect logoPosition;				//position du logo dans l'espace

	bool isActive = true;				//flag déterminant si la fenêtre est active


	//Initialisation de la SDL
	initVideo();

	//Paramètres de l'écran
	screen = setupScreen("Bubble Shooter", "greenBubble.bmp");
	
	//Paramètres du fond d'écran
	background = loadBitmap("menu_bg.bmp");
	setPosition(background, bgPosition, 0, 0);
	
	//Paramètres du rectangle du menu
	menu_rect = loadBitmap("rectangle_menu.bmp");
	setPosition(menu_rect, menuPosition, 39, 256);

	//Paramètres du logo
	game_logo = loadBitmap("game_logo2.bmp");
	setPosition(game_logo, logoPosition, 50, 56);

	//Mettre à jour l'écran une première fois
	updateScreen(screen, background, bgPosition);
	updateScreen(screen, game_logo, logoPosition);
	updateScreen(screen, menu_rect, menuPosition);

	while (isActive)	//tant que l'application est active
	{
		SDL_Flip(screen);

		SDL_WaitEvent(&e);
		switch (e.type)
		{

		case SDL_QUIT:		//si le joueur clique sur le X de la fenêtre, on ferme l'application
			isActive = 0;
			break;

		case SDL_KEYDOWN:	//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_q:	//si on appuie sur Q, fermer le programme
				isActive = 0;
				break;


			case SDLK_r:	//si on appuie sur R, montrer les règles du jeu
				showRules(screen, menu_rect, menuPosition, isActive);
				break;


			case SDLK_SPACE:	//si on appuie sur Espace, appeler la fonction playGame();
				playGame(screen, background, bgPosition, isActive);
				break;

			}

		}

	}

	//Libère les surfaces de la mémoire	
	SDL_FreeSurface(menu_rect);
	SDL_FreeSurface(game_logo);
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);

	//Arrêt de la SDL (vider la mémoire)
	SDL_Quit();
	return 0;

}


/* Montre les règles du jeu
============================= */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive)
{
	SDL_Surface *rules = NULL;			//règlements du jeu
	SDL_Event e;						//événement capté par SDL_WaitEvent
	SDL_Rect rulesPos;					//position des règles dans l'espace

	bool rulesIsActive = true;			//fenêtre des règles du jeu est active

	//Paramètres du rectangle du menu
	rules = loadBitmap("rectangle_rules.bmp");
	setPosition(rules, rulesPos, 39, 256);

	//Mettre à jour l'écran une première fois
	updateScreen(screen, rules, rulesPos);

	while (rulesIsActive)	//tant que les règlements sont actifs
	{

		SDL_Flip(screen);

		SDL_WaitEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:		//si le joueur clique sur le X de la fenêtre, on ferme l'application EN ENTIER
			rulesIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:	//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_q:	//si on appuie sur Q, fermer les règles et montrer le menu à nouveau

				updateScreen(screen, menu, menuPos);	//Redessiner le menu sur l'écran	
				rulesIsActive = 0;
				break;
			}

		}

	}

	SDL_FreeSurface(rules);		//Libérer les règles de la mémoire

}

/* Démarre le jeu en tant que tel
===================================== */
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive)
{

	SDL_Event e;										//événement capté par SDL_WaitEvent
	SDL_Event moveE;									//événement capté par SDL_PollEvent

	SDL_Rect activeBubblePos;							//position de la bulle active lorsqu'elle est en mouvement
	SDL_Rect nextBubblePos;								//position de la prochaine bulle

	int currentTime;									//temps actuel
	int previousTime;									//temps précédent
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

	//Redessiner l'écran avec le background pour faire disparaître le menu
	updateScreen(screen, bg, bgPos);

	//Générer l'aire de jeu et insérer les bulles qui seront en jeu au départ
	initGameGrid(bubbleGrid);
	insertLine(screen, bubbleGrid);

	//Dessiner une première fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);

	activeBubble->setSprite(0);
	activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);

	nextBubble->setSprite(1);
	nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);

	SDL_EnableKeyRepeat(50, 50);

	while (gameIsActive)	//tant que le jeu est actif
	{
		SDL_Flip(screen);

		//SDL_WaitEvent(&e);		//événements de type Wait
		//	switch (e.type)
		//	{
		//	case SDL_QUIT:			//si le joueur clique sur le X de la fenêtre, on ferme l'application EN ENTIER
		//		gameIsActive = 0;
		//		screenIsActive = 0;
		//		break;

		//	case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

		//		switch (e.key.keysym.sym)
		//		{

		//		case SDLK_LEFT:		//tourne le canon à gauche avec la flèche de gauche
		//			canon->rotate(Canon::LEFT);
		//			break;

		//		case SDLK_RIGHT:   //tourne le canon à droite avec la flèche de droite
		//			canon->rotate(Canon::RIGHT);
		//			break;
		//		}

		//		break;
		//	}


		SDL_PollEvent(&moveE);		//événements de type Poll
		switch (moveE.type)
		{
		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (moveE.key.keysym.sym)
			{
			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace

				hasCollided = false;	//initialise le fait qu'il n'y a pas eu de collision

				if (!hasCollided)		//si la bulle n'a pas eu de collisions
				{

					currentTime = SDL_GetTicks();		//initialise le timer en prenant les valeurs réelles

					if (currentTime - previousTime > REFRESH_TIME)		//à chaque rafraichissement de l'image
					{

						//Vérifier les collisions pour chaque bulle présentement en jeu
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
						
					//si la bulle ne bouge plus car elle est entrée en contact avec quelque chose, 
					//changer la bulle active pour la prochaine bulle, et générer une nouvelle prochaine bulle
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

		//Mettre à jour l'écran après chaque itération de la boucle
		updateScreen(screen, bg, bgPos);
		
		//Mettre à jour les bulles présentement en jeu

		for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
		{

			for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
			{

				//si la bulle est présentement en jeu, libérer la surface de la bulle de la mémoire
				bool status = bubbleGrid[i][j].bubble.getInGameStatus();
				if (status)
				{
					//******************************** ERREUR: Access Violation ***************************************
					bubbleGrid[i][j].bubble.update(screen, bubbleGrid[i][j].position.x, bubbleGrid[i][j].position.y);
				}

			}

		}

		//Mettre à jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);

		activeBubblePos = activeBubble->getPosition();
		activeBubble->update(screen, activeBubblePos.x, activeBubblePos.y);
		
		nextBubblePos = nextBubble->getPosition();
		nextBubble->update(screen, nextBubblePos.x, nextBubblePos.y);
		
	}


	//Libérer les surfaces de la mémoire

	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			//si la bulle est présentement en jeu, libérer la surface de la bulle de la mémoire
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

			grid[i][j].position.x = 47 * j + 10;		//il y a 10 pixels de décalage en X par rapport à la position de départ (0,0)
			grid[i][j].position.y = 47 * i;
			grid[i][j].position.h = 47;
			grid[i][j].position.w = 47;

		}

	}
}

/* Insére une ligne de bulles dans la grille
============================================== */
void insertLine(SDL_Surface *screen, GameGrid grid[GRID_HEIGHT][GRID_WIDTH])
{

	for (int i = 0; i < GRID_WIDTH; i++)		//pour chaque case de la première ligne (index 0)
	{
		int randColor = setRandomValue(NB_COLORS);
		grid[0][i].bubble.setSprite(randColor);
		grid[0][i].bubble.update(screen, grid[0][i].position.x, grid[0][i].position.y);

	}

}															

/* Vérifie la collision de bulles dans l'aire de jeu
======================================================= */
bool checkBubbleCollisions(Bubble *thisBubble, GameGrid grid[GRID_HEIGHT][GRID_WIDTH]) 
{
	
	bool collision = false;		//flag indiquant s'il y a eu collision
	bool status;				//indique si la bulle vérifiée dans la grille est bien en-jeu et affichée à l'écran
	
	for (int i = 0; i < GRID_HEIGHT; i++)		//pour chaque ligne de la grille
	{

		for (int j = 0; j < GRID_WIDTH; j++)		//pour chaque colonne de la grille
		{

			//si la bulle est présentement en jeu, 
			//vérifier si la bulle courante entre en collision avec elle
			//si et seulement si on n'a pas déjà trouvé de collision au préalable

			bool status = grid[i][j].bubble.getInGameStatus();
			if (status && !collision)
			{
				collision = thisBubble->checkCollisions(grid[i][j].bubble);
			}

		}

	}

	return collision;

}