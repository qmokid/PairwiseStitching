#ifndef __ITKTYPEDEF_H
#define __ITKTYPEDEF_H

#include "itkImage.h"

const unsigned int ImageDimension = 2;
const unsigned int SpaceDimension = ImageDimension;
typedef  itk::Image< float        , ImageDimension >  FloatImageType;
typedef  itk::Image< unsigned char, ImageDimension >  UcharImageType;

typedef double CoordinateRepType;


#endif