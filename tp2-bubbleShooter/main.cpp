/* Header
================
Name: main.cpp
Authors: Amélie Frappier & Marc-Antoine Larose
Creation Date: 26/03/2015
Description:  */

/* TODOs
=========== */
//Header: add description
//Draw start screen (integrate text? Sprite sheet for text?)
//Time to do some PIXEL AAAAAAAART~
//Animate start text???
//Implement structures into game (Bubble class, GameArea class, Cannon class)


/* Pre-Processing Information
============================== */
#include <iostream>
#include "../SDL/SDL.h"

using namespace std;

/* Constants
=============*/
const int SCREEN_HEIGHT = 650;
const int SCREEN_WIDTH = 400;

/* Method Prototypes
===================== */
void initVideo();
SDL_Surface* setupScreen(char* title, char* img, int height, int width);
SDL_Surface* loadBitmap(char* imgName);
void setTransparency(SDL_Surface *picture, int red, int blue, int green);
void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position);
void initCharset(SDL_Rect charset[4][3]);
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
void checkKeyPresses(SDL_Event e, SDL_Rect &position, int moveIncrement, int &dir, int &frame, bool &active);
int cycleWalkAnimation();


/* Main Method
================ */
int main(int argc, char *argv[])
{

	SDL_Surface *screen = NULL;		//game screen
	//SDL_Surface *player = NULL;		//image du personnage
	SDL_Event e;					//event caught by SDL_WaitEvent
	//SDL_Rect playerPosition;		//position du personnage dans l'espace
	//SDL_Rect posCharset[4][3];		//tableau des images captées sur le charset

	//int direction = 0;				//direction où marche le personnage
	//int frame = 0;					//frame d'animation du personnage
	bool isActive = true;			//flag: game screen is currently active


	//Initialize SDL screen
	initVideo();

	//Screen parameters
	screen = setupScreen("Bubble Shooter", "sdl_icone.bmp", SCREEN_HEIGHT, SCREEN_WIDTH);

	//Paramètres du joueur
	//player = loadBitmap("charset.bmp");					//charge l'image
	//initCharset(posCharset);							//initialisation du tableau des positions du charset
	//centerSprite(screen, player, playerPosition);		//met le sprite au centre de l'écran
	//setTransparency(player, 255, 255, 255);				//paramètre la transparence de l'image

	//Rendering: Update screen for the first time
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 153, 204));
	SDL_Flip(screen);
	/*updateScreen(screen, player, posCharset[0][0], playerPosition);*/

	//Permet les répétitions de touche (20 ms)
	/*SDL_EnableKeyRepeat(40, 40);*/

	while (isActive)	//while game screen is currently active
	{
		SDL_WaitEvent(&e);
		switch (e.type)
		{

		case SDL_QUIT:		//on Windows Close button, close the game
			isActive = 0;
			break;

		case SDL_KEYDOWN:	//on keyPress...
			
			switch (e.key.keysym.sym)
			{

				//on Q, quit game
			case SDLK_q:
				//show confirmation screen???
				isActive = 0;
				break;

				//on R, show rules of the game
			case SDLK_r:
				//showRules();
				break;

				//on SPACE, start the game!
			case SDLK_SPACE:
				//gameStart();
				break;

			}

			break;

		}

		//Rendering: update screen based on current event
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 153, 204));
		SDL_Flip(screen);
		
		/*updateScreen(screen, player, posCharset[direction][frame], playerPosition);*/

	}

	//Free surfaces from memory
	/*SDL_FreeSurface(player);*/
	SDL_FreeSurface(screen);

	//Quit the game
	SDL_Quit();
	return 0;

}

/* Initialise la SDL
====================== */
void initVideo()
{
	SDL_Init(SDL_INIT_VIDEO);	//Démarrage de la SDL (charger le système vidéo)

	if (SDL_Init(SDL_INIT_VIDEO) == -1)  //Si le démarrage de la librairie SDL échoue
	{
		//Gestion des erreurs de la librairie SDL
		cout << "Erreur lors de l'initialisation de la SDL", SDL_GetError();
		exit(EXIT_FAILURE);
	}
}

/* Initialise les paramètres de l'écran
========================================== */
SDL_Surface* setupScreen(char* title, char* img, int height, int width)
{
	SDL_Surface* screen;	//Écran à paramètrer

	//Chargement de la vidéo
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
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
	SDL_Surface* bmp;		//image au format bitmap

	bmp = SDL_LoadBMP(imgName);

	return bmp;
}

/* Met la couleur de fond d'une image en transparence
======================================================= */
void setTransparency(SDL_Surface *picture, int red, int blue, int green)
{
	SDL_SetColorKey(picture, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(picture->format, red, blue, green));
}

/* Met à jour l'écran du jeu
============================= */
void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 153, 204));
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

/* Met une image passée en paramètre au centre de l'écran
========================================================== */
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position)
{
	position.x = screen->w / 2 - picture->w / 2;
	position.y = screen->h / 2 - picture->h / 2;
}

/* Vérifie quelle touche a été tapée sur le clavier et agit en conséquence
=========================================================================== */
void checkKeyPresses(SDL_Event e, SDL_Rect &position, int moveIncrement, int &dir, int &frame, bool &active)
{

	switch (e.key.keysym.sym)
	{

		//si on appuie sur Échap, fermer le programme
	case SDLK_ESCAPE:
		active = 0;
		break;

		//le personnage se déplace vers le haut
	case SDLK_UP:
		position.y -= moveIncrement;
		dir = 3;
		frame = cycleWalkAnimation();
		break;

		//le personnage se déplace vers le bas
	case SDLK_DOWN:
		position.y += moveIncrement;
		dir = 0;
		frame = cycleWalkAnimation();
		break;

		//le personnage de déplace vers la gauche
	case SDLK_LEFT:
		position.x -= moveIncrement;
		dir = 1;
		frame = cycleWalkAnimation();
		break;

		//le personnage de déplace vers la droite
	case SDLK_RIGHT:
		position.x += moveIncrement;
		dir = 2;
		frame = cycleWalkAnimation();
		break;

	}

}

/* Cycle d'animation de marche du joueur
========================================= */
int cycleWalkAnimation()
{
	static int frameNumber = 0;		//numéro de la frame où on est rendu
	static int rewind = 0;			//détermine si l'animation revient en arrière (1)
	//ou avance ar en avant (0)

	if (frameNumber == 0)			//dès que le personnage atteint la première frame
	{
		rewind = 0;		//le personnage va par en avant
	}
	else if (frameNumber == 2)		//autrement, passer à la prochaine frame
	{
		rewind = 1;		//le personnage va par en arrière
	}

	if (rewind == 0)			//si le personnage va vers l'avant, incrémenter positivement
		//le numéro de frame
	{
		frameNumber++;
	}
	else if (rewind == 1)	//si le personnage va vers l'avant, incrémenter positivement
		//le numéro de frame
	{
		frameNumber--;
	}

	return frameNumber;
}