//
//  error_rate.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 27/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "error_rate.h"
#include "double_ei.h"

void get_error_rate_file_name(char *filename, u_int64_t n, Channel *channel, u_int64_t K) {
    sprintf(filename, "er_%" PRIu64 "_ch_%d_par_%f" "_k_%" PRIu64 ".txt", n, channel->channel_type, channel->par, K);
}

void get_error_rate_file_name_tmp(char *filename, u_int64_t n, Channel *channel, u_int64_t K) {
    sprintf(filename, "er_%" PRIu64 "_ch_%d_par_%f" "_k_%" PRIu64 ".tmp.txt", n, channel->channel_type, channel->par, K);
}

FILE *open_error_rate_file_in(const char *dir, u_int64_t n, Channel *channel, u_int64_t K) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_error_rate_file_name(name, n, channel, K);
    return fopen(filename, "r");
}

FILE *open_error_rate_file_out(const char *dir, u_int64_t n, Channel *channel, u_int64_t K) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_error_rate_file_name(name, n, channel, K);
    return fopen(filename, "w");
}

FILE *open_error_rate_file_out_temp(const char *dir, u_int64_t n, Channel *channel, u_int64_t K) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_error_rate_file_name_tmp(name, n, channel, K);
    return fopen(filename, "w");
}


void get_or_create_error_rate(ErrorRate *er, const char *dir, u_int64_t n, Channel *channel, u_int64_t K) {
    er->n = n;
    er->channel.channel_type = channel->channel_type;
    er->channel.par = channel->par;
    er->K = K;

    FILE *in = open_error_rate_file_in(dir, n, channel, K);
    if(in){
        if(1 != fscanf(in, "%" PRIu64, &(er->total_samples)))
            exit(1);
        er->BER = double_fget(in);
        er->BER_2 = double_fget(in);
        if(1 != fscanf(in, "%" PRIu64, &(er->frame_errors)))
            exit(1);
        
        fclose(in);
        in = NULL;
    }else{
        er->total_samples = 0;
        er->BER = 0;
        er->BER_2 = 0;
        er->frame_errors = 0;
    }
}

void get_error_rate(ErrorRate *er, const char *dir, u_int64_t n, Channel *channel, u_int64_t K) {
    er->n = n;
    er->channel.channel_type = channel->channel_type;
    er->channel.par = channel->par;
    er->K = K;

    FILE *in = open_error_rate_file_in(dir, n, channel, K);
    if(in){
        if(1 != fscanf(in, "%" PRIu64, &(er->total_samples)))
            exit(1);
        er->BER = double_fget(in);
        er->BER_2 = double_fget(in);
        if(1 != fscanf(in, "%" PRIu64, &(er->frame_errors)))
            exit(1);

        fclose(in);
        in = NULL;
    }else{
        exit(4);
    }
    
}

void save_error_rate(ErrorRate *er, const char *dir) {
    FILE *out = open_error_rate_file_out_temp(dir, er->n, &(er->channel), er->K);
    if(!out)
        exit(1);

    fprintf(out, "%" PRIu64 "\n", er->total_samples);
    double_fput(out, er->BER);
    fprintf(out, " ");
    double_fput(out, er->BER_2);
    fprintf(out, " ");
    fprintf(out, "%" PRIu64 "\n", er->frame_errors);

    fclose(out);
    out = NULL;
    
    char old[1024];
    strcpy(old, dir);
    char *name = old + strlen(dir);
    get_error_rate_file_name_tmp(name, er->n, &(er->channel), er->K);
    
    char new[1024];
    strcpy(new, dir);
    name = new + strlen(dir);
    get_error_rate_file_name(name, er->n, &(er->channel), er->K);
    
    rename(old, new);
    
}
