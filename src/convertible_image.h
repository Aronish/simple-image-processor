#include <vector>

/// @brief Wrapper for an image that can create modified images.
class ConvertibleImage
{
private:
    int width_, height_;
    std::vector<uint8_t> bytes_;
public:
    enum class KernelType
    {
        NOTHING, EDGE_DETECT, SHARPEN, GAUSSIAN_BLUR, BOX_BLUR
    };
public:
    /// @brief Initialize with bytes received from Python script.
    /// @param width width of image
    /// @param height height of image
    /// @param bytes bytes read by Python
    ConvertibleImage(int width, int height, std::vector<uint8_t> const & bytes) : width_(width), height_(height), bytes_(bytes) {}

    /// @brief Applies a kernel transformation of the specified type and returns the new image.
    /// @param type type of image processing kernel
    /// @return the new image as a vector of bytes
    std::vector<uint8_t> applyKernel(KernelType type);
};