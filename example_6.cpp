#include "example.h"


void read_data(ap_int<10> in_r[N], ap_int<10> out_r[N]) {
#pragma HLS PIPELINE II=1
    for (ap_int<10> i = 0; i < N; i++) {
        out_r[i] = in_r[i];  // Simple pass-through
    }
}

void compute_blur(ap_int<10> in_c[N], ap_int<10> out_c[N]) {
#pragma HLS PIPELINE II=1
#pragma HLS ALLOCATION instances=add limit=1 operation
#pragma HLS ALLOCATION instances=div limit=1 operation
    for (ap_int<10> i = 1; i < N - 1; i++) {
        ap_int<10> a = in_c[i - 1];
        ap_int<10> b = in_c[i];
        ap_int<10> c = in_c[i + 1];
        ap_int<12> sum = a + b + c;

        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        std::cout << "c = " << c << std::endl;
        std::cout << "Sum = " << sum << std::endl;

        out_c[i] = sum / 3;

        std::cout << "result = " << out_c[i] << std::endl<<std::endl;
    }
    out_c[0] = 0;
    out_c[N-1] = 0;
}

void write_data(ap_int<10> in_w[N], ap_int<10> out_w[N]) {
#pragma HLS PIPELINE II=1
    for (ap_int<10> i = 0; i < N; i++) {
        out_w[i] = in_w[i];  // Simple pass-through
    }
}

void example(ap_int<10> A[N], ap_int<10> B[N]) {
#pragma HLS DATAFLOW
    ap_int<10> temp1[N], temp2[N];

    read_data(A, temp1);
    compute_blur(temp1, temp2);
    write_data(temp2, B);
}
