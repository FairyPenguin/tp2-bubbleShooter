/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: gameGrid.h
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 12/04/2015
Description :  Classe GameGrid du jeu Bubble Shooter */

/* Directives au pré-processeur
================================= */
#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"
#include "bubble.h"
#include <vector>

using namespace std;
using namespace BubbleShooterUtil;



struct PositionStructure
{
	int x;
	int y;
};

#ifndef GAMEGRID_H
#define GAMEGRID_H

/* Définition de la classe
================================= */
class GameGrid
{
private:

	static const int GRID_HEIGHT = 10;				//hauteur de la grille de jeu
	static const int GRID_WIDTH = 8;				//largueur de la grille de jeu
	static const int REQUIRED_BUBBLES_TO_POP = 4;	//nombre de bulles adjacentes nécéssaires avant que ça "pop"
													//(3 + 1 car cette constante inclut la bulle active)

	SDL_Rect position;							//coordonnées en pixels d'une case dans la grille
	Bubble bubble[GRID_HEIGHT][GRID_WIDTH];		//bulle contenue dans une case donnée de la grille
	int offsetX;								//décalage en X de la grille par rapport à la position (0,0)


public:

	//Constructeur et destructeur
	GameGrid(int bubbleDiameter);
	void freeSurfaces();

	//Fonctions utilitaires
	void initPosition(int bubbleDiameter, int line, int column);		//initialise les positions des bulles de la grille
	void insertLine(SDL_Surface *screen);								//insère une ligne dans la grille
	void update(SDL_Surface *screen);									//met à jour les bulles de la gruille
	bool checkBubbleCollisions(Bubble* activeBubble);					//vérifie les collisions entre la bulle active et les bulles de la grille
	bool checkActiveBubbleCollision(Bubble* activeBubble);				//appelle checkActiveBubbleCollision() ainsi que les collisions sur les murs
	bool manageCollision(Bubble* &activeBubble);						//gère les collisions de la bulle active
	void stickBubbleInGrid(Bubble* &activeBubble);						//met la bulle dans la grille et vérifie l'adjacence
	bool checkLastLine();												//vérifie s'il y a des bulles en jeu dans la dernière ligne
	void shiftGridLines(SDL_Surface *screen);							//décale les bulles 1 ligne plus bas dans la grille avant d'en insérer une nouvelle
	void resetGridCheck();												//remet la grille à zéro avant de vérifier l'adjacence

	void countAdjacent(vector<PositionStructure> &toPop, int x, int y, int myColor);
	bool contains(vector<PositionStructure> toPop, PositionStructure position);
	void addedValue(int x, int y, int myColor);
	
};

#endif