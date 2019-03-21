#include "PairwiseStitching.h"

#include <fstream>
#include <direct.h>
#include <mutex>
#include "Stitching.h"
#include "itkFileOutputWindow.h"

using namespace std;

int total_count = 0;
mutex mutex_;

//
//!
//! \brief Do image stitching
//!
void PairwiseStitching()
{
#if ITK_VERSION_MAJOR < 5
    itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
#else
	itk::MultiThreaderBase::SetGlobalMaximumNumberOfThreads(1);
#endif

    typedef itk::FileOutputWindow myFileOutputWindow;
    myFileOutputWindow::Pointer window = myFileOutputWindow::New();
    window->SetFileName("C:/temp/ItkExceptionOutput26.txt");
    itk::OutputWindow::SetInstance(window);

    Stitching seq_stitch;
  
    int local_counter;
    // keep stitching same image pair
    for (int i = 0; i < 1e6; i++)
    {
        {//mutex block
            lock_guard<mutex> lock(mutex_);
            total_count++;
            local_counter = total_count;
            if (total_count % 1000 == 0)
                cout << total_count << endl;
        }

        try
        {
            seq_stitch.DoStitch();
        }
        catch (const itk::ExceptionObject & itk_err)
        {
            lock_guard<mutex> lock(mutex_);
            cout << ("ITK exception during DoStitch!\n\t" + string(itk_err.GetDescription()));
            cout << ("total_count : " + to_string(local_counter));
        }
    }
}