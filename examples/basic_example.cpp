/**
 * @file basic_example.cpp
 * @brief Example demonstrating C++26 features with Clang
 */

#include <cstdint>
#include <print>
#include <vector>

#include <nlohmann/json.hpp>

#include "cayene/decoder.hpp"

int main()
{
    try
    {
        using namespace cayene;

        Decoder decoder;
        // Temp 1      Temp 2      Acc                     Neg Temp    GPS
        // 03 67 01 10 05 67 00 FF 06 71 04 D2 FB 2E 00 00 01 67 FF D7 01 88 06 76 5f f2 96 0a 00 03
        // e8
        std::vector<uint8_t> payload = {
            0x03, 0x67, 0x01, 0x10, 0x05, 0x67, 0x00, 0xFF, 0x06, 0x71, 0x04,
            0xD2, 0xFB, 0x2E, 0x00, 0x00, 0x01, 0x67, 0xFF, 0xD7, 0x01, 0x88,
            0x06, 0x76, 0x5f, 0x0d, 0x69, 0xf6, 0x00, 0x03, 0xe8};  // Example payload
        auto res = decoder.decode(payload);

        if (!res)
        {
            std::println("Decoding error: {}", static_cast<uint8_t>(res.error()));
            return -1;
        }

        const Json& decoded = res.value();
        std::println("Decoded JSON: {}", decoded.dump(4));
    }
    catch (const std::exception& ex)
    {
        std::println("Unhandled exception: {}", ex.what());
        return -1;
    }
    catch (...)
    {
        std::println("Unhandled unknown exception");
        return -1;
    }
    return 0;
}
