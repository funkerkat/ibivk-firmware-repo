/*
 * test.c
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

typedef struct Tdata2 data2;



typedef struct Tdata1 {
   int a;
   data2 *d;
} data1;



struct Tdata2 {
   int a;
   data1 *d;
};
