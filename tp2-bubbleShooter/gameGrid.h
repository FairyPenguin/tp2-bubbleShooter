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

	struct PositionStructure
	{
		int x;
		int y;
	};

	//Constructeur et destructeur
	GameGrid(int bubbleDiameter);
	void freeSurfaces();

	//Fonctions utilitaires
	void initPosition(int bubbleDiameter, int line, int column);
	void insertLine(SDL_Surface *screen);
	void update(SDL_Surface *screen);
	bool checkBubbleCollisions(Bubble* activeBubble);
	bool checkActiveBubbleCollision(Bubble* activeBubble);
	bool manageCollision(Bubble* &activeBubble);
	void stickBubbleInGrid(Bubble* &activeBubble);
	bool checkLastLine();
	void shiftGridLines(SDL_Surface *screen);
	void manageAdjacentBubbles(Bubble *activeBubble);
	void resetGridCheck();
	void checkAdjacentBubbles(Bubble checkedBubble);
	int countAdjacentBubbles();
	void popBubbles();
	void countAdjacent(vector<PositionStructure> &toPop, int x, int y, int myColor);
	bool contains(vector<PositionStructure> toPop, PositionStructure position);
	void addedValue(int x, int y, int myColor);
	void pop(PositionStructure position);
	
};