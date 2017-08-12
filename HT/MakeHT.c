#include <stdint.h>
#include <stdio.h>
#include "MakeHT.h"

void MakeHT(uint16_t rgnET[NCrts*NCrds*NRgns], uint16_t hfET[NCrts*NHFRgns], uint16_t HT[1]) {
#pragma HLS PIPELINE II=6
#pragma HLS ARRAY_PARTITION variable=rgnET complete dim=0
#pragma HLS ARRAY_PARTITION variable=hfET complete dim=0
#pragma HLS ARRAY_PARTITION variable=HT complete dim=0
	uint16_t rgnHT = 0;
	uint16_t hfHT = 0;
	for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
#pragma HLS UNROLL
	  uint16_t j = (iRgn % (NCrds*NRgns));
	  uint16_t et = rgnETLUT[rgnET[iRgn]][j];
	  if(et > MinETCutForHT) rgnHT += et;
	  else rgnHT += 0;
	}
	for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
#pragma HLS UNROLL
	  uint16_t j = (iHFRgn % (NHFRgns));
	  uint16_t et = hfETLUT[hfET[iHFRgn]][j];
	  if(et > MinHFETCutForHT) hfHT += et;
	  else hfHT += 0;
	}
	HT[0] = rgnHT + hfHT;
}
