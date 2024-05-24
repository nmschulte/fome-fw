/**
 * @file	rusEfiFunctionalTest.h
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void rusEfiFunctionalTest(char const * const socketcanDevice);
void printPendingMessages(void);
void logMsg(const char *fmt, ...);
