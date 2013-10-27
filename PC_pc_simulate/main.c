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
#include "../PolarCodes/bhattacharyya_files.h"


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
    
    
    
    
    rnd = randi([0,1],Rows,N);
    u=zeros(Rows,N);
    u(:,A==1)=rnd(:,A==1);
    
    x = pc_encode_c(u);
    y = apply_channel(x,ch_type,ch_par);
    u_ = pc_decode_c( y, ch_type, ch_par, A);
    
    errors=sum(u~=u_,2);
    total=sum(A==1);
    ber = errors./total;
    
    sum_ber(n,p) = sum_ber(n,p) + sum(ber);
    sum_ber2(n,p) = sum_ber2(n,p) + sum(ber.^2);
    num_ber(n,p) = num_ber(n,p) + Rows;

    
    
    return 0;
}

