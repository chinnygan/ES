/******************************* SOURCE LICENSE *********************************
Copyright (c) 2015 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

// Begin header file, filter1.h

#ifndef FILTER1_H_ // Include guards
#define FILTER1_H_

static const int filter1_numStages = 4;
static const int filter1_coefficientLength = 20;
extern short filter1_coefficients[20];
extern short filter1_impulseInput[239];
extern float filter1_impulseOutput[239];
extern short filter1_noiseInput[511];
extern float filter1_noiseOutput[511];
extern short filter1_multiSineInput[478];
extern float filter1_multiSineOutput[478];
extern short filter1_overflowInput[239];
extern float filter1_overflowOutput[239];
extern short filter1_testOutput[511];

typedef struct
{
	short state[16];
	short output;
} filter1Type;

typedef struct
{
	short *pInput;
	short *pOutput;
	short *pState;
	short *pCoefficients;
	short count;
} filter1_executionState;


filter1Type *filter1_create( void );
void filter1_destroy( filter1Type *pObject );
 void filter1_init( filter1Type * pThis );
 void filter1_reset( filter1Type * pThis );
#define filter1_writeInput( pThis, input )  \
	filter1_filterBlock( pThis, &input, &pThis->output, 1 );

#define filter1_readOutput( pThis )  \
	pThis->output

 int filter1_filterBlock( filter1Type * pThis, short * pInput, short * pOutput, unsigned int count );
#define filter1_outputToFloat( output )  \
	(( (1.0f/1024) * (output) ))

#define filter1_inputFromFloat( input )  \
	((short)(32768f * (input)))

 void filter1_filterBiquad_12_14_15( filter1_executionState * pExecState );
 void filter1_filterBiquad_11_14_15( filter1_executionState * pExecState );
 void filter1_filterBiquad_10_14_15( filter1_executionState * pExecState );
 void filter1_runTests(  void  );
 void filter1_compareResult( short * pInput, float * pReference, int count, float maxThreshold, float msThreshold );
 int filter1_filterInChunks( filter1Type * pThis, short * pInput, short * pOutput, int length );
#endif // FILTER1_H_
