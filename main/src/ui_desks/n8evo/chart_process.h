/**
 * @file chart_process.h
 *
 */

#ifndef __CHART_PROCESS_H
#define __CHART_PROCESS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 *
 */
void ChannelChartDataProcess(uint8_t channel,int16_t voltage,uint8_t is_reset);

/**
 *	Get point data.
 */
uint16_t GetChannelPointData(uint8_t channel, uint32_t index, uint32_t width, int16_t sign);

/**
 *  Get the num of the sample times.
 */
uint32_t GetChannelSampleCnt(uint8_t channel);

/**
 * Get the mininum of the voltage.
 */
uint16_t GetChannelChartMinData(uint8_t channel);

/**
 * Get the maxium of the voltage.
 */
uint16_t GetChannelChartMaxData(uint8_t channel);

#ifdef __cplusplus
}
#endif
#endif
