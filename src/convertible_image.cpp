#include <array>
#include <algorithm>
#include <cmath>
#include <span>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "convertible_image.h"

std::array<float, 9> static identity_kernel = 
{
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 0.0
};

std::array<float, 9> static edge_detection_kernel = 
{
    0.0, -1.0, 0.0,
    -1.0, 4.0, -1.0,
    0.0, -1.0, 0.0
};

std::array<float, 9> static sharpen_kernel = 
{
    0.0, -1.0, 0.0,
    -1.0, 5.0, -1.0,
    0.0, -1.0, 0.0
};

std::array<float, 25> static gaussian_blur_kernel = 
{
    1.0 / 273, 4.0 / 273, 7.0 / 273, 4.0 / 273, 1.0 / 273,
    4.0 / 273, 16.0 / 273, 26.0 / 273, 16.0 / 273, 4.0 / 273,
    7.0 / 273, 26.0 / 273, 41.0 / 273, 26.0 / 273, 7.0 / 273,
    4.0 / 273, 16.0 / 273, 26.0 / 273, 16.0 / 273, 4.0 / 273,
    1.0 / 273, 4.0 / 273, 7.0 / 273, 4.0 / 273, 1.0 / 273
};

std::array<float, 81> static box_blur_kernel =
{
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81,
    1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81, 1.0 / 81
};
/// @brief Map a kernel type to the kernel itself. Supports any size of std::array thanks to std::span.
/// @param type type of image processing kernel
/// @return pointer to kernel array wrapped in a std::span for dynamic sizing
static constexpr std::span<float> kernelTypeToArray(ConvertibleImage::KernelType type)
{
    switch (type)
    {
    case ConvertibleImage::KernelType::EDGE_DETECT:
        return std::span{edge_detection_kernel};
    case ConvertibleImage::KernelType::SHARPEN:
        return sharpen_kernel;
    case ConvertibleImage::KernelType::GAUSSIAN_BLUR:
        return gaussian_blur_kernel;
    case ConvertibleImage::KernelType::BOX_BLUR:
        return box_blur_kernel;
    case ConvertibleImage::KernelType::NOTHING:
    default:
        return identity_kernel;
    }
}

std::vector<uint8_t> ConvertibleImage::applyKernel(ConvertibleImage::KernelType type)
{
    std::vector<uint8_t> result; // Store result
    int channels = bytes_.size() / (width_ * height_); // Compatible both with and without alpha

    std::span<float> const & kernel = kernelTypeToArray(type); // Get kernel and calculate side length
    int kernel_width = static_cast<int>(std::sqrt(kernel.size()));

    for (int byte_index = 0; byte_index < bytes_.size(); ++byte_index)
    {
        int y = byte_index / width_; // Avoid clutter by calculating pixel coordinates from 1D instead
        int x = byte_index % width_;
        std::vector<float> result_pixel_values(channels); // Intermediate pixel sum

        for (int i = 0; i < kernel.size(); ++i) // For every pixel, calculate average weighted by kernel
        {
            int offset_x = i % kernel_width - 1;
            int offset_y = i / kernel_width - 1;
            if (x + offset_x < 0 || x + offset_x > width_ - 1 || y + offset_y < 0 || y + offset_y > height_ - 1) continue; // Remove out-of-bounds offsets (wrap-around possible as well)
            for (int channel = 0; channel < channels; ++channel)
            {
                float pixel_value = static_cast<float>(bytes_.at(channels * width_ * (y + offset_y) + channels * (x + offset_x) + channel)); // Extract color channel
                result_pixel_values[channel] += pixel_value * kernel[i]; // Apply kernel weight and sum
            }
        }
        for (float pixel : result_pixel_values)
        {
            result.emplace_back(static_cast<uint8_t>(std::clamp(std::round(pixel), 0.0f, 255.0f))); // Store final pixel after removing extreme values
        }
    }
    return result;
}

// Python Bindings ////////////////
PYBIND11_MODULE(image_processor, mod)
{
    pybind11::class_<ConvertibleImage>(mod, "ConvertibleImage")
        .def(pybind11::init<int, int, std::vector<uint8_t>>())
        .def("applyKernel", &ConvertibleImage::applyKernel)
    ;
    pybind11::enum_<ConvertibleImage::KernelType>(mod, "KernelType")
        .value("NOTHING", ConvertibleImage::KernelType::NOTHING)
        .value("EDGE_DETECT", ConvertibleImage::KernelType::EDGE_DETECT)
        .value("SHARPEN", ConvertibleImage::KernelType::SHARPEN)
        .value("GAUSSIAN_BLUR", ConvertibleImage::KernelType::GAUSSIAN_BLUR)
        .value("BOX_BLUR", ConvertibleImage::KernelType::BOX_BLUR)
        .export_values()
    ;
}