/*
 * This file is part of the superiotool project.
 *
 * Copyright (C) 2010 Google Inc.
 * Written by David Hendricks <dhendrix@google.com> for Nuvoton Technology Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "superiotool.h"

#define DEVICE_ID_REG		0x20	/* Super I/O ID (SID) / family */
#define DEVICE_REV_REG		0x27	/* Super I/O revision ID (SRID) */

static const struct superio_registers reg_table[] = {
	{0xfc, "WPCE775x / NPCE781x", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
			 0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,EOT},
			{0xfc,0x11,RSVD,RSVD,RSVD,0x00,0x00,MISC,0x00,
			 0x04,RSVD,RSVD,RSVD,0x00,RSVD,RSVD,EOT}},
		{0x03, "CIR Port (CIRP)",	/* where supported */
			{0x30,0x60,0x61,0x70,0x71,0x74,0x75,0xf0,EOT},
			{0x00,0x03,0xf8,0x04,0x03,0x04,0x04,0x02,EOT}},
		{0x04, "Mobile System Wake-Up Control Config (MSWC)",
			{0x30,0x60,0x61,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x00,0x00,0x03,0x04,0x04,EOT}},
		{0x05, "Mouse config (KBC)",
			{0x30,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x0c,0x03,0x04,0x04,EOT}},
		{0x06, "Keyboard config (KBC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x60,0x00,0x64,0x01,0x03,0x04,0x04,EOT}},
		{0x0f, "Shared memory (SHM)",
			{0x30,0x60,0x61,0x70,0x71,0x74,0x75,0xf0,0xf1,0xf2,
			0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x04,0x04,MISC,0x07,RSVD,
			RSVD,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x11, "Power management I/F Channel 1 (PM1)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x62,0x00,0x66,0x01,0x03,0x04,0x04,EOT}},
		{0x12, "Power management I/F Channel 2 (PM2)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x68,0x00,0x6c,0x01,0x03,0x04,0x04,EOT}},
		{0x15, "Enhanced Wake On CIR (EWOC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x04,EOT}},
		{0x17, "Power Management I/F Channel 3 (PM3)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x71,0x74,0x75,EOT},
			{0x00,0x00,0x6a,0x00,0x6e,0x01,0x03,0x04,0x04,EOT}},
		{0x1a, "Serial Port with Fast Infrared Port (FIR)",
			{0x30,0x60,0x61,0x70,0x71,0x74,0x75,0xf0,EOT},
			{0x00,0x02,0xf8,0x03,0x03,0x04,0x04,0x02,EOT}},
		{EOT}}},
	{0x1a, "WPCM450", {
		{EOT}}},
	{0xb472, "NCT6775F (A)", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
			 0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,EOT},
			{0xb4,0x72,0xff,0x78,0x40,0x00,0x00,0x7d,0x00,
			 0x00,0x58,0x77,0xfc,0x04,0x00,MISC,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf4,
			 0xf5,EOT},
			{0x03,0x03,0xf0,0x06,0x02,0x8e,0x00,0xff,0x00,
			 0x00,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x07, "GPIO6, GPIO7, GPIO8, GPIO9",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xf4,0xf5,
			 0xf6,0xf7,0xf8,EOT},
			{0x18,0xff,0x00,0x00,0x00,0xef,0x00,0x00,0x00,
			 0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			 0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, GPIO0, GPIO1",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x00,0xff,0x00,0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO2, GPIO3, GPIO4, GPIO5",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,
			 0xf5,0xf6,0xf7,0xfe,EOT},
			{0x05,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xed,0xf2,0xf3,0xf4,0xf6,
			 0xf7,0xfe,EOT},
			{0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x00,
			 0x20,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,
			 0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,
			 0xfc,0xfd,0xfe,
			 EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0xc1,0x00,0x00,
			 0x00,0x00,0x10,0x00,0x87,0x47,0x00,0x00,0x00,
			 0x00,0x00,0x00,
			 EOT}},
		{0x0c, "PECI, SST",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xee,0xef,0xf1,0xf2,0xf3,0xfa,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0f,
			 0x47,0x5b,0x40,0x50,0x10,0x00,EOT}},
		{0x0d, "VID, VIDDAC, BUSSEL",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xeb,
			 0xec,0xed,0xee,0xef,0xf3,0xf4,0xf5,0xf6,0xf7,
			 0xf8,0xf9,EOT},
			{0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x14,
			 0x01,0x00,0x88,0x00,0x00,0x00,0x00,0x07,0x00,
			 0x00,0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf2,0xf3,0xf4,0xf8,0xfe,EOT},
			{0xff,0xff,0xf7,0xff,0xfb,0xcb,0xff,0xff,0xff,
			 0xff,0x00,0x00,0x00,0x00,0x70,0xff,EOT}},
		{EOT}}},
	{0xb473, "NCT6775F (B) / NCT5572D (B) (not all LDNs supported)", {
		{NOLDN, NULL,
			{0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
			 0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,EOT},
			{0xb4,0x73,0xff,0x78,0x40,0x00,0x00,0x7d,0x00,
			 0x00,0x58,0x77,0xfc,0x04,0x00,MISC,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf4,
			 0xf5,EOT},
			{0x03,0x03,0xf0,0x06,0x02,0x8e,0x00,0xff,0x00,
			 0x00,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,EOT},
			{0x01,0x03,0xf8,0x04,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x07, "GPIO6, GPIO7, GPIO8, GPIO9",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xf4,0xf5,
			 0xf6,0xf7,0xf8,EOT},
			{0x18,0xff,0x00,0x00,0x00,0xef,0x00,0x00,0x00,
			 0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			 0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, GPIO0, GPIO1",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x00,0xff,0x00,0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO2, GPIO3, GPIO4, GPIO5",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,
			 0xf5,0xf6,0xf7,0xfe,EOT},
			{0x05,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xed,0xf2,0xf3,0xf4,0xf6,
			 0xf7,0xfe,EOT},
			{0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x00,
			 0x20,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xf0,0xf1,0xf2,
			 0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,
			 0xfc,0xfd,0xfe,
			 EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0xc1,0x00,0x00,
			 0x00,0x00,0x10,0x00,0x87,0x47,0x00,0x00,0x00,
			 0x00,0x00,0x00,
			 EOT}},
		{0x0c, "PECI, SST",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xee,0xef,0xf1,0xf2,0xf3,0xfa,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0f,
			 0x47,0x5b,0x40,0x50,0x10,0x00,EOT}},
		{0x0d, "VID, VIDDAC, BUSSEL",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xeb,
			 0xec,0xed,0xee,0xef,0xf3,0xf4,0xf5,0xf6,0xf7,
			 0xf8,0xf9,EOT},
			{0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x14,
			 0x01,0x00,0x88,0x00,0x00,0x00,0x00,0x07,0x00,
			 0x00,0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf2,0xf3,0xf4,0xf8,0xfe,EOT},
			{0xff,0xff,0xf7,0xff,0xfb,0xcb,0xff,0xff,0xff,
			 0xff,0x00,0x00,0x00,0x00,0x70,0xff,EOT}},
		{EOT}}},
	{0xc332, "NCT6776F (B)", {
		{NOLDN, NULL,
			{0x10,0x11,0x13,0x14,0x16,0x17,0x18,0x19,0x1a,
			 0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,
			 0x24,0x25,0x26,0x27,0x28,0x2a,0x2b,0x2c,0x2d,
			 0x2e,0x2f,EOT},
			{0xff,0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xf0,
			 0x78,0x00,0x00,0xff,0xff,0xc3,0x32,0xff,0x00,
			 0x64,0x00,MISC,0x00,0x00,0xc0,0x00,0x81,0x00,
			 0x00,MISC,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf4,
			 0xf5,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x00,0xff,0x00,
			 0x00,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,EOT},
			{0x01,0x03,0xf8,0x04,0x00,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x00,0x00,0x00,0x08,0x09,0x32,0x00,EOT}},
		{0x07, "GPIO6, GPIO7, GPIO8, GPIO9",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xf4,0xf5,
			 0xf6,0xf7,0xf8,EOT},
			{0x03,0xff,0x00,0x00,0x00,0xef,0x00,0x00,0x00,
			 0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			 0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, GPIO0, GPIO1, GPIOA",
			{0x30,0x60,0x61,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x02,0x00,0x00,0xff,0x00,0x00,0x00,0xef,0xff,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO2, GPIO3, GPIO4, GPIO5",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,
			 0xf5,0xf6,0xf7,0xfe,EOT},
			{0x04,0xdf,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xee,0xf0,0xf2,0xf3,0xf4,0xf6,0xf7,0xfe,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xe0,0xe1,0xe2,
			 0xf0,0xf1,0xf2,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,
			 EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0xff,
			 0x00,0x00,0x00,0x10,0x00,0x87,0x47,0x00,0x00,
			 EOT}},
		{0x0d, "VID",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe9,0xee,
			 0xef,0xf0,0xf4,0xf5,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf1,0xf2,EOT},
			{0xff,0xdf,0xff,0xfe,0xf6,0xff,0xff,0xd3,0xff,
			 0x9f,0x00,0x00,0x00,EOT}},
		{0x14, "SVID",
			{0xe0,0xe1,0xe3,0xe4,EOT},
			{0x00,0x80,0x00,0x00,EOT}},
		{0x16, "Deep Sleep",
			{0x30,0xe0,0xe1,0xe2,EOT},
			{0x20,0x20,0x04,0x05,EOT}},
		{0x17, "GPIOA",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,EOT},
			{0x01,0x00,0x00,0x00,0x01,0x00,EOT}},
		{EOT}}},
	{0xc333, "NCT6776F/D (C)", {
		{NOLDN, NULL,
			{0x10,0x11,0x13,0x14,0x16,0x17,0x18,0x19,0x1a,
			 0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,
			 0x24,0x25,0x26,0x27,0x28,0x2a,0x2b,0x2c,0x2d,
			 0x2e,0x2f,EOT},
			{0xff,0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xf0,
			 0x78,0x00,0x00,0xff,0xff,0xc3,0x33,0xff,0x00,
			 0x64,0x00,MISC,0x00,0x00,0xc0,0x00,0x81,0x00,
			 0x00,MISC,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf4,
			 0xf5,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x00,0xff,0x00,
			 0x00,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,EOT},
			{0x01,0x03,0xf8,0x04,0x00,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x00,0x00,0x00,0x08,0x09,0x32,0x00,EOT}},
		{0x07, "GPIO6, GPIO7, GPIO8, GPIO9",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xf4,0xf5,
			 0xf6,0xf7,0xf8,EOT},
			{0x03,0xff,0x00,0x00,0x00,0xef,0x00,0x00,0x00,
			 0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
			 0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, GPIO0, GPIO1, GPIOA",
			{0x30,0x60,0x61,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x02,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO2, GPIO3, GPIO4, GPIO5",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,
			 0xf5,0xf6,0xf7,0xfe,EOT},
			{0x04,0xdf,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xee,0xf0,0xf2,0xf3,0xf4,0xf6,0xf7,0xfe,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,0x00,
			 0x00,0x00,0x5c,0x00,0x00,0x00,0xc0,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xe0,0xe1,0xe2,
			 0xf0,0xf1,0xf2,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,
			 EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0xff,
			 0x00,0x00,0x00,0x10,0x00,0x87,0x47,0x00,0x00,
			 EOT}},
		{0x0d, "VID",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe9,0xee,
			 0xef,0xf0,0xf4,0xf5,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
			 0xe9,0xf0,0xf1,0xf2,EOT},
			{0xff,0xdf,0xff,0xfe,0xf6,0xff,0xff,0xd3,0xff,
			 0x9f,0x00,0x00,0x00,EOT}},
		{0x14, "SVID",
			{0xe0,0xe1,0xe3,0xe4,EOT},
			{0x00,0x80,0x00,0x00,EOT}},
		{0x16, "Deep Sleep",
			{0x30,0xe0,0xe1,0xe2,EOT},
			{0x20,0x20,0x04,0x05,EOT}},
		{0x17, "GPIOA",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,EOT},
			{0x01,0x00,0x00,0x00,0x01,0x00,EOT}},
		{EOT}}},
	{0xc562, "NCT6779D", {
		{NOLDN, NULL,
			{0x10,0x11,0x13,0x14,0x1a,0x1b,0x1c,0x1d,0x20,
			 0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x2a,0x2b,
			 0x2c,0x2f,EOT},
			{0xff,0xff,0x00,0x00,0x30,0x70,0x10,0x00,0xc5,
			 0x62,0xff,0x04,0x00,MISC,0x00,0x00,0xc0,0x00,
			 0x01,MISC,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,EOT},
			{0x01,0x03,0xf8,0x04,0x00,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x00,0x00,0x00,0x08,0x09,0x32,0x00,EOT}},
		{0x07, "GPIO6, GPIO7, GPIO8",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xec,0xed,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x0f,0x00,0x00,0x00,0xff,0x00,0x00,0x00,
			 0x00,0x00,0xff,0x00,0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, GPIO0, GPIO1",
			{0x30,0x60,0x61,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,
			 0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
			 0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,
			 0xf5,0xf6,0xf7,0xfe,EOT},
			{0x00,0xff,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xee,0xf0,0xf2,0xf3,0xf4,0xf6,0xf7,0xfe,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,0x00,
			 0x00,0x10,0x5c,0x00,0x00,0x00,0xc0,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xe0,0xe1,0xe2,
			 0xe4,0xf0,0xf1,0xf2,0xf5,0xf6,0xf7,0xf8,0xf9,
			 0xfa,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0xff,
			 0xff,0x00,0x00,0x00,0x10,0x00,0x87,0x47,0x00,
			 0x00,EOT}},
		{0x0d, "WDT1",
			{0xf0,EOT},
			{0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,
			 0xf0,0xf1,0xf2,EOT},
			{0xff,0xff,0x7f,0xff,0xff,0xff,0x0f,0xff,0xff,
			 0x9d,0x00,0x00,EOT}},
		{0x14, "Port 80 UART",
			{0xe0,0xe1,0xe2,0xe3,0xe4,EOT},
			{0x80,0x00,0x00,0x10,0x00,EOT}},
		{0x16, "Deep Sleep",
			{0x30,0xe0,0xe1,0xe2,EOT},
			{0x20,0x20,0x04,0x05,EOT}},
		{EOT}}},
	{0xc452, "NCT6102D / NCT6106D", {
		{NOLDN, NULL,
			{0x07,0x10,0x11,0x13,0x14,0x1a,0x1b,0x20,0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2f,EOT},
			{0x00,0xff,0xff,0x00,0x00,0xcc,0x03,0x10,0x61,0x7F,0x00,0x00,MISC,0x00,0x00,0xf0,0x00,MISC,EOT}},
		{0x00, "FDC",
			{0x30,0x60,0x61,0x70,0x74,0xf0,0xf1,0xf2,0xf4,0xf5,EOT},
			{0x01,0x03,0xf0,0x06,0x02,0x0e,0x00,0xff,0x00,0x00,EOT}},
		{0x01, "PRT",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x01,0x03,0xf8,0x04,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x03, "UART B",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller (KBC)",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "CIR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x00,0x00,0x00,0x08,0x09,0x32,0x00,EOT}},
		{0x07, "GPIO",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,EOT},
			{0xdf,0xff,0x00,0x00,0x00,0xef,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x02,0x00,0x00,0x00,EOT}},
		{0x08, "GPIO,WDT1",
			{0x30,0x60,0x61,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xf0,0xf1,0xf2,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,EOT}},
		{0x09, "GPIO",
			{0xe0,0xe1,0xe2,EOT},
			{0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xfa,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x02,0x1c,0x00,0x80,0x00,0x40,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xe0,0xe1,0xe2,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0xff,0x00,0x00,0x00,0x00,0x00,0x87,0x47,0x00,EOT}},
		{0x0d, "WDT2",
			{0xe0,0xe1,0xe2,0xe3,0xe4,EOT},
			{0x00,0x32,0x14,0x00,0x00,EOT}},
		{0x0e, "CIR WAKE-UP",
			{0x30,0x60,0x61,0x70,0xe0,0xe1,EOT},
			{0x00,0x00,0x00,0x00,0x25,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-drain",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xf0,0xf1,0xf2,EOT},
			{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x03,0x9d,0x00,0x00,EOT}},
		{0x10, "UARTC",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x03,0x00,0x04,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x11, "UARTD",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x02,0x00,0x03,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x12, "UARTE",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x03,0xe8,0x04,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x13, "UARTF",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x02,0xe8,0x03,0x00,0x00,0x00,0xff,0xff,0x02,0x00,0x00,EOT}},
		{0x14, "PORT80 IR",
			{0x30,0x60,0x61,0x70,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,0xf1,EOT},
			{0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x10,0x00,0x00,0x00,EOT}},
		{0x15, "FADING LED",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,EOT},
			{0x00,0x3f,0x01,0x11,0x11,0x02,0x00,0x00,0x01,0x00,EOT}},
		{0x16, "Deep Sleep",
			{0x30,0xe0,0xe1,0xe2,EOT},
			{0x20,0x20,0x04,0x05,EOT}},
		{EOT}}},
	{0xc803, "NCT6791D", {
		{NOLDN, NULL,
			{0x07,0x10,0x11,0x13,0x14,0x1a,0x1b,0x1c,0x1d,0x20,0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x2a,0x2b,0x2c,0x2d,0x2f,EOT},
			{0x00,0xff,0xff,0x00,0x00,0x30,0x70,0x10,0x00,0xc8,0x03,0xff,0x04,0x00,MISC,0x00,0x00,0xc0,0x00,0x01,0x00,MISC,EOT}},
		{0x01, "Parallel Port",
			{0x30,0x60,0x61,0x70,0x74,0xf0,EOT},
			{0x01,0x03,0x78,0x07,0x04,0x3f,EOT}},
		{0x02, "UART A",
			{0x30,0x60,0x61,0x70,0xf0,0xf2,EOT},
			{0x01,0x03,0xf8,0x04,0x00,0x00,EOT}},
		{0x03, "UART B, IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,EOT},
			{0x01,0x02,0xf8,0x03,0x00,0x00,0x00,EOT}},
		{0x05, "Keyboard Controller",
			{0x30,0x60,0x61,0x62,0x63,0x70,0x72,0xf0,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,EOT}},
		{0x06, "Consumer IR",
			{0x30,0x60,0x61,0x70,0xf0,0xf1,0xf2,0xf3,EOT},
			{0x00,0x00,0x00,0x00,0x08,0x09,0x32,0x00,EOT}},
		{0x07, "GPIO 6, GPIO 7, GPIO 8",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xec,0xed,0xf4,0xf5,0xf6,0xf7,0xf8,EOT},
			{0x00,0x7f,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,EOT}},
		{0x08, "WDT1, WDT_MEM, GPIO 0, GPIO 1",
			{0x30,0x60,0x61,0xe0,0xe1,0xe2,0xe3,0xe4,0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfe,0xff,EOT},
			{0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,EOT}},
		{0x09, "GPIO 2, GPIO 3, GPIO 4, GPIO 5",
			{0x30,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xee,0xf0,0xf1,0xf2,0xf4,0xf5,0xf6,0xf7,0xfe,EOT},
			{0x00,0xff,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0x00,EOT}},
		{0x0a, "ACPI",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,0xec,0xed,0xee,0xf0,0xf2,0xf3,0xf4,0xf6,0xf7,0xfc,0xfe,EOT},
			{0x01,0x00,0x00,0x00,0x00,0x02,0x1a,0x00,0x00,0x00,0x00,0x00,0x10,0x5c,0x00,0x00,0x00,0xc0,0x00,0x00,EOT}},
		{0x0b, "Hardware Monitor, Front Panel LED",
			{0x30,0x60,0x61,0x62,0x63,0x70,0xe0,0xe1,0xe2,0xe3,0xe4,0xe6,0xe7,0xf0,0xf1,0xf2,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,EOT},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0xff,0xff,0xff,0x08,0xff,0x00,0x00,0x00,0x10,0x00,0x87,0x47,0x00,0x00,0x00,EOT}},
		{0x0d, "BCLK, WDT2, WDT_MEM",
			{0xe0,0xe1,0xe7,0xe8,0xeb,0xed,0xf0,0xf3,EOT},
			{0x00,MISC,0x00,0x32,0x14,0x00,0x00,0x00,EOT}},
		{0x0e, "CIR Wake-Up",
			{0x30,0x60,0x61,0x70,EOT},
			{0x00,0x00,0x00,0x00,EOT}},
		{0x0f, "GPIO Push-Pull or Open-Drain selection",
			{0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe9,0xf0,0xf1,0xf2,EOT},
			{0xff,0xff,0x7f,0xff,0xff,0xff,0x0f,0xff,0xff,0x9d,0x00,0x00,EOT}},
		{0x14, "Port 80 UART",
			{0xe0,0xe1,0xe2,0xe3,0xe4,EOT},
			{0x80,0x00,0x00,0x10,0x00,EOT}},
		{0x16, "Deep Sleep",
			{0x30,0xe0,0xe1,0xe2,0xe3,EOT},
			{0x20,0x20,0x04,0x05,0x01,EOT}},
		{EOT}}},
	{EOT}
};

