/**
 * @file vizzium.cpp
 *
 * @brief VIZZIUM vi-like debugger for z80 binaries
 *
 * @note Uses emuzeta80 as Z80 emulator
 *
 * @see LICENSE for licensing details.
 */

#include "console/consolez80.h"

int main()
{
    auto console = new ConsoleZ80();
    return console->execute();
}
