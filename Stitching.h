#ifndef __SEQUENTIALSTITCH_H
#define __SEQUENTIALSTITCH_H

#include <mutex>
#include "ItkTypeDef.h"

class Stitching
{
public:
    Stitching();
    ~Stitching() = default;

    bool DoStitch();

private:
    FloatImageType::Pointer ReadItkFloatImage(const std::string& file_path);
    UcharImageType::Pointer ReadItkUcharImage(const std::string& file_path);

private:

    FloatImageType::Pointer fixedImage_;
    FloatImageType::Pointer movingImage_;
    UcharImageType::Pointer fixedMask_;
    UcharImageType::Pointer movingMask_;

    static std::mutex mutex_;
};


#endif