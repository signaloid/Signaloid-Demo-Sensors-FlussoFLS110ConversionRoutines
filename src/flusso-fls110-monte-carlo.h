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
#include <stdlib.h>

/**
 *	@brief	Run `numberOfMonteCarloIterations` independent evaluations of the
 *		FLS110 conversion routine, one per Monte Carlo sample (each drawing
 *		its own fresh set of sensor inputs), writing each result into
 *		`monteCarloOutputSamples`.
 *
 *	@param	numberOfMonteCarloIterations	: Number of Monte Carlo iterations to run.
 *	@param	outputSelect			: Which output to calculate (or `kFlussoFLS110OutputVariableIndexMax` for all).
 *	@param	outputVariables			: Array of size `kFlussoFLS110OutputVariableIndexMax`,
 *						  filled with the result of the final iteration.
 *	@param	monteCarloOutputSamples		: A pointer to an array where the selected output's
 *						  sample for each iteration is stored.
 *	@return	double				: Returns the value of the selected output for the final iteration.
 */
double
flussoFLS110MonteCarlo(
	size_t      numberOfMonteCarloIterations,
	uint8_t     outputSelect,
	double *    outputVariables,
	double *    monteCarloOutputSamples);
