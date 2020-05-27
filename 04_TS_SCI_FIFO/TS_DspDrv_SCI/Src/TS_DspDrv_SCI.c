/***********************************************************************************
 * File                     :TS_DspDrv_SCI.c
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

/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/
#include "device.h"
#include "F2837xD_device.h"
#include "F2837xD_sci.h"
#include "F2837xD_Gpio_defines.h"
#include "F2837xD_GlobalPrototypes.h"
#include "TS_DspDrv_SCI_X.h"
#include "TS_SysMng_X.h"

/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/

#define LSPCLK_FREQ     (DEVICE_SYSCLK_FREQ/4)          /* Default value at reset */
#define SCI_FREQ        460800U                         /* Change the Bauds Speed */
#define SCI_PRD         ((LSPCLK_FREQ/(SCI_FREQ*8))-1)  /* Baud Rate LSB bit */


#define FIFO_SIZE       16U

/* Received data for SCI-A */
Uint16 t_u16RcvData[FIFO_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0 };
/* Send data for SCI-A */
Uint16 t_u16SendData[FIFO_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0 };


/**********************************************************************************
 * \function:       TS_DspCom_SciFifoConfig
 * \brief           TS_DspCom_SciFifoConfig `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciFifoConfig(void)
{

    SciaRegs.SCICCR.all = 0x0007;

    /*
     * 1 stop bit,  No loopback
     * No parity,8 char bits,
     * async mode, idle-line protocol
     * enable TX, RX, internal SCICLK,
     * Disable RX ERR, SLEEP, TXWAKE
     * */

    SciaRegs.SCICTL1.all = 0x0003;

    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    SciaRegs.SCIHBAUD.all = ((uint16_t)SCI_PRD  & 0xFF00U) >> 8U;
    SciaRegs.SCILBAUD.all = (uint16_t)SCI_PRD  & 0x00FFU;

    SciaRegs.SCIFFTX.all = 0xC030;
    SciaRegs.SCIFFRX.all = 0x0030;
    SciaRegs.SCIFFCT.all = 0x00;

    /* Relinquish SCI from Reset */
    SciaRegs.SCICTL1.all = 0x0023;
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

}
/**********************************************************************************
 * \function:       TS_DspCom_Error
 * \brief           TS_DspCom_Error `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_DspDrv_Error(void)
{
    asm("     ESTOP0"); // Test failed!! Stop!
    for (;;);
}
/**********************************************************************************
 * \function:       TS_DspCom_Error
 * \brief           TS_DspCom_Error `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_DspDrv_Sci(void)
{
    /*
     * Note: Clocks were turned on to the SCIA peripheral
     * in the InitSysCtrl() function
     * */

    SciaRegs.SCICCR.all = 0x0007;
    /*
     * 1 stop bit,  No loopback
     * No parity,8 char bits,
     * async mode, idle-line protocol
     * enable TX, RX, internal SCICLK,
     * Disable RX ERR, SLEEP, TXWAKE
     * */

    SciaRegs.SCICTL1.all = 0x0003;

    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    SciaRegs.SCIHBAUD.all = ((uint16_t) SCI_PRD & 0xFF00U) >> 8U;
    SciaRegs.SCILBAUD.all = (uint16_t) SCI_PRD & 0x00FFU;
    SciaRegs.SCICTL1.all = 0x0023;     // Relinquish SCI from Reset
}
/**********************************************************************************
 * \function:       TS_DspCom_SciTransChar
 * \brief           TS_DspCom_SciTransChar `1` numbers
 * \param[in]       Uint16
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciTransChar(Uint16 p_u16Char)
{
    SciaRegs.SCITXBUF.all = (p_u16Char & 0x00FF);
}
/**********************************************************************************
 * \function:       TS_DspCom_SciTransChar
 * \brief           TS_DspCom_SciTransChar `1` numbers
 * \param[in]       Uint16
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciTransUint16(Uint16 p_u16Data)
{
    SciaRegs.SCITXBUF.all = ((p_u16Data & 0xFF00) >> 8U);
    SciaRegs.SCITXBUF.all = (p_u16Data & 0x00FF);
}
/**********************************************************************************
 * \function:       TS_DspCom_SciTransChar
 * \brief           TS_DspCom_SciTransChar `1` numbers
 * \param[in]       Uint16
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciCom(enum TS_DSPDRV_PHYSICAL_VALUE p_ePhysicalValue)
{

    switch (p_ePhysicalValue)
    {

    case TS_DSPDRV_CURRENT:

        /* Update currents */
        t_u16SendData[1] = (Uint16) (I1 * 1000);
        t_u16SendData[2] = (Uint16) (I2 * 1000);
        t_u16SendData[3] = (Uint16) (I3 * 1000);

        /* Send data to replay if asked */
        TS_DspDrv_SciWriteBuffer(&t_u16RcvData[0], 16u);

        break;
    case TS_DSPDRV_VOLTAGE:

        /* Update voltages */
        t_u16SendData[1] = (Uint16) (V1 * 1000);
        t_u16SendData[2] = (Uint16) (V2 * 1000);
        t_u16SendData[3] = (Uint16) (V3 * 1000);

        /* Send data to replay if asked */
        TS_DspDrv_SciWriteBuffer(&t_u16RcvData[0], 16u);

        break;
    case TS_DSPDRV_TEMP:

        /* Update temperature */
        t_u16SendData[1] = s_i16SensorTemp;

        /* Send data to replay if asked */
        TS_DspDrv_SciWriteBuffer(&t_u16RcvData[0], 16u);

        break;
    case TS_DSPDRV_TEST_COM:

        /* Send data to replay if asked */
        TS_DspDrv_SciWriteBuffer(&t_u16RcvData[0], 16u);


        break;
    default:
        break;
    }
}
/**********************************************************************************
 * \function:       TS_DspDrv_SciReadBuffer
 * \brief           TS_DspDrv_SciReadBuffer `2` numbers
 * \param[in]       int16 * p_pu16Buffer, Uint16 p_u16Length)
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciReadBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length)
{
    Uint16 l_u16Index;

    for (l_u16Index = 0; l_u16Index < p_u16Length; l_u16Index++)
    {
        /* Read data */
        p_pu16Buffer[l_u16Index] = (SciaRegs.SCIRXBUF.all & 0x00FF);
    }
}
/**********************************************************************************
 * \function:       TS_DspDrv_SciWriteBuffer
 * \brief           TS_DspDrv_SciWriteBuffer `2` numbers
 * \param[in]       int16 * p_pu16Buffer, Uint16 p_u16Length)
 * \return          void
 **********************************************************************************/
