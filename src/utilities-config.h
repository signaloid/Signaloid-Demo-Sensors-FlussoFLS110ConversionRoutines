/*
 *	Copyright (c) 2024, Signaloid.
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

/*
 *	These example values of the FLS110's C1, C2, and C3
 *	are taken from the screenshot on page 6 of FL-000986-TN-7, 2022-01-30.
 */
#define kSensorCalibrationConstant1					(2499.26)
#define kSensorCalibrationConstant2					(117682.20)
#define kSensorCalibrationConstant3					(-314364.00)

#define kDefaultInputDistributionHxferUniformDistLow			(0.010)
#define kDefaultInputDistributionHxferUniformDistHigh			(0.050)

#define kDefaultInputDistributionTflowUniformDistLow			(293.0)
#define kDefaultInputDistributionTflowUniformDistHigh			(294.0)

#define kDefaultInputDistributionT0UniformDistLow			(273.0)
#define kDefaultInputDistributionT0UniformDistHigh			(273.5)

#define kDefaultInputDistributionPflowUniformDistLow			(420000.00)
#define kDefaultInputDistributionPflowUniformDistHigh			(425000.00)

#define kDefaultInputDistributionP0UniformDistLow			(400000.00)
#define kDefaultInputDistributionP0UniformDistHigh			(405000.00)

/*
 *	Input Distributions:
 *		kInputDistributionIndexHxfer	: Heat power transfer (in Watt)
 *		kInputDistributionIndexTflow	: Flow temperature (in Kelvin)
 *		kInputDistributionIndexT0	: Temperature at time 0 (in Kelvin)
 *		kInputDistributionIndexPflow	: Flow pressure (in Pascal)
 *		kInputDistributionIndexP0	: Pressure at time 0 (in Pascal)
 */
typedef enum
{
	kInputDistributionIndexHxfer					= 0,
	kInputDistributionIndexTflow					= 1,
	kInputDistributionIndexT0					= 2,
	kInputDistributionIndexPflow					= 3,
	kInputDistributionIndexP0					= 4,
	kInputDistributionIndexMax,
} InputDistributionIndex;

/*
 *	Output Distributions:
 *		kOutputDistributionIndexCalibratedMassFlowOutput		: Mass flow (in sccm)
 *		kOutputDistributionIndexCalibratedDifferentialPressureOutput	: Differential Pressure (in Pascal)
 */
typedef enum
{
	kOutputDistributionIndexCalibratedMassFlowOutput		= 0,
	kOutputDistributionIndexCalibratedDifferentialPressureOutput	= 1,
	kOutputDistributionIndexMax,
} OutputDistributionIndex;
