/*
 *	Copyright (c) 2024-2026, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include <stddef.h>
#include <stdint.h>
#include "flusso-fls110-monte-carlo.h"
#include "kernel.h"

double
flussoFLS110MonteCarlo(
	size_t      numberOfMonteCarloIterations,
	uint8_t     outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples)
{
	double result = 0.0;

	for (size_t ii = 0; ii < numberOfMonteCarloIterations; ii++)
	{
		/*
		 *	Set input distribution values, inside the Monte Carlo
		 *	loop, so that the same call that yields a full
		 *	distribution on real UxHw hardware instead yields one
		 *	freshly sampled value per input each iteration here.
		 */
		result = flussoFLS110SingleEvaluation(outputSelect, outputVariables);

		monteCarloOutputSamples[ii] = result;
	}

	return result;
}
