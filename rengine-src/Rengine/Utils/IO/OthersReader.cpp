#include "repch.hpp"
#include "OthersReader.hpp"
#include <cassert>

#include "itkConnectedComponentImageFilter.h"
#include <itkRescaleIntensityImageFilter.h>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"

namespace Rengin
{

OthersReader::OthersReader(const std::string &filepath) {
    const char *name_rule = "raw data should be named as `name_x_y_z_datatype`, "
                            "for example: isabel_500_500_100_uint8";
    auto filename = get_file_name(filepath);
    auto splited = split(filename, "_");
    if (splited.size() < 5) {
        throw std::runtime_error(name_rule);
    }

    fs.open(filepath, std::ios::binary);
    fs.unsetf(std::ios::skipws);
    if (!fs.is_open()) {
        throw std::runtime_error(std::string("cannot load file: ") + filepath);
    }

    for (int i = 0; i < 3; i++) {
        auto &dim = splited[splited.size() - 4 + i];
        dimensions[i] = strtoul(dim.c_str(), nullptr, 10);
        if (dimensions[i] == 0) {
        throw std::runtime_error(name_rule);
        }
    }

    auto &type = splited[splited.size() - 1];
    if (type == "uint8") {
        value_type = RawValueType::UINT8;
    } else if (type == "uint16") {
        value_type = RawValueType::UINT16;
    } else if (type == "int16") {
        value_type = RawValueType::INT16;
    } else if (type == "float") {
        value_type = RawValueType::FLOAT;
    } else {
        throw std::runtime_error(std::string("unsupported value type: ") + type);
    }
}

auto OthersReader::OthersReader(const std::string &filepath) {
::load() -> std::vector<float>& {
    if (value_type == RawValueType::UINT8) {
        std::vector<uint8_t> buffer;
        m_buffer.reserve(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(uint8_t));
        m_buffer = convert_uint8_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::UINT16) {
        std::vector<uint16_t> buffer;
        buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(uint16_t));
        m_buffer = convert_uint16_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::INT16) {
        std::vector<int16_t> buffer;
        buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(int16_t));
        m_buffer = convert_int16_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::FLOAT) {
        m_buffer.resize(Count());
        m_buffer.insert(m_buffer.begin(),std::istream_iterator<float>(fs),std::istream_iterator<float>());
    } else {
        throw std::runtime_error(std::string("unsupported value type"));
    }
    return m_buffer;
}

} // namespace Rengin