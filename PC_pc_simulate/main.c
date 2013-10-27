//
//  main.c
//  pc_simulate
//
//  Created by Nicola De Franceschi on 26/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include "../PolarCodes/polar_codes.h"
#include "../PolarCodes/bhattacharyya_files.h"
#include "../PolarCodes/pc_encode.h"
#include "../PolarCodes/pc_decode.h"
#include "../PolarCodes/channels.h"
#include "../PolarCodes/error_rate.h"


#define DEFAULT_max_n_samples (1<<30)


// call with arguments: n ch_type ch_par K n_samples max_n_samples dir
// max_n_samples==-1 for default
int main(int argc, const char * argv[])
{
    
    //random initialization
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((unsigned int)(time.tv_sec + time.tv_usec));

    
    Channel channel;
    u_int64_t n;
    u_int64_t K;
    u_int64_t n_samples;
    int64_t max_n_samples;
    const char *dir;
    
    if(argc != 8)
        exit(2);
    
    
    //n
    sscanf(argv[1], "%" PRIu64, &n);
    
    //ch_type
    sscanf(argv[2], "%d", &(channel.channel_type));
    
    //ch_par
    sscanf(argv[3], "%lf", &(channel.par));
    
    //K
    sscanf(argv[4], "%" PRIu64, &K);
    
    //n_samples
    sscanf(argv[5], "%" PRIu64, &n_samples);
    
    //max_n_samples
    sscanf(argv[6], "%" PRId64, &max_n_samples);
    if(max_n_samples == -1)
        max_n_samples=DEFAULT_max_n_samples;
    
    //dir
    dir = argv[7];
    
    
    u_int64_t N = 1 << n;
    
    Bhattacharyya b;
    get_bhattacharyya(&b, dir, n, &channel);
    
    
    Bit *A = malloc(sizeof(Bit)*N);
    get_frozen_bits(A, K, &b);
    
    Bit *u = malloc(sizeof(Bit)*N);
    Bit *x = malloc(sizeof(Bit)*N);


    ErrorRate error_rate;
    get_or_create_error_rate(&error_rate, dir, n, &channel, K);
    
    while (1) {
        
        u_int64_t i;
        for(i=0; i < n_samples; i++){
            
            random_bits_f(u, A, N);
            pc_encode(x, u, n);
            apply_channel(x, x, &channel, n);
            pc_decode(x, x, &channel, A, n);
            
            u_int64_t bit_errors = 0;
            u_int64_t j;
            for(j=0; j < N; j++){
                if(u[j] != x[j])
                    bit_errors++;
            }
            
            double ber = (double)bit_errors / (double)N;
            
            error_rate.total_samples++;
            error_rate.BER += ber;
            error_rate.BER_2 += ber*ber;
            if(bit_errors > 0)
                error_rate.frame_errors++;
            
        }
        
        
        save_error_rate(&error_rate, dir);
        printf("total_samples = %lld\n", error_rate.total_samples);
        
        if(error_rate.total_samples > max_n_samples)
            break;
    }
    
    return 0;
}

