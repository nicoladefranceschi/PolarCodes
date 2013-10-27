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


// call with arguments: n ch_type ch_par Kmin Kdelta Kmax n_samples max_n_samples dir
// max_n_samples==-1 for default
// Kmin Kdelta Kmax are actually Kmin/N Kdelta/N Kmax/N
int main(int argc, const char * argv[])
{
    
    //random initialization
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((unsigned int)(time.tv_sec + time.tv_usec));

    
    Channel channel;
    u_int64_t n;
    double Kmin, Kdelta, Kmax;
    u_int64_t n_samples;
    int64_t max_n_samples;
    const char *dir;
    
    if(argc != 10)
        exit(2);
    
    
    //n
    sscanf(argv[1], "%" PRIu64, &n);
    
    //ch_type
    sscanf(argv[2], "%d", &(channel.channel_type));
    
    //ch_par
    sscanf(argv[3], "%lf", &(channel.par));
    
    //Kmin Kdelta Kmax
    sscanf(argv[4], "%lf", &Kmin);
    sscanf(argv[5], "%lf", &Kdelta);
    sscanf(argv[6], "%lf", &Kmax);
    
    //n_samples
    sscanf(argv[7], "%" PRIu64, &n_samples);
    
    //max_n_samples
    sscanf(argv[8], "%" PRId64, &max_n_samples);
    if(max_n_samples == -1)
        max_n_samples=DEFAULT_max_n_samples;
    
    //dir
    dir = argv[9];
    
    
    u_int64_t N = 1 << n;
    
    Bhattacharyya b;
    get_bhattacharyya(&b, dir, n, &channel);
    
    
    u_int64_t lastK=0;
    u_int64_t numK = (u_int64_t)((Kmax-Kmin)/Kdelta)+1;
    u_int64_t realNumK=0;
    Bit **As = malloc(sizeof(Bit*)*numK);
    ErrorRate *ers = malloc(sizeof(ErrorRate)*numK);
    
    u_int64_t i;
    for(i=0;i<numK;i++){
        u_int64_t K = (u_int64_t)((Kmin+Kdelta*i) * N);
        if(K == lastK)
            continue;
        if(K > N)
            break;
        
        lastK = K;
        Bit *A = malloc(sizeof(Bit)*N);
        get_frozen_bits(A, K, &b);
        As[realNumK]=A;
        
        get_or_create_error_rate(&ers[realNumK], dir, n, &channel, K);

        realNumK++;
    }
    
    numK=realNumK;
    
    Bit *u = malloc(sizeof(Bit)*N);
    Bit *x = malloc(sizeof(Bit)*N);
    
    while (1) {
        
        u_int64_t num_done = 0;
        
        u_int64_t k;
        for(k=0; k<numK; k++){
            
            if(ers[k].total_samples > max_n_samples){
                num_done++;
                continue;
            }
            
            Bit *A = As[k];
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
                
                ers[k].total_samples++;
                ers[k].BER += ber;
                ers[k].BER_2 += ber*ber;
                if(bit_errors > 0)
                    ers[k].frame_errors++;
                
            }
            
            save_error_rate(&ers[k], dir);
            printf("K = %lld, total_samples = %lld\n", ers[k].K, ers[k].total_samples);
            
        }
        
        if(num_done == numK)
            break;
        
    }
    
    return 0;
}

