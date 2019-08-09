
#include "NAUI.h"

typedef struct BitPreferencesController BitPreferencesController;

BitPreferencesController* bitCreatePreferencesController();
void bitClearPreferencesController(BitPreferencesController* con);

void naShowPreferencesController(BitPreferencesController* con);
