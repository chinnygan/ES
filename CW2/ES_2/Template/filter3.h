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

// Begin header file, filter3.h

// 3000-3750Hz Band Pass single order filter

#ifndef FILTER3_H_ // Include guards
#define FILTER3_H_

static const int filter3_numStages = 1;
static const int filter3_coefficientLength = 5;
extern float filter3_coefficients[5];

typedef struct
{
	float state[4];
	float output;
} filter3Type;

typedef struct
{
	float *pInput;
	float *pOutput;
	float *pState;
	float *pCoefficients;
	short count;
} filter3_executionState;


filter3Type *filter3_create( void );
void filter3_destroy( filter3Type *pObject );
 void filter3_init( filter3Type * pThis );
 void filter3_reset( filter3Type * pThis );
#define filter3_writeInput( pThis, input )  \
	filter3_filterBlock( pThis, &input, &pThis->output, 1 );

#define filter3_readOutput( pThis )  \
	pThis->output

 int filter3_filterBlock( filter3Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define filter3_outputToFloat( output )  \
	(output)

#define filter3_inputFromFloat( input )  \
	(input)

 void filter3_filterBiquad( filter3_executionState * pExecState );
#endif // FILTER3_H_
