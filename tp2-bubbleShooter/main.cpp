/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 07/04/2015
Description :  */

/* TODO: am�liorations � apporter au programme
================================================= */

/* ------ Am�lie ---------- */
//Mettre en place les bulles dans l'aire de jeu->ins�rer bulles dans la structure GameArea
//Moteur de collision des bulles (� faire mercredi!!!)
//Check Bulles Adjacentes quand le moteur des collisions sera termin�

/* ------ Marc-Antoine ---------- */
//Int�grer scoring + librairie SDL_ttf
//Int�grer SDL_ttf de fa�on portative


/* Directives au pr�-processeur
================================= */
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"
#include "bubbleShooterUtil.h"
#include "canon.h"
#include "bubble.h"

using namespace BubbleShooterUtil;

/* Structures du projet
=========================== */

//D�signe les "slots" invisibles ou seront plac�es les bulles dnas l'aire de jeu
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


/* M�thode principale
======================= */
int main(int argc, char *argv[])
{

	SDL_Surface *screen = NULL;			//fen�tre du jeu
	SDL_Surface *background = NULL;		//image de fond du jeu
	SDL_Surface *menu_rect = NULL;		//rectangle du menu
	SDL_Surface *game_logo = NULL;		//logo (nom) du jeu)

	SDL_Event e;						//�v�nement capt� par SDL_WaitEvent

	SDL_Rect bgPosition;				//position du fond d'�cran dans l'espace
	SDL_Rect menuPosition;				//position du menu dans l'espace
	SDL_Rect logoPosition;				//position du logo dans l'espace

	bool isActive = true;				//flag d�terminant si la fen�tre est active


	//Initialisation de la SDL
	initVideo();

	//Param�tres de l'�cran
	screen = setupScreen("Bubble Shooter", "sdl_icone.bmp");
	
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
	freeSurface(menu_rect);
	freeSurface(game_logo);
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);

	//Arr�t de la SDL (vider la m�moire)
	SDL_Quit();
	return 0;

}

/* Met une image pass�e en param�tre au centre de l'�cran
========================================================== */
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position)
{
	position.x = screen->w / 2 - picture->w / 2;
	position.y = screen->h / 2 - picture->h / 2;
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
	Canon* canon = new Canon();			//le canon tirant les bulles 

	GameArea bubbleArray[10][8] = {0, 0};		//le tableau stockant les bulles en jeu ainsi que leur position
	
	Bubble* activeBubble = new Bubble(0);		//la bulle active
	Bubble* nextBubble = new Bubble(1);			//la prochaine bulle en jeu

	SDL_Event e;						//�v�nement capt� par SDL_WaitEvent

	int bubbleCounter = 0;				//compte le nombre de bulles en jeu et permet d'identifier les bulles (set bubbleID??)
	bool gameIsActive = true;			//le jeu est actif

	//Redessiner l'�cran avec le background pour faire dispara�tre le menu
	updateScreen(screen, bg, bgPos);

	//Dessiner une premi�re fois le canon, la bulle active et la prochaine bulle en jeu
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
		case SDL_QUIT:			//si le joueur clique sur le X de la fen�tre, on ferme l'application EN ENTIER
			gameIsActive = 0;
			screenIsActive = 0;
			break;

		case SDL_KEYDOWN:		//si l'on appuie sur une touche quelconque

			switch (e.key.keysym.sym)
			{

			case SDLK_LEFT:		//tourne le canon � gauche avec la fl�che de gauche
				canon->rotate(Canon::LEFT);
				break;


			case SDLK_RIGHT:   //tourne le canon � droite avec la fl�che de droite
				canon->rotate(Canon::RIGHT);
				break;

			case SDLK_SPACE:	//lance la bulle lorsque l'on appuie sur Espace
				/*activeBubble->shoot();*/
				break;
			}

		}

		updateScreen(screen, bg, bgPos);
		
		//Mettre � jour l'�cran apr�s chaque it�ration de la boucle
		canon->update(screen);
		activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
		nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 20);
		
	}

	//Lib�rer les surfaces de la m�moire
	delete canon;
	delete activeBubble;
	delete nextBubble;

}

/* Initialiser l'aire de jeu 
================================ */
void initGameArea(GameArea area[10][8], int nbBubbles) 
{
	
		for (int i = 0; i < 10; i++)		//pour chaque rang�e de l'aire de jeu
		{
			for (int j = 0; j < 8; j++)
			{

				area[i][j].positionX = 47 * i;
				area[i][j].positionX = 47 * j;
			
				if (nbBubbles > 0)			//tant que l'on peut mettre des bulles en jeu
				{
					//ins�rer bulle dans l'endroit correspondant
					//nbBubbles--;
				}

			}	

		}
	
}

/* Lib�re la surface d'une image si elle n'est pas NULL
========================================================== */
void freeSurface(SDL_Surface *surface) 
{
	if (surface != NULL)
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}
}