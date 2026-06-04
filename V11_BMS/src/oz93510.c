/*
 * oz93510.c
 *
 * Compatibility scaffold for selecting OZ93510 as the BMS frontend architecture.
 *
 * This file provides the same external API currently used by the rest of the
 * firmware (bq7693_* symbols) so the state machine can be compiled against an
 * alternate frontend while register-level OZ93510 integration is completed.
 *
 * Safety-first behavior:
 * - reports undervoltage status by default
 * - keeps charge/discharge control APIs as no-ops
 * - returns zero current and zero cell voltages
 *
 * Author :  OpenAI Copilot Task Agent
 * License: GNU GPL v3 or later
 */

// Intentionally include bq7693.h because it currently declares the shared
// frontend API consumed by the rest of the firmware.
#include "bq7693.h"

#if BMS_FRONTEND_ARCH == BMS_FRONTEND_OZ93510

#include <string.h>

#define OZ93510_CELL_COUNT 7

static uint16_t oz93510_cell_voltages[OZ93510_CELL_COUNT] = {0};
static uint8_t oz93510_sys_stat = STAT_UV;

void bq7693_init(void)
{
  oz93510_sys_stat = STAT_UV;
}

bool bq7693_read_register(uint8_t addr, size_t len, uint8_t *buf)
{
  if (buf == NULL || len == 0)
    return false;

  memset(buf, 0, len);

  if (addr == SYS_STAT)
  {
    buf[0] = oz93510_sys_stat;
  }

  return true;
}

bool bq7693_write_register(uint8_t addr, uint8_t data)
{
  if (addr == SYS_STAT)
  {
    // Match SYS_STAT "write 1 to clear" behavior expected by existing logic.
    oz93510_sys_stat &= (uint8_t)(~data);
  }

  return true;
}

uint16_t *bq7693_get_cell_voltages(void)
{
  return oz93510_cell_voltages;
}

int bq7693_get_pack_voltage(void)
{
  return 0;
}

void bq7693_enable_charge(void)
{
}

void bq7693_enable_discharge(void)
{
}

void bq7693_disable_charge(void)
{
}

void bq7693_disable_discharge(void)
{
}

void bq7693_enter_sleep_mode(void)
{
}

int16_t bq7693_read_cc(void)
{
  return 0;
}

#endif // BMS_FRONTEND_ARCH == BMS_FRONTEND_OZ93510
