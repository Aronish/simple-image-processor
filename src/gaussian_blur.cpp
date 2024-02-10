#include <array>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "gaussian_blur.h"

namespace py = pybind11;

std::array<float, 9> static constexpr blur_kernel = 
{
    1.0 / 16,   1.0 / 8,    1.0 / 16,
    1.0 / 8,    1.0 / 4,    1.0 / 8,
    1.0 / 16,   1.0 / 8,    1.0 / 16
};

std::array<float, 9> static constexpr edge_detection_kernel = 
{
    0.0, -1.0, 0.0,
    -1.0, 4.0, -1.0,
    0.0, -1.0, 0.0
};

std::array<float, 25> static constexpr test_kernel = 
{
    1.0 / 273, 4.0 / 273, 7.0 / 273, 4.0 / 273, 1.0 / 273,
    4.0 / 273, 16.0 / 273, 26.0 / 273, 16.0 / 273, 4.0 / 273,
    7.0 / 273, 26.0 / 273, 41.0 / 273, 26.0 / 273, 7.0 / 273,
    4.0 / 273, 16.0 / 273, 26.0 / 273, 16.0 / 273, 4.0 / 273,
    1.0 / 273, 4.0 / 273, 7.0 / 273, 4.0 / 273, 1.0 / 273
};

BlurredImage::BlurredImage(int width, int height, std::vector<uint8_t> const & bytes) : width_(width), height_(height), bytes_(bytes)
{
}

std::vector<uint8_t> BlurredImage::blur()
{
    std::vector<uint8_t> result;
    int channels = bytes_.size() / (width_ * height_);

    auto const & kernel = blur_kernel;
    int kernel_width = static_cast<int>(std::sqrt(kernel.size()));

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            uint8_t result_r = 0, result_g = 0, result_b = 0;
            for (int i = 0; i < kernel.size(); ++i)
            {
                int offset_x = i % kernel_width - 1;
                int offset_y = i / kernel_width - 1;
                if (x + offset_x < 0 || x + offset_x > width_ - 1 || y + offset_y < 0 || y + offset_y > height_ - 1) continue;
                uint8_t r = bytes_.at(channels * width_ * (y + offset_y) + channels * (x + offset_x));
                uint8_t g = bytes_.at(channels * width_ * (y + offset_y) + channels * (x + offset_x) + 1);
                uint8_t b = bytes_.at(channels * width_ * (y + offset_y) + channels * (x + offset_x) + 2);
                result_r += static_cast<uint8_t>(static_cast<float>(r) * kernel[i]);
                result_g += static_cast<uint8_t>(static_cast<float>(g) * kernel[i]);
                result_b += static_cast<uint8_t>(static_cast<float>(b) * kernel[i]);
            }
            result.push_back(result_r);
            result.push_back(result_g);
            result.push_back(result_b);
        }
    }
    return result;
}

PYBIND11_MODULE(gaussian_blur, mod)
{
    py::class_<BlurredImage>(mod, "BlurredImage")
        .def(py::init<int, int, std::vector<uint8_t>>())
        .def("blur", &BlurredImage::blur)
    ;
}