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
#include "../PolarCodes/pc_construction.h"
#include "../PolarCodes/bhattacharyya_files.h"


// call with arguments: n ch_type ch_par n_samples dir
int main(int argc, const char * argv[])
{
    Channel channel;
    u_int64_t n;
    u_int64_t n_samples;
    const char *dir;
    
    if(argc != 6)
        exit(2);
    
    
    //n
    sscanf(argv[1], "%" PRIu64, &n);
    
    //ch_type
    sscanf(argv[2], "%d", &(channel.channel_type));

    //ch_par
    sscanf(argv[3], "%lf", &(channel.par));
    
    //n_samples
    sscanf(argv[4], "%" PRIu64, &n_samples);

    //dir
    dir = argv[5];
    
    Bhattacharyya b;
    get_bhattacharyya(&b, dir, n, &channel);
    
    while (1) {
        
        pc_construction(b.Z, b.Zvar, n, &(b.channel), n_samples);
        b.total_samples += n_samples;
        
        save_bhattacharyya(&b, dir);
        printf("total_samples = %lld\n", b.total_samples);
    }
    
    return 0;
}


