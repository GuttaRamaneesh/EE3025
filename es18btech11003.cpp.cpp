#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

void fft(double complex* x, long int N, int flag){
        if(N==1)
                return;
        double complex* odd = malloc(N/2 * sizeof(double complex));
        double complex* even = malloc(N/2 * sizeof(double complex));
        for(long int i=0;i<N/2;i++){
                odd[i] = x[2*i+1];
                even[i] = x[2*i];
        }
        fft(even, N/2, flag);
        fft(odd, N/2, flag);
        for(int i=0;i<N/2;i++){
                double temp = M_PI*i*I*flag/N;                  
                double complex w = exp(2*temp);                //Twiddle Factor
                x[i] = even[i] + w*odd[i];
                x[i+N/2] = even[i] - w*odd[i];
        }
        return;
}
void ifft(double complex* x, long int N){
        fft(x, N, -1);
        for(int i=0;i<N;i++)
                x[i] = x[i]/N;
        return;
}
int main(){
        int n = 1048576;
        double* x1 = (double*)malloc(n*sizeof(double));
        double* x2 = (double*)malloc(n*sizeof(double));
        double complex* X1 = (double complex*)malloc(n*sizeof(double complex));
        double complex* X2 = (double complex*)malloc(n*sizeof(double complex));
        FILE *ft1, *ft2, *fF1, *fF2;
        ft1 = fopen("Sound_Noise.dat", "r");
        ft2 = fopen("betterReducedNoise.dat", "r");
        int len = 0;
        while(!feof(ft1) && len<n){
                fscanf(ft1, "%1f", &(x1[len]));
                X1[len] = CMPLX(x1[len], 0);
                len++;
        }
        len = 0;
        while(!feof(ft2) && len<n){
                fscanf(ft2, "%1f", &(x2[len]));
                X2[len] = CMPLX(x2[len], 0);
                len++;
        }
        fft(X1, n, 1);
        fF1 = fopen("X1.dat", "w");
        for(int i=0;i<n;i++)
                fprintf(fF1, "%1f+%1fi\n", creal(X1[i]), cimag(X1[i]));
        fft(X2, n, 1);
        fF2 = fopen("X2.dat", "w");
        for(int i=0;i<n;i++)
                fprintf(fF2, "%1f+%1fi\n", creal(X2[i]), cimag(X2[i]));
        // close all files.
        fclose(ft1);fclose(ft2);fclose(fF1);fclose(fF2);
        return 0;
}