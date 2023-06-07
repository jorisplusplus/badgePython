#ifndef FSOB_BACKEND_H
#define FSOB_BACKEND_H

#include <sdkconfig.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "filefunctions.h"
#include "specialfunctions.h"
#include "messageutils.h"


void fsob_backend_init();

void fsob_backend_write_packet(const void *data);

void fsob_backend_reset();

#endif