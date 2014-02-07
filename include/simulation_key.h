#ifndef __GET_CYCLE_WAVE__
#define __GET_CYCLE_WAVE__

#define END_PIN	193

void get_cycle_wave(unsigned int *pin, unsigned long low_level_time, unsigned long high_level_time);

void simulation_random_key(unsigned int *pin, unsigned long min_time, unsigned long max_time, unsigned int level);
#endif
