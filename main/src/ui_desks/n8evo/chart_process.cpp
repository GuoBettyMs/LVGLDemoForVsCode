/**
 * @file chart_process.c
 *
 */


#include "TChartUpdator.h"
#include "chart_process.h"
// #include "hor_page_main.h"


#define   WAVE_SIZE										384
static int16_t 	   g_asChanXPoints[36][WAVE_SIZE];
TChartUpdator g_tChartUpdator[36] =
{
	TChartUpdator(g_asChanXPoints[0], sizeof(g_asChanXPoints[0]) / sizeof(g_asChanXPoints[0][0]), 500),
	TChartUpdator(g_asChanXPoints[1], sizeof(g_asChanXPoints[1]) / sizeof(g_asChanXPoints[1][0]), 500),
	TChartUpdator(g_asChanXPoints[2], sizeof(g_asChanXPoints[2]) / sizeof(g_asChanXPoints[2][0]), 500),
	TChartUpdator(g_asChanXPoints[3], sizeof(g_asChanXPoints[3]) / sizeof(g_asChanXPoints[3][0]), 500),
	TChartUpdator(g_asChanXPoints[4], sizeof(g_asChanXPoints[4]) / sizeof(g_asChanXPoints[4][0]), 500),
	TChartUpdator(g_asChanXPoints[5], sizeof(g_asChanXPoints[5]) / sizeof(g_asChanXPoints[5][0]), 500),
	TChartUpdator(g_asChanXPoints[6], sizeof(g_asChanXPoints[6]) / sizeof(g_asChanXPoints[6][0]), 500),
	TChartUpdator(g_asChanXPoints[7], sizeof(g_asChanXPoints[7]) / sizeof(g_asChanXPoints[7][0]), 500),
	TChartUpdator(g_asChanXPoints[8], sizeof(g_asChanXPoints[8]) / sizeof(g_asChanXPoints[8][0]), 500),
	TChartUpdator(g_asChanXPoints[9], sizeof(g_asChanXPoints[9]) / sizeof(g_asChanXPoints[9][0]), 500),
	TChartUpdator(g_asChanXPoints[10], sizeof(g_asChanXPoints[10]) / sizeof(g_asChanXPoints[10][0]), 500),
	TChartUpdator(g_asChanXPoints[11], sizeof(g_asChanXPoints[11]) / sizeof(g_asChanXPoints[11][0]), 500),
	TChartUpdator(g_asChanXPoints[12], sizeof(g_asChanXPoints[12]) / sizeof(g_asChanXPoints[12][0]), 500),
	TChartUpdator(g_asChanXPoints[13], sizeof(g_asChanXPoints[13]) / sizeof(g_asChanXPoints[13][0]), 500),
	TChartUpdator(g_asChanXPoints[14], sizeof(g_asChanXPoints[14]) / sizeof(g_asChanXPoints[14][0]), 500),
	TChartUpdator(g_asChanXPoints[15], sizeof(g_asChanXPoints[15]) / sizeof(g_asChanXPoints[15][0]), 500),
	TChartUpdator(g_asChanXPoints[16], sizeof(g_asChanXPoints[16]) / sizeof(g_asChanXPoints[16][0]), 500),
	TChartUpdator(g_asChanXPoints[17], sizeof(g_asChanXPoints[17]) / sizeof(g_asChanXPoints[17][0]), 500),
	TChartUpdator(g_asChanXPoints[18], sizeof(g_asChanXPoints[18]) / sizeof(g_asChanXPoints[18][0]), 500),
	TChartUpdator(g_asChanXPoints[19], sizeof(g_asChanXPoints[19]) / sizeof(g_asChanXPoints[19][0]), 500),
	TChartUpdator(g_asChanXPoints[20], sizeof(g_asChanXPoints[20]) / sizeof(g_asChanXPoints[20][0]), 500),
	TChartUpdator(g_asChanXPoints[21], sizeof(g_asChanXPoints[21]) / sizeof(g_asChanXPoints[21][0]), 500),
	TChartUpdator(g_asChanXPoints[22], sizeof(g_asChanXPoints[22]) / sizeof(g_asChanXPoints[22][0]), 500),
	TChartUpdator(g_asChanXPoints[23], sizeof(g_asChanXPoints[23]) / sizeof(g_asChanXPoints[23][0]), 500),
	TChartUpdator(g_asChanXPoints[24], sizeof(g_asChanXPoints[24]) / sizeof(g_asChanXPoints[24][0]), 500),
	TChartUpdator(g_asChanXPoints[25], sizeof(g_asChanXPoints[25]) / sizeof(g_asChanXPoints[25][0]), 500),
	TChartUpdator(g_asChanXPoints[26], sizeof(g_asChanXPoints[26]) / sizeof(g_asChanXPoints[26][0]), 500),
	TChartUpdator(g_asChanXPoints[27], sizeof(g_asChanXPoints[27]) / sizeof(g_asChanXPoints[27][0]), 500),
	TChartUpdator(g_asChanXPoints[28], sizeof(g_asChanXPoints[28]) / sizeof(g_asChanXPoints[28][0]), 500),
	TChartUpdator(g_asChanXPoints[29], sizeof(g_asChanXPoints[29]) / sizeof(g_asChanXPoints[29][0]), 500),
	TChartUpdator(g_asChanXPoints[30], sizeof(g_asChanXPoints[30]) / sizeof(g_asChanXPoints[30][0]), 500),
	TChartUpdator(g_asChanXPoints[31], sizeof(g_asChanXPoints[31]) / sizeof(g_asChanXPoints[31][0]), 500),
	TChartUpdator(g_asChanXPoints[32], sizeof(g_asChanXPoints[32]) / sizeof(g_asChanXPoints[32][0]), 500),
	TChartUpdator(g_asChanXPoints[33], sizeof(g_asChanXPoints[33]) / sizeof(g_asChanXPoints[33][0]), 500),
	TChartUpdator(g_asChanXPoints[34], sizeof(g_asChanXPoints[34]) / sizeof(g_asChanXPoints[34][0]), 500),
	TChartUpdator(g_asChanXPoints[35], sizeof(g_asChanXPoints[35]) / sizeof(g_asChanXPoints[35][0]), 500),
};


TChartUpdator* GetChartUpdator(uint8_t channel)
{
	if (channel >= 36)
	{
		channel = 0;
	}
	return &g_tChartUpdator[channel];
}


void ChannelChartDataProcess(uint8_t channel,int16_t voltage,uint8_t is_reset)
{
	TChartUpdator *ptChartUpdator = GetChartUpdator(channel);

	if(is_reset)
		ptChartUpdator->reset();
	else
		ptChartUpdator->push(voltage);

}

uint16_t GetChannelChartMinData(uint8_t channel)
{
	TChartUpdator *ptChartUpdator = GetChartUpdator(channel);
	return ptChartUpdator->getMinValue();
}

uint16_t GetChannelChartMaxData(uint8_t channel)
{
	TChartUpdator *ptChartUpdator = GetChartUpdator(channel);
	return ptChartUpdator->getMaxValue();
}

uint32_t GetChannelSampleCnt(uint8_t channel)
{
	TChartUpdator *ptChartUpdator = GetChartUpdator(channel);
	return ptChartUpdator->getSampleCnt();
}

uint16_t GetChannelPointData(uint8_t channel, uint32_t index, uint32_t width, int16_t sign)
{
	TChartUpdator *ptChartUpdator = GetChartUpdator(channel);
	return ptChartUpdator->getYPos(index, width, sign);
}


