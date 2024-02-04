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

#include "modules/register_access.h"

int main(void) {
  uint32_t value = register_read(0x20000000);

  value = ~value;

  register_write(0x20000000, value);

  return 0;
}
