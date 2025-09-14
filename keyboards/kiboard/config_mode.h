#pragma once

#include <stdbool.h>
#include "quantum.h"

#define CONFIG_MODE_ADDR 100
#define CONFIG_MODE_MAGIC 0x42

bool is_config_mode(void);
void enter_config_mode(void);
void exit_config_mode(void);
void init_config_mode(void);
