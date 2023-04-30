#pragma once
#include <opencv2/opencv.hpp>

#include <iostream>

using namespace cv;

using namespace std;

void compress(Mat& img, double a, cv::Rect region) {

	if (region.width <= 1 || region.height <= 1) {

		return;

	}

	Mat roi = img(region);

	Scalar mean, stddev;

	meanStdDev(roi, mean, stddev);

	double threshold = a * stddev[0];

	if (stddev[0] < 1e-10) {

		return;

	}

	if (region.width == 2 && region.height == 2) {

		double val = roi.at<uchar>(0, 0);

		for (int i = region.y; i < region.y + region.height; i++) {

			for (int j = region.x; j < region.x + region.width; j++) {

				img.at<uchar>(i, j) = (uchar)val;

			}

		}

		return;

	}

	cv::Rect subregions[4];

	subregions[0] = cv::Rect(region.x, region.y, region.width / 2, region.height / 2);

	subregions[1] = cv::Rect(region.x + region.width / 2, region.y, region.width / 2, region.height / 2);

	subregions[2] = cv::Rect(region.x, region.y + region.height / 2, region.width / 2, region.height / 2);

	subregions[3] = cv::Rect(region.x + region.width / 2, region.y + region.height / 2, region.width / 2, region.height / 2);

	for (int i = 0; i < 4; i++) {

		compress(img, a, subregions[i]);

	}

	bool is_homogeneous = true;

	for (int i = region.y; i < region.y + region.height; i++) {

		for (int j = region.x; j < region.x + region.width; j++) {

			if (abs(img.at<uchar>(i, j) - mean[0]) > threshold) {

				is_homogeneous = false;

				break;

			}

		}

		if (!is_homogeneous) {

			break;

		}

	}

	if (is_homogeneous) {

		double val = mean[0];

		for (int i = region.y; i < region.y + region.height; i++) {

			for (int j = region.x; j < region.x + region.width; j++) {

				img.at<uchar>(i, j) = (uchar)val;

			}

		}

	}

}


