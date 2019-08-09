
#include "NAUI.h"

typedef struct BitConverterController BitConverterController;

BitConverterController* bitCreateConverterController();
void bitClearConverterController(BitConverterController* con);

void naShowConverterController(BitConverterController* con);
void bitUpdateConverterController();
void bitKeepConverterOnTop(BitConverterController* con, NABool onTop);
