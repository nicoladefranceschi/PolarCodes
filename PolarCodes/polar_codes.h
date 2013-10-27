//
//  polar_codes.h
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#ifndef PolarCodes_polar_codes_h
#define PolarCodes_polar_codes_h

#include <stdlib.h>


#define MIN_TRANSITION_PROBABILITY (1.0 / (double)(1 << 30))
#define MAX_TRANSITION_PROBABILITY ((double)(1 << 30))


typedef u_int8_t Bit;


void random_bits(Bit *bits, u_int64_t N);
void random_bits_f(Bit *bits, Bit *A, u_int64_t N);

void print_bits(Bit *bits, u_int64_t N);
void print_doubles(double *doubles, u_int64_t N);

u_int64_t bit_reverse_index(u_int64_t index, u_int64_t n);

#endif
