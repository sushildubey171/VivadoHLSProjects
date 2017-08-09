#include <stdint.h>
#include "MakeHT.h"

void MakeHT(uint16_t rgnET[NCrts*NCrds*NRgns], uint16_t hfET[NCrts*NHFRgns], uint16_t HT[1]) {
#pragma HLS PIPELINE II=6
#pragma HLS ARRAY_PARTITION variable=rgnET complete dim=0
#pragma HLS ARRAY_PARTITION variable=hfET complete dim=0
#pragma HLS ARRAY_PARTITION variable=HT complete dim=0
	uint16_t rgnHT = 0;
	uint16_t hfHT = 0;
iRgn:
	for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
#pragma HLS UNROLL
		if(rgnET[iRgn] > MinETCutForHT) rgnHT += rgnET[iRgn];
		else rgnHT += 0;
	}
iHFRgn:
	for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
#pragma HLS UNROLL
		if(hfET[iHFRgn] > MinHFETCutForHT) hfHT += hfET[iHFRgn];
		else hfHT += 0;
	}
	HT[0] = rgnHT + hfHT;
}
