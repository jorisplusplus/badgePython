#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sdkconfig.h"
#include "sntp.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "rom/crc.h"

#include <esp_log.h>
#include <esp_err.h>

#include "include/fluidsim.h"
#include "driver_hub75.h"

#define TAG "fluidsim"

#include <stdint.h>
#include <math.h>
#include <string.h>

#define WIDTH 32
#define HEIGHT 19
#define MAX_PARTICLES 100

typedef struct {
  float x;
  float y;
  float vx;
  float vy;
  float density;
  float pressure;
} Particle;

Particle particles[MAX_PARTICLES];
int num_particles = 0;

// Constants for the SPH algorithm
const float rest_density = 1000.0f;
const float stiffness = 2000.0f;
const float epsilon = 0.1f; // Small number to make fix low density causing jumps
const float max_velocity = 2.0f;
const float viscosity = 250.0f;
const float gravity_strength = 9.81f;
const float h = 1.0f;  // Smoothing length
const float h2 = 1.0f; // h squared

typedef struct {
  float x;
  float y;
  float z;
} accel_data_t;

// Add a particle to the system
void AddParticle(float x, float y) {
  if (num_particles < MAX_PARTICLES) {
    particles[num_particles].x = x;
    particles[num_particles].y = y;
    particles[num_particles].vx = 0.0f;
    particles[num_particles].vy = 0.0f;
    num_particles++;
  }
}

// Initialize the SPH simulation
void InitSimulation() {
  memset(particles, 0, sizeof(particles));

  // Add some particles to the system
  for (int i = 10; i < 22; i++) {
    AddParticle(i, 0.5f);
  }
}

// Compute the density and pressure for each particle
void ComputeDensityPressure() {
  for (int i = 0; i < num_particles; i++) {
    Particle *p = &particles[i];
    p->density = 0.0f;

    for (int j = 0; j < num_particles; j++) {
      Particle *pj = &particles[j];
      float dx = p->x - pj->x;
      float dy = p->y - pj->y;
      float r2 = dx * dx + dy * dy;

      if (r2 < h2) {
        p->density += 315.0f / (64.0f * M_PI * pow(h, 9)) * pow(h2 - r2, 3);
      }
    }
    p->pressure = stiffness * (p->density - rest_density);
  }
}

// Compute forces for each particle
void ComputeForces(accel_data_t *gravity) {
  for (int i = 0; i < num_particles; i++) {
    Particle *p = &particles[i];
    float fx = 0.0f;
    float fy = 0.0f;

    for (int j = 0; j < num_particles; j++) {
      if (i == j) continue;

      Particle *pj = &particles[j];
      float dx = p->x - pj->x;
      float dy = p->y - pj->y;
      float r2 = dx * dx + dy * dy;

      if (r2 < h2 && r2 > 0.0f) {
        float r = sqrtf(r2);
        float q = r / h;

        // Pressure force
        float pressure_term =
          -45.0f / (M_PI * pow(h, 6)) * (p->pressure + pj->pressure) / (2.0f * pj->density) * pow(1.0f - q, 2);
        fx += pressure_term * dx / r;
        fy += pressure_term * dy / r;

        // Viscosity force
//        float viscosity_term = 45.0f / (M_PI * pow(h, 6)) * (pj->vx - p->vx) * (1.0f - q);
//        fx += viscosity_term * dx / r;
//        fy += viscosity_term * dy / r;
      }
    }

    // Gravity force
    fx += gravity->x * p->density;
    fy += gravity->y * p->density;

    // Update velocity
    p->vx += fx / (p->density + epsilon);
    p->vy += fy / (p->density + epsilon);
  }
}

// Integrate the positions of the particles
void Integrate(float dt) {
  for (int i = 0; i < num_particles; i++) {
    Particle *p = &particles[i];

    p->x += p->vx * dt;
    p->y += p->vy * dt;

    if (p->vx > max_velocity) p->vx = max_velocity;
    if (p->vy > max_velocity) p->vy = max_velocity;

    // Boundary conditions (reflective)
    if (p->x < 0.0f) {
      p->x = 0.0f;
      p->vx *= -0.5f;  // Dampen the velocity
    } else if (p->x > WIDTH - 1) {
      p->x = WIDTH - 1;
      p->vx *= -0.5f;
    }

    if (p->y < 0.0f) {
      p->y = 0.0f;
      p->vy *= -0.5f;
    } else if (p->y > HEIGHT - 1) {
      p->y = HEIGHT - 1;
      p->vy *= -0.5f;
    }
  }
}

// Render the fluid simulation (For demonstration purposes, assuming some display function exists)
void RenderSimulation(void) {
  // Clear the screen
  compositor_clear();

  compositor_disable();
  // Draw the particles
  for (int i = 0; i < num_particles; i++) {
    int x = (int) particles[i].x;
    int y = (int) particles[i].y;

    ESP_LOGI(TAG, "Moving part %d to (%d, %d)", i, x, y);

    Color *val = malloc(sizeof(Color));
    val->value = 0x00FFFF00;

    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
      compositor_addImage(val, x, y, 1, 1);  // Draw the particle
    }
  }
  compositor_enable();
}

void fluidsim_step(float accel_x, float accel_y, float accel_z) {
  accel_data_t gravity = {accel_x, accel_y, accel_z};
  ComputeDensityPressure();
  ComputeForces(&gravity);
  Integrate(0.01f);  // Fixed time step
  RenderSimulation();
}

esp_err_t fluidsim_init(void) {
  InitSimulation();
  return ESP_OK;
}
