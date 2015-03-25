//
//  file_blocks.c
//  mydatabase
//
//  Created by Oleg on 05.03.15.
//  Copyright (c) 2015 Oleg. All rights reserved.
//

#include "file_blocks.h"
#include "stdio.h"
#include "stdlib.h"



int go_block(size_t n){
    return fseek(mybase,(n*block_size)+1, SEEK_SET);
}

char* get_mask(){
    char *vac_mask= malloc(mask_size_bytes);
    int i;
    
    go_block(0);
    for(i=0; i<mask_size_bytes; i++)
        vac_mask[i]= fgetc(mybase);
    return vac_mask;
}

int put_mask(char* in){
    int i;

    go_block(0);
    for (i=0; i<mask_size_bytes; i++)
        fputc(in[i], mybase);
    return 0;
}

int reserv_block(size_t n){
    if (chvac(n)){
        printf("%s","The block is already reserved");
        return 1;
    }
    char mask = 0b10000000;
    char *vac_mask= get_mask();
    size_t pos= n/8;
    
    mask= mask>> (n%8);
    vac_mask[pos]= vac_mask[pos]| mask;
    put_mask(vac_mask);
    
    return 0;
}

int free_block(size_t n){
    char mask= 0b01111111;
    char *vac_mask= get_mask();
    size_t pos= n/8;
    
    mask= mask>> (n%8);
    vac_mask[pos]= vac_mask[pos] & mask;
    put_mask(vac_mask);
    
    return 0;
}

int chvac(size_t n){
    char mask= 0b10000000;
    char *vac_mask= get_mask();
    size_t pos= n/8;
    
    mask= mask>> (n%8);
    mask= mask & vac_mask[pos];
    return mask== 0b00000000;
}
/********** public API ************/

int initfile(const char* name){
    mybase= fopen(name, "a+");
    char *vac_mask= malloc(mask_size_bytes);
    size_t i;
    
    for (i=0; i<mask_size_bytes; i= i+1) {
        vac_mask[i]= vac_mask[i]& 0b00000000;
    }
    
    put_mask(vac_mask);
    
    for(i=0; i<mask_size_blocks; i++)
        reserv_block(i);
    
 /*   for (i=0;i< mask_size_blocks;i= i+1){
        mask= 0b10000000;
        mask = mask >> (i%8);
        n= i/8;
        vac_mask[n]= vac_mask[n]| mask;
    }
    for (i=0; i<mask_size_bytes; i= i+1) {
        fputc(vac_mask[i], mybase);
    }
  */
    return 1;
}

void* read_block(size_t block_num){
    
    //read one block if it's not empty
    if (chvac(block_num)){
        printf("%s","readin an empty block");
        return NULL;
    }
    void* res= malloc(block_size);
    go_block(block_num);
    fread(res, 1, block_size, mybase);
    return res;
}
int write_block(size_t block_num, void* ptr){
    go_block(block_num);
    fwrite(ptr, 1, block_size, mybase);
    return 0;
    
}

size_t new_block(){
    //reserve one block and return number
    size_t i;
    
    for (i=0; i<block_count; i= i+1) {
        if (chvac(i))
            reserv_block(i);
            return i;
    }
    return -1;
}


