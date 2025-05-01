#include "example.h"
#include <cstdlib>  // For rand() and srand()
#include <cassert>  // For assertions
#include <stdio.h>


void get_randoms(ap_int<10> *arr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    arr[i] = (rand() % 500) + 1;  // Generate numbers between 1 and 500
  }
}

void get_expected(ap_int<10> *temp_in, ap_int<10> *temp_out){
  for(size_t i = 1; i<N-1; i++){
    std::cout<<"a = "<<temp_in[i-1]<<std::endl;
    std::cout<<"b = "<<temp_in[i]<<std::endl;
    std::cout<<"c = "<<temp_in[i+1]<<std::endl;
    ap_int<27> sum = temp_in[i-1]+temp_in[i]+temp_in[i+1];
    std::cout<<"Sum = "<<sum<<std::endl;

    temp_out[i] = (temp_in[i-1] + temp_in[i] + temp_in[i+1])/3;
    std::cout<<"result = "<<temp_out[i]<<std::endl<<std::endl;  
  }
}

int main () {

  ap_int<10> input[N];
  ap_int<10> temp[N];
  ap_int<10> output[N];
  for (size_t i = 0; i < N; ++i) {
    temp[i]=0;
    output[i]=0;
  }

  get_randoms(input, N);

  std::cout<<"RUNNING EXAMPLE"<<std::endl;
  example(input, output);
  std::cout<<"CALCULATING EXPECTED"<<std::endl;
  get_expected(input, temp);

  int retval = 0;

  ofstream FILE;
  FILE.open("result.dat");

  // Create input data
  for (size_t i = 0; i < N; ++i) {
    cout<<input[i]<<" "<<output[i]<<" "<<temp[i]<<endl;
    FILE << i <<" "<<input[i]<<" "<<output[i] << endl;
  }
  // Call the function
  FILE.close();

  // Compare the results file with the golden results
  for (size_t i = 0; i < N; ++i) {
    if(output[i] != temp[i])
      retval=1;
  }

  if (retval != 0) {
    printf("Test failed  !!!\n");
    retval = 1;
  } else {
    printf("Test passed !\n");
  }

  // Return 0 if the test passes
  return retval;

}
