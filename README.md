# PairwiseStitching

There is a bug in my developing program (Not sure if it's ITK library bug),
this is a minimum sharable and reproducible example of it.
The program repeat doing translation registration on 2 images (+ 2 masks)
It use threads, each thread is doing the same registration again and again.

"Ocassionally" there will be an ITK exception.
The "Ocassion" has strange pattern.
I trace the "total_count" in PairwiseStitching.cpp, the exception usually happen around
total count 26xxx, 52xxx, 79xxx, ... (xxx means some number)
For example, 26367, 26369, 26373, 52693, 52699, 79070, 105413, 105416

I run the code on a Intel Xeon CPU E5-2699A v4 server (2 CPU)
Each CPU has 44 logical processors.
When the program run, it is doing stitching on 44 threads independently


## Other Notes
- The ITK version I'm using is 4.12.2. I've tried update to 4.13.1 and 5.0beta3, which didn't help
- The compiler I use is Visual Studio 2017 version 15.7.3
- The "itkImageIOFactoryRegisterManager.h" and "itkTransformIOFactoryRegisterManager.h" is copied from older vesrion ITK. Somehow the code can't compile with these 2 files
- Please find the example error messages and image/mask for registration in the files
