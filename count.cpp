#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<bits/stdc++.h>
#define NORMALISE_THRESH_WBC	.4
#define NORMALISE_THRESH_RBC	.34
#define LOW_LUM_COUNT		120
#define GREEN_INTENSITY_RBC	145
#define INTENSITY_RBC 		180
#define SATURATION_WBC 		50
#define BLUR_DIM_WBC 		71
#define BLUR_DIM_RBC 		11

using namespace cv;
using namespace std;

vector<vector<Point> > count(Mat img, int type=0)
{

	Mat dist;
	distanceTransform(img, dist, CV_DIST_L2, 3);
	normalize(dist, dist, 0.0, 1.0, NORM_MINMAX);
	// namedWindow("normalized",WINDOW_NORMAL);
	// resizeWindow("normalized", 600,600);
	// imshow("normalized", dist);
	// namedWindow("thresh",WINDOW_NORMAL);
	// resizeWindow("thresh", 600,600);
	// imwrite("thresh.jpg",thresh);
	Mat dist2 = dist.clone();
	if (type)
		threshold(dist, dist, NORMALISE_THRESH_WBC, 1., CV_THRESH_BINARY);
	else
	{
		threshold(dist, dist, NORMALISE_THRESH_RBC, 1., CV_THRESH_BINARY);
		threshold(dist2, dist2, NORMALISE_THRESH_WBC, 1., CV_THRESH_BINARY);
	}

	// imshow("thresh", dist);
	Mat now;
	Mat now2;

	dist.convertTo(now, CV_8U);
	dist2.convertTo(now2, CV_8U);

	vector<vector<Point> > contours;
	vector<vector<Point> > contours2;
	vector<Vec4i> hierarchy;
	vector<Vec4i> hierarchy2;
	findContours( now, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	findContours( now2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	 
	// Show blobs
	// imshow("thresh", dist );
	// cout<<"contours "<<contours.size()<<" "<<contours2.size()<<endl;


	if (contours2.size() > LOW_LUM_COUNT)
		return contours2;
	if (contours.size() > contours2.size())
		return contours;
	else
		return contours2;
}


int main(int argc, char *argv[])
{	
	Mat img;
	if (argc>1)
		img = imread(argv[1]);
	else
	{
		cerr<<"Please provide path to image as argument.\n";
		exit(-1);
	}
	Mat hsv, wbc = img.clone();
	Mat rbc1;
	Mat sat = img.clone();
	Mat green = img.clone();
	Mat org = img.clone();
	cvtColor(img, hsv, CV_BGR2HSV);
	cvtColor(img, rbc1, CV_RGB2GRAY);
	cvtColor(wbc, wbc, CV_RGB2GRAY);
	cvtColor(sat, sat, CV_RGB2GRAY);
	cvtColor(green, green, CV_RGB2GRAY);

	GaussianBlur(rbc1,rbc1, Size(3, 3), 0, 0);
	GaussianBlur(wbc,wbc, Size(9, 9), 0, 0);

	for (int i=0;i<hsv.rows; i++)
		for (int j=0; j<hsv.cols; j++)
		{
			sat.at<uchar>(i,j) = hsv.at<Vec3b>(i,j)[1];
			green.at<uchar>(i,j) = org.at<Vec3b>(i,j)[1];
			if(int(sat.at<uchar>(i,j) > SATURATION_WBC))
			{
				wbc.at<uchar>(i,j) = 255;
				rbc1.at<uchar>(i,j) = 0;
			}
			else
				wbc.at<uchar>(i,j) = 0;
		}


	fastNlMeansDenoising(wbc, wbc, 3, 7);
	GaussianBlur(wbc,wbc, Size(BLUR_DIM_WBC, BLUR_DIM_WBC), 0, 0);
	// medianBlur(wbc,wbc,5);

	// namedWindow("sat",WINDOW_NORMAL);
	// resizeWindow("sat", 600,600);
	// imshow("sat", wbc);

	Mat rbc2 = rbc1.clone();
	for (int i=0; i<rbc1.rows; i++)
		for (int j=0; j<rbc1.cols; j++)
			if (org.at<Vec3b>(i,j)[1]<GREEN_INTENSITY_RBC && rbc1.at<uchar>(i,j)!=0)
				rbc1.at<uchar>(i,j) = 255;
			else
				rbc1.at<uchar>(i,j) = 0;



	for (int i=0; i<rbc2.rows; i++)
		for (int j=0; j<rbc2.cols; j++)
			if (rbc2.at<uchar>(i,j)<INTENSITY_RBC && rbc2.at<uchar>(i,j)!=0)
				rbc2.at<uchar>(i,j) = 255;
			else
				rbc2.at<uchar>(i,j) = 0;

	GaussianBlur(rbc2,rbc2, Size(BLUR_DIM_RBC, BLUR_DIM_RBC), 0, 0);
	// namedWindow("rbc2",WINDOW_NORMAL);
	// resizeWindow("rbc2", 600,600);
	// imshow("rbc2", rbc2);

	// namedWindow("green",WINDOW_NORMAL);
	// resizeWindow("green", 600,600);
	// imshow("green", green);

	vector<vector<Point> > count3 = count(wbc,1);
	vector<vector<Point> > count1 = count(rbc1);
	vector<vector<Point> > count2 = count(rbc2);
	// cout<<count1<<"  "<<count2<<endl;

	namedWindow("RBC final",WINDOW_NORMAL);
	resizeWindow("RBC final", 600,600);

	namedWindow("WBC final",WINDOW_NORMAL);
	resizeWindow("WBC final", 600,600);

	Mat display_rbc(rbc1.rows, rbc1.cols, CV_8UC1, Scalar(0));
	Mat display_wbc(rbc1.rows, rbc1.cols, CV_8UC1, Scalar(0));
	// Mat display_wbc = zeros(wbc.rows, wbc.cols);
	if (count1.size() > count2.size())
	{
		cout<<"RBC: "<<count1.size()<<endl;
		for (int i=0; i<count1.size(); i++)
			for(int j=0; j<count1[i].size(); j++)
				display_rbc.at<uchar>(count1[i][j].y, count1[i][j].x) = 255;
	}
	else
	{
		cout<<"RBC: "<<count2.size()<<endl;
		for (int i=0; i<count2.size(); i++)
			for(int j=0; j<count2[i].size(); j++)
				display_rbc.at<uchar>(count2[i][j].y, count2[i][j].x) = 255;

	}
	cout<<"WBC: "<<count3.size()<<endl;
	for (int i=0; i<count3.size(); i++)
		for(int j=0; j<count3[i].size(); j++)
			display_wbc.at<uchar>(count3[i][j].y, count3[i][j].x) = 255;
	cout<<endl;
	imshow("RBC final",display_rbc);
	imshow("WBC final",display_wbc);
	waitKey(0);
}
