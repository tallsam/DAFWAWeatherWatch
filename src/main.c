#include <pebble.h>
  
#define KEY_STATIONNAME 0
#define KEY_STATIONCODE 1
#define KEY_AIRTEMP 2
#define KEY_FEELSLIKE 3
#define KEY_HUMIDITY 4
#define KEY_WINDDIRECTION 5
#define KEY_WINDSPEED 6
#define KEY_DEWPOINT 7
#define KEY_SOILTEMP 8
#define KEY_RAINFALL9AM 9
  
static Window *s_main_window;

static GFont s_time_font;
static GFont s_weather_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static TextLayer *s_time_layer;
static TextLayer *s_time_layer;
static TextLayer *s_stationname_layer;
static TextLayer *s_stationcode_layer;
static TextLayer *s_airtemp_layer;
static TextLayer *s_feelslike_layer;
static TextLayer *s_humidity_layer;
static TextLayer *s_winddirection_layer;
static TextLayer *s_windspeed_layer;
static TextLayer *s_dewpoint_layer;
static TextLayer *s_soiltemp_layer;
static TextLayer *s_rainfall9am_layer;

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
  //bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Fonts
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  
  //Position layers
  s_time_layer           = text_layer_create(GRect(5,2,139,50));
  s_stationname_layer    = text_layer_create(GRect(5,50,144,20));
  s_airtemp_layer        = text_layer_create(GRect(5,70,70,20));
  s_rainfall9am_layer    = text_layer_create(GRect(90,70,54,20));
  s_winddirection_layer  = text_layer_create(GRect(5,90,144,20));
  s_windspeed_layer      = text_layer_create(GRect(90,90,70,20));
  
  s_dewpoint_layer       = text_layer_create(GRect(0,120,144,20));  
  s_feelslike_layer      = text_layer_create(GRect(0,100,144,25));
  s_humidity_layer       = text_layer_create(GRect(0,125,144,25));
  s_stationcode_layer    = text_layer_create(GRect(0,75,144,25));
  
  // Configure time TextLayer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Configure stationname TextLayer
  text_layer_set_background_color(s_stationname_layer, GColorClear);
  text_layer_set_text_color(s_stationname_layer, GColorBlack);
  text_layer_set_text_alignment(s_stationname_layer, GTextAlignmentCenter);
  text_layer_set_text(s_stationname_layer, "?");
  text_layer_set_font(s_stationname_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_stationname_layer));
  
  // Configure stationcode TextLayer
  /*
  text_layer_set_background_color(s_stationcode_layer, GColorClear);
  text_layer_set_text_color(s_stationcode_layer, GColorBlack);
  text_layer_set_text_alignment(s_stationcode_layer, GTextAlignmentCenter);
  text_layer_set_text(s_stationcode_layer, "?");
  text_layer_set_font(s_stationcode_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_stationcode_layer));
  */
  
  // Configure airtemp TextLayer
  text_layer_set_background_color(s_airtemp_layer, GColorClear);
  text_layer_set_text_color(s_airtemp_layer, GColorBlack);
  text_layer_set_text_alignment(s_airtemp_layer, GTextAlignmentCenter);
  text_layer_set_text(s_airtemp_layer, "?");
  text_layer_set_font(s_airtemp_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_airtemp_layer));
  
  // Configure feelslike TextLayer
  /*
  text_layer_set_background_color(s_feelslike_layer, GColorClear);
  text_layer_set_text_color(s_feelslike_layer, GColorBlack);
  text_layer_set_text_alignment(s_feelslike_layer, GTextAlignmentCenter);
  text_layer_set_text(s_feelslike_layer, "?");
  text_layer_set_font(s_feelslike_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_feelslike_layer));
  */
  
  // Configure humidity TextLayer
  /*
  text_layer_set_background_color(s_humidity_layer, GColorClear);
  text_layer_set_text_color(s_humidity_layer, GColorBlack);
  text_layer_set_text_alignment(s_humidity_layer, GTextAlignmentCenter);
  text_layer_set_text(s_humidity_layer, "?");
  text_layer_set_font(s_humidity_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_humidity_layer));
  */
  
  // Configure winddirection TextLayer
  text_layer_set_background_color(s_winddirection_layer, GColorClear);
  text_layer_set_text_color(s_winddirection_layer, GColorBlack);
  text_layer_set_text_alignment(s_winddirection_layer, GTextAlignmentLeft);
  text_layer_set_text(s_winddirection_layer, "?");
  text_layer_set_font(s_winddirection_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_winddirection_layer));
  
  // Configure windspeed TextLayer
  text_layer_set_background_color(s_windspeed_layer, GColorClear);
  text_layer_set_text_color(s_windspeed_layer, GColorBlack);
  text_layer_set_text_alignment(s_windspeed_layer, GTextAlignmentLeft);
  text_layer_set_text(s_windspeed_layer, "?");
  text_layer_set_font(s_windspeed_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_windspeed_layer));
  
  // Configure dewpoint TextLayer
  /*
  text_layer_set_background_color(s_dewpoint_layer, GColorClear);
  text_layer_set_text_color(s_dewpoint_layer, GColorBlack);
  text_layer_set_text_alignment(s_dewpoint_layer, GTextAlignmentCenter);
  text_layer_set_text(s_dewpoint_layer, "?");
  text_layer_set_font(s_dewpoint_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dewpoint_layer));
  */
  
  // Configure soiltemp TextLayer
  /*
  s_soiltemp_layer = text_layer_create(GRect(0,130,144,25));
  text_layer_set_background_color(s_soiltemp_layer, GColorClear);
  text_layer_set_text_color(s_soiltemp_layer, GColorBlack);
  text_layer_set_text_alignment(s_soiltemp_layer, GTextAlignmentCenter);
  text_layer_set_text(s_soiltemp_layer, "?");
  text_layer_set_font(s_soiltemp_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_soiltemp_layer));
  */
  
  // Configure rainfall9am TextLayer
  text_layer_set_background_color(s_rainfall9am_layer, GColorClear);
  text_layer_set_text_color(s_rainfall9am_layer, GColorBlack);
  text_layer_set_text_alignment(s_rainfall9am_layer, GTextAlignmentRight);
  text_layer_set_text(s_rainfall9am_layer, "?");
  text_layer_set_font(s_rainfall9am_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_rainfall9am_layer));
  
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
  fonts_unload_custom_font(s_weather_font);
  
  // Destroy Textlayers.
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_stationname_layer);
  text_layer_destroy(s_stationcode_layer);
  text_layer_destroy(s_airtemp_layer);
  text_layer_destroy(s_feelslike_layer);
  text_layer_destroy(s_humidity_layer);
  text_layer_destroy(s_winddirection_layer);
  text_layer_destroy(s_windspeed_layer);
  text_layer_destroy(s_dewpoint_layer);
  text_layer_destroy(s_soiltemp_layer);
  text_layer_destroy(s_rainfall9am_layer);
}


