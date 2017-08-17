#ifndef MakeHT_h
#define MakeHT_h

#define NCrts 18
#define NCrds 7
#define NRgns 2
#define NHFRgns 8

#define MinETCutForHT 10
#define MinHFETCutForHT 10

#include <stdint.h>
#ifdef Vivado
#include "ap_int.h"
#define uint12_t ap_uint<12>
#else
#define uint12_t uint16_t
#endif

#include "LUTs.h"

void MakeHT(uint12_t rgnET[NCrts*NCrds*NRgns], uint12_t hfET[NCrts*NHFRgns], uint12_t HT[1]);

#endif
