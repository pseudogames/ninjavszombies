#include "app.h"
#include "data/missile1.h"
#include "data/missile2.h"


void missile_init(App *app) {

	sprite_init(&app->sprite_missile1,
		0, 0, // origin
		16, 16, // frame size
		1, 4, // action, frame counts
		missile1_png, missile1_png_len // source
	);

	sprite_init(&app->sprite_missile2,
		0, 0, // origin
		16, 16, // frame size
		1, 4, // action, frame counts
		missile2_png, missile2_png_len // source
	);

}