/**
 * Callback to update the time.
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Update the time
  update_time();
  
  // Get weather update every 30 minutes  
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
  
    // Send the message!
    app_message_outbox_send();
  }
}

/**
 * Inbox received callback
 */
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char stationname_buffer[64];
  static char stationcode_buffer[32];
  static char airtemp_buffer[32];
  static char feelslike_buffer[32];
  static char humidity_buffer[32];
  static char winddirection_buffer[32];
  static char windspeed_buffer[32];
  static char dewpoint_buffer[32];
  static char soiltemp_buffer[32];
  static char rainfall9am_buffer[32];
  // Read first item
  Tuple *t = dict_read_first(iterator);
  while (t != NULL) {
    switch(t->key) {
      case KEY_STATIONNAME:
        snprintf(stationname_buffer, sizeof(stationname_buffer), "%s", t->value->cstring);
        text_layer_set_text(s_stationname_layer, stationname_buffer);
        break;
      case KEY_STATIONCODE:
        snprintf(stationcode_buffer, sizeof(stationcode_buffer), "%s", t->value->cstring);
        //text_layer_set_text(s_stationcode_layer, stationcode_buffer);
        break;
      case KEY_AIRTEMP:
        snprintf(airtemp_buffer, sizeof(airtemp_buffer), "%s°C", t->value->cstring);
        text_layer_set_text(s_airtemp_layer, airtemp_buffer);
        break;
      case KEY_FEELSLIKE:
        snprintf(feelslike_buffer, sizeof(feelslike_buffer), "Feels like: %s°C", t->value->cstring);
        //text_layer_set_text(s_feelslike_layer, feelslike_buffer);
        break;
      case KEY_HUMIDITY:
        snprintf(humidity_buffer, sizeof(humidity_buffer), "Humidity: %s%%", t->value->cstring);
        //text_layer_set_text(s_humidity_layer, humidity_buffer);
        break;
      case KEY_WINDDIRECTION:
        snprintf(winddirection_buffer, sizeof(winddirection_buffer), "%s", t->value->cstring);
        text_layer_set_text(s_winddirection_layer, winddirection_buffer);
        break;
      case KEY_WINDSPEED:
        snprintf(windspeed_buffer, sizeof(windspeed_buffer), "%skm/h", t->value->cstring);
        text_layer_set_text(s_windspeed_layer, windspeed_buffer);
        break;
      case KEY_DEWPOINT:
        snprintf(dewpoint_buffer, sizeof(dewpoint_buffer), "Dewpoint: %s°C", t->value->cstring);
        //text_layer_set_text(s_dewpoint_layer, dewpoint_buffer);
        break;
      case KEY_SOILTEMP:
        snprintf(soiltemp_buffer, sizeof(soiltemp_buffer), "Soiltemp: %s°C", t->value->cstring);
        //text_layer_set_text(s_soiltemp_layer, soiltemp_buffer);
        break;
      case KEY_RAINFALL9AM:
        snprintf(rainfall9am_buffer, sizeof(rainfall9am_buffer), "%smm", t->value->cstring);
        text_layer_set_text(s_rainfall9am_layer, rainfall9am_buffer);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognised",(int)t->key);
        break;
    }
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
