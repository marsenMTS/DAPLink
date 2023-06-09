/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the MAX32670
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

#include "flash_blob.h"

#define FLC_BASE        0x40029000
#define CLOCK_DIV       0x00000060
#define BURST_SIZE      0x00000080
#define FLASH_BASE      0x10000000
#define FLASH_SIZE      KB(384)
#define FLASH_SECTOR    KB(8)

static const uint32_t flash_algo_blob[] = {
    0xE00ABE00, 
    0x44494975, 0x68826808, 0x0f07f012, 0x2001bf1c, 0x68494770, 0x20006041, 0x486f4770, 0x68004448,
    0xf0216881, 0x60814170, 0x47702000, 0xb510486a, 0x68044448, 0xf8b4f000, 0x68a0b9b0, 0x407ff420,
    0x402af440, 0x68a060a0, 0x0002f040, 0x68a060a0, 0x0f02f010, 0x68a0d1fb, 0x4070f020, 0x6a6060a0,
    0x0002f010, 0xbd10bf08, 0xbd102001, 0x4605b570, 0x44484859, 0xf0006804, 0xb9b8f893, 0xf42068a0,
    0xf440407f, 0x60a040aa, 0x68a06025, 0x0004f040, 0x68a060a0, 0x0f04f010, 0x68a0d1fb, 0x4070f020,
    0x6a6060a0, 0x0002f010, 0xbd70bf08, 0xbd702001, 0x41f0e92d, 0x8120f8df, 0x44c8460d, 0xf8d84606,
    0x46174000, 0x0f03f012, 0xf000d167, 0x2800f869, 0xe00dd163, 0xf8576026, 0x63200b04, 0xf04068a0,
    0x60a00001, 0xf01068a0, 0xd1fb0f01, 0x1d361f2d, 0xd3022d04, 0x0f1ff016, 0xf8d8d1ec, 0x28800008,
    0x2d10d11a, 0x6026d318, 0x0b04f857, 0xf8576320, 0x63600b04, 0x0b04f857, 0xf85763a0, 0x63e00b04,
    0xf04068a0, 0x60a00001, 0xf01068a0, 0xd1fb0f01, 0x36103d10, 0xd2e62d10, 0xd30f2d04, 0xf8576026,
    0x63200b04, 0xf04068a0, 0x60a00001, 0xf01068a0, 0xd1fb0f01, 0x1d361f2d, 0xd2ef2d04, 0x683ab19d,
    0x31fff04f, 0xea6220ff, 0x40190300, 0x1e6d0200, 0x6026d1f9, 0x68a06321, 0x0001f040, 0x68a060a0,
    0x0f01f010, 0x68a0d1fb, 0x4070f020, 0x6a6060a0, 0x0002f010, 0xe8bdbf08, 0x200181f0, 0x81f0e8bd,
    0x4448480d, 0x68816800, 0x0f07f011, 0x6a41d109, 0x2100b149, 0x6a416241, 0x2102b129, 0x6a416241,
    0x2001b109, 0x68814770, 0x4170f021, 0x5100f041, 0x20006081, 0x00004770, 0x00000004, 0x00000000,
    FLC_BASE, CLOCK_DIV, BURST_SIZE,
};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
    FLASH_BASE, FLASH_SECTOR,
};

static const program_target_t flash = {
    0x20000005, // Init
    0x2000001f, // UnInit
    0x20000031, // EraseChip
    0x20000071, // EraseSector
    0x200000b5, // ProgramPage
    0x120000003, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000001,
        0x200001e0,
        0x20000400
    },

    0x20000000 + 0x00000A00,  // mem buffer location
    0x20000000,               // location to write prog_blob in target RAM
    sizeof(flash_algo_blob),   // prog_blob size
    flash_algo_blob,           // address of prog_blob
    0x00002000       // ram_to_flash_bytes_to_be_written
};
