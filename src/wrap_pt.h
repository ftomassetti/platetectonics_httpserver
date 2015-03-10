#ifndef WRAP_PT_H
#define WRAP_PT_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void *  platec_api_create(
    long seed,
    size_t width,
    size_t height,
    float sea_level,
    size_t erosion_period, float folding_ratio,
    size_t aggr_overlap_abs, float aggr_overlap_rel,
    size_t cycle_count, size_t num_plates);

void    platec_api_destroy(void*);
const size_t* platec_api_get_agemap(size_t);
float* platec_api_get_heightmap(void*);
size_t* platec_api_get_platesmap(void*);
size_t  platec_api_is_finished(void*);
void    platec_api_step(void*);

#ifdef __cplusplus
}
#endif

#endif
