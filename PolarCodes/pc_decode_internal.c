//
//  pc_decode_internal.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 16/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "pc_decode_internal.h"

void pc_decode_calc_f(double *new_val, double *val, u_int64_t step, u_int64_t n) {
    
    u_int64_t D = 1 << (n-step);
    
    u_int64_t i;
    for(i = 0; i < D; i++){
        double a = val[2*i];
        double b = val[2*i+1];
        
        double v = (1+a*b) / (a+b);
        
        if(v < MIN_TRANSITION_PROBABILITY){
            v = MIN_TRANSITION_PROBABILITY;
        }else if(v > MAX_TRANSITION_PROBABILITY){
            v = MAX_TRANSITION_PROBABILITY;
        }
        
        new_val[i] = v;
        
    }
    
}

void pc_decode_calc_us(Bit *usOut, u_int64_t stage, Bit *u, u_int64_t uindex){
    
    u_int64_t dim = 1 << stage;
    u_int64_t dim2 = 1;
    
    memcpy(usOut, u + uindex - dim, dim);
    
    u_int64_t s;
    
    Bit *temp2 = malloc(dim*sizeof(Bit));
    
    for(s = 1; s <= stage; s++){
        
        u_int64_t i1, i2;
        for(i1 = 0; i1 < 1<<(stage-s); i1++){
            for(i2 = 0; i2 < 1<<(s-1); i2++){
                
                Bit even = usOut[(2*i1+1)*dim2+i2]; //even in matlab!
                Bit odd = usOut[(2*i1)*dim2+i2]; //even in matlab!
                
                temp2[i1*dim2*2 + i2*2] = (even + odd) & 1;
                temp2[i1*dim2*2 + i2*2+1] = even;
                
            }
        }
        
        dim2 *= 2;
        
        memcpy(usOut, temp2, dim);
        
    }
    
    free(temp2);
    
}

void pc_decode_calc_g(double *new_val, double *val, u_int64_t step, u_int64_t n, Bit *u, u_int64_t uindex) {
    
    
    u_int64_t D = 1 << (n-step);
    
    u_int64_t i;
    
    Bit us[D];
    pc_decode_calc_us(us, n-step, u, uindex);
    
    for(i = 0; i < D; i++){
        double a = val[2*i];
        double b = val[2*i+1];
        double v;
        
        if(us[i] == 0){
            v = b*a;
        }else{
            v = b/a;
        }
        
        if(v < MIN_TRANSITION_PROBABILITY){
            v = MIN_TRANSITION_PROBABILITY;
        }else if(v > MAX_TRANSITION_PROBABILITY){
            v = MAX_TRANSITION_PROBABILITY;
        }
        
        new_val[i] = v;
        
    }
    
}
