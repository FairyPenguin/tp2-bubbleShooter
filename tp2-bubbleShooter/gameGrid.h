/* En-t�te du programme
=========================
Programme: Bubble Shooter
Fichier: gameGrid.h
Auteur : Am�lie Frappier et Marc-Antoine Larose
Date cr�ation : 23/03/2015
Date modification: 12/04/2015
Description :  Classe GameGrid du jeu Bubble Shooter */

/* Directives au pr�-processeur
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

/* D�finition de la classe
================================= */
class GameGrid
{
private:

	static const int GRID_HEIGHT = 10;				//hauteur de la grille de jeu
	static const int GRID_WIDTH = 8;				//largueur de la grille de jeu
	static const int REQUIRED_BUBBLES_TO_POP = 4;	//nombre de bulles adjacentes n�c�ssaires avant que �a "pop"
													//(3 + 1 car cette constante inclut la bulle active)

	SDL_Rect position;							//coordonn�es en pixels d'une case dans la grille
	Bubble bubble[GRID_HEIGHT][GRID_WIDTH];		//bulle contenue dans une case donn�e de la grille
	int offsetX;								//d�calage en X de la grille par rapport � la position (0,0)


public:

	//Constructeur et destructeur
	GameGrid(int bubbleDiameter);
	void freeSurfaces();

	//Fonctions utilitaires
	void initPosition(int bubbleDiameter, int line, int column);		//initialise les positions des bulles de la grille
	void insertLine(SDL_Surface *screen);								//ins�re une ligne dans la grille
	void update(SDL_Surface *screen);									//met � jour les bulles de la gruille
	bool checkBubbleCollisions(Bubble* activeBubble);					//v�rifie les collisions entre la bulle active et les bulles de la grille
	bool checkActiveBubbleCollision(Bubble* activeBubble);				//appelle checkActiveBubbleCollision() ainsi que les collisions sur les murs
	bool manageCollision(Bubble* &activeBubble);						//g�re les collisions de la bulle active
	void stickBubbleInGrid(Bubble* &activeBubble);						//met la bulle dans la grille et v�rifie l'adjacence
	bool checkLastLine();												//v�rifie s'il y a des bulles en jeu dans la derni�re ligne
	void shiftGridLines(SDL_Surface *screen);							//d�cale les bulles 1 ligne plus bas dans la grille avant d'en ins�rer une nouvelle
	void resetGridCheck();												//remet la grille � z�ro avant de v�rifier l'adjacence

	void countAdjacent(vector<PositionStructure> &toPop, int x, int y, int myColor);
	bool contains(vector<PositionStructure> toPop, PositionStructure position);
	void addedValue(int x, int y, int myColor);
	
};

#endif