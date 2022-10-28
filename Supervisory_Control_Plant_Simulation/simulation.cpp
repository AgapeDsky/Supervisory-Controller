#include<stdio.h>

float prev_input = 0, prev_output = 0;
float cur_input = 0, cur_output = 0;
float tau = 1.2;
float T = 0.01;

int main() {
    int i = 0;
    while (i < 500) {
        cur_input = 1; // Step Response, input konstan 1
        cur_output = prev_input/(2*tau+T)*T + cur_input/(2*tau+T)*T - prev_output*(T-2*tau)/(2*tau+float(T));
        prev_input = cur_input;
        prev_output = cur_output;

        if ( i % 120 == 0) {
            printf("Response at %d * Time Constant: %f\n", i/120, cur_output);
        }
        i++;
    }

    return 0;
}