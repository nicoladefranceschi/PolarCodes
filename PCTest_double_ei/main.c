//
//  main.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 25/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>

#include "../PolarCodes/double_ei.h"

int main(int argc, const char * argv[])
{

    double d = 1234.0/(1<<5);
    
    printf("%f ",d);
    
    double_fput(stdout, d);
    printf("\n");
    
    double i = double_fget(stdin);
    double i2 = double_fget(stdin);
    
    printf("%f\n",i);
    printf("%f\n",i2);
    
    
    return 0;
}

