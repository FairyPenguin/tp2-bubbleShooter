/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 
Description :  */

/* TODO: améliorations à apporter au programme
================================================= */
//Créer des classes pour le canon, les bulles et l'aire de jeu
//Mettre en place le jeu en lui-même
//Intégrer classement + scoring + inclure librairie SDL_ttf

/* Directives au pré-processeur
================================= */
#include <iostream>
#include "../SDL/SDL.h"

using namespace std;

/* Constantes du programme
=========================== */
const int SCREEN_HEIGHT = 450;
const int SCREEN_WIDTH = 600;

/* Prototype des fonctions
=========================== */
void initVideo();
SDL_Surface* setupScreen(char* title, char* img);
SDL_Surface* loadBitmap(char* imgName);
void setPosition(SDL_Surface *picture, SDL_Rect &position, int posX, int posY);
void setTransparency(SDL_Surface *picture, int red, int green, int blue);
void updateScreenNoCharset(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position);
void initCharset(SDL_Rect charset[4][3]);
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
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
	//setTransparency(game_logo, 255, 0, 255);

	//Mettre à jour l'écran une première fois
	updateScreenNoCharset(screen, background, bgPosition);
	updateScreenNoCharset(screen, game_logo, logoPosition);
	updateScreenNoCharset(screen, menu_rect, menuPosition);

	while (isActive)	//tant que l'application est active
	{
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


			case SDLK_SPACE:	//Si cela fonctionne, appeler la fonction play();
				//play();
				break;

			}

		}

		////Rendering: Mettre à jour la fenêtre de jeu
		//updateScreen(screen, player, posCharset[direction][frame], playerPosition);

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

/* Initialise la SDL
====================== */
void initVideo()
{
	SDL_Init(SDL_INIT_EVERYTHING);	//Démarrage de la SDL (charger le système vidéo)

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)  //Si le démarrage de la librairie SDL échoue
	{
		//Gestion des erreurs de la librairie SDL
		cout << "Erreur lors de l'initialisation de la SDL", SDL_GetError();
		exit(EXIT_FAILURE);
	}
}

/* Initialise les paramètres de l'écran
========================================== */
SDL_Surface* setupScreen(char* title, char* img)
{
	SDL_Surface* screen;	//Écran à paramètrer

	//Chargement de la vidéo
	screen = SDL_SetVideoMode(SCREEN_HEIGHT, SCREEN_WIDTH, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	if (screen == NULL)
	{
		cout << "Erreur lors de l'initialisation de la SDL" << SDL_GetError();
		exit(EXIT_FAILURE);
	}

	//Modification des paramètres de la vidéo
	SDL_WM_SetCaption(title, NULL);
	SDL_WM_SetIcon(SDL_LoadBMP(img), NULL);

	return screen;
}

/* Télécharge une image en format .bmp
========================================== */
SDL_Surface* loadBitmap(char* imgName)
{
	SDL_Surface* loadedImage = NULL;		//Temporary storage for the image that's loaded
	SDL_Surface* optimizedImage = NULL;		//The optimized image that will be used

	loadedImage = SDL_LoadBMP(imgName);		//Load the image

	//If nothing went wrong in loading the image
	if (loadedImage != NULL)
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);

		//Free the old image
		SDL_FreeSurface(loadedImage);
	}

	return optimizedImage;
}

/* Met la couleur de fond d'une image en transparence
======================================================= */
void setTransparency(SDL_Surface *picture, int red, int green, int blue)
{
	//If nothing went wrong in loading the image
	if (picture != NULL)
	{
		SDL_SetColorKey(picture, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(picture->format, red, green, blue));
	}
}

/* Met à jour l'écran du jeu - Ne prend pas en compte les charset
================================================================== */
void updateScreenNoCharset(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position) 
{
	SDL_BlitSurface(picture, NULL, screen, &position);
	SDL_Flip(screen);
}

/* Met à jour l'écran du jeu - Prend en compte les charset
=========================================================== */
void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position)
{
	SDL_BlitSurface(picture, &currentSprite, screen, &position);
	SDL_Flip(screen);
}

/* Initialise le tableau des sprites du charset
================================================= */
void initCharset(SDL_Rect charset[4][3])
{

	for (int x = 0; x < 4; x++)		//pour chaque ligne de l'image
	{

		for (int y = 0; y < 3; y++)		//pour chaque frame d'animation par ligne
		{
			charset[x][y].x = y * 32;	//position du posCharset sur l'axe des X
			charset[x][y].y = x * 32;	//position de posCharset sur l'axe des Y
			charset[x][y].w = 32;		//largeur en pixels du rectangle
			charset[x][y].h = 32;		//hauteur en pixels du rectangle
		}

	}

}


/* Paramètre la position d'une image dans l'espace
==================================================== */
void setPosition(SDL_Surface *picture, SDL_Rect &position, int posX, int posY)
{
	position.x = posX;
	position.y = posY;
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
	updateScreenNoCharset(screen, rules, rulesPos);

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

				updateScreenNoCharset(screen, menu, menuPos);	//Redessiner le menu sur l'écran	
				rulesIsActive = 0;
				break;
			}

		}

	}

	SDL_FreeSurface(rules);		//Libérer les règles de la mémoire

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