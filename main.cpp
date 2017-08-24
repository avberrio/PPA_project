//#include "stdafx.h"
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;


int main()
{
    
    /* start producing Gaussian filter kernel*/
    const double PI = 4.0*atan(1.0) ;
    double sigma=  2;
    const int kernalWidth=20;
    const int kernalHeight=20;
    
    float kernalArray[kernalWidth][kernalHeight];
    
    double total=0;
    
    //calculate each relavant value to neighour pixels and store it in 2d array
    for(int row=0;row<kernalWidth;row++){
        
        for(int col=0;col<kernalHeight;col++){
            
            float value=(1/(2*PI*pow(sigma,2)))*exp(-(pow(row-kernalWidth/2,2)+pow(col-kernalHeight/2,2))/(2*pow(sigma,2)));
            
            kernalArray[row][col]=value;
            
            total+=value;
        }
    }
    
    //Scale value in 2d array in to 1
    for(int row=0;row<kernalWidth;row++){
        for(int col=0;col<kernalHeight;col++){
            
            kernalArray[row][col]=kernalArray[row][col]/total;
            
        }
    }
    
    /*End producing Gaussian filter kernel*/
    
    Mat RGBImage;
    
    RGBImage =imread("my_image.jpg");
    
    Mat grayScaleImage(RGBImage.size(),CV_8UC1);
    Mat openCvGaussianFilterImage(RGBImage.size(),CV_8UC1);
    
    Mat FinalImage(RGBImage.size(),CV_8UC1);
    cvtColor(RGBImage,grayScaleImage,CV_RGB2GRAY);
    
    
    
    int rows=grayScaleImage.rows;
    int cols=grayScaleImage.cols;
    
    
    int verticleImageBound=(kernalHeight-1)/2;
    int horizontalImageBound=(kernalWidth-1)/2;
    
    //Assian Gaussian Blur value of the center point.Repeating this process for all other points through image
    
//    for(int row=0+verticleImageBound;row<rows-verticleImageBound;row++){
//        
//        for(int col=0+horizontalImageBound;col<cols-horizontalImageBound;col++){
//            
//            float value=0.0;
//            
//            for(int kRow=0;kRow<kernalHeight;kRow++){
//                for(int kCol=0;kCol<kernalWidth;kCol++){
//                    //multiply pixel value with corresponding gaussian kernal value
//                    float pixel=grayScaleImage.at<uchar>(kRow+row-verticleImageBound,kCol+col-horizontalImageBound)*kernalArray[kRow][kCol];
//                    value+=pixel;
//                }
//            }
//            //assign new values to central point
//            FinalImage.at<uchar>(row,col)=cvRound(value);
//            
//        }
//        
//    }
    
    // Linear matrix
    for (int i = 0; i < rows * cols; i ++)
    {
        int row = floor(i/cols);
        int col = i % cols;
        if (row >= verticleImageBound && row < rows - verticleImageBound && col >= horizontalImageBound && col < cols-horizontalImageBound)
        {
            float value = 0.0;
            // Code Here
            for(int kRow = 0; kRow < kernalHeight; kRow++)
            {
                for(int kCol = 0; kCol < kernalWidth; kCol++)
                {
                    //multiply pixel value with corresponding gaussian kernal value
                    float pixel = grayScaleImage.at<uchar>(kRow + row - verticleImageBound, kCol + col - horizontalImageBound) * kernalArray[kRow][kCol];
                    value += pixel;
                }
            }
            FinalImage.at<uchar>(row,col)=cvRound(value);
        }
    }
    
    //Genarate same GaussianBlur image using inbuilt openCv function
    GaussianBlur( grayScaleImage, openCvGaussianFilterImage, Size( 5, 5 ), 0 ,0 );
    
    
//    namedWindow("Original Image",1);
    imwrite("1 - Original Image.png",grayScaleImage);
    
//    namedWindow("Filtered Image",1);
    imwrite("2 - Filtered Image.png",FinalImage);
    
//    namedWindow("openCvGaussianFilterImage",1);
    imwrite("3 - openCvGaussianFilterImage.png",openCvGaussianFilterImage);
    
    return 0;
}
