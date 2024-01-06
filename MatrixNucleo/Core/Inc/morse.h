/*
 * morse.h
 *
 *  Created on: 3 июн. 2023 г.
 *      Author: JV4K
 */

#ifndef INC_MORSE_H_
#define INC_MORSE_H_

#include "main.h"
#include "string.h"
#include <wchar.h>
#include <wctype.h>

struct MorseCode {
	char letter;
	const char *code;
};

struct MorseCode morseCodes[] = {
    {0xC0, ".-"}, {0xC1, "-..."}, {0xC2, ".--"}, {0xC3, "--."}, {0xC4, "-.."},
    {0xC5, "."}, {0xA8, "."}, {0xC6, "...-"}, {0xC7, "--.."}, {0xC8, ".."},
    {0xC9, ".---"}, {0xCA, "-.-"}, {0xCB, ".-.."}, {0xCC, "--"}, {0xCD, "-."},
    {0xCE, "---"}, {0xCF, ".--."}, {0xD0, ".-."}, {0xD1, "..."}, {0xD2, "-"},
    {0xD3, "..-"}, {0xD4, "..-."}, {0xD5, "...."}, {0xD6, "-.-."}, {0xD7, "---."},
    {0xD8, "----"}, {0xD9, "--.-"}, {0xDA, "--.--"}, {0xDB, "-.--"}, {0xDC, "-..-"},
    {0xDD, "..-.."}, {0xDE, "..--"}, {0xDF, ".-.-"}, {0xB8, "-----"}, {0xB9, ".----"},
    {0xBA, "..---"}, {0xBB, "...--"}, {0xBC, "....-"}, {0xBD, "....."}, {0xBE, "-...."},
    {0xBF, "--..."}, {0xE0, "---.."}, {0xE1, "----."}, {0xA9, ".-.-.-"}, {0xAC, "--..--"},
    {0xBF, "..--.."}, {0xA1, "-.-.--"}, {0xAD, "-....-"}, {0xBA, "-..-."}, {0x80, ".--.-."},
};

const int numCodes = sizeof(morseCodes) / sizeof(morseCodes[0]);
char *dot = ".";
char *dash = "-";
char *space = " ";

char* codeToLetter(const char* code) {
    for (int i = 0; i < numCodes; i++) {
        if (strcmp(morseCodes[i].code, code) == 0)
            return &(morseCodes[i].letter);
    }
    return NULL;
}


#endif /* INC_MORSE_H_ */
