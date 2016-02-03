/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "graphics.h"
#include "bluetooth.h"
#include "gps.h"

int main()
{
  printf("HELLO\nHOW ARE YOU\n");

  init_gps();
  init_btport();

  return 0;
}
