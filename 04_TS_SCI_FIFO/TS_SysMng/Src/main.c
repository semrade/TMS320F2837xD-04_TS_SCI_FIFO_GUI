/***********************************************************************************
 * File                     :main.c
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :Serial Communication Interface tutorial
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
#include "TS_DspDrv_SCI_X.h"
#include "TS_DspDrv_GPIO_X.h"
#include "TS_DspDrv_ADC_X.h"
#include "F28x_Project.h"
#include "main.h"
#include "device.h"
#include "gpio.h"
#include "math.h"
/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/
#define FIFO_SIZE       16U
#define PI              3.14159265359
#define ADC_RESOLUTION_SIGNALMODE 4095
/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/


/* Used for checking the received data */
Uint16 rdata_pointA;
Uint16 u16Index;
bool bSendData = false;

/* sin and cos wave form simulation */
float32 I1,I2,I3;
float32 V1,V2,V3;
float32 rIamp = 1;
float32 rVamp = 1;


float32 rFreq;
float32 rTheta;
Uint16 u16Theta = 0u;

/* 100 us */
Uint16 u16SamplePeriod = 100;
float32 rTsample = 100E-6;

/* Buffer data */
Uint16 t_u16InputData[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
Uint16 t_u16OutputData[8] = { 0, 0, 0, 0, 0, 0, 0, 0};

/* Temperature measurement */
Uint16 s_u16SensorSample;
int16 s_i16SensorTemp;
/**********************************************************************************
 * \function:       main
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void main(void)
{

    /* Set up system flash and turn peripheral clocks */
    InitSysCtrl();

    /* Init all gpio to input */
    InitGpio();

    /* Globally disable maskable CPU interrupts */
    DINT;

    /* Clear and disable all PIE interrupts */
    InitPieCtrl();

    /* Individually disable maskable CPU interrupts */
    IER = 0x0000;

    /* Clear all CPU interrupt flags */
    IFR = 0x0000;

    /* Populate the PIE interrupt vector table */
    InitPieVectTable();

    /***********************Interrupt linking functions*****************************/
    EALLOW;

    /* Interrupt for RX interrupt service routine */
    PieVectTable.SCIA_RX_INT = &TS_DspDrv_SciaRxFifoISR;

    /* Interrupt for TX interrupt service routine */
    //PieVectTable.SCIA_TX_INT = &TS_DspDrv_SciaTxFifoISR;
	
	/* Timer0 ISR function linking */
	PieVectTable.TIMER0_INT = &Timer0_ISR;

    EDIS;
    /************************Peripheral Initialization*****************************/

    /* PIE Group 9, INT1 */
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;

    /* PIE Group 9, INT2 */
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 1;

    /* Enable TINT0 in the PIE: Group 1 interrupt 7 */
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	
	/* Enable group 1 interrupts */
    IER |= M_INT1;
	
    /* Enable CPU INT */
    IER |= M_INT9;

    /* Enable the PIE block */
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    EINT;

    /* FIFO configuration */
    TS_DspDrv_SciFifoConfig();

    /* GPIO configuration for SCI */
    TS_DspDrv_SciPinsConfig();

    /* GPIO Led init */
    TS_DspDrv_GpioLedInit();

    /* ADC Reset */
    TS_SysMng_AdcConfig();

    /* Initialize the temperature sensor */
    InitTempSensor(3.0);

    /* Initialize all timer with default values */
    InitCpuTimers();

    /* 200MHz CPU Freq, 50 uSeconds Period (in uSeconds) */
    ConfigCpuTimer(&CpuTimer0, 200, 100);

    /* Start Timer0 */
    StartCpuTimer0();
	
	
    /* Infinite led loop */
    while (1)
    {

    }

}
/**********************************************************************************
 * \function:       Timer0_ISR
 * \brief           `0` Param
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void Timer0_ISR(void)
{

    /* Clear Timer interrupt flag */
    CpuTimer0Regs.TCR.bit.TIF = 0;

    /* Increment timer0 interrupt counter */
    CpuTimer0.InterruptCount++;

    /* Wait acquisition ending*/
    TS_SysMng_AdcAcqEnding();

    /* Store the ADC result Lsb */
    s_u16SensorSample = AdcaResultRegs.ADCRESULT0;

    /* Convert to temp using GetTemperaturesC function */
    s_i16SensorTemp = GetTemperatureC(s_u16SensorSample);

    /* led blinking with =! frequency */
    if (5000UL == CpuTimer0.InterruptCount)
    {
        /* Toggle Red Led with 250ms = 5 000 * 50us*/
        GPIO_togglePin(RED_LED);

    }

    if (10000UL == CpuTimer0.InterruptCount)
    {
        /* Toggle Blue Led with 500ms = 10000 * 50us*/
        GPIO_togglePin(BLUE_LED);

        /* restart interrupt counter here please */
        CpuTimer0.InterruptCount = 0;
    }

    /* Compute angle depending of frequency */
    ComputTheta(rFreq);

    /* Current simulation for SCI Test */
    I1 = rIamp * sinf(rTheta);
    I2 = rIamp * sinf(rTheta + 2 * PI / 3);
    I3 = rIamp * sinf(rTheta + 4 * PI / 3);

    /* Voltage simulation for SCI Test */
    V1 = rVamp * cosf(rTheta);
    V2 = rVamp * cosf(rTheta + 2 * PI / 3);
    V3 = rVamp * cosf(rTheta + 4 * PI / 3);

    if (true == bSendData)
    {
        /* Call the function from the xcom */
        TS_DspDrv_SciCom(t_u16InputData[0]);

        /* Disable Sending data */
        bSendData = false;
    }

    /* Acknowledge this interrupt to receive more interrupts from group 1 */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}
/**********************************************************************************
 * \function:       ComputTheta
 * \brief           ComputTheta `1` Param
 * \param[in]       frequency
 * \return          void
 **********************************************************************************/
void ComputTheta(Uint16 frequency)
{

    float32 step;
    /* Sinus waves */



    step = 65535 * rTsample * frequency/(2*PI);
    u16Theta =(Uint16)(u16Theta + step);
    rTheta = 2 * PI * (u16Theta / 65535);

}
