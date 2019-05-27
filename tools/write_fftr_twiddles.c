/*
* @Author: Esten, Ian
* @Date:   2019-05-22 09:43:46
* @Last Modified by:   Esten, Ian
* @Last Modified time: 2019-05-24 10:16:59
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


void write_twiddles(FILE* fp, const char* var_name, int little_endian, const char* which_twiddles, kiss_fft_cpx* twiddles, size_t lenmem)
{
    fprintf(stderr, "0x%x, 0x%x\n", twiddles[0].r, twiddles[0].i);
    uint8_t* twiddles_bytes = (uint8_t*)twiddles;
    fprintf(fp, "const uint8_t %s%s[] = {\n", var_name, which_twiddles);
    if(is_little_endian() == little_endian)
    {
        for(size_t b = 0; b < lenmem; b++)
            fprintf(fp, "0x%04x, ", twiddles_bytes[b]);
    }
    else
    {
        fprintf(stderr, "WARNING: endianness mismatch.\n");
        for(size_t b = 0; b < lenmem; b++)
            fprintf(fp, "0x%04x, ", twiddles_bytes[b]);
    }
    fprintf(fp, "\n};\n");

}


void kiss_fftr_write_twiddles(int nfft, int inverse, const char* file_name, const char* var_name, bool little_endian)
{
    FILE* fp = fopen(file_name, "w");
    if(NULL == fp)
        return;

    size_t super_twiddles_lenmem = 0, substate_twiddles_lenmem = 0;
    //print how much mem needed
    kiss_fftr_get_twiddles(nfft, inverse, NULL, &super_twiddles_lenmem, NULL, &substate_twiddles_lenmem);
    fprintf(stderr, "kiss_fftr_get_twiddles coefficients use %zu bytes\n", super_twiddles_lenmem + substate_twiddles_lenmem);
    //write required headers
    fprintf(fp, "#include <stdint.h>\n\n");
    //write super_twiddles
    kiss_fft_cpx* super_twiddles = (kiss_fft_cpx*)malloc(super_twiddles_lenmem);
    kiss_fft_cpx* substate_twiddles = (kiss_fft_cpx*)malloc(substate_twiddles_lenmem);
    kiss_fftr_get_twiddles(nfft, inverse, super_twiddles, &super_twiddles_lenmem, substate_twiddles, &substate_twiddles_lenmem);
    write_twiddles(fp, var_name, little_endian, "_super", super_twiddles, super_twiddles_lenmem);
    write_twiddles(fp, var_name, little_endian, "_substate", substate_twiddles, substate_twiddles_lenmem);
    free(substate_twiddles);
    free(super_twiddles);
    fclose(fp);
}


int main(int argc, char **argv)
{
    if(argc != 6)
    {
        fprintf(stderr, "Incorrect input arguments given.\n");
        fprintf(stderr, "Please invoke as follows:\n");
        fprintf(stderr, "%s nfft inverse little_endian output_file_name fft_cfg_variable_name\n", argv[0]);
        return 1;
    }
    int nfft = atoi(argv[1]);
    int inverse = atoi(argv[2]);
    bool little_endian = atoi(argv[3]);
    const char* file_name = argv[4];
    const char* var_name = argv[5];
    kiss_fftr_write_twiddles(nfft, inverse, file_name, var_name, little_endian);
    return 0;
}
