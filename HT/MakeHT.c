#include <stdint.h>
#include <stdio.h>
#include "MakeHT.h"

void MakeHT(uint16_t rgnET[NCrts*NCrds*NRgns], uint16_t hfET[NCrts*NHFRgns], uint16_t HT[3]) {
#pragma HLS PIPELINE II=6
#pragma HLS ARRAY_PARTITION variable=rgnET complete dim=0
#pragma HLS ARRAY_PARTITION variable=hfET complete dim=0
#pragma HLS ARRAY_PARTITION variable=HT complete dim=0
	uint32_t rgnHT = 0;
	uint32_t hfHT = 0;
	uint32_t sum = 0;
	int iRgn;
	int iHFRgn;
	for(iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
#pragma HLS UNROLL
	  uint16_t j = (iRgn % (NCrds*NRgns));
	  uint16_t et = rgnETLUT[rgnET[iRgn]][j];
	  if(et > MinETCutForHT) rgnHT += et;
	  else rgnHT += 0;
	  if(rgnHT > 0xFFFF) rgnHT = 0xFFFF;
	}
	for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
#pragma HLS UNROLL
	  uint16_t j = (iHFRgn % (NHFRgns));
	  uint16_t et = hfETLUT[hfET[iHFRgn]][j];
	  if(et > MinHFETCutForHT) hfHT += et;
	  else hfHT += 0;
	  if(hfHT > 0xFFFF) hfHT = 0xFFFF;
	}
	sum = rgnHT + hfHT;
	if(sum > 0xFFFF) sum = 0xFFFF;
	HT[0] = (uint16_t) sum;
	HT[1] = (uint16_t) rgnHT;
	HT[2] = (uint16_t) hfHT;
}
