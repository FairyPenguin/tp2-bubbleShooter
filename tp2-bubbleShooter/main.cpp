/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 07/04/2015
Description :  */

/* TODO: améliorations à apporter au programme
================================================= */

/* ------ Amélie ---------- */
//Mettre en place les bulles dans l'aire de jeu->insérer bulles dans la structure GameArea
//Moteur de collision des bulles (à faire mercredi!!!)
//Check Bulles Adjacentes quand le moteur des collisions sera terminé

/* ------ Marc-Antoine ---------- */
//Intégrer scoring + librairie SDL_ttf
//Intégrer SDL_ttf de façon portative


/* Directives au pré-processeur
================================= */
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"
#include "bubbleShooterUtil.h"
#include "canon.h"
#include "bubble.h"

using namespace BubbleShooterUtil;

/* Constantes du projet 
======================== */
const int STARTING_BUBBLES = 7;

/* Structures du projet
=========================== */

////Désigne les "slots" invisibles ou seront placées les bulles dans l'aire de jeu
//struct GameArea
//{
//	int positionX;		//position en X dans l'aire de jeu
//	int positionY;		//position en Y dans l'aire de jeu
//	//set bubbleID??
//};

/* Prototype des fonctions
=========================== */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive);
void initGame(SDL_Surface *screen, Bubble *bubbleArray[300], int &bubbleCounter, int startingNb);
void freeSurface(SDL_Surface *surface);


/* Méthode principale
======================= */
int main(int argc, char *argv[])
{

	SDL_Surface *screen = NULL;			//fenêtre du jeu
	SDL_Surface *background = NULL;		//image de fond du jeu
	SDL_Surface *menu_rect = NULL;		//rectangle du menu
	SDL_Surface *game_logo = NULL;		//logo (nom) du jeu)

	SDL_Event e;						//événement capté par SDL_WaitEvent

	SDL_Rect bgPosition;				//position du fond d'écran dans l'espace
	SDL_Rect menuPosition;				//position du menu dans l'espace
	SDL_Rect logoPosition;				//position du logo dans l'espace

	bool isActive = true;				//flag déterminant si la fenêtre est active


	//Initialisation de la SDL
	initVideo();

	//Paramètres de l'écran
	screen = setupScreen("Bubble Shooter", "sdl_icone.bmp");
	
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
	freeSurface(menu_rect);
	freeSurface(game_logo);
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);

	//Arrêt de la SDL (vider la mémoire)
	SDL_Quit();
	return 0;

}

/* Met une image passée en paramètre au centre de l'écran
========================================================== */
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position)
{
	position.x = screen->w / 2 - picture->w / 2;
	position.y = screen->h / 2 - picture->h / 2;
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

	int bubbleCounter;									//compte le nombre de bulles en jeu et permet d'identifier les bulles par un identifiant numérique
	bool gameIsActive;									//le jeu est actif

	Bubble* bubbleArray[300];							//tableau contenant les bulles en jeu

	Canon* canon = new Canon();							//le canon tirant les bulles 
	Bubble* activeBubble = new Bubble(0);				//la bulle active
	Bubble* nextBubble = new Bubble(1);					//la prochaine bulle en jeu

	//Initialisation des variables
	bubbleCounter = 0;
	gameIsActive = true;

	//Redessiner l'écran avec le background pour faire disparaître le menu
	updateScreen(screen, bg, bgPos);

	//Générer l'aire de jeu et dessiner les bulles qui sont en jeu
	initGame(screen, bubbleArray, bubbleCounter, 7);

	//Dessiner une première fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);
	activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);
	nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);

	SDL_EnableKeyRepeat(50, 50);

	while (gameIsActive)	//tant que le jeu est actif
	{
		SDL_Flip(screen);

		SDL_WaitEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:			//si le joueur clique sur le X de la fenêtre, on ferme l'application EN ENTIER
			gameIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_LEFT:		//tourne le canon à gauche avec la flèche de gauche
				canon->rotate(Canon::LEFT);
				break;


			case SDLK_RIGHT:   //tourne le canon à droite avec la flèche de droite
				canon->rotate(Canon::RIGHT);
				break;

			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace
				/*activeBubble->move();*/
				/*activeBubble->checkSurroundingBubbles()*/
				break;
			}

		}

		//Mettre à jour l'écran après chaque itération de la boucle
		updateScreen(screen, bg, bgPos);
		
		//Mettre à jour les bulles présentement en jeu
		for (int i = 0; i < bubbleCounter; i++)
		{
			bubbleArray[i]->update(screen, 47 * i + 24, 0);
		}

		//Mettre à jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);
		activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
		nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 20);
		
	}

	//Libérer les surfaces de la mémoire
	delete canon;
	delete activeBubble;
	delete nextBubble;

}

/* Initialise les bulles qui sont en jeu
========================================== */
void initGame(SDL_Surface *screen, Bubble *bubbleArray[300], int &bubbleCounter, int startingNb)
{
	int startingPattern[STARTING_BUBBLES] = {0, 1, 2, 2, 0, 1, 0};				//indique les couleurs situées dans le pattern de départ

	for (int i = 0; i < startingNb; i++)										//pour le nombre de bulles à mettre en jeu, en insérer une dans l'aire de jeu
	{
		bubbleArray[i] = new Bubble(startingPattern[i]);
		bubbleArray[i]->update(screen, 47 * i + 24, 0);
	}

	bubbleCounter = startingNb;													//indique au compteur de bulles qu'il y a maintenant (X) bulles en jeu


}

/* Libère la surface d'une image si elle n'est pas NULL
========================================================== */
void freeSurface(SDL_Surface *surface) 
{
	if (surface != NULL)
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}
}