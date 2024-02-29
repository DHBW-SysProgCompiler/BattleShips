/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Eric Erath
 *
 * @brief *Main* Entry
 **/

#pragma region includes

#include "modules/asciiart.h"
#include "modules/board.h"
#include "modules/print_boards.h"
#include "modules/random.h"
#include "modules/register_access.h"
#include "modules/term_control.h"
#include "modules/timer.h"

#pragma endregion

#pragma region globs

board player;
board bot;
cursor_position cursor;

int difficulty = 2;

uint8_t print_pls = 0;
uint8_t check_game_over = 0;
int victory = 0;

#pragma endregion

#pragma region support_methods

// set the timer for the next shot of the bot
void bot_scedule_next_shot() {
  const int tenth_sec = 50;
  int rand = math_mod(rng_getRandomValue_immediately(), 4);
  timer0_CC0_set_delay((rand + 1) * tenth_sec * difficulty);
}

// methot to be called by the timer-interrupt
void bot_action() {
  uint8_t temp = board_bot_shoot(&player);
  print_pls = temp;
  check_game_over = (temp == 4);
  bot_scedule_next_shot();
}

// wraps all nessecasy inits and setups the boards
void main_init() {
  // init all the shit
  term_init(0, 0);
  rng_init();
  timer_init(TIMER0, 15, 3, 2397, bot_action);
  bot_scedule_next_shot();

  // first print some ascii-art, so the player knows stuff is happening
  term_clear_screen();
  art_print_main();

  // place the ships an both boards
  board_place_ships(&player);
  board_place_ships(&bot);
}

// interupt execution until dificulty is selected
void select_dificulty() {
  term_print("\n\nselect difficulty;\n[1] = hard\n[2] = medim (default)\n[3] = easy");
  while (1) {
    char c = term_stdin_read();
    if (c >= '1' && c <= '3') {
      difficulty = c - '0';
    }
    if (c) {
      break;
    }
  }
}

// main gameplay loop
void main_game_loop() {

  term_clear_screen();
  print_boards(&player, &bot, &cursor);
  timer_start(TIMER0);

  while (1) {
    // read user-input until something else has to be done
    while (!print_pls && !check_game_over) {
      char inp = term_stdin_read();
      if (inp) {
        int state = cursor_parse_input(&bot, &cursor, inp);
        print_pls = state;
        check_game_over = (state == 4);
      }
    }
    term_stdin_clear();

    // print board if needed
    if (print_pls) {
      update_boards(&player, &bot, &cursor);
      print_pls = 0;
    }

    // check for game_over if needed
    if (check_game_over) {
      if (board_stats(&bot, SHIP) == 0) {
        victory = 1;
        break;
      }
      if (board_stats(&player, SHIP) == 0) {
        victory = 0;
        break;
      }
      check_game_over = 0;
    }
  }
  timer_stop(TIMER0);
}

// print ascii-art reporting win or loss
void print_victoryscreen() {
  term_clear_screen();
  switch (victory) {
  case 1:
    art_print_victory();
    break;
  case 0:
    art_print_loss();
    break;
  }
}

// wait for user to continue to statistics, and print
void print_stats() {
  // wait for user to continue to statistics
  term_print("\n[press 'q' to continue to the statistics]");
  while (term_stdin_read() != 'q') {
  }

  // print statistics
  term_clear_screen();
  print_boards(&player, &bot, &cursor);
  int misses;
  int hits;
  misses = board_stats(&bot, MISS);
  hits = board_stats(&bot, HIT);
  term_print("\nYour stats:");
  term_print("\n misses: ");
  term_printnr(misses);
  term_print("\n hits: ");
  term_printnr(hits);
  term_print("\n shots: ");
  term_printnr(misses + hits);
  term_println("\n");
  art_print_bargraph(hits, misses);

  misses = board_stats(&player, MISS);
  hits = board_stats(&player, HIT);
  term_print("\n\nBot stats:");
  term_print("\n misses: ");
  term_printnr(misses);
  term_print("\n hits: ");
  term_printnr(hits);
  term_print("\n shots: ");
  term_printnr(misses + hits);
  term_println("\n");
  art_print_bargraph(hits, misses);

  term_print("\n\n[press 'Ctrl+A, X' to terminate QEMU]");
}

#pragma endregion

int main(void) {
  main_init();
  select_dificulty();
  main_game_loop();
  print_victoryscreen();
  print_stats();
  return 0;
}
