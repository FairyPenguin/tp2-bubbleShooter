/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: main.cpp
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 08/04/2015
Description :  */

/* TODO: am�liorations � apporter au programme
================================================= */

/* ------ Am�lie ---------- */
//Conflit WaitEvent/PollEvent � r�gler. Conflit entre actions du canon et actions de la bulle. Je ne peux ni mettre la canon en PollEvent ou la bulle en WaitEvent... =/
//Calculer la v�locit� correctement. On peut toutefois tester le reste avec des valeurs hardcod�es en attendant
//R�gler changement v�locit� en X quand on touche les coins. Il stick sur le bord.
//Collision des bulles � d�boguer. �a overlap un tit-tit peu... 
//Check Bulles Adjacentes quand le moteur des collisions sera termin�. Ressortir le D�mineur des boules � mites. LOL. 

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
const int STARTING_BUBBLES = 7;
const int REFRESH_TIME = 600;

/* Prototype des fonctions
=========================== */
void showRules(SDL_Surface *screen, SDL_Surface *menu, SDL_Rect &menuPos, bool &screenIsActive);
void centerSprite(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position);
void playGame(SDL_Surface *screen, SDL_Surface *bg, SDL_Rect &bgPos, bool &screenIsActive);
void initGame(SDL_Surface *screen, Bubble *bubbleArray[300], int &bubbleCounter, int startingNb);
bool checkCollisions(Bubble *thisBubble, Bubble *bubbleArray[300], int &bubbleCounter);
void freeSurface(SDL_Surface *surface);


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

	int currentTime;									//temps actuel
	int previousTime;									//temps pr�c�dent
	int bubbleCounter;									//compte le nombre de bulles en jeu
	bool gameIsActive;									//le jeu est actif
	bool bubbleIsMoving;								//la bulle bouge dans l'�cran

	Bubble* bubbleArray[300];							//tableau contenant les bulles en jeu

	Canon* canon = new Canon();							//le canon tirant les bulles 
	Bubble* activeBubble = new Bubble(0);				//la bulle active
	Bubble* nextBubble = new Bubble(1);					//la prochaine bulle en jeu

	//Initialisation des variables
	bubbleCounter = 0;
	previousTime = 0;
	currentTime = 0;
	gameIsActive = true;
	bubbleIsMoving = false;

	//Redessiner l'�cran avec le background pour faire dispara�tre le menu
	updateScreen(screen, bg, bgPos);

	//G�n�rer l'aire de jeu et dessiner les bulles qui sont en jeu
	initGame(screen, bubbleArray, bubbleCounter, 7);

	//Dessiner une premi�re fois le canon, la bulle active et la prochaine bulle en jeu
	canon->update(screen);
	activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25);
	nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 25);

	SDL_EnableKeyRepeat(50, 50);

	while (gameIsActive)	//tant que le jeu est actif
	{
		SDL_Flip(screen);

		//SDL_WaitEvent(&e);	//�v�nements de type Wait
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

					bubbleIsMoving = true;

					//activeBubble->setVelocity();		//calculer la v�locit� de la bulle via l'angle du canon

					//TEST� ICI AVEC DES VALEURS ARBITRAIRES!!!
					activeBubble->setVelocity(3, -3);	//calcule la vitesse de la bulle en X et en Y

					if (bubbleIsMoving)		//si la bulle bouge et n'est pas arr�t�e
					{

						currentTime = SDL_GetTicks();		//initialise le timer en prenant les valeur r�elles

						if (currentTime - previousTime > REFRESH_TIME)	
						{
							activeBubble->move();
							currentTime = previousTime;
						}
						else
						{
							//on endort le programme pour ne pas qu'il prenne trop de CPU
							SDL_Delay(REFRESH_TIME - (previousTime - currentTime));
						}

    					//V�rifier les collisions avec le reste des bulles en jeu
						bubbleIsMoving = checkCollisions(activeBubble, bubbleArray, bubbleCounter);

						//Si jamais la bulle atteint le haut de l'�cran, elle est "stick�e" l� et ne bouge plus
						SDL_Rect activeBubblePos;
						activeBubblePos = activeBubble->getPosition();

						if (activeBubblePos.y < 0)
						{
							bubbleIsMoving = false;
						}
						
						//si la bulle ne bouge plus car elle est entr�e en contact avec quelque chose, 
						//changer la bulle active pour la prochaine bulle, et g�n�rer une nouvelle prochaine bulle
						if (!bubbleIsMoving)
						{
							bubbleArray[bubbleCounter] = activeBubble;
							bubbleCounter++;

							activeBubble = nextBubble;
							//int color = setRandomValue();
							int color = 0;
							nextBubble = new Bubble(color);
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
		for (int i = 0; i < bubbleCounter; i++)
		{
			SDL_Rect pos = bubbleArray[i]->getPosition();
			bubbleArray[i]->update(screen, pos.x, pos.y);
		}

		//Mettre � jour le canon, la bulle active et la prochaine bulle 
		canon->update(screen);

		//Si la bulle active est en mouvement, la bulle active poss�de une nouvelle position.
		if (bubbleIsMoving)		
		{
			activeBubblePos = activeBubble->getPosition();
			activeBubble->update(screen, activeBubblePos.x, activeBubblePos.y);
		}
		//Autrement, elle poss�de les valeurs par d�faut de activeBubble, elle est donc en bas de l'�cran
		else					
		{
			activeBubble->update(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);		
		}
		
		//La prochaine bulle demeure � la m�me position en toutes circonstances
		nextBubble->update(screen, SCREEN_WIDTH / 2 + 47, SCREEN_HEIGHT - 20);
		
	}


	//Lib�rer les surfaces de la m�moire

	for (int i = 0; i < bubbleCounter; i++)		//pour chaque bulle en jeu
	{
		bool status = bubbleArray[i]->getInGameStatus();	//si le garbage collector n'est pas pass� auparavant, enlever les surfaces de la m�moire!
		if (status)
		{
			delete bubbleArray[i];
		}
	}

	delete canon;
	delete activeBubble;
	delete nextBubble;

}

/* Initialise les bulles qui sont en jeu
========================================== */
void initGame(SDL_Surface *screen, Bubble *bubbleArray[300], int &bubbleCounter, int startingNb)
{
	int startingPattern[STARTING_BUBBLES] = {0, 1, 2, 2, 0, 1, 0};				//indique les couleurs situ�es dans le pattern de d�part

	for (int i = 0; i < startingNb; i++)										//pour le nombre de bulles � mettre en jeu, en ins�rer une dans l'aire de jeu
	{
		bubbleArray[i] = new Bubble(startingPattern[i]);
		bubbleArray[i]->update(screen, 47 * i + 24, 0);
	}

	bubbleCounter = startingNb;													//indique au compteur de bulles qu'il y a maintenant (X) bulles en jeu

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

/* V�rifie la collision de bulles
==================================== */
bool checkCollisions(Bubble *thisBubble, Bubble *bubbleArray[300], int &bubbleCounter) 
{
	double distance;						//la distance entre deux bulles
	bool noCollision;						//flag d�terminant s'il y a eu collision

	Bubble::Circle thisBubbleHitbox;		//hitbox de la bulle courante
	Bubble::Circle theOtherHitbox;			//hitbox de l'autre bulle � comparer

	noCollision = true;		//initialisation du flag

	//Param�tres de la bulle courante
	thisBubbleHitbox = thisBubble->getHitbox();

	for (int i = 0; i < bubbleCounter; i++)		//pour chaque bulle pr�sente dans l'aire de jeu
	{
		int gameStatus = bubbleArray[i]->getInGameStatus();

		if (gameStatus)		//si la bulle � comparer est bien en jeu
		{

			//V�rifier tant que l'on a pas trouv� de collision: autrement on ne v�rifie pas la condition
			if (noCollision)
			{

				//Param�tres de la bulle � comparer
				theOtherHitbox = bubbleArray[i]->getHitbox();

				//Calcule la distance entre les centres du cercle
				distance = sqrt( (double)pow(theOtherHitbox.centerX - thisBubbleHitbox.centerX, 2) +  (double)pow(theOtherHitbox.centerY - thisBubbleHitbox.centerY, 2));

				//Il y a collision si la distance entre les centres du cercles est plus petite que la somme de leurs rayons
				if (distance < (thisBubbleHitbox.radius + theOtherHitbox.radius))
				{
					noCollision = false;
				}
				else
				{
					noCollision = true;
				}

			}
			
		}

	}

	return noCollision;

}