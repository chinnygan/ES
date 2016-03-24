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

// Begin header file, filter2.h
// 2000-2500Hz Band Pass single order filter

#ifndef FILTER2_H_ // Include guards
#define FILTER2_H_

static const int filter2_numStages = 1;
static const int filter2_coefficientLength = 5;
extern float filter2_coefficients[5];

typedef struct
{
	float state[4];
	float output;
} filter2Type;

typedef struct
{
	float *pInput;
	float *pOutput;
	float *pState;
	float *pCoefficients;
	short count;
} filter2_executionState;


filter2Type *filter2_create( void );
void filter2_destroy( filter2Type *pObject );
 void filter2_init( filter2Type * pThis );
 void filter2_reset( filter2Type * pThis );
#define filter2_writeInput( pThis, input )  \
	filter2_filterBlock( pThis, &input, &pThis->output, 1 );

#define filter2_readOutput( pThis )  \
	pThis->output

 int filter2_filterBlock( filter2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define filter2_outputToFloat( output )  \
	(output)

#define filter2_inputFromFloat( input )  \
	(input)

 void filter2_filterBiquad( filter2_executionState * pExecState );
#endif // FILTER2_H_
