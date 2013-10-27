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

void get_or_create_bhattacharyya(Bhattacharyya *b, const char *dir, u_int64_t n, Channel *channel) {
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
        exit(4);
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

struct frozen_bits_index_struct {
    u_int64_t index;
    Bhattacharyya *b;
};

int frozen_bits_sort_cmp(const void *x, const void *y)
{
    struct frozen_bits_index_struct *xx = (struct frozen_bits_index_struct*)x;
    struct frozen_bits_index_struct *yy = (struct frozen_bits_index_struct*)y;
    Bhattacharyya *b = xx->b;
    if (b->Z[xx->index] < b->Z[yy->index]) return -1;
    if (b->Z[xx->index] > b->Z[yy->index]) return 1;
    return 0;
}

void get_frozen_bits(Bit *bits, u_int64_t K, Bhattacharyya *b) {
    u_int64_t N = 1 << b->n;
    struct frozen_bits_index_struct *indexes = (struct frozen_bits_index_struct*)malloc(sizeof(struct frozen_bits_index_struct)*N);
    u_int64_t i;
    for(i=0;i<N;i++){
        indexes[i].index = i;
        indexes[i].b = b;
    }
    
    qsort(indexes, N, sizeof(struct frozen_bits_index_struct), &frozen_bits_sort_cmp);
        
    memset(bits, 0, N);
    for(i=0;i<K;i++){
        u_int64_t index = indexes[i].index;
        bits[index] = 1;
    }
    
}

