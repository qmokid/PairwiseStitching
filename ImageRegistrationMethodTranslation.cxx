#include "ImageRegistrationMethodTranslation.h"

#include "itkImageMaskSpatialObject.h"

using namespace std;
using namespace itk;
using namespace TranslationRegistration;

//!
//! \brief Constructor
//!        Setup registration_ and optimizer_
//!
Registrator::
Registrator() 
  : registration_   (RegistrationType::New())
  , optimizer_      (OptimizerType   ::New())
  , transform_      (TransformType   ::New())
{
    registration_->SetNumberOfThreads(1);
    registration_->SetOptimizer(optimizer_);

    optimizer_->SetLearningRate(10.00);
    optimizer_->SetMinimumStepLength(1);
    optimizer_->SetGradientMagnitudeTolerance(1e-5);
    optimizer_->SetNumberOfIterations(200);
};

//!
//! \brief Destructor
//!
Registrator::
~Registrator(){}

//!
//! \brief Main entry of this class. Do translation registration
//! \param fixed_img        registration target
//! \param moving_img       image to be shifted to match fixed image
//! \param fixed_msk        0/255 mask of fixed_image
//! \param moving_msk       0/255 mask of moving_image
//!                         input  : initial displacement to last warped image center
//!                         output : best registrated initial displacement
//! \return registration success or not
//!         fail reason (1) none of the initial guess succeed
//!                     (2) itk exception (may be removed)
//!
bool Registrator::RunTranslationRegistration(const FloatImageType::Pointer& fixed_img, const FloatImageType::Pointer& moving_img, 
                                             const UcharImageType::Pointer& fixed_msk, const UcharImageType::Pointer& moving_msk)
{
    SetPyramid(registration_);

    // set up initial guesses
    const vector<int> ys = { -50, -25, 0, 25, 50};
    vector<ParametersType> init_paras(ys.size(), ParametersType(2));
    for (int i = 0; i < ys.size(); i++)
    {
        init_paras[i][0] = 0;
        init_paras[i][1] = ys[i];
    }
    
    for (auto init_para : init_paras)// try different initial setting and output best result by minimum metric value
    {
        // Setup the registration
        registration_->SetFixedImage (fixed_img );
        registration_->SetMovingImage(moving_img);

        // Setup NCC metric
        SetMetric(fixed_msk, moving_msk, registration_);

        // Setup transformation
        transform_->SetParameters(init_para);

        // Pass the parameters of the current transform as the initial parameters to be used when the registration process starts.
        registration_->SetInitialTransform(transform_);
        registration_->SetMetricSamplingPercentage(0.5);
        registration_->SetMetricSamplingStrategy(RegistrationType::RANDOM);
        registration_->MetricSamplingReinitializeSeed(1);// if no para : random (take time as seed)
        registration_->InPlaceOn();// as shown in example "MultiResImageRegistration1.cxx". The purpose of this function as shown in doxygen is "Request that the InitialTransform be grafted onto the output, there by not creating a copy." Don't know what it means
        
        try
        {
            registration_->Update();
            
            // the metric_value and warpedImage have their purpose.
            // I remove the usage of them for simplification of the code (since the exception is still present)
            // But removing these 2 function calls will highly reduce the exception occur rate, so I keep them here
            double metric_value = registration_->GetMetric()->GetValue();
            FloatImageType::Pointer warpedImage = WarpImage(moving_img, transform_);
        }
        catch (ExceptionObject & err)
        {
            cerr << "ExceptionObject caught !\n" << err << endl;
            throw err;
        }
    }

    return true;
}

//!
//! \brief Setup pyramid setting for registration method
//!        currently single level resize image only
//! \param registration     pyramid info to be set here
//!
void Registrator::
SetPyramid(RegistrationType::Pointer& registration)
{
    // Set up the schedules of multi-resolution registrations
    int numberOfLevels = 1;
    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize(numberOfLevels);
    shrinkFactorsPerLevel[0] = 4;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize(numberOfLevels);
    smoothingSigmasPerLevel[0] = 0;

    registration->SetNumberOfLevels(numberOfLevels);
    registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);
    registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
}

// 
//!
//! \brief Setup NCC metric for registration method
//! \param fixed_msk        mask of fixed  image
//! \param moving_msk       mask of moving image
//! \param registration     metric info to be set here
//!
void Registrator::
SetMetric(const UcharImageType::Pointer & fixed_msk, const UcharImageType::Pointer & moving_msk, RegistrationType::Pointer& registration)
{
    // set up fixed and moving image mask    
    typedef ImageMaskSpatialObject< ImageDimension > MaskType;
    MaskType::Pointer spatialObjectFixedMask  = MaskType::New();
    MaskType::Pointer spatialObjectMovingMask = MaskType::New();
    spatialObjectFixedMask ->SetImage( fixed_msk);
    spatialObjectMovingMask->SetImage(moving_msk);

    // use NCC metric
    NCCMetricType::Pointer ncc_metric = NCCMetricType::New();
    ncc_metric->SetFixedImageMask (spatialObjectFixedMask );
    ncc_metric->SetMovingImageMask(spatialObjectMovingMask);
    registration->SetMetric(ncc_metric);
}

//!
//! \brief do warping by itk function
//! \param im           rgb image to be warped
//! \param transform    warping transform
//! \return warped rgb image
//!
FloatImageType::Pointer Registrator::
WarpImage(const FloatImageType::Pointer& im, const TransformType::Pointer& transform)
{
    typedef ResampleImageFilter<FloatImageType, FloatImageType> ResampleFilterType;
    ResampleFilterType::Pointer resample = ResampleFilterType::New();

    resample->SetTransform(transform);
    resample->SetInput          (im);
    resample->SetSize           (im->GetLargestPossibleRegion().GetSize());
    resample->SetOutputOrigin   (im->GetOrigin());
    resample->SetOutputSpacing  (im->GetSpacing());
    resample->SetOutputDirection(im->GetDirection());
    resample->SetDefaultPixelValue(0);
    try
    {
        resample->Update();
    }
    catch (itk::ExceptionObject & err)
    {
        cerr << "ExceptionObject caught !\n" << err << endl;
        throw err;
    }

    return resample->GetOutput();
}
