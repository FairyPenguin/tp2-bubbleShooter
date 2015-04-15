/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 14/04/2015
Description :  Dans le jeu Bubble Shooter, il faut faire des combinaisons de 3 bulles de m�me couleur ou plus pour les faire dispara�tre de la grille de jeu.
En usant des fl�ches, le joueur fait tourner un canon dans la direction voulue, et en appuyant sur Espace, il tire une bulle dans l'aire de jeu. En touchant une bulle, 
le jeu v�rifie s'il y a 3 couleurs identiques ou plus autour de lui: si c'est le cas, les bulles disparaissent. Si les bulles descendent trop bas, le jeu se termine.*/

/* TODO: am�liorations � apporter au programme
================================================= */
//popBubbles fait crasher le jeu � certaines moments (out of bounds ne fonctionne pas bien??)
//Les destructeurs ne fonctionnenent pas correctement

/* Directives au pr�-processeur
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
				playGame(screen, background, bgPosition, game_logo, logoPosition, menu_rect, menuPosition, isActive);
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
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, SDL_Surface *logo, SDL_Rect &logoPos, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive)
{
	SDL_Event moveE;									//�v�nement capt� par SDL_PollEvent

	int randomColor;									//couleur choisie au hasard
	int newColor;										//nouvelle couleur assign�e � une bulle donn�e

	bool gameIsActive;									//le jeu est actif
	bool hasLost;										//flage d�terminant si le jeu a �t� gagn� ou perdu
	bool bubbleHasCollided;								//d�termine si la bulle active a collisionn� ou non

	GameGrid* bubbleGrid = new GameGrid(47);			//grille de jeu contenant les bulles

	Canon* canon = new Canon();							//le canon tirant les bulles 

	Bubble* activeBubble = new Bubble(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);		//la bulle active
	Bubble* nextBubble = new Bubble(SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);		//la prochaine bulle en jeu

	//Initialisation des variables
	gameIsActive = true;
	hasLost = false;
	bubbleHasCollided = false;

	//Initialisation du g�n�rateur de nombres al�atoires
	srand(time(NULL));

	//Redessiner l'�cran avec le background pour faire dispara�tre le menu
	updateScreen(screen, bg, bgPos);
	
	//Ins�rer une ligne de bulles dans la grille
	bubbleGrid->insertLine(screen);

	//Dessiner une premi�re fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);

	randomColor = getRandomValue(3);
	activeBubble->setSprite(randomColor);
	activeBubble->update(screen);

	randomColor = getRandomValue(3);
	nextBubble->setSprite(randomColor);
	nextBubble->update(screen);

	SDL_EnableKeyRepeat(50, 50);

	//prend le temps �coul� en millisecondes
	long nextUpdateTimeInMilliseconds = clock();
	long nextInsertTime = nextUpdateTimeInMilliseconds + TIME_BETWEEN_INSERT;

	while (gameIsActive)	//tant que le jeu est actif
	{

		//S'assure que le jeu roule � 60 Frames par Secondes (FPS)
		while (nextUpdateTimeInMilliseconds > clock());

		nextUpdateTimeInMilliseconds += TIME_IN_MILLISECONDS_BEFORE_NEXT_UPDATE;

		//Ins�re une ligne � la grille � toutes les 30 secondes
		if (nextInsertTime <= clock())
		{
			/*bubbleGrid->insertLine(screen);*/
			bubbleGrid->shiftGridLines(screen);
			nextInsertTime += TIME_BETWEEN_INSERT;
		}

		//Charger l'�cran avec toutes les images pr�c�demment "blitt�es"
 		SDL_Flip(screen);

 		SDL_PollEvent(&moveE);	//�v�nements de type Poll
		switch (moveE.type)
		{

		case SDL_QUIT:			//si le joueur clique sur le X de la fen�tre, on ferme l'application EN ENTIER
			gameIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (moveE.key.keysym.sym)
			{

			case SDLK_LEFT:		//tourne le canon � gauche avec la fl�che de gauche
				canon->rotate(Canon::LEFT);
				break;

			case SDLK_RIGHT:   //tourne le canon � droite avec la fl�che de droite
				canon->rotate(Canon::RIGHT);
				break;

			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace	
				activeBubble->setVelocity(canon->getRotationAngle());
				break;
			}
			break;
		}

		//V�rifie si la bulle entre en collision
		bubbleHasCollided = bubbleGrid->manageCollision(activeBubble);

		//Si la bulle active doit �tre chang�e, la prochaine bulle devient la bulle active
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

		//Mettre � jour l'�cran
		updateScreen(screen, bg, bgPos);

		//Mettre � jour la grille des bulles
		bubbleGrid->update(screen);

		//Mettre � jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);
		activeBubble->update(screen);
		nextBubble->update(screen);

		//V�rifier les conditions de d�faite
		hasLost = bubbleGrid->checkLastLine();
		if (hasLost)
		{
			gameIsActive = false;
		}
		
	}

	//Redessine les �l�ments du menu principal
	updateScreen(screen, bg, bgPos);
	updateScreen(screen, logo, logoPos);
	updateScreen(screen, menu, menuPos);

	//Lib�re les surfaces de la grille de jeu
	/*bubbleGrid->freeSurfaces();*/
	/*delete canon;
	delete activeBubble;
	delete nextBubble;*/

}


