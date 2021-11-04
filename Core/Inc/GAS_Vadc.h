/*
 * GAS_Vadc.h
 *
 *  Created on: 2020. 10. 30.
 *      Author: Suprhimp
 */

#ifndef INC_GAS_VADC_H_
#define INC_GAS_VADC_H_

#include "dma.h"
#include "adc.h"

extern volatile uint32_t ValueOfADC[2];
extern void GAS_Vadc_dmaIn(void);
//extern void GAS_Vadc_getValue(void);

#endif /* INC_GAS_VADC_H_ */
