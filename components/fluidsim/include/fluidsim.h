#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

__BEGIN_DECLS

extern esp_err_t fluidsim_init(void);
void fluidsim_step(float accel_x, float accel_y, float accel_z);

__END_DECLS

#endif // FLUIDSIM_H
