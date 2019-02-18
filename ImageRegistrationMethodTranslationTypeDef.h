#ifndef __IMAGEREGISTRATIONMETHODTRANSLATIONTYPEDEF_H
#define __IMAGEREGISTRATIONMETHODTRANSLATIONTYPEDEF_H

#include "ItkTypeDef.h"
#include "itkTranslationTransform.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkCorrelationImageToImageMetricv4.h"
#include "itkImageRegistrationMethodv4.h"

namespace TranslationRegistration
{
    typedef itk::TranslationTransform<
        CoordinateRepType,
        SpaceDimension > TransformType;
    typedef TransformType::ParametersType ParametersType;

    typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
    typedef OptimizerType * OptimizerPointer;
    typedef const OptimizerType * ConstOptimizerPointer;

    typedef itk::CorrelationImageToImageMetricv4  <FloatImageType, FloatImageType> NCCMetricType;

    typedef itk::ImageRegistrationMethodv4 <
        FloatImageType,
        FloatImageType,
        TransformType >                 RegistrationType;
}
#endif
