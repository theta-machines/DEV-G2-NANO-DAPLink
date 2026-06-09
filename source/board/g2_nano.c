/**
 * @file    g2_nano.c
 * @brief   board ID and custom CMSIS-DAP vendor commands for the
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

#include "DAP.h"
#include "DAP_config.h"
#include "IO_Config_Override.h"
#include "target_board.h"
#include "target_family.h"

// BOOT_MODE command constants
#define COMMAND_ID_BOOT_MODE        0xA0
#define OPERATION_GET_MODE          0x00
#define OPERATION_SERIAL_DOWNLOADER 0x01
#define OPERATION_INTERNAL_BOOT     0x02

// General status values
#define STATUS_SUCCESS              0x00
#define STATUS_FAILURE              0x01



/*
    Returns the value of BOOT_MODE[1:0] pins
*/
static uint8_t get_boot_mode() {
    uint8_t value_0 = (PIN_BOOT_MODE_0_GPIO->PDOR & PIN_BOOT_MODE_0) ? 0b01 : 0b00;
    uint8_t value_1 = (PIN_BOOT_MODE_1_GPIO->PDOR & PIN_BOOT_MODE_1) ? 0b10 : 0b00;
    return value_0 | value_1;
}



/*
    Sets the BOOT_MODE[1:0] pins to the last two bits of `state`
*/
static void set_boot_mode(uint8_t state) {
    (state & 0b01) ? (PIN_BOOT_MODE_0_GPIO->PSOR = PIN_BOOT_MODE_0) : (PIN_BOOT_MODE_0_GPIO->PCOR = PIN_BOOT_MODE_0);
    (state & 0b10) ? (PIN_BOOT_MODE_1_GPIO->PSOR = PIN_BOOT_MODE_1) : (PIN_BOOT_MODE_1_GPIO->PCOR = PIN_BOOT_MODE_1);
}



/*
    Board-specific initialization code
*/
static void prerun_board_config() {
    // Configure pins as GPIO
    PIN_BOOT_MODE_0_PORT->PCR[PIN_BOOT_MODE_0_BIT] = PORT_PCR_MUX(1);
    PIN_BOOT_MODE_1_PORT->PCR[PIN_BOOT_MODE_1_BIT] = PORT_PCR_MUX(1);

    // Configure pins as output
    PIN_BOOT_MODE_0_GPIO->PDDR |= PIN_BOOT_MODE_0;
    PIN_BOOT_MODE_1_GPIO->PDDR |= PIN_BOOT_MODE_1;

    // Initialize BOOT_MODE[1:0] pins
    set_boot_mode(OPERATION_INTERNAL_BOOT);
}



/*
    Handles custom CMSIS-DAP vendor commands.

    This function overrides the definition in source/daplink/cmsis-dap/DAP.c
*/
uint32_t DAP_ProcessVendorCommandEx(const uint8_t* request, uint8_t* response) {
    // Keep track of number of request and response bytes consumed
    uint16_t num_request_bytes = 0;
    uint16_t num_response_bytes = 0;

    switch(request[0]) {
        case COMMAND_ID_BOOT_MODE: {
            // Status defaults to failure
            uint8_t status = STATUS_FAILURE;

            // This command consumes two request bytes (the ID and operation)
            num_request_bytes = 2;

            // Parse operation byte
            uint8_t operation_byte = request[1];
            if(operation_byte == OPERATION_GET_MODE) {
                status = STATUS_SUCCESS;
            }
            else if(operation_byte == OPERATION_SERIAL_DOWNLOADER
                || operation_byte == OPERATION_INTERNAL_BOOT) {
                    // Set BOOT_MODE[1:0] to value of operation byte
                    set_boot_mode(operation_byte);

                    // Reset the target microcontroller
                    PIN_nRESET_OUT(0); // Assert RESET_N
                    Delayms(10);
                    PIN_nRESET_OUT(1); // Release RESET_N

                    status = STATUS_SUCCESS;
            }

            // Build response
            response[0] = COMMAND_ID_BOOT_MODE;
            response[1] = get_boot_mode();
            response[2] = status;
            num_response_bytes = 3;

            break;
        }

        default: {
            num_request_bytes = 1;

            response[0] = ID_DAP_Invalid;
            num_response_bytes = 1;

            break;
        }
    }

    return (num_request_bytes << 16) | num_response_bytes;
}



const board_info_t g_board_info = {
    .info_version = 1,
    .family_id = kNXP_Mimxrt_FamilyID,
    .board_id = "TMG2",
    .flags = kEnablePageErase,
    .target_cfg = NULL,

    // MSD Customization
    .daplink_url_name = "G2-NANO HTM",
    .daplink_drive_name = "DEV-G2-NANO",
    .daplink_target_url = "https://www.thetamachines.com/",

    // Board initialization
    .prerun_board_config = prerun_board_config,

    // CMSIS-DAP Board Strings
    .board_vendor = "Theta Machines",
    .board_name = "DEV-G2-NANO",
};
