/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 14/04/2015
Description :  Dans le jeu Bubble Shooter, il faut faire des combinaisons de 3 bulles de même couleur ou plus pour les faire disparaître de la grille de jeu.
En usant des flèches, le joueur fait tourner un canon dans la direction voulue, et en appuyant sur Espace, il tire une bulle dans l'aire de jeu. En touchant une bulle, 
le jeu vérifie s'il y a 3 couleurs identiques ou plus autour de lui: si c'est le cas, les bulles disparaissent. Si les bulles descendent trop bas, le jeu se termine.*/

/* TODO: améliorations à apporter au programme
================================================= */
//popBubbles fait crasher le jeu à certaines moments (out of bounds ne fonctionne pas bien??)
//Les destructeurs ne fonctionnenent pas correctement

/* Directives au pré-processeur
================================= */
#include <math.h>
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"
#include "bubbleShooterUtil.h"
#include "canon.h"
#include "bubble.h"
#include "gameGrid.h"

using namespace BubbleShooterUtil;

/* Constantes du projet 
======================== */
const int GRID_HEIGHT = 10;
const int GRID_WIDTH = 8;
const int REFRESH_TIME = 600;
const int TIME_IN_MILLISECONDS_BEFORE_NEXT_UPDATE = 16;
const int TIME_BETWEEN_INSERT = 15000;

/* Prototype des fonctions
=========================== */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, SDL_Surface *logo, SDL_Rect &logoPos, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
bool checkIfGameContinues(GameGrid grid[GRID_HEIGHT][GRID_WIDTH]);


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
				playGame(screen, background, bgPosition, game_logo, logoPosition, menu_rect, menuPosition, isActive);
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
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, SDL_Surface *logo, SDL_Rect &logoPos, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive)
{
	SDL_Event moveE;									//événement capté par SDL_PollEvent

	int randomColor;									//couleur choisie au hasard
	int newColor;										//nouvelle couleur assignée à une bulle donnée

	bool gameIsActive;									//le jeu est actif
	bool hasLost;										//flage déterminant si le jeu a été gagné ou perdu
	bool bubbleHasCollided;								//détermine si la bulle active a collisionné ou non

	GameGrid* bubbleGrid = new GameGrid(47);			//grille de jeu contenant les bulles

	Canon* canon = new Canon();							//le canon tirant les bulles 

	Bubble* activeBubble = new Bubble(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);		//la bulle active
	Bubble* nextBubble = new Bubble(SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);		//la prochaine bulle en jeu

	//Initialisation des variables
	gameIsActive = true;
	hasLost = false;
	bubbleHasCollided = false;

	//Initialisation du générateur de nombres aléatoires
	srand(time(NULL));

	//Redessiner l'écran avec le background pour faire disparaître le menu
	updateScreen(screen, bg, bgPos);
	
	//Insérer une ligne de bulles dans la grille
	bubbleGrid->insertLine(screen);

	//Dessiner une première fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);

	randomColor = getRandomValue(3);
	activeBubble->setSprite(randomColor);
	activeBubble->update(screen);

	randomColor = getRandomValue(3);
	nextBubble->setSprite(randomColor);
	nextBubble->update(screen);

	SDL_EnableKeyRepeat(50, 50);

	//prend le temps écoulé en millisecondes
	long nextUpdateTimeInMilliseconds = clock();
	long nextInsertTime = nextUpdateTimeInMilliseconds + TIME_BETWEEN_INSERT;

	while (gameIsActive)	//tant que le jeu est actif
	{

		//S'assure que le jeu roule à 60 Frames par Secondes (FPS)
		while (nextUpdateTimeInMilliseconds > clock());

		nextUpdateTimeInMilliseconds += TIME_IN_MILLISECONDS_BEFORE_NEXT_UPDATE;

		//Insère une ligne à la grille à toutes les 30 secondes
		if (nextInsertTime <= clock())
		{
			/*bubbleGrid->insertLine(screen);*/
			bubbleGrid->shiftGridLines(screen);
			nextInsertTime += TIME_BETWEEN_INSERT;
		}

		//Charger l'écran avec toutes les images précédemment "blittées"
 		SDL_Flip(screen);

 		SDL_PollEvent(&moveE);	//événements de type Poll
		switch (moveE.type)
		{

		case SDL_QUIT:			//si le joueur clique sur le X de la fenêtre, on ferme l'application EN ENTIER
			gameIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (moveE.key.keysym.sym)
			{

			case SDLK_LEFT:		//tourne le canon à gauche avec la flèche de gauche
				canon->rotate(Canon::LEFT);
				break;

			case SDLK_RIGHT:   //tourne le canon à droite avec la flèche de droite
				canon->rotate(Canon::RIGHT);
				break;

			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace	
				activeBubble->setVelocity(canon->getRotationAngle());
				break;
			}
			break;
		}

		//Vérifie si la bulle entre en collision
		bubbleHasCollided = bubbleGrid->manageCollision(activeBubble);

		//Si la bulle active doit être changée, la prochaine bulle devient la bulle active
		//et une nouvelle prochaine bulle prend sa place.
		if (bubbleHasCollided)
		{	
			//la bulle active devient la prochaine bulle
			newColor = nextBubble->getColor();
			activeBubble->setSprite(newColor);
			activeBubble->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);

			//la prochaine bulle devient une nouvelle bulle
			randomColor = getRandomValue(3);
			nextBubble = new Bubble(SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);
			nextBubble->setSprite(randomColor);

		}

		//Mettre à jour l'écran
		updateScreen(screen, bg, bgPos);

		//Mettre à jour la grille des bulles
		bubbleGrid->update(screen);

		//Mettre à jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);
		activeBubble->update(screen);
		nextBubble->update(screen);

		//Vérifier les conditions de défaite
		hasLost = bubbleGrid->checkLastLine();
		if (hasLost)
		{
			gameIsActive = false;
		}
		
	}

	//Redessine les éléments du menu principal
	updateScreen(screen, bg, bgPos);
	updateScreen(screen, logo, logoPos);
	updateScreen(screen, menu, menuPos);

	//Libère les surfaces de la grille de jeu
	/*bubbleGrid->freeSurfaces();*/
	/*delete canon;
	delete activeBubble;
	delete nextBubble;*/

}


