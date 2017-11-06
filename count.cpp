#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<bits/stdc++.h>

using namespace cv;
using namespace std;

Mat img, gs, hsv;
int main(){
	

	//img = Mat::zeros(100, 1000, CV_8U);

	
	img = imread("Blood Images Dataset/image_150310_001.JPG");
	// imshow("load", img);
	cvtColor(img, img, CV_RGB2GRAY);

	for (int i=0; i<img.rows; i++)
		for (int j=0; j<img.cols; j++)
			if (img.at<uchar>(i,j)>180)
				img.at<uchar>(i,j) = 0;
			else
				img.at<uchar>(i,j) = 255;


	Mat dist;
	distanceTransform(img, dist, CV_DIST_L2, 3);
	normalize(dist, dist, 0.0, 1.0, NORM_MINMAX);
	// imshow("normalized", dist);
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
	cout<<contours.size()<<endl;

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
