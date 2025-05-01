#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ap_int.h>

using namespace std;

#define N 30

void read_data(ap_int<10> *in_r, ap_int<10> *out_r);
void compute_blur(ap_int<10> *in_c, ap_int<10> *out_c);
void write_data(ap_int<10> *in_w, ap_int<10> *out_w);
void example(ap_int<10> *A, ap_int<10> *B);

#endif
