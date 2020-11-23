/*
Title : Image Processing Task - To find local maxima in a horizontal row of +-11 pixels.
Author: Akshaya Chadaga Pandeshwara
Date: 17/11/2020

Description: The Algorithm finds a local maxima in a horizontal row of +-11 pixels, which is averaged before with a horizontal +/-5 pixels.
*/

#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <highlevelmonitorconfigurationapi.h> 
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>  
#include <string>

using namespace cv;
using namespace std;

//function declaration
void AverageFilter(int horizontalbox[], int n, int slidingwindow);
void LocalMaxima(std::vector <int> const& a);

//Global Variables
Mat Image;
int row; // Row Index of an Image
int col; // Column Index of an Image
int const threshold = 10; // Threshold value above which local maxima is chosen.
int counter = 0; // Counter to count the number of average above threshold = 10.
int slidingwindow = 11; // Number of pixels to be considered in sliding window
int intensity; //Intensity of a pixel
size_t numberofimage; //Number of Images within a directory
string output_string = " "; //To store the pixel coordinates and write into a file.txt
ofstream myfile; // file object to store the pixel coordinates

/* START OF MAIN FUNCTION */
int main() {

	clock_t t1, t2; // Clock variable to measure the time required to run the program.
	t1 = clock(); 

	String Directory("C:/Users/akshay/Desktop/FARO/*.bmp"); //Path where the images are stored.
	vector<String> ImageFileVector; 
	glob(Directory, ImageFileVector, false);

	for (numberofimage = 0; numberofimage < ImageFileVector.size(); numberofimage++) // Read images present in the directory 
	{
		string f_index = to_string(numberofimage+1);
		string filename = "Pixel Coordinates_" + f_index + ".txt";
		myfile.open(filename);
		
		Image = imread(ImageFileVector[numberofimage],0);
		cout << "---------------------------------------------------------------------" << endl;
		cout << "Reading Image " << ImageFileVector[numberofimage] << endl;
		if (Image.empty()) //Check whether the image is read or not.
		{
			cout << "Image loading failed.....!" << endl;
			return 0;
		}

		int nrow = Image.rows; // Initialise number of rows of an image to nrow.
		int ncol = Image.cols; // Initialise number of columns of an image to ncol.
		
		cout << "(Row,col):: " << "(" << nrow << "," << ncol << ")" << endl;
		int horizontalbox[21] = { 0 }; // Initialise an array of horizontal box to store 21 pixels.

		//change some pixel value
		for (row = 0; row < nrow; row++) // Iterate through the rows of an image.
		{

			for (col = 0; col < ncol; col = (col + 21)) // Iterate through the columns of an image.
			{
				int filter = 0; //filter vaiable to navigate through the columns of an image.
				while (filter < 21 && (filter + col) < ncol) // Traverse through the column for every 21 pixels.
				{
					int index = filter % 21;	
					intensity = (int)Image.at<uchar>(row, filter + col); //Store the intensity value of pixel cordinates into a variable intensity.
					horizontalbox[index] = intensity; //Store the value into an array(array size: 21).
					filter++;
				}

				int n = sizeof(horizontalbox) / sizeof(horizontalbox[0]); 
				AverageFilter(horizontalbox, n, slidingwindow); // Function call to find average using sliding window concept.

			}
		}

		cout << "Image: " << ImageFileVector[numberofimage] << " completed!" << endl;		
		myfile.close(); //Close file for every image.
	}

	t2 = clock();
	double time_dif = (double)(t2-t1) / CLOCKS_PER_SEC; //Time measured in seconds to execute the code.
	cout << "Total time taken to execute: " << time_dif << endl;

	waitKey(0);
	return 0;
}


/*
This function takes an array of 21 pixels and calculates average using a sliding window concept. 
*/
void AverageFilter(int horizontalbox[], int n, int slidingwindow) //n=21, slidingwindow = 11
{
	int j, sum;
	vector<int> vectaverage; // A vector to store the average.

	for (int i = 0; i <= n - slidingwindow; i++) // 0 to 10
	{
		sum = 0;
		for (j = 0; j < slidingwindow; j++) // 0 to 10 :: output of this loop at a time will have just 1 number, because of sum of 10 numbers inside the window
		{
			sum = sum + horizontalbox[i+j];
		}
		
		int average = sum / slidingwindow; // find the average of 11 pixel values.
		vectaverage.push_back(average); //store the average value in a vector
	}
	LocalMaxima(vectaverage); // Function to find the local maxima.
	vectaverage.clear(); 
}


/*
Function accepts a vector which contains average values. A local maxima is chosen among the averages. Based on the chosen local maxima, 
the 11 pixel coordinates are stored in a text file respectively.
*/
void LocalMaxima(std::vector <int> const& myvector) {

	int maxElementIndex = (int)(max_element(myvector.begin(), myvector.end()) - myvector.begin()); //Index value of maximum element.
	int localmaxima = *max_element(myvector.begin(), myvector.end()); //Maxima among the average values present in the vector.
		if (localmaxima > ::threshold) //Check if the maxima is above the specifies threshold.
		{	
			int localindex = 0;
			while (localindex < 11) // Loop to print all the 11 pixels in the chosen local maxima.
			{
				int newcolumn = col + maxElementIndex + localindex; // To find the column pixel cordinate.
				int Intensity = (int)Image.at<uchar>(row, newcolumn);
				output_string = "Local Maxima: " + to_string(localmaxima) + "\t" + "|Intensity: " + to_string(Intensity) + "\t" + "|Pixel Coordinates: (" + to_string(row) + "," + to_string(newcolumn) + ")" + '\n';
				myfile << output_string; //writing to a file
				localindex++;
			}
			
		}
}