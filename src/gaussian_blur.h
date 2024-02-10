#include <vector>

class BlurredImage
{
private:
    int width_, height_;
    std::vector<uint8_t> bytes_;

public:
    BlurredImage(int width, int height, std::vector<uint8_t> const & bytes);

    std::vector<uint8_t> blur();
    //TODO: more kernels and general kernel applicator
};