/*
  Copyright 2018 dhq
  ZYNQ RGB controller
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_io.h"
#include "Delay.h"

#include "platform.h"
#include "xuartlite.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "Delay.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UARTLITE_DEVICE_ID		XPAR_UARTLITE_0_DEVICE_ID
#define MY_PWM XPAR_RGB_PWM_0_S00_AXI_BASEADDR //This value is found in the Address editor tab in Vivado (next to Diagram tab)


int UartLiteSelfTestExample(u16 DeviceId);
void MainDemoPrintMenu(void);

XUartLite UartLite;		 /* Instance of the UartLite device */


#define TEST_BUFFER_SIZE 16
uint8_t ReceiveBuffer[TEST_BUFFER_SIZE] = "\r\n";	/* Buffer for Transmitting Data */



int main()
{
    init_platform();
    char buf[16] = {};
    int ret = 0;
    unsigned char num = 0;
    unsigned char ledidx = 1;

    //size_t len = sizeof(buf)/sizeof(buf[0]);
    int Status;

	/*
	 * Run the UartLite self test example, specify the Device ID that is
	 * generated in xparameters.h
	 */
	Status = UartLiteSelfTestExample(UARTLITE_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Uartlite selftest Example Failed\r\n");
		//return XST_FAILURE;
	}

	xil_printf("Successfully ran Uartlite selftest Example\r\n");

	XUartLite_DisableInterrupt(&UartLite);
	XUartLite_ResetFifos(&UartLite);
	MainDemoPrintMenu();

	Xil_Out32((MY_PWM), 50);
	Xil_Out32((MY_PWM+4), 50);
	Xil_Out32((MY_PWM+8), 50);

    	while (1) {

            int n = XUartLite_Recv(&UartLite, buf, 3);
    		if (n != 0) {
    		  // when a question character is received over serial
   			 ret = atoi(buf);
 			 num = (unsigned char)(ret);

    		xil_printf("RGB Duty cycle: %d \r\n", num);

    		if(ledidx > 3)
    		{
    			ledidx = 1;
    		}else
    		{
        		ledidx++;
    		}

    				switch (ledidx)
    				{
    				case 1:
    					xil_printf("Red LED setting. \r\n");
    			        Xil_Out32(MY_PWM, num);
    					break;
    				case 2:
    					xil_printf("Green LED setting. \r\n");
    			        Xil_Out32((MY_PWM+4), num);
    					break;
    				case 3:
    					xil_printf("Blue LED setting. \r\n");
    			        Xil_Out32((MY_PWM+8), num);

    					break;
    				case 'q':
    					xil_printf("Reset RGB. \r\n");
    					//XUartLite_Send(&UartLite, buf, 1);
    			        Xil_Out32((MY_PWM), 50);
    			        Xil_Out32((MY_PWM+4), 50);
    			        Xil_Out32((MY_PWM+8), 50);

    					break;

    				}
    			}

    		}


    cleanup_platform();
    return 0;
}



/*****************************************************************************/
/**
*
* This function does a minimal test on the UartLite device and driver as a
* design example. The purpose of this function is to illustrate
* how to use the XUartLite component.
*
*
* @param	DeviceId is the XPAR_<uartlite_instance>_DEVICE_ID value from
*		xparameters.h.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
int UartLiteSelfTestExample(u16 DeviceId)
{
	int Status;

	/*
	 * Initialize the UartLite driver so that it is ready to use.
	 */
	Status = XUartLite_Initialize(&UartLite, DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XUartLite_SelfTest(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


void MainDemoPrintMenu(void)
{
	//xil_printf("\x1B[H"); //Set cursor to top left of terminal
	//xil_printf("\x1B[2J"); //Clear terminal
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("*         Minized  RGB console                   *\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("\n\r");
	xil_printf("1 - Red LED Duty Cycle \n\r");
	xil_printf("2 - Green LED Duty Cycle \n\r");
	xil_printf("3 - Blue LED Duty Cycle \n\r");
	xil_printf("q - Quit\n\r");
	xil_printf("\n\r");
	xil_printf("Select a demo to run:");
}
