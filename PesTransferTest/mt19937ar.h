// mt19937ar.h
#ifndef MT19937AR_H
#define MT19937AR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_by_array(uint32_t init_key[], int key_length);
uint32_t genrand_int32(void);

#ifdef __cplusplus
}
#endif

#endif
