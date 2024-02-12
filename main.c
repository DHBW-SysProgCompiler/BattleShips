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
#include "modules/print_boards.h"
#include "modules/term_control.h"

void empty_board(char board[]) {
  for (int i = 0; i < 100; i++) {
    board[i] = ' ';
  }
}

int main(void) {
  char player[100];
  char bot[100];
  empty_board(player);
  empty_board(bot);
  term_init(0, 0);
  term_clear_screen();
  print_boards(player, bot, 5, 6);
  return 0;
}
