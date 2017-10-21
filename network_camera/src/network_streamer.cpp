/*
 * network_streamer.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: murat
 */

/*
 * camera_node.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: murat
 */

/*
 * publisher.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: murat
 */
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/Image.h"
#include "image_transport/image_transport.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

struct cameraObject{
	std::string ip;
	std::string port;
	std::string streamURL;
};

int main(int argc, char** argv){
	ros::init(argc, argv, "network_streamer");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Publisher imagePublisher = it.advertise("/networkCam", 1);

    cameraObject networkCamera;
    networkCamera.ip = "http://192.168.1.123";
    networkCamera.port = "81";
    networkCamera.streamURL = "/videostream.cgi?loginuse=admin&loginpas=12345&dummy=param.mjpg";

    cv::namedWindow("Network Stream",1);
    std::string fullURL = networkCamera.ip + ":" + networkCamera.port + networkCamera.streamURL;
    std::cout<<"URL to stream from: "<<fullURL<<std::endl;
    cv::VideoCapture capture(fullURL);
    std::cout<<"capture object initialized"<<std::endl;

    cv::Mat frame;
    std::cout<<"frame created"<<std::endl;

    cv_bridge::CvImage rosImage;

	rosImage.encoding = "bgr8";
	rosImage.header.stamp = ros::Time::now();
	rosImage.header.frame_id = "bar";
	std::cout<<"ros image initialized"<<std::endl;

    while(ros::ok()){
//    	frame = cv::imread("~/Desktop/opencv-logo-small.png");
    	capture>>frame;
    	capture.read(frame);
    	if(frame.empty()){
    		std::cout<<"Frame is empty"<<std::endl;
    		continue;
    	}
    	else{
    		rosImage.image = frame.clone();
			imagePublisher.publish(rosImage.toImageMsg());
			cv::imshow("Network Stream",frame);
			if(cv::waitKey(30) >= 0)
				break;
    	}
    	ros::spinOnce();
	}

	return 0;
}



