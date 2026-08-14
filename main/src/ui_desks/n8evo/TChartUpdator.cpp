/*
 * TChartUpdator.cpp
 *
 *  Created on: 2020年10月31日
 *      Author: Barlader
 */

#include "TChartUpdator.h"
#include <string.h>

TChartUpdator::TChartUpdator(int16_t *pDataPoints, uint32_t size, uint32_t time_base) {
	this->m_dataPoints = pDataPoints;
	this->m_dataSize = size;
	this->m_timeBase = time_base;
	this->reset();
}


TChartUpdator::~TChartUpdator() {

}

void TChartUpdator::push(int16_t data)
{
	if (m_dataPoints == NULL)	return;

	/* Counter time. */
	m_sampleTimeCnt += m_timeBase;
	if (m_sampleTimeCnt < m_sampleInterval) 		return;
	m_sampleTimeCnt = 0;

	if (m_sampleCnt >= m_dataSize)	m_sampleCnt = m_dataSize - 1;

	int16_t index = m_sampleCnt;

	if (m_maxValue < data)	m_maxValue = data;
	if (m_minValue > data)	m_minValue = data;

	m_dataPoints[index] = data;
	m_sampleCnt++;

	/* Compress the data array. */
	if (m_sampleCnt >= m_dataSize)
	{
		int len = m_dataSize / 2;

		/* Get the average of value. */
		for (int i = 0, index = i; i < len; i++, index += 2)
		{
			m_dataPoints[i] = (m_dataPoints[index] + m_dataPoints[index+1]) / 2;
		}
		m_sampleCnt = len;
		m_sampleInterval *= 2;
	}
}

int16_t TChartUpdator::getYPos(int16_t x, int16_t width, int16_t sign)
{
	int index;

	if (m_sampleCnt >= (m_dataSize / 2)) {
		index = x * m_sampleCnt / width;
	}
	else {
		index = x;
	}

	if (m_maxValue == m_minValue)
	{
		if (sign == -1) m_minValue -= 10; else m_maxValue += 10;		
	}

	int data = (int)m_dataPoints[index];
	int delta = ABS(data - m_minValue);
	int offset = ABS(m_maxValue - m_minValue);
	int yPos = 0;

	if (sign == -1) 	delta = ABS(data - m_maxValue);

//	if (offset < DEFAULT_CHART_MAX_VALUE)
//	{
//		yPos = delta;
//	}
//	else
	{
		yPos = delta * 100 / offset;
	}

	if (sign == -1)		yPos = ABS(DEFAULT_CHART_MAX_VALUE - yPos);
	if (yPos == 0 || yPos < DEFAULT_CHART_MIN_VALUE)	yPos = DEFAULT_CHART_MIN_VALUE;
	return yPos;
}

void TChartUpdator::reset(void)
{
	m_sampleCnt = 0;
	m_sampleInterval = DEFAULT_SAMPLE_TIME_MS;
	m_maxValue = DEFAULT_MAX_VALUE;
	m_minValue = DEFAULT_MIN_VALUE;
	m_beginSampleCnt = 0;
}

void TChartUpdator::clear(void)
{
	if (m_dataPoints == nullptr)		return;
	memset(m_dataPoints, 0, m_dataSize);
}

int16_t TChartUpdator::getMaxValue(void)
{
	return m_maxValue;
}

int16_t TChartUpdator::getMinValue(void)
{
	return m_minValue;
}

uint32_t TChartUpdator::getSampleCnt()
{
	return m_sampleCnt;
}

uint32_t TChartUpdator::getSampleInterval(void)
{
	return m_sampleInterval;
}
