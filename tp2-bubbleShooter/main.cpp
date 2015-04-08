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

/* Structures du projet
=========================== */

//Désigne les "slots" invisibles ou seront placées les bulles dnas l'aire de jeu
struct GameArea
{
	int positionX;		//position en X dans l'aire de jeu
	int positionY;		//position en Y dans l'aire de jeu
	//set bubbleID??
};

/* Prototype des fonctions
=========================== */
void initCharset(SDL_Rect charset[4][3]);
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive);
void initGameArea(GameArea area[10][13], int nbBubbles);
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
	Canon* canon = new Canon();			//le canon tirant les bulles 

	GameArea bubbleArray[10][8] = {0, 0};		//le tableau stockant les bulles en jeu ainsi que leur position
	
	Bubble* activeBubble = new Bubble(0);		//la bulle active
	Bubble* nextBubble = new Bubble(1);			//la prochaine bulle en jeu

	SDL_Event e;						//événement capté par SDL_WaitEvent

	int bubbleCounter = 0;				//compte le nombre de bulles en jeu et permet d'identifier les bulles (set bubbleID??)
	bool gameIsActive = true;			//le jeu est actif

	//Redessiner l'écran avec le background pour faire disparaître le menu
	updateScreen(screen, bg, bgPos);

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
				/*activeBubble->shoot();*/
				break;
			}

		}

		updateScreen(screen, bg, bgPos);
		
		//Mettre à jour l'écran après chaque itération de la boucle
		canon->update(screen);
		activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
		nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 20);
		
	}

	//Libérer les surfaces de la mémoire
	delete canon;
	delete activeBubble;
	delete nextBubble;

}

/* Initialiser l'aire de jeu 
================================ */
void initGameArea(GameArea area[10][8], int nbBubbles) 
{
	
		for (int i = 0; i < 10; i++)		//pour chaque rangée de l'aire de jeu
		{
			for (int j = 0; j < 8; j++)
			{

				area[i][j].positionX = 47 * i;
				area[i][j].positionX = 47 * j;
			
				if (nbBubbles > 0)			//tant que l'on peut mettre des bulles en jeu
				{
					//insérer bulle dans l'endroit correspondant
					//nbBubbles--;
				}

			}	

		}
	
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