#ifndef __IMAGEREGISTRATIONMETHODTRANSLATION_H
#define __IMAGEREGISTRATIONMETHODTRANSLATION_H

#include "ImageRegistrationMethodTranslationTypeDef.h"

namespace TranslationRegistration
{
    class Registrator
    {
    public:

        Registrator();
        ~Registrator();

        // run the registration, return success or not
        // this function could return false due to ITK exception
        bool RunTranslationRegistration(const FloatImageType::Pointer& fixed_image, const FloatImageType::Pointer& moving_image, 
                                        const UcharImageType::Pointer& fixed_mask , const UcharImageType::Pointer& moving_mask);

    private:

        // Setup pyramid setting for registration method
        void SetPyramid(RegistrationType::Pointer& registration);

        // Setup metric for registration method
        void SetMetric(const UcharImageType::Pointer& fixed_mask, const UcharImageType::Pointer& moving_mask, RegistrationType::Pointer& registration);

        // Do registration and get warped image
        FloatImageType::Pointer WarpImage(const FloatImageType::Pointer& im, const TransformType::Pointer& transform);

        // parameters for translation registration
        /////////////////////////////////////////////////////

        // ITK objects
        RegistrationType::Pointer registration_;
        OptimizerType::Pointer optimizer_;
        TransformType::Pointer transform_;
    };
}

using TranslationRegisrator = TranslationRegistration::Registrator;

#endif