
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NAUI.h)

typedef struct BitConverterController BitConverterController;

BitConverterController* bitCreateConverterController(void);
void bitClearConverterController(BitConverterController* con);

void naShowConverterController(BitConverterController* con);
void bitUpdateConverterController(BitConverterController* con);
void bitKeepConverterOnTop(BitConverterController* con, NABool onTop);

