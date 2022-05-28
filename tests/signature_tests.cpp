#include "signature.h"

#include <fstream>

#include <gmock/gmock.h>

using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

TEST(Example, BadFiles) {
    EXPECT_THAT(
        [] {
            signature::Signer("not_exist1", "output1", 1, 1);
        },
        ThrowsMessage<signature::SignerError>(HasSubstr("Unable to open input file"))
    );
    EXPECT_THAT(
        [] {
            signature::Signer("empty", "output1", 1, 1);
        },
        ThrowsMessage<signature::SignerError>(HasSubstr("Unable to open input file"))
    );

    std::remove("output1");
}

TEST(Example, SmallFile) {
    signature::Signer signer("test", "output2", 1, 10);
    signer.run();

    std::ifstream file("output2");
    std::stringstream buffer;
    buffer << file.rdbuf();
    EXPECT_EQ(
        buffer.str(),
        std::string(
            "\xb7\xdb\xed\xf6\x0d\x06\x83\xc1\x9b\x4d\xa6\xd3\x38\x9c\xce\x67"
            "\xae\xd7\xeb\x75\x14\x0a\x85\x42",
            6 * 4)
    );

    std::remove("output2");
}

// TODO: big file test
