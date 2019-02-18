#include "Stitching.h"

#include "ImageRegistrationMethodTranslation.h"
#include "itkImageFileReader.h"

using namespace std;

mutex Stitching::mutex_;

//!
//! \brief Constructor
//!        create mats and load svm models
//!
Stitching::Stitching()
    : fixedImage_   ()
    , movingImage_  ()
    , fixedMask_    ()
    , movingMask_   ()
{
    lock_guard<mutex> lock(mutex_);

    fixedImage_  = ReadItkFloatImage("fixed_img.png");
    movingImage_ = ReadItkFloatImage("moving_img.png");
    fixedMask_   = ReadItkUcharImage("fixed_msk.png");
    movingMask_  = ReadItkUcharImage("moving_msk.png");
}

bool Stitching::DoStitch()
{
    return TranslationRegisrator().RunTranslationRegistration(fixedImage_, movingImage_, fixedMask_, movingMask_);
}

/************************************************************************/
/*  Private Functions                                                   */
/************************************************************************/

FloatImageType::Pointer Stitching::ReadItkFloatImage(const std::string& file_path)
{
    typedef itk::ImageFileReader<FloatImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(file_path);
    reader->Update();
    return reader->GetOutput();
}

UcharImageType::Pointer Stitching::ReadItkUcharImage(const std::string& file_path)
{
    typedef itk::ImageFileReader<UcharImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(file_path);
    reader->Update();
    return reader->GetOutput();
}