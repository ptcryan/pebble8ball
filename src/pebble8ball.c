#include <pebble.h>

Window *window;
TextLayer *text_layer;
//InverterLayer *inverter_layer;
bool question_asked = false;

void ask_question(void);

const char *answers[20] = {
  "It is certain",
  "It is decidedly so",
  "Without a doubt",
  "Yes definitely",
  "You may rely on it",
  "As I see it, yes",
  "Most likely",
  "Outlook good",
  "Yes",
  "Signs point to yes",
  "Reply hazy try again",
  "Ask again later",
  "Better not tell you now",
  "Cannot predict now",
  "Concentrate and ask again",
  "Don't count on it",
  "My reply is no",
  "My sources say no",
  "Outlook not so good",
  "Very doubtful"
};

int response = 0;

void handle_tap(AccelAxisType axis, int32_t direction) {
  if (true == question_asked) {
    //layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(inverter_layer));

    // tap occured. start 8-ball
    response = (int)(rand() % 20);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Random number generated: %d", response);
    text_layer_set_text_color(text_layer, GColorWhite);
    text_layer_set_background_color(text_layer, GColorBlack);
    text_layer_set_text(text_layer, answers[response]);
    question_asked = false;
  } else {
    ask_question();
  }
}

void ask_question(void) {
  // Set the text, font, and text alignment
  text_layer_set_text(text_layer, "Ask your question and tap your pebble.");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  question_asked = 1;
}

void handle_init(void) {
  // Create a window and text layer
  window = window_create();
  text_layer = text_layer_create(GRect(0, 0, 144, 154));
  //inverter_layer = inverter_layer_create(GRect(0, 0, 144, 154));
  
  // seed the random number generator
  srand(time(NULL));

  accel_tap_service_subscribe(handle_tap);
  
  // Add the text layer to the window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

  // Push the window
  window_stack_push(window, true);
  
  // Prompt the user
  ask_question();
}

void handle_deinit(void) {
  // unsubscribe from tap events
  accel_data_service_unsubscribe();
  
  // Destroy the text layer
  text_layer_destroy(text_layer);
  //inverter_layer_destroy(inverter_layer);
  
  // Destroy the window
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}