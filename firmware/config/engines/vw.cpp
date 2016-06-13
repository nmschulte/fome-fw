/*
 * @file vw.cpp
 *
 * set_engine_type 32
 *
 * @date May 24, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "vw.h"
#include "custom_engine.h"
#include "ego.h"
#include "engine_math.h"

EXTERN_ENGINE;

void setVwAba(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	setWholeTimingTable(20 PASS_ENGINE_PARAMETER);
	engineConfiguration->cranking.baseFuel = 12;
	// set_cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 20;

	// set_whole_fuel_map 12
	setWholeFuelMap(12 PASS_ENGINE_PARAMETER);

	// set_global_trigger_offset_angle 90
	engineConfiguration->globalTriggerAngleOffset = 100;


	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->trigger.type = TT_60_2_VW;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;


	//Base engine setting
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.0;
	engineConfiguration->injector.flow = 320; // 30lb/h
	// set_algorithm 3
	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER);
	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	float mapRange = 110;

	setEgoSensor(ES_PLX PASS_ENGINE_PARAMETER);
	setFuelTablesLoadBin(20, mapRange PASS_ENGINE_PARAMETER);
	setTimingLoadBin(20, mapRange PASS_ENGINE_PARAMETER);

	boardConfiguration->isSdCardEnabled = false;
	engineConfiguration->tpsMin = 740;
	engineConfiguration->tpsMax = 135;
}
