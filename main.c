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

#include "modules/board.h"
#include "modules/print_boards.h"
#include "modules/random.h"
#include "modules/register_access.h"
#include "modules/term_control.h"
#include "modules/timer.h"

board player;
board bot;
cursor_position cursor;

int difficulty = 2;

uint8_t print_pls = 0;
uint8_t check_game_over = 0;
int victory = 0;

void bot_scedule_next_shot() {
  const int tenth_sec = 50;
  int rand = math_mod(rng_getRandomValue_immediately(), 4);
  timer0_CC0_set_delay((rand + 1) * tenth_sec * difficulty);
}

void bot_action() {
  uint8_t temp = board_bot_shoot(&player);
  print_pls = temp;
  check_game_over = (temp == 4);
  bot_scedule_next_shot();
}

int main(void) {
  // first print some ascii-art, so the player knows stuff is happening
  term_init(0, 0);
  term_clear_screen();
  term_println("//start-ascii-art");

  // init all the shit
  rng_init();
  timer_init(TIMER0, 15, 3, 2397, bot_action);
  bot_scedule_next_shot();

  // place the ships an both boards
  board_place_ships(&player);
  board_place_ships(&bot);

  // now that everything is ready we tell the player to start the game
  term_print("\n\nselect dificulty;\n[1] = hard\n[2] = medim (default)\n[3] = easy");
  while (1) {
    char c = term_stdin_read();
    if (c >= '1' && c <= '3') {
      difficulty = c - '0';
    }
    if (c) {
      break;
    }
  }

  // start the game
  term_clear_screen();
  print_boards(&player, &bot, &cursor);
  timer_start(TIMER0);

  while (1) {
    // read user-input
    char inp = term_stdin_read();
    if (inp) {
      int state = cursor_parse_input(&bot, &cursor, inp);
      print_pls = state;
      check_game_over = (state == 4);
      term_stdin_clear();
    }

    // print board if needed
    if (print_pls) {
      print_boards(&player, &bot, &cursor);
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

  // get eveything ready for ascii-art
  timer_stop(TIMER0);
  term_clear_screen();

  // do the ascii-art
  switch (victory) {
  case 1:
    term_println("//won-ascii-art");
    break;
  case 0:
    term_println("//lost-ascii-art");
    break;
  }

  // wait for user to continue to statistics
  term_print("\n\n[press 'q' to continue to the statistics]");
  while (term_stdin_read() != 'q') {
  }

  // print statistics
  term_clear_screen();
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

  misses = board_stats(&player, MISS);
  hits = board_stats(&player, HIT);
  term_print("\n\nBot stats:");
  term_print("\n misses: ");
  term_printnr(misses);
  term_print("\n hits: ");
  term_printnr(hits);
  term_print("\n shots: ");
  term_printnr(misses + hits);

  return 0;
}
