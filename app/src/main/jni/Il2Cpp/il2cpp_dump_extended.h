//
// Extended dumper for IDA Pro support
// Generates script.json, il2cpp.h, and StringLiteral.json
//

#ifndef AUTO_IL2CPPDUMPER_IL2CPP_DUMP_EXTENDED_H
#define AUTO_IL2CPPDUMPER_IL2CPP_DUMP_EXTENDED_H

#include <string>

// Generate script.json with method addresses for IDA Pro
void dump_script_json(const char *outDir);

// Generate il2cpp.h with C++ struct definitions
void dump_il2cpp_header(const char *outDir);

// Generate StringLiteral.json with all string literals
void dump_string_literals(const char *outDir);

// Main extended dump function that generates all files
void il2cpp_dump_extended(const char *outDir);

#endif //AUTO_IL2CPPDUMPER_IL2CPP_DUMP_EXTENDED_H
