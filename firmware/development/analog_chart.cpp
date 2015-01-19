/**
 * @file	analog_chart.cpp
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "analog_chart.h"
#include "rpm_calculator.h"
#include "status_loop.h"
#include "engine_configuration.h"

#if EFI_ANALOG_CHART || defined(__DOXYGEN__)

static char LOGGING_BUFFER[5000];
static Logging logging;

static int pendingData = FALSE;
static int initialized = FALSE;

extern engine_configuration_s *engineConfiguration;

void acAddData(float angle, float value) {
	if (!initialized) {
		return; // this is possible because of initialization sequence
	}

	if (engineConfiguration->analogChartFrequency < 1) {
		//todofirmwareError()
		return;
	}

	if (getRevolutionCounter() % engineConfiguration->analogChartFrequency != 0) {
		if (pendingData) {
			// message terminator
			appendPrintf(&logging, DELIMETER);
			// output pending data
			if (getFullLog()) {
				scheduleLogging(&logging);
			}
			pendingData = false;
		}
		return;
	}
	if (!pendingData) {
		pendingData = true;
		resetLogging(&logging);
		// message header
		appendPrintf(&logging, "analog_chart%s", DELIMETER);
	}

	if (remainingSize(&logging) > 100) {
		appendPrintf(&logging, "%f|%f|", angle, value);
	}
}

static void setAnalogChartFrequency(int value) {
	engineConfiguration->analogChartFrequency = value;
}

void initAnalogChart(void) {
	initLoggingExt(&logging, "analog chart", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	addConsoleActionI("set_analog_chart_freq", setAnalogChartFrequency);

	initialized = true;
}

#endif /* EFI_ANALOG_CHART */
