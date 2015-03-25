//
//  file_blocks.h
//  mydatabase
//
//  Created by Oleg on 05.03.15.
//  Copyright (c) 2015 Oleg. All rights reserved.
//

#ifndef __mydatabase__file_blocks__
#define __mydatabase__file_blocks__

#include <stdio.h>

#endif /* defined(__mydatabase__file_blocks__) */

/******* Constants (parameters) ********/
// by default file- 1_gb, block- 4kb
// ******* 256000 blocks  4000 bytes each   ********

FILE* mybase;
const size_t block_size= 4000;    //size of one block in bytes
const size_t block_count= 256000; //number of blocks
const int mask_size_bytes= block_count/8;
const int mask_size_blocks= mask_size_bytes/block_size;

/******* Functions ********/

int go_block(size_t n);

char* get_mask();

int put_mask(char* in);

int reserv_block(size_t n);

int free_block(size_t n);

int chvac(size_t n);
/********** public API ************/

int initfile(const char* name);

void* read_block(size_t block_num);

int write_block(size_t block_num, void* ptr);

size_t new_block();