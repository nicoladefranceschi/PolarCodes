//
//  bhattacharyya_files.c
//  PolarCodes
//
//  Created by Nicola De Franceschi on 25/10/13.
//  Copyright (c) 2013 Nicola De Franceschi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "channels.h"
#include "bhattacharyya_files.h"
#include "double_ei.h"

void get_bhattacharyya_file_name(char *filename, u_int64_t n, Channel *channel) {
    sprintf(filename, "bh_%" PRIu64 "_ch_%d_par_%f" ".txt", n, channel->channel_type, channel->par);
}

void get_bhattacharyya_file_name_tmp(char *filename, u_int64_t n, Channel *channel) {
    sprintf(filename, "bh_%" PRIu64 "_ch_%d_par_%f" ".tmp.txt", n, channel->channel_type, channel->par);
}

FILE *open_bhattacharyya_file_in(const char *dir, u_int64_t n, Channel *channel) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_bhattacharyya_file_name(name, n, channel);
    return fopen(filename, "r");
}

FILE *open_bhattacharyya_file_out(const char *dir, u_int64_t n, Channel *channel) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_bhattacharyya_file_name(name, n, channel);
    return fopen(filename, "w");
}

FILE *open_bhattacharyya_file_out_temp(const char *dir, u_int64_t n, Channel *channel) {
    char filename[1024];
    strcpy(filename, dir);
    char *name = filename + strlen(filename);
    get_bhattacharyya_file_name_tmp(name, n, channel);
    return fopen(filename, "w");
}


void get_bhattacharyya(Bhattacharyya *b, const char *dir, u_int64_t n, Channel *channel) {
    u_int64_t N = 1<<n;

    b->n = n;
    b->channel.channel_type = channel->channel_type;
    b->channel.par = channel->par;
    b->Z = malloc(sizeof(double)*N);
    b->Zvar = malloc(sizeof(double)*N);
    b->total_samples = 0;
    
    u_int64_t i;
    FILE *in = open_bhattacharyya_file_in(dir, n, channel);
    if(in){
        if(1 != fscanf(in, "%" PRIu64, &(b->total_samples)))
            exit(1);
        for(i = 0; i < N; i++){
            b->Z[i] = double_fget(in);
            b->Zvar[i] = double_fget(in);
        }
        fclose(in);
        in = NULL;
    }else{
        memset(b->Z, 0, sizeof(double)*N);
        memset(b->Zvar, 0, sizeof(double)*N);
    }

}

void save_bhattacharyya(Bhattacharyya *b, const char *dir) {
    u_int64_t N = 1 << b->n;
    u_int64_t i;
    FILE *out = open_bhattacharyya_file_out_temp(dir, b->n, &(b->channel));
    if(!out)
        exit(1);
    fprintf(out, "%" PRIu64 "\n", b->total_samples);
    for(i = 0; i < N; i++){
        double_fput(out, b->Z[i]);
        fprintf(out, " ");
        double_fput(out, b->Zvar[i]);
        fprintf(out, "\n");
    }
    fclose(out);
    out = NULL;
    
    char old[1024];
    strcpy(old, dir);
    char *name = old + strlen(dir);
    get_bhattacharyya_file_name_tmp(name, b->n, &(b->channel));
    
    char new[1024];
    strcpy(new, dir);
    name = new + strlen(dir);
    get_bhattacharyya_file_name(name, b->n, &(b->channel));
    
    rename(old, new);

}
