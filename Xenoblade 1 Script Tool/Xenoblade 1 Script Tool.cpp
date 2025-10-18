#include <iostream>
#include <fstream>
#include <cstring>
#include "Script.h"
#include "Tests.h"

void printUsageStatement(std::string progName)
{
    std::cout << "Usage:\n" <<
        "Generate Output File: " << progName << " decompile <input_script> <output_csv>\n" <<
        "Generate Script File: " << progName << " compile <input_csv> <output_script> [original_script] [--strings-only]\n";
}

int main(int argc, char *argv[])
{
    /* Generate Output File: <prog_name> decompile <input_script> <output_csv>
       Generate Script File: <prog_name> compile <input_csv> <output_script>
    */
    try {
        if (argc < 4 || argc > 6) printUsageStatement(argv[0]);
        else {
            if (strcmp(argv[1], "decompile") == 0) {
                Script script(argv[2], true);
                script.generateOutfile(argv[3]);
                std::cout << "Decompiled " << argv[2] << " to " << argv[3] << ".csv";
            }
            else if (strcmp(argv[1], "compile") == 0) {
                Script::RebuildPolicy policy = Script::RebuildPolicy::Automatic;
                const char* originalPath = nullptr;
                for (int i = 4; i < argc; i++) {
                    if (strcmp(argv[i], "--strings-only") == 0) policy = Script::RebuildPolicy::StringsOnly;
                    else if (originalPath == nullptr) originalPath = argv[i];
                    else {
                        printUsageStatement(argv[0]);
                        return 0;
                    }
                }

                Script script(argv[2], false);
                if (originalPath != nullptr) {
                    Script original(originalPath, true);
                    script.generateScriptFile(argv[3], &original, policy);
                }
                else {
                    script.generateScriptFile(argv[3], nullptr, policy);
                }
                std::cout << "Compiled " << argv[2] << " to " << argv[3] << ".sb";
            }
            else if (strcmp(argv[1], "test") == 0) {
                std::cout << "Running Tests...\n";
                Tests tests(argv[2], argv[3]);
                std::cout << "Completed Tests";
            }
            else printUsageStatement(argv[0]);
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
