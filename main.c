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
#include "modules/term_control.h"
#include "modules/timer.h"

board player;
board bot;
cursor_position cursor;

uint8_t print_pls = 0;

void bot_action() {
  board_bot_shoot(&player);
  print_pls = 8;
}

int main(void) {
  // init all the shit
  term_init(0, 0);
  rng_init();
  timer_init(TIMER0, 15, 3, 100, bot_action);

  // place the ships an both boards
  board_place_ships(&player);
  board_place_ships(&bot);

  // start the game
  term_clear_screen();
  print_boards(&player, &bot, &cursor);
  timer_start(TIMER0);

  while (1) {
    // read user-input
    char inp = term_stdin_read();
    if (inp) {
      print_pls = cursor_parse_input(&bot, &cursor, inp);
      term_stdin_clear();
    }

    // print board if needed
    if (print_pls) {
      print_boards(&player, &bot, &cursor);
      print_pls = 0;
    }
  }

  return 0;
}
