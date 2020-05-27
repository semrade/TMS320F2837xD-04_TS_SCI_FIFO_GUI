/***********************************************************************************
 * File                     :TS_DspDrv_SCI_X.h
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :
 *
 * Version                  :
 *
 * Copyright (c) 2020 Tarik SEMRADE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/
#ifndef TS_DSPDRV_SCI_TS_DSPDRV_SCI_X_H_
#define TS_DSPDRV_SCI_TS_DSPDRV_SCI_X_H_

#include "F2837xD_device.h"

enum TS_DSPDRV_PHYSICAL_VALUE
{
    TS_DSPDRV_CURRENT,
    TS_DSPDRV_VOLTAGE,
    TS_DSPDRV_TEMP,
    TS_DSPDRV_TEST_COM,

    TS_DSPDRV_MAX
};
/**********************************************************************************
 *  External prototypes
 *
 *********************************************************************************/
extern void TS_DspDrv_SciFifoConfig(void);
extern void TS_DspDrv_SciPinsConfig(void);
extern void TS_DspDrv_SciCom(enum TS_DSPDRV_PHYSICAL_VALUE p_ePhysicalValue);
extern void TS_DspDrv_SciReadBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length);
extern void TS_DspDrv_SciWriteBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length);
interrupt void TS_DspDrv_SciaTxFifoISR(void);
interrupt void TS_DspDrv_SciaRxFifoISR(void);
extern Uint16 t_u16RcvData[16];
extern Uint16 t_u16SendData[16];

#endif /* TS_DSPDRV_SCI_TS_DSPDRV_SCI_X_H_ */
