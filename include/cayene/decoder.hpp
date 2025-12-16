#ifndef CAYENE_DECODER_HPP
#define CAYENE_DECODER_HPP

/**
 * @file decoder.hpp
 * @brief Main header for the Cayene Decoder library
 *
 * This library is licensed under the GNU General Public License v2 (GPLv2).
 * See LICENSE file for details.
 */

#include <cstdint>
#include <expected>

#include <nlohmann/json.hpp>

namespace cayene
{
using namespace nlohmann;
using Json = nlohmann::json;

class Decoder
{
public:
    Decoder();
    ~Decoder();

    auto decode() -> std::expected<Json, int>;  // Placeholder return type
    void add_data_type(const std::string& name, uint8_t type_id);
};

}  // namespace cayene

#endif  // CAYENE_DECODER_HPP
