#include <boxes.h>
#include <math.h>
#include <navigation.h>
#include <robot_pose.h>
#include <imagePipeline.h>
#define RAD2DEG(rad) ((rad)*180. / M_PI)
#define DEG2RAD(deg) ((deg)*M_PI / 180.)


int BlockDist = 1;

int goal_box;


/*
void coordsconvert(Boxes boxes){

	for (auto it = boxes.coords.begin(); it != boxes.coords.end(); it++)
	{
		it[0] += DISTSCAN/cos(it[2]);
		it[1] += DISTSCAN/sin(it[2]);
		
		if (it[2]>0){
			it[2] = -(M_PI-it[2]);
		}
		else{
			it[2]= M_PI+it[2];
		}		
	}
}
*/

int shortestdist(std::vector<std::vector<float>> boxcoords, RobotPose posecoords){
	
	int dist = [boxcoords.size()];
	
	for (int i = 0; i < boxcoords.size(); ++i){
		dist[i] = sqrt((posecoords[0] - boxcoords[i][0])^2 + (posecoords[1] - boxcoords[i][1])^2);		
	}
	goal_box =findSmallestElement(dist);
	
	return goal_box;
	
}








int findSmallestElement(int arr[]){
   int temp = arr[0];
   for(int i=0; i<arr.size; i++) {
      if(temp>arr[i]) {
         temp=arr[i];
		 ind = i;
      }
   }
   return ind;
}



int main(int argc, char** argv) {
    // Setup ROS.
    ros::init(argc, argv, "contest2");
    ros::NodeHandle n;
    // Robot pose object + subscriber.
    RobotPose robotPose(0,0,0);
    ros::Subscriber amclSub = n.subscribe("/amcl_pose", 1, &RobotPose::poseCallback, &robotPose);
    // Initialize box coordinates and templates
    Boxes boxes; 
	Boxes new_boxes; 
    if(!boxes.load_coords() || !boxes.load_templates()) {
        std::cout << "ERROR: could not load coords or templates" << std::endl;
        return -1;
    }
	
	//Remove this 
	/*
    for(int i = 0; i < boxes.coords.size(); ++i) {
        std::cout << "Box coordinates: " << std::endl;
        std::cout << i << " x: " << boxes.coords[i][0] << " y: " << boxes.coords[i][1] << " z: " 
                  << boxes.coords[i][2] << std::endl;
    }
	*/
	
    // Initialize image objectand subscriber.
    ImagePipeline imagePipeline(n);
	
	
	//put this into a function
	for(int i = 0; i < new_boxes.coords.size(); ++i){
		new_boxes.coords[i][0] = round((new_boxes.coords[i][0] + BlockDist * cos(new_boxes.coords[i][2]))*1000)/1000;
		new_boxes.coords[i][1] = round((new_boxes.coords[i][1] + BlockDist * sin(new_boxes.coords[i][2]))*1000)/1000;
		if (new_boxes.coords[i][2] < 0) {
			new_boxes.coords[i][2] = new_boxes.coords[i][2] + 3.14;
		}
		else {
			new_boxes.coords[i][2] = new_boxes.coords[i][2] - 3.14;
		}
		cout << i << " x: " << new_boxes.coords[i][0] << " y: " << new_boxes.coords[i][1] << " z: " << new_boxes.coords[i][2] << endl;
	}
	
	
	
	
	//coordsconvert(new_boxes);
	
	
    // Execute strategy.
    while(ros::ok()) {
        ros::spinOnce();
        /***YOUR CODE HERE***/
        // Use: boxes.coords
		
		goal_box = shortestdist (boxes.coords,robotPose);
		
		moveToGoal(boxes.coords[goal_box][0],boxes.coords[goal_box][1],boxes.coords[goal_box][2]);
			
		
        // Use: robotPose.x, robotPose.y, robotPose.phi
        imagePipeline.getTemplateID(boxes);
        ros::Duration(0.01).sleep();
    }
    return 0;
}


