//
//  pc_construction.h
//  PolarCodes
//
//  Created by Nicola De Franceschi on 03/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#ifndef PolarCodes_pc_construction_h
#define PolarCodes_pc_construction_h

#include "polar_codes.h"
#include "channels.h"


void pc_construction(double *Z, double *Zvar, u_int64_t n, const Channel *channel, u_int64_t n_samples);


#endif