void probe_idregs_nuvoton(uint16_t port)
{
	uint8_t sid, srid;
	uint16_t chip_id = 0;
	uint8_t chip_rev = 0;
	uint16_t iobase = 0;
	int i;

	/* Probe for the 16bit IDs first to avoid collisions */
	probing_for("Nuvoton", "", port);
	enter_conf_mode_winbond_fintek_ite_8787(port);
	chip_id = (regval(port, DEVICE_ID_REG) << 8) |
	      regval(port, DEVICE_ID_REG + 1);
	regwrite(port, LDN_SEL, 0x0b);
	iobase = (regval(port, 0x60) << 8) | (regval(port, 0x61) & ~7);

	exit_conf_mode_winbond_fintek_ite_8787(port);

	if (!superio_unknown(reg_table, chip_id)) {
		printf("Found Nuvoton %s (id=0x%02x) at 0x%x\n",
		       get_superio_name(reg_table, chip_id), chip_id, port);
		chip_found = 1;
		enter_conf_mode_winbond_fintek_ite_8787(port);
		dump_superio("Nuvoton", reg_table, port, chip_id, LDN_SEL);
		exit_conf_mode_winbond_fintek_ite_8787(port);
		goto extra;
	}

	if (verbose)
		printf(NOTFOUND "chip_id=0x%04x\n", chip_id);

	probing_for("Nuvoton", "(sid=0xfc) ", port);

	sid = regval(port, DEVICE_ID_REG);
	srid = regval(port, DEVICE_REV_REG);

	if (sid == 0xfc) {	/* WPCE775xL family */
		/*
		 * bits 7-5: Chip ID
		 * bits 4-0: Chip revision
		 */
		chip_id = srid >> 5;
		chip_rev = srid & 0x1f;
	}

	if (superio_unknown(reg_table, sid)) {
		if (verbose)
			printf(NOTFOUND
			       "sid=0x%02x, id=0x%02x, rev=0x%02x\n", sid,
			       chip_id, chip_rev);
		return;
	}

	printf("Found Nuvoton %s (id=0x%02x, rev=0x%02x) at 0x%x\n",
	       get_superio_name(reg_table, sid), chip_id, chip_rev, port);
	chip_found = 1;
	dump_superio("Nuvoton", reg_table, port, sid, LDN_SEL);

extra:
	if (extra_dump && iobase) {
		switch (chip_id & 0xfff0) {
		case 0xb470:	/* NCT6775F */
			for (i = 0; i < 7; i++)
				dump_data(iobase + 5, i);
			dump_data(iobase + 5, 0xa);
			dump_data(iobase + 5, 0xc);
			dump_data(iobase + 5, 0xd);
			break;
		case 0xc330:	/* NCT6776F */
			for (i = 0; i < 8; i++)
				dump_data(iobase + 5, i);
			break;
		case 0xc560:	/* NCT6779D */
			for (i = 0; i < 10; i++)
				dump_data(iobase + 5, i);
			break;
		}
	}
}

void print_nuvoton_chips(void)
{
	print_vendor_chips("Nuvoton", reg_table);
}
