#ifndef FIX16_FEEDFORWARD_H_
#define FIX16_FEEDFORWARD_H_
#include <stdio.h>
#include <stdlib.h>

#include "../../../parameters/fix16/W1.h"
#include "../../../parameters/fix16/W2.h"
#include "../../../parameters/fix16/W3.h"
#include "../../../parameters/fix16/W4.h"
#include "../../../parameters/fix16/W5.h"

#include "../../../parameters/fix16/B1.h"
#include "../../../parameters/fix16/B2.h"
#include "../../../parameters/fix16/B3.h"
#include "../../../parameters/fix16/B4.h"
#include "../../../parameters/fix16/B5.h"

#include "../../../parameters/parameters.h"

#include "../../../../../../utils/fixedpoint/fixedpoint.h"

#define IWL 6

typedef short fix16;

void passIL(fix16 x[INPUT_LENGTH], fix16 result[HL_LENGTH]);
void passHL2(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]);
void passHL3(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]);
void passHL4(fix16 x[HL_LENGTH], fix16 result[HL_LENGTH]);
void passOL(fix16 x[HL_LENGTH], fix16 result[OUTPUT_LENGTH]);

#endif
