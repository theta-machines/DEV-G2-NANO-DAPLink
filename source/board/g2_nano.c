/**
 * @file    g2_nano.c
 * @brief   board ID for the Theta Machines DEV-G2-NANO board
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

#include "target_board.h"
#include "target_family.h"

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

    // CMSIS-DAP Board Strings
    .board_vendor = "Theta Machines",
    .board_name = "DEV-G2-NANO",
};
