//
//  pc_decode.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pc_decode.h"
#include "pc_decode_internal.h"


void pc_decode_step(Bit *u_, Bit *A, u_int64_t n, u_int64_t step, double *val, u_int64_t *uindex);


void pc_decode(Bit *u_, Bit *y, const Channel *channel, Bit *A, u_int64_t n) {
    
    u_int64_t N = 1 << n;
    u_int64_t uindex = 0;
    
    double *delta = malloc(N*sizeof(double));
    u_int64_t i;
    for(i = 0; i < N; i++){
        
        u_int64_t i2 = bit_reverse_index(i, n);
        double n,d,v;
        
        n = channel_probability(channel, y[i2], 0);
        d = channel_probability(channel, y[i2], 1);
        
        if(d == 0){
            v = MAX_TRANSITION_PROBABILITY;
        }else{
            v = n/d;
            if(v < MIN_TRANSITION_PROBABILITY){
                v = MIN_TRANSITION_PROBABILITY;
            }else if(v > MAX_TRANSITION_PROBABILITY){
                v = MAX_TRANSITION_PROBABILITY;
            }
        }
        
        delta[i] = v;
        
    }
    
    pc_decode_step(u_, A, n, 1, delta, &uindex);
    
    free(delta);
    
}

void pc_decode_update_u_(Bit *u_, Bit *A, double v, u_int64_t *uindex){
    
    if(A[*uindex] == 0){
        u_[*uindex] = 0;
    }else if(v >= 1){
        u_[*uindex] = 0;
    }else{
        u_[*uindex] = 1;
    }
    
    (*uindex)++;
}

void pc_decode_step(Bit *u_, Bit *A, u_int64_t n, u_int64_t step, double *val, u_int64_t *uindex){
    
    u_int64_t D = 1 << (n-step);
    double *new_val = malloc(sizeof(double)*D);
    
    //calcolate f
    pc_decode_calc_f(new_val, val, step, n);
    
    if(step == n){
        pc_decode_update_u_(u_, A, new_val[0], uindex);
    }else{
        pc_decode_step(u_, A, n, step+1, new_val, uindex);
    }
    
    
    //calcolate f
    pc_decode_calc_g(new_val, val, step, n, u_, *uindex);
    
    if(step == n){
        pc_decode_update_u_(u_, A, new_val[0], uindex);
    }else{
        pc_decode_step(u_, A, n, step+1, new_val, uindex);
    }
    
    free(new_val);
}


