
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NAUI.h)

typedef struct BitASCIIController BitASCIIController;

BitASCIIController* bitCreateASCIIController(void);
void bitClearASCIIController(BitASCIIController* con);

void naShowASCIIController(BitASCIIController* con);
