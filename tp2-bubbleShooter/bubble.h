#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"

using namespace BubbleShooterUtil;

class Bubble
{
private:

	SDL_Surface *spriteSheet;			//charset des bulles
	SDL_Rect sprite[3];					//rectangle des bulles
	SDL_Rect position;					//position de la bulle dans l'espace
	int color;							//couleur de la bulle (0 - vert, 1 - rouge, 2 - jaune)


public:

	//Constructeur et destructeur
	Bubble(int colorValue);
	~Bubble();

	void update(SDL_Surface *screen, int x, int y);
	//void rotate(Orientation dir);

	//Getters
	int getColor();

	//Setters
	void setColor(int color);

};