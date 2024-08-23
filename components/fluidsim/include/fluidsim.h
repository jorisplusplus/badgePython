#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

__BEGIN_DECLS

esp_err_t fluidsim_init(void);
void fluidsim_begin(int num_particles, int colour);

uint16_t fluidsim_num_particles(void);
bool fluidsim_has_particle(uint16_t x, uint16_t y);
void fluidsim_push_particle(void);
void fluidsim_pop_particle(void);

__END_DECLS

#endif // FLUIDSIM_H
