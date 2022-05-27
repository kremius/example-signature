#pragma once

#include <atomic>
#include <string>
#include <thread>

#include <boost/iostreams/device/mapped_file.hpp>

namespace signature {

struct SignerError : public std::runtime_error {
    SignerError(const std::string& what) : std::runtime_error(what) {}
};

class Signer {
public:
    Signer(
        const std::string& inputFile,
        const std::string& outputFile,
        size_t chunkSize, // In bytes
        size_t numberThreads
    );
    void run();
private:
    void runWorker();

    std::atomic<size_t> readIndex_;
    size_t chunkSize_;
    size_t numberThreads_;
    boost::iostreams::mapped_file_source inputFile_;
    boost::iostreams::mapped_file_sink outputFile_;
};

} // namespace signature
