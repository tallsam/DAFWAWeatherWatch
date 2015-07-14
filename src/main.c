#include <pebble.h>
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
  
static Window *s_main_window;
static TextLayer *s_time_layer;

static GFont s_time_font;
static GFont s_weather_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static TextLayer *s_time_layer;
static TextLayer *s_weather_layer;

/**
 * Updates the time layer.
 */
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

/**
 * Load the main window.
 */
static void main_window_load(Window *window) {
  //Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
 
  // Create weather TextLayer
  s_weather_layer = text_layer_create(GRect(0,130,144,25));
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading");
  text_layer_set_font(s_weather_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

/**
 * Clean up when the main window is unloaded.
 */
static void main_window_unload(Window *window) {
  //Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // Destroy time TextLayer
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_time_layer);
  
  // Destroy weather Textlayer.
  fonts_unload_custom_font(s_weather_font);
  text_layer_destroy(s_weather_layer);
}

/**
 * Callback to update the time.
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

/**
 * Inbox received callback
 */
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  // Read first item
  Tuple *t = dict_read_first(iterator);
  while (t != NULL) {
    switch(t->key) {
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognised",(int)t->key);
        break;
    }
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
    t = dict_read_next(iterator);
  }
}

/**
 * Inbox dropped callback
 */
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped");
}

/**
 * Outbox failed callback
 */
static void outbox_failed_callback(DictionaryIterator *iterator,  AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!")  ;
}

/**
 * Outbox sent callback.
 */
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/**
 * Initialise the watchface.
 */
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register message callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);

  // Open app message.
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
}

/**
 * Destroy the watchface.
 */
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

/**
 * Main.
 */
int main(void) {
  init();
  app_event_loop();
  deinit();
}
