#include "signature.h"

#include <boost/crc.hpp>

namespace signature {

namespace {

const size_t CHECKSUM_SIZE = 4;

uint32_t crc32(const char* data, size_t size) {
    boost::crc_32_type result;
    result.process_bytes(data, size);
    return result.checksum();
}

}

Signer::Signer(
    const std::string& inputFile,
    const std::string& outputFile,
    size_t chunkSize,
    size_t numberThreads
) : readIndex_(0),
    chunkSize_(chunkSize),
    numberThreads_(numberThreads) {

    try {
        // Memory mapped files can be much greater than memory size.
        // The limit should be around hunderds of terabytes - and OS will handle
        // all manipulations by itself. Because we will read in straighforward
        // manner performance should be close to optimal.
        //
        // Also empty files are not supported - can be handled separately
        // if it's really needed.
        inputFile_.open(inputFile);
    } catch (const std::ios_base::failure& ex) {
        throw SignerError("Unable to open input file: " + inputFile + ", " + ex.what());
    }

    const bool partialLastChunk = (inputFile_.size() % chunkSize_ > 0);
    const size_t numberChunks = inputFile_.size() / chunkSize_ + (partialLastChunk ? 1 : 0);

    try {
        boost::iostreams::mapped_file_params parameters;
        parameters.path = outputFile;
        parameters.new_file_size = numberChunks * CHECKSUM_SIZE;
        outputFile_.open(parameters);
    } catch (const std::ios_base::failure& ex) {
        throw SignerError("Unable to open output file: " + outputFile + ", " + ex.what());
    }
}

void Signer::run() {
    std::vector<std::thread> workers;
    for (size_t index = 0; index < numberThreads_; ++index) {
        workers.emplace_back([this] { runWorker(); });
    }
    for (auto& worker : workers) {
        worker.join();
    }
}

void Signer::runWorker() {
    while (true) {
        // Possible place for optimization: we can handle more than one
        // chunk in a single thread - that way it might be a little bit
        // better for cache locality.
        const size_t readIndex = readIndex_.fetch_add(chunkSize_, std::memory_order_relaxed);
        if (readIndex >= inputFile_.size()) {
            break;
        }
        const size_t chunkSize = std::min(chunkSize_, static_cast<size_t>(inputFile_.size() - readIndex));
        uint32_t checksum = crc32(inputFile_.data() + readIndex, chunkSize);
        const size_t writeIndex = (readIndex / chunkSize_) * CHECKSUM_SIZE;
        for (size_t shift = 0; shift < CHECKSUM_SIZE; ++shift) {
            outputFile_.data()[writeIndex + shift] = checksum & 255;
            checksum >>= 1;
        }
    }
}

} // namespace signature
