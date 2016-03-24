#include "filter1.h"
#include <stdio.h>




int main( int argc, char **argv )                 // Typical C style main()
{

  // FILE *fp;
  //
  // fp = fopen("/RAW/100-1000.raw", "r");
  // fprintf(fp, "This is testing for fprintf...\n");
printf("aaaaaaaaaaaaaaaaaaaaaagt :\n");

  FILE *fp;
  char * line = NULL;
  size_t len = 0;
  int read;
  filter1Type *filter1 = filter1_create();
  short out;

  fp = fopen("/afs/inf.ed.ac.uk/user/s12/s1231893/Desktop/ES/CW2/ES_2/filtertest/RAW/10-1000.raw", "r");


  if (fp == NULL){
    printf("xxxxxxxxxxxxx :\n");

    return 1;
  }

  while ((read = getc(fp)) != -1) {
    printf("%#02x - ", read);
    filter1_writeInput( filter1, read);       // Read a sample from the ADC and write it into the filter
    out = filter1_readOutput( filter1 );
    printf("%d\n", out);
  }

  fclose(fp);
  if (line)
      free(line);
      return 0;
    //
    //
    // filter1Type *filter1 = filter1_create();               // Create the filter
    // while( 1 )                                    // Infinite loop
    // {
    //     filter1_writeInput( filter1, ADCRead() );       // Read a sample from the ADC and write it into the filter
    //     DACWrite( filter1_readOutput( filter1 ) );      // Read the output from the filter and write it to the DAC
    // }
    //
    // filter1_destroy( filter1 );                       // Done. If we every reach here, destroy the filter


    return 0;
}
