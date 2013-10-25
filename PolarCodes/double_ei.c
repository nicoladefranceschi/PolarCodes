//
//  double_ei.c
//  
//
//  Created by Nicola De Franceschi on 25/10/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "double_ei.h"


#define FRAC_MAX 9223372036854775807LL /* 2**63 - 1 */


struct double_packed
{
    int exp;
    long long frac;
};

void double_pack(double x, struct double_packed *r);
double double_unpack(const struct double_packed *p);


void double_pack(double x, struct double_packed *r)
{
    double xf = fabs(frexp(x, &r->exp)) - 0.5;
    
    if (xf < 0.0)
    {
        r->frac = 0;
        return;
    }
    
    r->frac = 1 + (long long)(xf * 2.0 * (FRAC_MAX - 1));
    
    if (x < 0.0)
        r->frac = -r->frac;
}

double double_unpack(const struct double_packed *p)
{
    double xf, x;
    
    if (p->frac == 0)
        return 0.0;
    
    xf = ((double)(llabs(p->frac) - 1) / (FRAC_MAX - 1)) / 2.0;
    
    x = ldexp(xf + 0.5, p->exp);
    
    if (p->frac < 0)
        x = -x;
    
    return x;
}

void double_fput(FILE *out, double x) {
    struct double_packed dp;
    double_pack(x, &dp);
    fprintf(out, "%d %lld", dp.exp, dp.frac);
}

double double_fget(FILE *in) {
    struct double_packed dp;
    int n = fscanf(in, "%d %lld", &dp.exp, &dp.frac);
    if(n != 2)
        exit(1);
    return double_unpack(&dp);
}
