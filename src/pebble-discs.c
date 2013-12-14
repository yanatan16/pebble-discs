#include <pebble.h>
#include <math.h>

static const uint16_t CENTER_DISC_RADIUS = 20;
static const uint16_t HOUR_DISC_RADIUS = 10;
static const uint16_t MINUTE_DISC_RADIUS = 5;

static const uint16_t HOUR_ROT_RADIUS = 40;
static const uint16_t MINUTE_ROT_RADIUS = 60;

static Window *window;
static Layer *Center;
static Layer *Hour;
static Layer *Minute;

static uint16_t MinuteLocation = 0;
static uint16_t HourLocation = 0;

#define TAU 6.283185307179586

// Calculate a center point for any disc with a given radius and clock location (a number from 0 to 359 representing 1 degree each)
GPoint calculate_center_point(uint16_t radius, uint16_t location) {
  double radians = ((double) location) / TAU;
  GPoint point = {
    72 - radius * sin(radians),
    84 - radius * cos(radians)
  };
  return point;
}

uint16_t time_to_hour_location(struct tm *tick_time) {
  return tick_time->tm_hour * 30 + tick_time->tm_min / 2;
}

uint16_t time_to_minute_location(struct tm *tick_time) {
  return tick_time->tm_min * 6 + tick_time->tm_sec / 10;
}

static void center_update_proc(Layer *layer, GContext *ctx) {
  graphics_draw_circle(ctx, calculate_center_point(0,0), CENTER_DISC_RADIUS);
}

static void hour_update_proc(Layer *layer, GContext *ctx) {
  graphics_draw_circle(ctx, calculate_center_point(HOUR_ROT_RADIUS, HourLocation), HOUR_DISC_RADIUS);
}

static void minute_update_proc(Layer *layer, GContext *ctx) {
  graphics_draw_circle(ctx, calculate_center_point(MINUTE_ROT_RADIUS, MinuteLocation), MINUTE_DISC_RADIUS);
}

static void tick_minute_handler(struct tm *tick_time, TimeUnits units_changed) {
  HourLocation = time_to_hour_location(tick_time);
  MinuteLocation = time_to_minute_location(tick_time);

  layer_mark_dirty(Hour);
  layer_mark_dirty(Minute);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  Center = layer_create(bounds);
  Hour = layer_create(bounds);
  Minute = layer_create(bounds);

  layer_set_update_proc(Center, center_update_proc);
  layer_set_update_proc(Hour, hour_update_proc);
  layer_set_update_proc(Minute, minute_update_proc);
}

static void window_unload(Window *window) {
  layer_destroy(Center);
  layer_destroy(Hour);
  layer_destroy(Minute);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_minute_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
