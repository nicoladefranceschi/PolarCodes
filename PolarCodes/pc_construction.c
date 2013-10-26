//
//  pc_construction.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pc_construction.h"
#include "pc_encode.h"
#include "pc_decode_internal.h"

/*
int pc_sort_cmp(const void *x, const void *y)
{
    double xx = *(double*)x, yy = *(double*)y;
    if (xx < yy) return 1;
    if (xx > yy) return -1;
    return 0;
}
*/

void pc_decode_construction_step(double *Z, double *Zvar, Bit *u, u_int64_t n, u_int64_t step, double *val, u_int64_t *uindex);
void pc_decode_construction(double *Z, double *Zvar, Bit *y, const Channel *channel, Bit *u, u_int64_t n);


void pc_construction(double *Z, double *Zvar, u_int64_t n, const Channel *channel, u_int64_t n_samples) {
    
    u_int64_t N = 1 << n;
    
//    memset(Z, 0, sizeof(double)*N);
//    memset(Zvar, 0, sizeof(double)*N);
    
    u_int64_t i;
    Bit *u = malloc(N*sizeof(Bit));
    Bit *x = malloc(N*sizeof(Bit));
    for(i = 0; i < n_samples; i++){
        
        //printf("samples: %d\n",i);
        
        random_bits(u, N);
        
        pc_encode(x, u, n);
        
        Bit *y = x;
        apply_channel(y, x, channel, n);
        
        pc_decode_construction(Z, Zvar, y, channel, u, n);
        
    }
    
    free(u);
    free(x);
    
//    qsort(Z, N, sizeof(double), &pc_sort_cmp);
    
//    [Z,I]=sort(Z_sum,'descend');
  //  A(I(1:N-K))=0;
    
    

}


void pc_decode_construction(double *Z, double *Zvar, Bit *y, const Channel *channel, Bit *u, u_int64_t n) {
    
    u_int64_t N = 1 << n;
    u_int64_t uindex = 0;
    
    double *delta = malloc(N*sizeof(double));

    u_int64_t i;
    for(i = 0; i < N; i++){
        
        u_int64_t i2 = bit_reverse_index(i, n);
        double n_,d_,v;
        
        n_ = channel_probability(channel, y[i2], 0);
        d_ = channel_probability(channel, y[i2], 1);
        
        if(d_ == 0){
            v = MAX_TRANSITION_PROBABILITY;
        }else{
            v = n_/d_;
            if(v < MIN_TRANSITION_PROBABILITY){
                v = MIN_TRANSITION_PROBABILITY;
            }else if(v > MAX_TRANSITION_PROBABILITY){
                v = MAX_TRANSITION_PROBABILITY;
            }
        }
        
        delta[i] = v;
        
    }
    
    pc_decode_construction_step(Z, Zvar, u, n, 1, delta, &uindex);
    
    free(delta);
    
}


void pc_decode_construction_update_Z(double *Z, double *Zvar, double v, u_int64_t *uindex){
    
    if(v > 1){
        v = 1/v;
    }
    
    Zvar[*uindex] += v;

    v = sqrt(v);
    
    Z[*uindex] += v;
    
    (*uindex)++;
}


void pc_decode_construction_step(double *Z, double *Zvar, Bit *u, u_int64_t n, u_int64_t step, double *val, u_int64_t *uindex){
    
    u_int64_t D = 1 << (n-step);
    double *new_val = malloc(D*sizeof(double));
    
    //calcolate f
    pc_decode_calc_f(new_val, val, step, n);
    
    if(step == n){
        pc_decode_construction_update_Z(Z, Zvar, new_val[0], uindex);
    }else{
        pc_decode_construction_step(Z, Zvar, u, n, step+1, new_val, uindex);
    }
    
    //calcolate f
    pc_decode_calc_g(new_val, val, step, n, u, *uindex);

    if(step == n){
        pc_decode_construction_update_Z(Z, Zvar, new_val[0], uindex);
    }else{
        pc_decode_construction_step(Z, Zvar, u, n, step+1, new_val, uindex);
    }
    
    free(new_val);
    
}


