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

// Begin header file, filter0.h
// 500-100Hz Band Pass 1st Order

#ifndef FILTER0_H_ // Include guards
#define FILTER0_H_

static const int filter0_numStages = 1;
static const int filter0_coefficientLength = 5;
extern float filter0_coefficients[5];

typedef struct
{
	float state[4];
	float output;
} filter0Type;

typedef struct
{
	float *pInput;
	float *pOutput;
	float *pState;
	float *pCoefficients;
	short count;
} filter0_executionState;


filter0Type *filter0_create( void );
void filter0_destroy( filter0Type *pObject );
 void filter0_init( filter0Type * pThis );
 void filter0_reset( filter0Type * pThis );
#define filter0_writeInput( pThis, input )  \
	filter0_filterBlock( pThis, &input, &pThis->output, 1 );

#define filter0_readOutput( pThis )  \
	pThis->output

 int filter0_filterBlock( filter0Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define filter0_outputToFloat( output )  \
	(output)

#define filter0_inputFromFloat( input )  \
	(input)

 void filter0_filterBiquad( filter0_executionState * pExecState );
#endif // FILTER0_H_
