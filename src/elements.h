Window *window;

TextLayer *hour_1, *hour_2, *minute_1, *minute_2, *date_layer, *date_layer_adv;

BitmapLayer *background_layer;
GBitmap *background_image;

//Battery stuff
Layer *battery_layer;
AppTimer *charge_timer;
bool cancelled = 0;
int battery_percent;
bool invert;
//End battery stuff

bool showing_date = 0;
int minute = 0;
int hour = 0;
int seconds = 0;
bool public_connection = 0;

Layer *circle_layer;

//Animation stuff.
TextLayer *cov_1, *cov_2, *cov_3, *cov_4;
#define COVER_W 31
#define COVER_L 46
	
//"Initializer element is not constant"
//Like fuck off
	
GRect final_1, final_2, final_3, final_4;
GRect initial_1, initial_2, initial_3, initial_4;