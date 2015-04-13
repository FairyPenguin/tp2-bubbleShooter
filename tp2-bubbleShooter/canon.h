#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"

using namespace BubbleShooterUtil;

class Canon
{
	private:

		const int MAX_CANON_ANGLE = 60;				//angle maximal où le canon peut aller
		const int MIN_CANON_ANGLE = -60;			//angle minimal où le canon peut aller

		SDL_Surface *originalSprite;				//image originale du canon
		SDL_Surface *displayedSprite;				//image du canon transformée par la fonction rotozoom
		SDL_Rect position;							//position du canon dans l'espace
		int angle;									//angle du canon


	public:

		//orientation (droite ou gauche) du canon
		enum Orientation
		{
			RIGHT,
			LEFT
		};

		//Constructeur et destructeur
		Canon();
		~Canon();

		//Fonctions utilitaires
		void update(SDL_Surface *screen);
		void rotate(Orientation dir);
		int getWidth();
		int getHeight();

		//Getters
		SDL_Rect getPosition();
		int getRotationAngle();

		

};