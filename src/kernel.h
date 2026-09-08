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

#pragma once

#include <stddef.h>
#include <stdint.h>


/*
 *	These example values of the FLS110's C1, C2, and C3
 *	are taken from the screenshot on page 6 of FL-000986-TN-7, 2022-01-30.
 */
#define kFlussoFLS110SensorCalibrationConstant1 (2499.26)
#define kFlussoFLS110SensorCalibrationConstant2 (117682.20)
#define kFlussoFLS110SensorCalibrationConstant3 (-314364.00)

#define kFlussoFLS110DefaultInputVariableHxferUniformDistLow    (0.010)
#define kFlussoFLS110DefaultInputVariableHxferUniformDistHigh   (0.050)

#define kFlussoFLS110DefaultInputVariableTflowUniformDistLow    (293.0)
#define kFlussoFLS110DefaultInputVariableTflowUniformDistHigh   (294.0)

#define kFlussoFLS110DefaultInputVariableT0UniformDistLow   (273.0)
#define kFlussoFLS110DefaultInputVariableT0UniformDistHigh  (273.5)

#define kFlussoFLS110DefaultInputVariablePflowUniformDistLow    (420000.00)
#define kFlussoFLS110DefaultInputVariablePflowUniformDistHigh   (425000.00)

#define kFlussoFLS110DefaultInputVariableP0UniformDistLow   (400000.00)
#define kFlussoFLS110DefaultInputVariableP0UniformDistHigh  (405000.00)

/*
 *	Input Variables:
 *		kInputVariableIndexHxfer	: Heat power transfer (in Watt)
 *		kInputVariableIndexTflow	: Flow temperature (in Kelvin)
 *		kInputVariableIndexT0		: Temperature at time 0 (in Kelvin)
 *		kInputVariableIndexPflow	: Flow pressure (in Pascal)
 *		kInputVariableIndexP0		: Pressure at time 0 (in Pascal)
 */
typedef enum
{
	kFlussoFLS110InputVariableIndexHxfer    = 0,
	kFlussoFLS110InputVariableIndexTflow    = 1,
	kFlussoFLS110InputVariableIndexT0       = 2,
	kFlussoFLS110InputVariableIndexPflow    = 3,
	kFlussoFLS110InputVariableIndexP0       = 4,
	kFlussoFLS110InputVariableIndexMax,
} FlussoFLS110InputVariableIndex;

/*
 *	Output Variables:
 *		kOutputVariableIndexCalibratedMassFlowOutput		: Mass flow (in sccm)
 *		kOutputVariableIndexCalibratedDifferentialPressureOutput	: Differential Pressure (in Pascal)
 */
typedef enum
{
	kFlussoFLS110OutputVariableIndexCalibratedMassFlowOutput                = 0,
	kFlussoFLS110OutputVariableIndexCalibratedDifferentialPressureOutput    = 1,
	kFlussoFLS110OutputVariableIndexMax,
} FlussoFLS110OutputVariableIndex;

/**
 *	@brief	Sensor calibration routines taken from the screenshot on page 6 of
 *		FL-000986-TN-7, 2022-01-30.
 *
 *	@param	outputVariables	: An array of output variables. Writes the result to `outputVariables[outputSelect]` (or fills all outputs when `outputSelect == kFlussoFLS110OutputVariableIndexMax`).
 */
double
FlussoFLS110_calculateOutput(
	uint8_t     outputSelect,
	double *    inputVariables,
	double *    outputVariables);

/**
 *	@brief	Set the FLS110 input variables by sampling each of the sensor's
 *		distributional input parameters via `UxHwDoubleUniformDist()`.
 *		This one call is shared, unmodified, by both the UxHw dispatch
 *		kernel (where it yields a full uniform distribution per input)
 *		and the Monte Carlo dispatch kernel (where, run once per
 *		iteration, it yields one sampled value per input).
 *
 *	@param	inputVariables	: Array of size `kFlussoFLS110InputVariableIndexMax` to fill.
 */
void
FlussoFLS110_setInputVariablesViaUxHwCall(double * inputVariables);

/**
 *	@brief	Perform a single evaluation of the FLS110 conversion routine:
 *		sample the sensor's distributional inputs once via
 *		`FlussoFLS110_setInputVariablesViaUxHwCall()` and run the
 *		conversion routine on them once. `UxHwDoubleUniformDist` returns
 *		the full input distributions in UxHw mode, yielding a fully
 *		distributional result. In Monte Carlo (compat) mode it returns a
 *		single sample per input, yielding one Monte Carlo sample result.
 *		This is the one shared per-evaluation helper that both
 *		`flussoFLS110UxHw()` and `flussoFLS110MonteCarlo()` call.
 *
 *	@param	outputSelect		: Which output to calculate (or `kFlussoFLS110OutputVariableIndexMax` for all).
 *	@param	outputVariables		: Array of size `kFlussoFLS110OutputVariableIndexMax` to fill.
 *	@return	double			: Returns the value of the selected output (or the differential pressure output when all outputs are selected).
 */
double
flussoFLS110SingleEvaluation(uint8_t outputSelect, double * outputVariables);

/**
 *	@brief	UxHw-mode calculation kernel. Delegates to `flussoFLS110UxHw()`
 *		for a single distributional evaluation. Writes per-output results
 *		into `outputVariables` and the single distributional result into
 *		`monteCarloOutputSamples[0]`.
 *
 *	@param	outputSelect		: Which output to calculate (or `kFlussoFLS110OutputVariableIndexMax` for all).
 *					  A `size_t`, so that the caller passes
 *					  `CommonCommandLineArguments.outputSelect`
 *					  through unnarrowed: the narrowing to the
 *					  `uint8_t` below stays in `kernel.c`.
 *	@param	outputVariables		: Array of size `kFlussoFLS110OutputVariableIndexMax` to fill.
 *	@param	monteCarloOutputSamples	: Single-element array for the distributional result.
 *	@return	double			: Returns the value of the selected output (or the differential pressure output when all outputs are selected).
 */
double
flussoFLS110CalculateOutputUxHw(
	size_t      outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples);

/**
 *	@brief	Monte Carlo calculation kernel. Delegates to
 *		`flussoFLS110MonteCarlo()`, which runs
 *		`numberOfMonteCarloIterations` independent evaluations into
 *		`monteCarloOutputSamples`. Writes per-output results (from the
 *		final iteration) into `outputVariables`.
 *
 *	@param	numberOfMonteCarloIterations	: Number of Monte Carlo iterations to run.
 *	@param	outputSelect		: Which output to calculate (or `kFlussoFLS110OutputVariableIndexMax` for all).
 *					  A `size_t`, so that the caller passes
 *					  `CommonCommandLineArguments.outputSelect`
 *					  through unnarrowed: the narrowing to the
 *					  `uint8_t` below stays in `kernel.c`.
 *	@param	outputVariables		: Array of size `kFlussoFLS110OutputVariableIndexMax` to fill.
 *	@param	monteCarloOutputSamples	: Array of `numberOfMonteCarloIterations` doubles, filled with samples.
 *	@return	double			: Returns the value of the selected output for the final iteration.
 */
double
flussoFLS110CalculateOutputMonteCarlo(
	size_t      numberOfMonteCarloIterations,
	size_t      outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples);
