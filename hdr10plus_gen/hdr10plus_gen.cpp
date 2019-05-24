
#include <fcntl.h>
#include <io.h>
#include <cstdio>
#include <cstdint>
#include <memory>
#include <iostream>
#include <string>
#include "hdr10plus.h"

static const char *EXE_VER = "0.00";

void print_help() {
    std::cerr << "hdr10plus_gen v" << EXE_VER << std::endl;
    std::cerr << "Usage:" << std::endl;
    std::cerr << "  hdr10plus_gen.exe -i <input json file> -o <output binary>" << std::endl;
}

int main(int argc, char **argv) {
    std::string inputFile;
    std::string outputFile;
    for (int iarg = 1; iarg < argc; iarg++) {
        if (std::string(argv[iarg]) == "-i") {
            iarg++;
            inputFile = argv[iarg];
        } else if (std::string(argv[iarg]) == "-o") {
            iarg++;
            outputFile = argv[iarg];
        } else if (std::string(argv[iarg]) == "-h") {
            print_help();
            return 1;
        }
    }
    if (inputFile.length() == 0) {
        std::cerr << "input file not specified!" << std::endl;
        print_help();
        return 1;
    }
    if (outputFile.length() == 0) {
        outputFile = inputFile + ".bin";
    }

    if (outputFile == "-") {
        if (_setmode(_fileno(stdout), _O_BINARY) == 1) {
            std::cerr << "failed to switch stdout to binary mode." << std::endl;
            return 1;
        }
    }
    std::unique_ptr<FILE, decltype(&fclose)> fp(outputFile == "-" ? stdout : fopen(outputFile.c_str(), "wb"), fclose);

    const auto hdr10plus = hdr10plus_api_get();
    if (hdr10plus == nullptr) {
        std::cerr << "failed to get hdr10plus api!" << std::endl;
        return 1;
    }

    uint8_t **cim;
    const int32_t cimNum = hdr10plus->hdr10plus_json_to_movie_cim(inputFile.c_str(), cim);

    for (int32_t icim = 0; icim < cimNum; icim++) {
        int i = 0;
        int32_t dataSize = 0;
        while (cim[icim][i] == 0xFF) {
            dataSize += cim[icim][i++];
        }
        dataSize += cim[icim][i++];
        if (   fwrite(&icim,     sizeof(icim),     1, fp.get()) != 1
            || fwrite(&dataSize, sizeof(dataSize), 1, fp.get()) != 1) {
            return 1;
        }
        if (dataSize > 0) {
            if (fwrite(&cim[icim][i], 1, dataSize, fp.get()) != dataSize) {
                return 1;
            }
            fflush(fp.get());
        }
        std::cerr << "cim[" << icim << "] size: " << dataSize << std::endl;
    }
    fp.reset();
    return 0;
}
