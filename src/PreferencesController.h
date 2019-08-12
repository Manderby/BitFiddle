
#include "NAUI.h"

typedef struct BitPreferencesController BitPreferencesController;

BitPreferencesController* bitCreatePreferencesController(void);
void bitClearPreferencesController(BitPreferencesController* con);

void naShowPreferencesController(BitPreferencesController* con);
