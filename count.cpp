#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<bits/stdc++.h>

using namespace cv;
using namespace std;

Mat img, gs, hsv;

int count(Mat img)
{

	Mat dist;
	distanceTransform(img, dist, CV_DIST_L2, 3);
	normalize(dist, dist, 0.0, 1.0, NORM_MINMAX);
	namedWindow("normalized",WINDOW_NORMAL);
	resizeWindow("normalized", 600,600);
	imshow("normalized", dist);
	namedWindow("thresh",WINDOW_NORMAL);
	resizeWindow("thresh", 600,600);
	// imwrite("thresh.jpg",thresh);
	threshold(dist, dist, .4, 1., CV_THRESH_BINARY);
	// imshow("thresh", dist);
	// cvtColor(dist, dist, CV_BGR2GRAY);
	Mat now;

	dist.convertTo(now, CV_8U);
	// cvtColor(dist, dist, CV_BGR2GRAY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// RNG rng(12345);
	findContours( now, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	// Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();
	// // Detect blobs.
	// std::vector<KeyPoint> keypoints;
	// detector->detect(img_bw, keypoints);

	// // Draw detected blobs as red circles.
	// // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	// Mat im_with_keypoints;
	// drawKeypoints( dist, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	 
	// Show blobs
	imshow("thresh", dist );
	return contours.size();
}
int main(int argc, char *argv[]){
	

	//img = Mat::zeros(100, 1000, CV_8U);
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

	GaussianBlur(img,img, Size(3, 3), 0, 0);
	GaussianBlur(wbc,wbc, Size(9, 9), 0, 0);

	for (int i=0;i<hsv.rows; i++)
		for (int j=0; j<hsv.cols; j++)
		{
			sat.at<uchar>(i,j) = hsv.at<Vec3b>(i,j)[1];
			green.at<uchar>(i,j) = org.at<Vec3b>(i,j)[1];
			if(int(sat.at<uchar>(i,j)>50))
			{
				wbc.at<uchar>(i,j) = 255;
				rbc1.at<uchar>(i,j) = 0;
			}
			else
				wbc.at<uchar>(i,j) = 0;
		}


	fastNlMeansDenoising(wbc, wbc, 3, 7);
	GaussianBlur(wbc,wbc, Size(71, 71), 0, 0);
	// medianBlur(wbc,wbc,5);

	namedWindow("sat",WINDOW_NORMAL);
	resizeWindow("sat", 600,600);
	imshow("sat", wbc);

	Mat rbc2 = rbc1.clone();
	for (int i=0; i<rbc1.rows; i++)
		for (int j=0; j<rbc1.cols; j++)
			if (org.at<Vec3b>(i,j)[1]<145 && rbc1.at<uchar>(i,j)!=0)
				rbc1.at<uchar>(i,j) = 255;
			else
				rbc1.at<uchar>(i,j) = 0;



	for (int i=0; i<rbc2.rows; i++)
		for (int j=0; j<rbc2.cols; j++)
			if (rbc2.at<uchar>(i,j)<180 && rbc2.at<uchar>(i,j)!=0)
				rbc2.at<uchar>(i,j) = 255;
			else
				rbc2.at<uchar>(i,j) = 0;

	GaussianBlur(rbc2,rbc2, Size(11, 11), 0, 0);
	namedWindow("rbc2",WINDOW_NORMAL);
	resizeWindow("rbc2", 600,600);
	imshow("rbc2", rbc2);

	namedWindow("green",WINDOW_NORMAL);
	resizeWindow("green", 600,600);
	imshow("green", green);

	int count1 = count(rbc1);
	int count2 = count(rbc2);
	// cout<<count1<<"  "<<count2<<endl;
	if (count1 > count2)
		cout<<"RBC: "<<count1<<endl;
	else
		cout<<"RBC: "<<count2<<endl;
	cout<<"WBC: "<<count(wbc)<<endl;
	cout<<endl;


	// gs = Mat::zeros(img.size(), CV_8U);
	// cvtColor(img, hsv, CV_RGB2HSV);
	// imshow("gs", gs);

	// createTrackbar("A", "win", &thresh, 10, on_trackbar2);
	// setMouseCallback("win", onClick, NULL);

		/*
	Mat gs, hsv;
	cvtColor(img,gs,CV_RGB2GRAY);
	cvtColor(img, hsv, CV_RGB2HSV);
	
	imshow("test image", img);
	imshow("grayscale", gs);
	imshow("hsv", hsv);
	*/

	//Mat hsv;
	//cvtColor(img, hsv, CV_RGB2HSV);
	//imshow("hsv",hsv);

	waitKey(0);
}
