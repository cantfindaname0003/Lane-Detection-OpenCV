#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;
int main() {
	const  Scalar WHITE = Scalar(255, 255, 255);
	string file_name = "e:/solidYellowLeft.mp4";
	VideoCapture cap(file_name);
	system("cls");
	cout << "\n\tFile Processing: " << file_name<<endl<<endl;
	if (!cap.isOpened())  // check if we succeeded
	{	
		cout << "\n\tFile Coudn't Be Loaded!\n\tProgram Will Exit Now!" << endl;
		return -1;
	}
	while (1) {
		Mat kernel = getStructuringElement(0, Size(3, 3), Point(-1, -1));
		Mat frame,gray;
		cap >> frame;			//only process every fifth frame
		if (frame.empty())	break;
		cvtColor(frame,gray,CV_BGR2GRAY);
		equalizeHist(gray, gray);
		//dilate(gray, gray,kernel,Point(-1,-1) ,4);
		threshold(gray, gray, 128, 255, THRESH_OTSU);
		vector<Point> points;
		points.push_back(Point(0,gray.rows));
		points.push_back(Point(gray.cols/2,gray.rows/2));
		points.push_back(Point(gray.cols, gray.rows));
		
		Mat mask = Mat::zeros(gray.size(), CV_8UC1);		
		fillConvexPoly(mask, points, Scalar(255));
		//imshow("mask", mask);
		bitwise_and(gray,mask,mask);
		imshow("roi", mask);
		

		vector<Vec2f> lines;
		Canny(mask, mask, 50, 200, 3);
		imshow("Preprocessed", mask);
		HoughLines(mask,lines,6,CV_PI/60,160,40,25);
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(frame, pt1, pt2,WHITE , 1, CV_AA);
		}
		imshow("test", frame);
		waitKey(2);
	}
	return 0;
}