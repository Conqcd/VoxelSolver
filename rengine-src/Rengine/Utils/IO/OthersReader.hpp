#pragma once
#include "Rengine/Core/core.hpp"
#include "Reader.hpp"
#include <fstream>
#include <vector>

namespace Rengin
{

struct OthersReader : public AbstractReader{
    std::ifstream fs;
    std::array<uint32_t, 3> dimensions;
    RawValueType value_type;
    std::vector<float> m_buffer;

    explicit OthersReader(const std::string &filepath);
    auto load() -> std::vector<float>& override;
    inline uint64_t Count() const
    {
        return dimensions[0] * dimensions[1] * dimensions[2];
    }
};


} // namespace Rengin