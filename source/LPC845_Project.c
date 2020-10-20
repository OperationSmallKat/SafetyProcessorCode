/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    LPC845_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */


static void InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(GPIO, BOARD_INITBUTTONSPINS_powerState_PORT, BOARD_INITBUTTONSPINS_powerState_PIN, &gpioPinConfigStruct);

    SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, DEMO_PINT_PIN_INT0_SRC);

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */
}


int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    InitWakeupPin();

    //enable 3.3v & 6v
    GPIO_PinWrite(BOARD_INITLEDSPINS_en_6v_GPIO, BOARD_INITLEDSPINS_en_6v_PORT, BOARD_INITLEDSPINS_en_6v_PIN, 1);
    GPIO_PinWrite(BOARD_INITLEDSPINS_en_3v_GPIO, BOARD_INITLEDSPINS_en_3v_PORT, BOARD_INITLEDSPINS_en_3v_PIN, 1);

    //init FUSB303 //I2C 2
    //init BQ76925 //I2C 3
    //init BQ24770 //I2c 1
    //start balancing on BQ76925

    uint8_t conn_to_pwr = 0;
    while(1) {
    	//poll fusb303 for connected to power status
    	if(conn_to_pwr){
    		//start charging
    	}
    }
    return 0 ;
}

void Wakeup_btn(pint_pin_int_t pintr ,uint32_t pmatch_status){
	//disable 3.3v & 6v

	GPIO_PinWrite(BOARD_INITLEDSPINS_en_6v_GPIO, BOARD_INITLEDSPINS_en_6v_PORT, BOARD_INITLEDSPINS_en_6v_PIN, 0);
	GPIO_PinWrite(BOARD_INITLEDSPINS_en_3v_GPIO, BOARD_INITLEDSPINS_en_3v_PORT, BOARD_INITLEDSPINS_en_3v_PIN, 0);

    POWER_EnterDeepPowerDownMode();

}
