#include <ros/ros.h>
#include <ros/package.h>
#include <file_server/GetBinaryFile.h>
#include <fstream>

bool get_file(
	file_server::GetBinaryFile::Request &req,
	file_server::GetBinaryFile::Response &res)
{

	// analyse request:
	if (req.name.compare(0,10,"package://")!=0)
	{
		ROS_INFO("only \"package://\" adresses allowed.");
		return true;
	}	
	std::string adress = req.name.substr(10);
	std::string package = adress.substr(0,adress.find("/"));
	std::string filepath = adress.substr(package.length());
	std::string directory = ros::package::getPath(package);
	directory+=filepath;
	
	// open file:
	std::ifstream inputfile(directory.c_str(),std::ios::binary);

	// stop if file does not exist:
	if(!inputfile.is_open())
	{
		ROS_INFO("file \"%s\" not found.\n", req.name.c_str());
		return true;
	}

	// read file contents:	
	res.value.assign(
		(std::istreambuf_iterator<char>(inputfile)),
		std::istreambuf_iterator<char>());

	ROS_INFO("get_file request: %s\n", req.name.c_str());
	//ROS_INFO("package: %s\n",package.c_str());
	//ROS_INFO("filepath: %s\n",filepath.c_str());
	//ROS_INFO("directory: %s\n",directory.c_str());

	return true;
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "file_server");
	ros::NodeHandle n;
	ros::ServiceServer serviceServer = n.advertiseService("/file_server/get_file", get_file);
	ROS_INFO("ROSbridge File Server initialized.");
	ros::spin();

	return 0;
}


	
