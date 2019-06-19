/*
* @Author: Esten, Ian
* @Date:   2019-05-22 09:43:46
* @Last Modified by:   Esten, Ian
* @Last Modified time: 2019-06-19 10:40:06
*/


#include "kiss_fftr.h"
#include <stdio.h>
#include <stdbool.h>


int is_little_endian()
{
  unsigned int x = 1;
  char *c = (char*) &x;
  return (int)*c;
}


void write_twiddles(FILE* fp, const char* var_name, const char* which_twiddles, kiss_fft_cpx* twiddles, size_t lenmem)
{
    size_t num_twiddles = lenmem / sizeof(kiss_fft_cpx);
    fprintf(fp, "const kiss_fft_cpx %s%s[] = {\n.   ", var_name, which_twiddles);
    for(size_t b = 0; b < num_twiddles; b++)
        fprintf(fp, "{.r=%d,.i=%d},", twiddles[b].r, twiddles[b].i);
    fprintf(fp, "\n};\n");

}


void kiss_fftr_write_twiddles(int nfft, int inverse, const char* file_name, const char* var_name)
{
    FILE* fp = fopen(file_name, "w");
    if(NULL == fp)
        return;

    size_t super_twiddles_lenmem = 0, substate_twiddles_lenmem = 0;
    //print how much mem needed
    size_t cfg_size = 0;
    kiss_fftr_alloc_with_twiddles(nfft, inverse, NULL, NULL, NULL, &cfg_size);
    kiss_fftr_get_twiddles(nfft, inverse, NULL, &super_twiddles_lenmem, NULL, &substate_twiddles_lenmem);
    fprintf(stderr, "memory required for kiss_fftr_cfg %zu\n", cfg_size);
    fprintf(stderr, "kiss_fftr_get_twiddles coefficients use %zu bytes\n", super_twiddles_lenmem + substate_twiddles_lenmem);
    //write required headers
    fprintf(fp, "#include <stdint.h>\n\n");
    //write super_twiddles
    kiss_fft_cpx* super_twiddles = (kiss_fft_cpx*)malloc(super_twiddles_lenmem);
    kiss_fft_cpx* substate_twiddles = (kiss_fft_cpx*)malloc(substate_twiddles_lenmem);
    kiss_fftr_get_twiddles(nfft, inverse, super_twiddles, &super_twiddles_lenmem, substate_twiddles, &substate_twiddles_lenmem);
    write_twiddles(fp, var_name, "_super", super_twiddles, super_twiddles_lenmem);
    write_twiddles(fp, var_name, "_substate", substate_twiddles, substate_twiddles_lenmem);
    free(substate_twiddles);
    free(super_twiddles);
    fclose(fp);
}


int main(int argc, char **argv)
{
    if(argc != 5)
    {
        fprintf(stderr, "Incorrect input arguments given.\n");
        fprintf(stderr, "Please invoke as follows:\n");
        fprintf(stderr, "%s nfft inverse output_file_name fft_cfg_variable_name\n", argv[0]);
        return 1;
    }
    int nfft = atoi(argv[1]);
    int inverse = atoi(argv[2]);
    const char* file_name = argv[3];
    const char* var_name = argv[4];
    kiss_fftr_write_twiddles(nfft, inverse, file_name, var_name);
    return 0;
}
