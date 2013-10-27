//
//  main.c
//  pc_construction
//
//  Created by Nicola De Franceschi on 25/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include "../PolarCodes/pc_construction.h"
#include "../PolarCodes/bhattacharyya_files.h"


#define DEFAULT_max_n_samples (1<<30)


// call with arguments: n ch_type ch_par n_samples max_n_samples dir
// max_n_samples==-1 for default
int main(int argc, const char * argv[])
{
    
    //random initialization
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((unsigned int)(time.tv_sec + time.tv_usec));

    
    Channel channel;
    u_int64_t n;
    u_int64_t n_samples;
    int64_t max_n_samples;
    const char *dir;
    
    if(argc != 7)
        exit(2);
    
    
    //n
    sscanf(argv[1], "%" PRIu64, &n);
    
    //ch_type
    sscanf(argv[2], "%d", &(channel.channel_type));

    //ch_par
    sscanf(argv[3], "%lf", &(channel.par));
    
    //n_samples
    sscanf(argv[4], "%" PRIu64, &n_samples);
    
    //max_n_samples
    sscanf(argv[5], "%" PRId64, &max_n_samples);
    if(max_n_samples == -1)
        max_n_samples=DEFAULT_max_n_samples;

    //dir
    dir = argv[6];
    
    Bhattacharyya b;
    get_or_create_bhattacharyya(&b, dir, n, &channel);
    
    while (1) {
        
        pc_construction(b.Z, b.Zvar, n, &(b.channel), n_samples);
        b.total_samples += n_samples;
        
        save_bhattacharyya(&b, dir);
        printf("total_samples = %lld\n", b.total_samples);
        
        if(b.total_samples > max_n_samples)
            break;
    }
    
    return 0;
}


