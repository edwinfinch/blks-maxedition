/*
BLKS - Max Edition
Edwin Finch

Design by Philipp Weder
Date idea by Max Hostetler
*/

#include <pebble.h>
#include "elements.h"
void animation_callback(void *data);
	
static TextLayer* text_layer_init(GRect location)
{
	TextLayer *layer = text_layer_create(location);
	text_layer_set_text_color(layer, GColorBlack);
	text_layer_set_background_color(layer, GColorClear);
	text_layer_set_text_alignment(layer, GTextAlignmentCenter);
	text_layer_set_font(layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMPACT_48)));
	return layer;
}

void stopped(Animation *anim, bool finished, void *context)
{
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay)
{
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
     
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
     
    AnimationHandlers handlers = {
        .stopped = (AnimationStoppedHandler) stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
     
    animation_schedule((Animation*) anim);
}

void animate(bool cov1, bool cov2, bool cov3, bool cov4, bool boot){
	if(seconds == 59 && boot == 1){
		return;
	}
	if(!boot){
		if(cov1){
			animate_layer(text_layer_get_layer(cov_1), &initial_1, &final_1, 700, 300);
		}
		if(cov2){
			animate_layer(text_layer_get_layer(cov_2), &initial_2, &final_2, 700, 300);
		}
		if(cov3){
			animate_layer(text_layer_get_layer(cov_3), &initial_3, &final_3, 700, 300);
		}
		if(cov4){
			animate_layer(text_layer_get_layer(cov_4), &initial_4, &final_4, 700, 300);
		}
	}
		
	int wait_time;
	if(boot){
		wait_time = 300;
	}
	else{
		wait_time = 1010;
	}
	
	if(cov1){
		animate_layer(text_layer_get_layer(cov_1), &final_1, &initial_1, 700, wait_time);
	}
	if(boot){
		wait_time += 200;
	}
	if(cov2){
		animate_layer(text_layer_get_layer(cov_2), &final_2, &initial_2, 700, wait_time);
	}
	if(boot){
		wait_time += 200;
	}
	if(cov3){
		animate_layer(text_layer_get_layer(cov_3), &final_3, &initial_3, 700, wait_time);
	}
	if(boot){
		wait_time += 200;
	}
	if(cov4){
		animate_layer(text_layer_get_layer(cov_4), &final_4, &initial_4, 700, wait_time);
	}
}

int get_minute_change(){
	/*
	Adaptive minute change means that it will only animate
	the numbers that are changing. Easy fix.
	*/
	int mode = -1;
	int fixmin = minute+1;
	int fixhour = 0;
	bool hourIsDiff = 0;
	if(fixmin == 60){
		fixmin = 0;
		fixhour++;
		if(fixhour > 23){
			fixhour = 0;
		}
		hourIsDiff = 1;
	}
	
	if(hourIsDiff){
		if(hour == 9){
			mode = 4;
		}
		else if(hour == 23){
			mode = 4;
		}
		else{
			mode = 3;
		}
		return mode;
	}
	else{
		if(minute%10 == 9){
			mode = 2;
		}
		else{
			mode = 1;
		}
		return mode;
	}
}

void tick_handler(struct tm *t, TimeUnits units_changed){
	minute = t->tm_min;
	hour = t->tm_hour;
	seconds = t->tm_sec;
	static char min_1_buf[] = "1";
	static char min_2_buf[] = "2";
	static char hour_1_buf[] = "1";
	static char hour_2_buf[] = "2";
	static char date_buf[] = "Thu.";
	static char date_adv_buf[] = "08";
	
	int fixmin1 = minute/10;
	int fixmin2 = minute%10;
	int fixhour1, fixhour2;
	if(clock_is_24h_style()){
		fixhour1 = hour/10;
		fixhour2 = hour%10;
	}
	else{
		if(hour > 12){
			hour -= 12;
		}
		fixhour1 = hour/10;
		fixhour2 = hour%10;
	}
	
	snprintf(min_1_buf, sizeof(min_1_buf), "%d", fixmin1);
	snprintf(min_2_buf, sizeof(min_2_buf), "%d", fixmin2);
	snprintf(hour_1_buf, sizeof(hour_1_buf), "%d", fixhour1);
	snprintf(hour_2_buf, sizeof(hour_2_buf), "%d", fixhour2);
	
	text_layer_set_text(minute_1, min_1_buf);
	text_layer_set_text(minute_2, min_2_buf);
	text_layer_set_text(hour_1, hour_1_buf);
	text_layer_set_text(hour_2, hour_2_buf);
	
	strftime(date_buf, sizeof(date_buf), "%a", t);
	text_layer_set_text(date_layer, date_buf);
	
	strftime(date_adv_buf, sizeof(date_adv_buf), "%d", t);
	text_layer_set_text(date_layer_adv, date_adv_buf);
	
	if(seconds == 59){
		int mode = get_minute_change();
		switch(mode){
			case 1:
				animate(false, false, false, true, false);
				break;
			case 2:
				animate(false, false, true, true, false);
				break;
			case 3:
				animate(false, true, true, true, false);
				break;
			case 4:
				animate(true, true, true, true, false);
				break;
			//Catch
			default:
				animate(true, true, true, true, false);
				break;
		}
	}
}

void battery_proc(Layer *layer, GContext *ctx){
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_context_set_stroke_color(ctx, GColorWhite);
	int height = 158;
	int circle_radius = 4;
	int k, l;
	for(k = 10; k > 0; k--){
		l = (13*k);
		graphics_draw_circle(ctx, GPoint(l, height), circle_radius);
	}
	
	int i, j;
	for(i = battery_percent/10; i > 0; i--){
		j = (i*13);
		graphics_fill_circle(ctx, GPoint(j, height), circle_radius);
	}
}

void charge_invert(void *data){
	invert = !invert;
	if(invert){
		if(battery_percent != 100){
			battery_percent += 10;
		}
		layer_mark_dirty(battery_layer);
	}
	else{
		if(battery_percent != 0){
			battery_percent -= 10;
		}
		layer_mark_dirty(battery_layer);
	}
	charge_timer = app_timer_register(1000, charge_invert, NULL);
}

void battery_handler(BatteryChargeState charge){
	battery_percent = charge.charge_percent;
	layer_mark_dirty(battery_layer);
	
	if(charge.is_charging){
		cancelled = 0;
		app_timer_cancel(charge_timer);
		charge_timer = app_timer_register(1000, charge_invert, NULL);
	}
	else{
		if(!cancelled){
			app_timer_cancel(charge_timer);
			cancelled = 1;
		}
	}
}

void tap(AccelAxisType axis, int32_t direction){
	if(showing_date){
		layer_set_hidden(text_layer_get_layer(date_layer_adv), false);
		layer_set_hidden(text_layer_get_layer(date_layer), true);
	}
	else{
		layer_set_hidden(text_layer_get_layer(date_layer_adv), true);
		layer_set_hidden(text_layer_get_layer(date_layer), false);
	}
	showing_date = !showing_date;
}
	
void circle_proc(Layer *layer, GContext *ctx){
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_circle(ctx, GPoint(72, 76), 21);
}

void window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	final_1 = GRect(19, 14, COVER_W, COVER_L);
	final_2 = GRect(95, 14, COVER_W, COVER_L);
	final_3 = GRect(19, 90, COVER_W, COVER_L);
	final_4 = GRect(95, 90, COVER_W, COVER_L);

	initial_1 = GRect(19, 14, COVER_W, 1);
	initial_2 = GRect(95, 14, COVER_W, 1);
	initial_3 = GRect(19, 90, COVER_W, 1);
	initial_4 = GRect(95, 90, COVER_W, 1);
	
	background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(background_layer, background_image);
	layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
	
	circle_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(circle_layer, circle_proc);
	layer_add_child(window_layer, circle_layer);
	
	hour_1 = text_layer_init(GRect(22, 8, 28, 50));
	layer_add_child(window_layer, text_layer_get_layer(hour_1));
	
	hour_2 = text_layer_init(GRect(97, 8, 28, 50));
	layer_add_child(window_layer, text_layer_get_layer(hour_2));
	
	minute_1 = text_layer_init(GRect(22, 85, 28, 50));
	layer_add_child(window_layer, text_layer_get_layer(minute_1));
	
	minute_2 = text_layer_init(GRect(97, 85, 28, 50));
	layer_add_child(window_layer, text_layer_get_layer(minute_2));
	
	cov_1 = text_layer_init(final_1);
	text_layer_set_background_color(cov_1, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(cov_1));
	
	cov_2 = text_layer_init(final_2);
	text_layer_set_background_color(cov_2, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(cov_2));
	
	cov_3 = text_layer_init(final_3);
	text_layer_set_background_color(cov_3, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(cov_3));
	
	cov_4 = text_layer_init(final_4);
	text_layer_set_background_color(cov_4, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(cov_4));
	
	date_layer = text_layer_init(GRect(0, 65, 144, 168));
	text_layer_set_font(date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMPACT_18)));
	text_layer_set_text_color(date_layer, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(date_layer));
	
	date_layer_adv = text_layer_init(GRect(0, 62, 144, 168));
	text_layer_set_font(date_layer_adv, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMPACT_24)));
	text_layer_set_text_color(date_layer_adv, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(date_layer_adv));
	
	layer_set_hidden(text_layer_get_layer(date_layer_adv), true);
	
	battery_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(battery_layer, battery_proc);
	layer_add_child(window_layer, battery_layer);
	
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	
	tick_handler(t, SECOND_UNIT);
	
	BatteryChargeState bat = battery_state_service_peek();
	battery_handler(bat);
	
	animate(1, 1, 1, 1, 1);
}

void window_unload(Window *window){
	text_layer_destroy(hour_1);
	text_layer_destroy(hour_2);
	text_layer_destroy(minute_1);
	text_layer_destroy(minute_2);
	bitmap_layer_destroy(background_layer);
	layer_destroy(battery_layer);
}
	
void init(){
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers){
		.load = window_load,
		.unload = window_unload,
	});
	tick_timer_service_subscribe(SECOND_UNIT, &tick_handler);
	battery_state_service_subscribe(battery_handler);
	accel_tap_service_subscribe(&tap);
	
	background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BASE);
	window_stack_push(window, true);
}

void deinit(){
	tick_timer_service_unsubscribe();
	battery_state_service_unsubscribe();
	accel_tap_service_unsubscribe();
	gbitmap_destroy(background_image);
}

int main(){
	init();
	app_event_loop();
	deinit();
}