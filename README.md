# Enviro project - Leader and followers
Winter2024 Final Project for EEP520

## Overview
The goal of this project is a cohesive system where follower robots trail after leader robots, creating a dynamic train of robots. In this area, even though obstacles emerge randomly, follower robots are still able to find the way to their precedents. In addition, user could control the leader robot with WASD in the area. and you will find that the robot train will exist persistently no matter how you control the leader shuttle through obstacles.

## Key challenges
### How to Follow
First, to make the robots as a train, it is unreasonable to ask the follower to follow the leader. We should set the sequence of the queue and assign precedent for each follower by defining a variable "watch_name" in class "Follower". Consequently, every follower could accurately watch events, which contains the target location that they should follow.
Second, collision always occurs so that several followers may stick together so that they stop the movements from each other. To avoid the circumstance, three sensors have been utilized to test if they are closed to obstacles or other agents. When the distance is close enough, followers will move backwards and rotate in a random direction. With the random rotation, the follower will have chance to bypass any blocks.

### Leader control
To make the leader move in any direction, rotating should happen before moving forward. So I define a varible "target" to describe the direction that leader should face. Then function "track_velocity" could adjust direction. In particular, angular velocity is initially set as the angle difference between target and current. However, when angle difference become smaller, the angular velocity become smaller, too, which will cost so much time on that. Hence, I have given an mimimum angular velocity of 0.2 to save time of rotation.

### Obstacle randomness
To prove the effectiveness of the project, several abstacles should emerge at any place of the area. Therefore the number of abstacles should be given first, then the position and angle. I have define 2 fixed obstacles and 3-5 random obstacles, which makes the following mission difficult enough.

## Installation and running
To start the project, excute the following command on your terminal:
```bash
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.6 bash
cd $PWD
make
esm start
enviro
```
Then navigate to `http://localhost` you should see a rectangular walled area with 1 leader, 5 followers, 2 fixed abstacles, several random obstacles. You can control the leader with WASD and observe how the followers follow their precedents.
<img src='https://github.com/Fighoture/Leader-and-followers/blob/main/example.png' width=70%>

## Acknowledgments
List sources of information or code used in this project, including libraries, frameworks, and tutorials.
ENVIRO: The multi-agent, multi-user, multi-everything simulator
-https://github.com/klavinslab/enviro

Elma: An event loop manager for embedded systems
-http://klavinslab.org/elma/
