#include "GerberParser.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <crtdbg.h>

int main(int argc, char** argv)
{
    // A failing regression must not open a dialog on an unattended workstation.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#ifdef _DEBUG
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
    if (argc != 2) return 2;
    const std::string name(argv[1]);
    std::string input;
    bool reject = false;
    if (name == "empty") input = "G04*";
    else if (name == "text") input = "G04 normal comment*";
    else if (name == "following") input = "G04 first*G04 second*M02*";
    else if (name == "multiline") input = "G04 first\r\nsecond*\r\nM02*";
    else if (name == "long_valid") input = "G04 " + std::string(4096, 'x') + "*";
    else if (name == "missing_empty") { input = "G04"; reject = true; }
    else if (name == "missing_text") { input = "G04 unfinished"; reject = true; }
    else if (name == "missing_long") { input = "G04 " + std::string(4096, 'x'); reject = true; }
    else if (name == "missing_after_valid") { input = "G04 valid*G04 unfinished"; reject = true; }
    else return 2;

    CGerberParser parser;
    try {
        if (!parser.Parse(input.c_str()) || reject) {
            std::fprintf(stderr, "%s: unexpected parse result\n", argv[1]);
            return 1;
        }
    } catch (const char* error) {
        if (!reject || std::strcmp(error, "Unterminated G04 comment") != 0) {
            std::fprintf(stderr, "%s: unexpected parser error: %s\n", argv[1], error);
            return 1;
        }
    } catch (...) {
        std::fprintf(stderr, "%s: unexpected exception\n", argv[1]);
        return 1;
    }
    std::printf("PASS %s\n", argv[1]);
    return 0;
}
