//
//  channels.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include "channels.h"
#include <math.h>


void apply_channel(Bit *yOut, Bit *x, const Channel *channel, u_int64_t n) {
    
    switch (channel->channel_type) {
            
        case ChannelType_BSC:
        {
            u_int64_t N = 1 << n;
            
            u_int64_t i;
            for(i=0; i<N; i++){
                
                double r = (double)rand() / (double)RAND_MAX;
                
                if(r < channel->par)
                    yOut[i] = (x[i] + 1) & 1;
                else
                    yOut[i] = x[i];
                
            }
            
            break;
        }
            
        case ChannelType_BEC:
        {
            
            u_int64_t N = 1 << n;
            
            u_int64_t i;
            for(i=0; i<N; i++){
                
                double r = (double)rand() / (double)RAND_MAX;
                
                if(r < channel->par)
                    yOut[i] = 2;
                else
                    yOut[i] = x[i];
                
            }
            
            break;
        }
            
    }
 
}

double channel_probability(const Channel *channel, Bit bOut, Bit bIn) {

    switch (channel->channel_type) {
        case ChannelType_BSC:
        {
            if(bOut != bIn)
                return channel->par;
            else
                return 1-channel->par;
        }
            
        case ChannelType_BEC:
        {
            if(bOut == 2)
                return channel->par;
            else if(bOut == bIn)
                return 1-channel->par;
            else
                return 0;
            
        }
    }

}

double channel_capacity(const Channel *channel) {
    
    switch (channel->channel_type) {
        case ChannelType_BSC:
        {
            double entropy = - channel->par*log2(channel->par) - (1-channel->par)*log2(1-channel->par);
            return 1-entropy;
        }
        
        case ChannelType_BEC:
        {
            return 1-channel->par;
        }
    }
    
}

