/***********************************************************************************
 * File                     :TS_DspDrv_SCI.h
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
#ifndef TS_DSPDRV_SCI_INC_TS_DSPDRV_SCI_H_
#define TS_DSPDRV_SCI_INC_TS_DSPDRV_SCI_H_


void TS_DspCom_SciFifoConfig(void);
void TS_DspCom_SciPinsConfig(void);
void TS_DspCom_SendWork(Uint16 p_u16Send);
void TS_DspCom_Error(void);
void TS_DspDrv_SciCom(TS_DSPDRV_PHYSICAL_VALUE p_ePhysicalValue);
void TS_DspDrv_SciReadBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length);
void TS_DspDrv_SciWriteBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length);



#endif /* TS_DSPDRV_SCI_INC_TS_DSPDRV_SCI_H_ */
