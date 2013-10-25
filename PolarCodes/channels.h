//
//  channels.h
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#ifndef PolarCodes_channels_h
#define PolarCodes_channels_h

#include "polar_codes.h"

typedef enum {
    ChannelType_BSC = 1,
    ChannelType_BEC = 2
} ChannelType;


typedef struct {
    ChannelType channel_type;
    double par;
} Channel;


void apply_channel(Bit *yOut, Bit *x, const Channel *channel, u_int64_t n);

double channel_probability(const Channel *channel, Bit bOut, Bit bIn);

double channel_capacity(const Channel *channel);

#endif
