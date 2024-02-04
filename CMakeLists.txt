# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Thomas Vogt

cmake_minimum_required( VERSION 3.22 )

project( arm_cortex_m0_c LANGUAGES ASM C )

# Define all Files:
add_executable( testApp )

target_sources(
  testApp
  PRIVATE
    entry.S
    entry_c.c
    main.c 
    
    # Register Access
    modules/register_access.h
    modules/register_access.c
    
    # Interrupts
    modules/nvic.h
    
    # RNG
    modules/random.h
    modules/random.c

    # Timer
    modules/timer.h
    modules/timer.c

    # UART
    modules/uart.h
    modules/uart.c

    # Heap-Allocations
    modules/heap_allocations.h

    # Sreen-Print
    modules/screen_control.h
    modules/screen_control.c
  )

target_link_options(
  testApp
  PRIVATE
    -Wl,-Map=testApp.map
    -T ${CMAKE_SOURCE_DIR}/linker_script.lds )
set_target_properties(
  testApp
  PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker_script.lds )


#####################################################################
# Post Build Commands:
# - extract binary


# To copy out the binary:
#arm-none-eabi-objcopy -O binary base.elf output.bin

add_custom_command(
  TARGET testApp
  POST_BUILD
  COMMAND "${OBJ_COPY}"
  ARGS -O binary $<TARGET_FILE:testApp> $<TARGET_FILE_BASE_NAME:testApp>.bin )

set_target_properties(
  testApp
  PROPERTIES ADDITIONAL_CLEAN_FILES $<TARGET_FILE_BASE_NAME:testApp>.bin )