void TS_DspDrv_SciWriteBuffer(Uint16 * p_pu16Buffer, Uint16 p_u16Length)
{
    Uint16 l_u16Index;

    /* Send the received table */
    for (l_u16Index = 0; l_u16Index < p_u16Length; l_u16Index++)
    {
        /* Send data */
        SciaRegs.SCITXBUF.all = p_pu16Buffer[l_u16Index];
    }

}
/**********************************************************************************
 * \function:       TS_DspDrv_ReadFromCom
 * \brief           TS_DspDrv_ReadFromCom `1` numbers
 * \param[in]       p_u16InputData
 * \return          void
 **********************************************************************************/
void TS_DspDrv_ReadFromCom(Uint16 *p_u16InputData)
{
    Uint16 l_tu16Receiver[FIFO_SIZE];
    Uint16 l_u16i, l_u16j;

    TS_DspDrv_SciReadBuffer(&l_tu16Receiver[0], 16);

    for (l_u16i = 0; l_u16i < FIFO_SIZE/2; l_u16i = l_u16i + 1)
    {
        for (l_u16j = 0; l_u16j < FIFO_SIZE; l_u16j++)
        {
            p_u16InputData[l_u16i] = (Uint16) ((l_tu16Receiver[l_u16j] & 0x00FF)
                    | (l_tu16Receiver[l_u16j + 1] & 0x00FF) << 8);
        }

    }
}
/**********************************************************************************
 * \function:       TS_DspDrv_WriteToCom
 * \brief           TS_DspDrv_WriteToCom `1` numbers
 * \param[in]       p_u16OutputData
 * \return          void
 **********************************************************************************/
void TS_DspDrv_WriteToCom(Uint16 * p_u16OutputData)
{
    Uint16 l_tu16Sender[FIFO_SIZE];
    Uint16 l_u16i, l_u16j;

    /*  write to local buffer */
    for (l_u16i = 0; l_u16i < FIFO_SIZE; l_u16i = l_u16i + 1)
    {
        for (l_u16j = 0; l_u16j < FIFO_SIZE/2; l_u16j++)
        {
            l_tu16Sender[l_u16i] = p_u16OutputData[l_u16j] & 0xFF00;
            l_tu16Sender[l_u16i + 1] = p_u16OutputData[l_u16j] & 0x00FF;
        }

    }

    /* call sci write function */
    TS_DspDrv_SciWriteBuffer(&l_tu16Sender[0], 16);
}

/**********************************************************************************
 * \function:       Scia_Rx_Fifo_ISR
 * \brief           Scia_Rx_Fifo_ISR `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void TS_DspDrv_SciaRxFifoISR(void)
{

    TS_DspDrv_SciReadBuffer(&t_u16RcvData[0], 16u);

    bSendData = true;

    /* Clear Overflow flag */
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

    /* Clear Interrupt flag */
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;

    /*  Issue PIE ACK */
    PieCtrlRegs.PIEACK.bit.ACK9 = 0x1;
}
/**********************************************************************************
 * \function:       Scia_Tx_Fifo_ISR
 * \brief           Scia_Tx_Fifo_ISR `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void TS_DspDrv_SciaTxFifoISR(void)
{
    Uint16 l_u16Index;

    for (l_u16Index = 0; l_u16Index < FIFO_SIZE; l_u16Index++)
    {
        /* Send data */
        SciaRegs.SCITXBUF.all = t_u16SendData[l_u16Index];
    }

    /* Clear SCI Interrupt flag */
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;

    /*  Issue PIE ACK */
    PieCtrlRegs.PIEACK.bit.ACK9 = 0x1;
}
