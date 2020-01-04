
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NAUI.h)

typedef struct BitPreferencesController BitPreferencesController;

BitPreferencesController* bitCreatePreferencesController(void);
void bitClearPreferencesController(BitPreferencesController* con);

void naShowPreferencesController(BitPreferencesController* con);
