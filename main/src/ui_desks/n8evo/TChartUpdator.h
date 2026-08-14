/*
 * TChartUpdator.h
 *
 *  Created on: 2020年10月31日
 *      Author: Barlader
 */

#ifndef USER_UI_TCHARTUPDATOR_H_
#define USER_UI_TCHARTUPDATOR_H_

#include <stdint.h>

class TChartUpdator {
	#define NULL	nullptr
	#define ABS(x)	(x) < 0 ? (-(x)) : (x)
	enum {
		DEFAULT_SAMPLE_TIME_MS	= 500,
		DEFAULT_MAX_VALUE	= -32768,
		DEFAULT_MIN_VALUE	= 32767,
		DEFAULT_CHART_MAX_VALUE		= 99,
		DEFAULT_CHART_MIN_VALUE		= 6
	};
	typedef int16_t (*fun_ptr_t)(void);

public:
	TChartUpdator(int16_t *pDataPoints, uint32_t size, uint32_t time_base);
	virtual ~TChartUpdator();

	void reset(void);
	void clear(void);
	void push(int16_t data);
	int16_t getYPos(int16_t x, int16_t width, int16_t sign);
	int16_t getMaxValue(void);
	int16_t getMinValue(void);
	uint32_t getSampleCnt(void);
	uint32_t getSampleInterval(void);

	uint32_t m_beginSampleCnt;
private:
	uint32_t m_timeBase;
	uint32_t m_sampleCnt;
	uint32_t m_sampleInterval;
	uint32_t m_sampleTimeCnt;
	int16_t  m_maxValue;
	int16_t  m_minValue;
	int16_t *m_dataPoints;
	uint32_t m_dataSize;
};

#endif /* USER_UI_TCHARTUPDATOR_H_ */
