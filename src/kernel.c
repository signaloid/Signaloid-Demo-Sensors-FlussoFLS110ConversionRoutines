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
#include <stdbool.h>
#include <math.h>
#include <uxhw.h>
#include "kernel.h"
#include "flusso-fls110-uxhw.h"
#include "flusso-fls110-monte-carlo.h"

void
FlussoFLS110_setInputVariablesViaUxHwCall(double * inputVariables)
{
	inputVariables[kFlussoFLS110InputVariableIndexHxfer] = UxHwDoubleUniformDist(
		kFlussoFLS110DefaultInputVariableHxferUniformDistLow,
		kFlussoFLS110DefaultInputVariableHxferUniformDistHigh
	);

	inputVariables[kFlussoFLS110InputVariableIndexTflow] = UxHwDoubleUniformDist(
		kFlussoFLS110DefaultInputVariableTflowUniformDistLow,
		kFlussoFLS110DefaultInputVariableTflowUniformDistHigh
	);

	inputVariables[kFlussoFLS110InputVariableIndexT0] = UxHwDoubleUniformDist(
		kFlussoFLS110DefaultInputVariableT0UniformDistLow,
		kFlussoFLS110DefaultInputVariableT0UniformDistHigh
	);

	inputVariables[kFlussoFLS110InputVariableIndexPflow] = UxHwDoubleUniformDist(
		kFlussoFLS110DefaultInputVariablePflowUniformDistLow,
		kFlussoFLS110DefaultInputVariablePflowUniformDistHigh
	);

	inputVariables[kFlussoFLS110InputVariableIndexP0] = UxHwDoubleUniformDist(
		kFlussoFLS110DefaultInputVariableP0UniformDistLow,
		kFlussoFLS110DefaultInputVariableP0UniformDistHigh
	);

	return;
}

double
FlussoFLS110_calculateOutput(uint8_t outputSelect, double *  inputVariables, double *  outputVariables)
{
	double  calibratedValue;
	double  m;
	double  h;

	double  Tflow;
	double  T0;
	double  Pflow;
	double  P0;

	bool calculateAllOutputs = (outputSelect == kFlussoFLS110OutputVariableIndexMax);

	h = inputVariables[kFlussoFLS110InputVariableIndexHxfer];

	/*
	 *	The calculation of mass flow is common in the two output calculations.
	 */
	m = kFlussoFLS110SensorCalibrationConstant3 * pow(h, 3) + kFlussoFLS110SensorCalibrationConstant2 * pow(h, 2) + kFlussoFLS110SensorCalibrationConstant1;

	if (calculateAllOutputs ||
	    (outputSelect == kFlussoFLS110OutputVariableIndexCalibratedMassFlowOutput))
	{
		calibratedValue = m;

		outputVariables[kFlussoFLS110OutputVariableIndexCalibratedMassFlowOutput] = calibratedValue;
	}

	if (calculateAllOutputs ||
	    outputSelect == kFlussoFLS110OutputVariableIndexCalibratedDifferentialPressureOutput)
	{
		Tflow   = inputVariables[kFlussoFLS110InputVariableIndexTflow];
		T0      = inputVariables[kFlussoFLS110InputVariableIndexT0];
		Pflow   = inputVariables[kFlussoFLS110InputVariableIndexPflow];
		P0      = inputVariables[kFlussoFLS110InputVariableIndexP0];

		calibratedValue = m * (Tflow / T0) * (P0 / Pflow);
		outputVariables[kFlussoFLS110OutputVariableIndexCalibratedDifferentialPressureOutput] = calibratedValue;
	}

	return calibratedValue;
}

double
flussoFLS110SingleEvaluation(uint8_t outputSelect, double * outputVariables)
{
	double inputVariables[kFlussoFLS110InputVariableIndexMax];

	/*
	 *	Set input distribution values via a single call: on real UxHw
	 *	hardware, `UxHwDoubleUniformDist()` yields a full distributional
	 *	value per input, so one call is all a distributional evaluation
	 *	needs. Run repeatedly (once per Monte Carlo iteration), it yields
	 *	one freshly sampled value per input each time.
	 */
	FlussoFLS110_setInputVariablesViaUxHwCall(inputVariables);

	return FlussoFLS110_calculateOutput(outputSelect, inputVariables, outputVariables);
}

double
flussoFLS110CalculateOutputUxHw(
	size_t      outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples)
{
	double result;

	result = flussoFLS110UxHw((uint8_t) outputSelect, outputVariables);

	monteCarloOutputSamples[0] = result;

	return result;
}

double
flussoFLS110CalculateOutputMonteCarlo(
	size_t      numberOfMonteCarloIterations,
	size_t      outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples)
{
	return flussoFLS110MonteCarlo(
		numberOfMonteCarloIterations,
		(uint8_t) outputSelect,
		outputVariables,
		monteCarloOutputSamples
	);
}
