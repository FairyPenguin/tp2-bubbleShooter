#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"
#include "../SDL/SDL_rotozoom.h"

using namespace BubbleShooterUtil;

class Canon
{
	private:

		const int MAX_CANON_ANGLE = 60;
		const int MIN_CANON_ANGLE = -60;
		Sprite *originalSprite;
		Sprite *displayedSprite;
		SDL_Rect position;
		int angle;


	public:

		enum Orientation
		{
			RIGHT,
			LEFT
		};

		Canon();
		~Canon();
		void update(SDL_Surface *screen);
		void rotate(Orientation dir);
		int getWidth();
		int getHeight();
		SDL_Rect getPosition();
		int getRotationAngle();

		

};