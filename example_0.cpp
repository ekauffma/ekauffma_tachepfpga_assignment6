#include "example.h"


void read_data(int in_r[N], int out_r[N]) {
    for (int i = 0; i < N; i++) {
        out_r[i] = in_r[i];  // Simple pass-through
    }
}

void compute_blur(int in_c[N], int out_c[N]) {
    for (int i = 1; i < N - 1; i++) {
        out_c[i] = (in_c[i - 1] + in_c[i] + in_c[i + 1]) / 3;  // Box blur operation
    }
}

void write_data(int in_w[N], int out_w[N]) {
    for (int i = 0; i < N; i++) {
        out_w[i] = in_w[i];  // Simple pass-through
    }
}

void example(int A[N], int B[N]) {
    int temp1[N], temp2[N];

    for (size_t i = 0; i < N; i++) 
      temp2[i]=0;

    read_data(A, temp1);
    compute_blur(temp1, temp2);
    write_data(temp2, B);
}
