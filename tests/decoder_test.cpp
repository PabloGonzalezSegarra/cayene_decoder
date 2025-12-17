/**
 * @file decoder_test.cpp
 * @brief Unit tests for the Cayene Decoder
 *
 * This library is licensed under the GNU General Public License v2 (GPLv2).
 * See LICENSE file for details.
 */

#include "cayene/decoder.hpp"

#include <gtest/gtest.h>

namespace cayene::test
{

// Test 0 length payload
TEST(DecoderTest, DecodeEmptyPayload)
{
    Decoder decoder;
    std::vector<uint8_t> empty_payload;
    auto res = decoder.decode(empty_payload);
    ASSERT_FALSE(res);
    EXPECT_EQ(res.error(), Error::PayloadEmpty);
}

// Test bad payload size
TEST(DecoderTest, DecodeBadPayloadSize)
{
    Decoder decoder;
    // Payload with incomplete data for a known data type
    std::vector<uint8_t> bad_payload = {0x01, 0x67, 0x01};  // Incomplete temperature data
    auto res = decoder.decode(bad_payload);
    ASSERT_FALSE(res);
    EXPECT_EQ(res.error(), Error::BadPayloadFormat);

    bad_payload = {0x01};  // Incomplete payload
    res = decoder.decode(bad_payload);
    ASSERT_FALSE(res);
    EXPECT_EQ(res.error(), Error::BadPayloadFormat);

    bad_payload = {0x01, 0x67, 0x01, 0x10, 0x02};  // Extra byte at the end
    res = decoder.decode(bad_payload);
    ASSERT_FALSE(res);
    EXPECT_EQ(res.error(), Error::BadPayloadFormat);
}

// Test unknown data type
TEST(DecoderTest, DecodeUnknownDataType)
{
    Decoder decoder;
    std::vector<uint8_t> unknown_type_payload = {0x01, 0xFF,
                                                 0x00};  // 0xFF is not a known data type
    auto res = decoder.decode(unknown_type_payload);
    ASSERT_FALSE(res);
    EXPECT_EQ(res.error(), Error::UnkwownDataType);
}

// Test decode digital input
TEST(DecoderTest, DecodeDigitalInput)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x00, 0x01};  // Channel 1, Digital Input, Value 1
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_EQ(decoded["Digital Input_1"], 1);

    data = {0x02, 0x00, 0x00};  // Channel 2, Digital Input, Value 0
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_EQ(decoded2["Digital Input_2"], 0);
}

// Test decode digital output
TEST(DecoderTest, DecodeDigitalOutput)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x01, 0x01};  // Channel 1, Digital Output, Value 1
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_EQ(decoded["Digital Output_1"], 1);

    data = {0x02, 0x01, 0x00};  // Channel 2, Digital Output, Value 0
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_EQ(decoded2["Digital Output_2"], 0);
}

// Test decode analog input
TEST(DecoderTest, DecodeAnalogInput)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x02, 0x0B, 0xB8};  // Channel 1, Analog Input, Value 3000
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_DOUBLE_EQ(decoded["Analog Input_1"], 30.0);

    data = {0x02, 0x02, 0xFF, 0x9C};  // Channel 2, Analog Input, Value -100
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_DOUBLE_EQ(decoded2["Analog Input_2"], -1.0);
}

// Test decode analog output
TEST(DecoderTest, DecodeAnalogOutput)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x03, 0x0C, 0x80};  // Channel 1, Analog Output, Value 3200
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_DOUBLE_EQ(decoded["Analog Output_1"], 32.0);

    data = {0x02, 0x03, 0xFF, 0x38};  // Channel 2, Analog Output, Value -200
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_DOUBLE_EQ(decoded2["Analog Output_2"], -2.0);
}

// Test luminosity decoding
TEST(DecoderTest, DecodeLuminosity)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x65, 0x03, 0xE8};  // Represents 1000 in big-endian
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_EQ(decoded["Luminosity_1"], 1000);

    data = {0x02, 0x65, 0x27, 0x10};  // Represents 10000 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_EQ(decoded2["Luminosity_2"], 10000);

    data = {0x03, 0x65, 0x00, 0x00};  // Represents 0 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded3 = res.value();
    EXPECT_EQ(decoded3["Luminosity_3"], 0);

    data = {0x04, 0x65, 0xFF, 0xFF};  // Represents 65535 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded4 = res.value();
    EXPECT_EQ(decoded4["Luminosity_4"], 65535);

    data = {0x05, 0x65, 0x12, 0x34};  // Represents 4660 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded5 = res.value();
    EXPECT_EQ(decoded5["Luminosity_5"], 4660);
}

// Test presence decoding
TEST(DecoderTest, DecodePresence)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x66, 0x01};  // Presence detected
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_EQ(decoded["Presence_1"], 1);

    data = {0x02, 0x66, 0x00};  // No presence
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_EQ(decoded2["Presence_2"], 0);
}

// Test temperature decoding
TEST(DecoderTest, DecodeTemperature)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x67, 0x01, 0x90};  // Represents 400 in big-endian
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_DOUBLE_EQ(decoded["Temperature_1"], 40.0);

    data = {0x02, 0x67, 0xFF, 0x9C};  // Represents -100 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_DOUBLE_EQ(decoded2["Temperature_2"], -10.0);

    data = {0x03, 0x67, 0x00, 0x00};  // Represents 0 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded3 = res.value();
    EXPECT_DOUBLE_EQ(decoded3["Temperature_3"], 0.0);

    data = {0x04, 0x67, 0x7F, 0xFF};  // Represents 32767 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded4 = res.value();
    EXPECT_DOUBLE_EQ(decoded4["Temperature_4"], 3276.7);

    data = {0x05, 0x67, 0x80, 0x00};  // Represents -32768 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded5 = res.value();
    EXPECT_DOUBLE_EQ(decoded5["Temperature_5"], -3276.8);

    data = {0x06, 0x67, 0x12, 0x34};  // Represents 4660 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded6 = res.value();
    EXPECT_DOUBLE_EQ(decoded6["Temperature_6"], 466.0);
}

// Test humidity decoding
TEST(DecoderTest, DecodeHumidity)
{
    Decoder decoder;
    std::vector<uint8_t> data = {0x01, 0x68, 0x02, 0x58};  // Represents 600 in big-endian
    auto res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded = res.value();
    EXPECT_DOUBLE_EQ(decoded["Humidity_1"], 60.0);

    data = {0x02, 0x68, 0x00, 0x00};  // Represents 0 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded2 = res.value();
    EXPECT_DOUBLE_EQ(decoded2["Humidity_2"], 0.0);

    data = {0x03, 0x68, 0x27, 0x10};  // Represents 10000 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded3 = res.value();
    EXPECT_DOUBLE_EQ(decoded3["Humidity_3"], 1000.0);

    data = {0x04, 0x68, 0xFF, 0xFF};  // Represents 65535 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded4 = res.value();
    EXPECT_DOUBLE_EQ(decoded4["Humidity_4"], 6553.5);

    data = {0x05, 0x68, 0x12, 0x34};  // Represents 4660 in big-endian
    res = decoder.decode(data);
    ASSERT_TRUE(res);
    const Json& decoded5 = res.value();
    EXPECT_DOUBLE_EQ(decoded5["Humidity_5"], 466.0);
}

}  // namespace cayene::test
