#include <avr/io.h>
#include <avr/interrupt.h>

#include "os_cpu.h"
#include "os_cfg.h" /* os_cpu.h utan kell lennie, mert abban van az INT8U definialva */
#include "ucos_ii.h" /* os_cfg.h utan kell lennie, hogy rendelkezesre alljanak a konfiguracios konstansok */

#include "avr_isr.h"
