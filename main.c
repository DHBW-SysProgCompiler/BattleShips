/**
 * @file
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Thomas Vogt
 * @author Sven Reis
 *
 * @brief *Main* Entry
 **/

#include "modules/board.h"
#include "modules/print_boards.h"
#include "modules/random.h"
#include "modules/term_control.h"

int main(void) {
  term_init(0, 0);
  rng_init();

  struct board b;
  struct cursor_position c;

  board_place_ships(&b);

  term_clear_screen();
  print_boards(&b, &b, &c);
  while (1) {
    char inp = term_stdin_read();
    if (inp) {
      if (cursor_parse_input(&b, &c, inp)) {
        print_boards(&b, &b, &c);
      }
      term_stdin_clear();
    }
  }

  return 0;
}
