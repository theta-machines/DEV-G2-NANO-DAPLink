/**
 * @file    g2_nano_bl.c
 * @brief   bootloader initialization code and overrides for the
 *          Theta Machines DEV-G2-NANO board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2026, Theta Machines LLC
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "IO_Config_Override.h"

/*
    Board-specific initialization code (executed by bootloader)

    This function overrides the definition in
    source/daplink/bootloader/main_bootloader.c
*/
void board_bootloader_init() {
    // Configure pins as GPIO
    PIN_BOOT_MODE_0_PORT->PCR[PIN_BOOT_MODE_0_BIT] = PORT_PCR_MUX(1);
    PIN_BOOT_MODE_1_PORT->PCR[PIN_BOOT_MODE_1_BIT] = PORT_PCR_MUX(1);

    // Configure pins as output
    PIN_BOOT_MODE_0_GPIO->PDDR |= PIN_BOOT_MODE_0;
    PIN_BOOT_MODE_1_GPIO->PDDR |= PIN_BOOT_MODE_1;

    // Initialize BOOT_MODE[1:0] pins to 0b10 (internal boot mode)
    PIN_BOOT_MODE_0_GPIO->PCOR = PIN_BOOT_MODE_0;
    PIN_BOOT_MODE_1_GPIO->PSOR = PIN_BOOT_MODE_1;

    // Add pull-up resistor to PIN_nRESET
    PIN_nRESET_PORT->PCR[PIN_nRESET_BIT] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}
