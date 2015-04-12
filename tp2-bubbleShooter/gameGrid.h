#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"
#include "bubble.h"

using namespace BubbleShooterUtil;

class GameGrid
{
private:

	static const int GRID_HEIGHT = 10;
	static const int GRID_WIDTH = 8;

	SDL_Rect position;							//coordonnées en pixels de la grille
	Bubble bubble[GRID_HEIGHT][GRID_WIDTH];		//bulle contenue dans la grille
	int offsetX;								//décalage en X de la grille par rapport à la position (0,0)


public:

	//Constructeur et destructeur
	GameGrid(int bubbleDiameter);
	~GameGrid();

	//Setters

	//Fonctions utilitaires
	void initPosition(int bubbleDiameter, int line, int column);
	void insertLine(SDL_Surface *screen);
	void update(SDL_Surface *screen);
	void checkBubbleCollisions(Bubble* activeBubble);
	void freeSurfaces();
	/*void move(bool &hasCollided);
	bool checkCollisions(Bubble otherBubble);
	void update(SDL_Surface *screen, int x, int y);*/
	//void rotate(Orientation dir);

	//Getters

	/*SDL_Surface* getSpriteSheet();
	SDL_Rect getPosition();
	int getWidth();
	int getHeight();
	int getColor();
	int getVelocityX();
	int getVelocityY();
	bool getInGameStatus();
	Circle getHitbox();*/

};