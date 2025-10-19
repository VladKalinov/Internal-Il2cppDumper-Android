//
// Created by Perfare on 2020/7/4.
//

#ifndef AUTO_IL2CPPDUMPER_CONFIG_H
#define AUTO_IL2CPPDUMPER_CONFIG_H

// Try increase sleep time if having issues with the game. Decrease sleep time if anti-cheat detection it earlier
#define Sleep 2

// Uncomment for fake lib mode
// It is to load our fake libmain.so or libunity.so and load game's real librealmain.so or librealunity.so
#define UseFakeLib

// Enable extended dump for IDA Pro (generates script.json, il2cpp.h, StringLiteral.json)
// Comment this line to disable extended features and only generate dump.cs
#define EnableExtendedDump

#endif //AUTO_IL2CPPDUMPER_CONFIG_H
