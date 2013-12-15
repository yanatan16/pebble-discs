#include <pebble.h>
#include <time.h>

static const int FACE_DISC_RADIUS = 70;
static const int CENTER_DISC_RADIUS = 20;
static const int HOUR_DISC_RADIUS = 10;
static const int MINUTE_DISC_RADIUS = 5;

static const int HOUR_ROT_RADIUS = 40;
static const int MINUTE_ROT_RADIUS = 60;

static const GColor WINDOW_COLOR = GColorBlack;
static const GColor FACE_COLOR = GColorWhite;
static const GColor DISC_COLOR = GColorBlack;

static Window *window;
static Layer *Face;
static Layer *Center;
static Layer *Hour;
static Layer *Minute;

// Calculate a center point for any disc with a given radius and clock location (a number from 0 to 359 representing 1 degree each)
GPoint calculate_center_point(int radius, int32_t angle) {
  GPoint center;

  center.y = 84 + (-cos_lookup(angle) * radius / TRIG_MAX_RATIO);
  center.x = 72 + (sin_lookup(angle) * radius / TRIG_MAX_RATIO);

  return center;
}

int32_t time_to_hour_angle(struct tm *tick_time) {
  return TRIG_MAX_ANGLE * ((tick_time->tm_hour % 12) * 60 + tick_time->tm_min) / 720;
}

int32_t time_to_minute_angle(struct tm *tick_time) {
  return TRIG_MAX_ANGLE * ((tick_time->tm_min * 60) + tick_time->tm_sec) / 3600;
}

static void face_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, calculate_center_point(0,0), FACE_DISC_RADIUS);
}

static void center_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, DISC_COLOR);
  graphics_fill_circle(ctx, calculate_center_point(0,0), CENTER_DISC_RADIUS);
}

static void hour_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  int32_t hour_angle = time_to_hour_angle(t);
  GPoint center = calculate_center_point(HOUR_ROT_RADIUS, hour_angle);

  graphics_context_set_fill_color(ctx, DISC_COLOR);
  graphics_fill_circle(ctx, center, HOUR_DISC_RADIUS);
}

static void minute_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  int32_t minute_angle = time_to_minute_angle(t);
  GPoint center = calculate_center_point(MINUTE_ROT_RADIUS, minute_angle);

  graphics_context_set_fill_color(ctx, DISC_COLOR);
  graphics_fill_circle(ctx, center, MINUTE_DISC_RADIUS);
}

static void tick_minute_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void window_load(Window *window) {
  window_set_background_color(window, WINDOW_COLOR);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = GRect(0, 0, 144, 168);

  Face = layer_create(bounds);
  Center = layer_create(bounds);
  Hour = layer_create(bounds);
  Minute = layer_create(bounds);

  layer_set_update_proc(Face, face_update_proc);
  layer_set_update_proc(Center, center_update_proc);
  layer_set_update_proc(Hour, hour_update_proc);
  layer_set_update_proc(Minute, minute_update_proc);

  layer_add_child(window_layer, Face);
  layer_add_child(Face, Center);
  layer_add_child(Face, Hour);
  layer_add_child(Face, Minute);
}

static void window_unload(Window *window) {
  layer_destroy(Face);
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

  tick_timer_service_subscribe(SECOND_UNIT, tick_minute_handler);
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
