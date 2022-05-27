#include "signature.h"

#include <iostream>

int main(int argc, char* argv[]) {
    // Proper argparsing can be done here
    if (argc < 3) {
        std::cout
            << "There should at least three arguments: "
            << "input_file, output_file and optional chunk_size (in bytes)"
            << std::endl;
        return -1;
    }

    try {
        const int chunkSize = (argc > 3 ? std::stoi(argv[3]) : 1024 * 1024);
        if (chunkSize <= 0) {
            std::cout << "Chunk size should be greater than 0!" << std::endl;
            return -5;
        }

        std::cout
            << "Input file: " << argv[1]
            << ", output file: " << argv[2]
            << ", chunk size: " << chunkSize
            << std::endl;

        signature::Signer signer(
            argv[1],
            argv[2],
            chunkSize,
            std::thread::hardware_concurrency()
        );
        signer.run();
    } catch (const std::invalid_argument& ex) {
        std::cout << "Error: cannot parse chunk_size: " << ex.what() << std::endl;
        return -2;
    } catch (const std::out_of_range& ex) {
        std::cout << "Error: chunk size is too big: " << ex.what() << std::endl;
        return -3;
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return -4;
    }

    std::cout << "Done!" << std::endl;

    return 0;
}
