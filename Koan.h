/*
 * Boids.h
 *
 *  Created on: 17 dic. 2016
 *      Author: irenerrrd
 */

#ifndef KOAN_H_
#define KOAN_H_

//#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "opencv/cv.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>				//basic building blocks
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "Boid.cpp"

#define width 800
#define height 500
#define degree2radian 0.01745329252
#define radian2degree 57.2958279
#define PI 3.14159

#define myopia 15
#define comfort_dis 40
#define mod_vel 1

#define EMPTY 0
#define FRIEND 1
#define FOOD 2
#define ENEMY 3
#define WALL 4

using namespace cv;
using namespace std;

//void createBoid(sf::ConvexShape & boid_name);
//void createFood(sf::ConvexShape & food_name);
//void loopedWorld(sf::ConvexShape & boid);
//bool lookForFood(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map);
//void avoidWalls(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map);
//bool lookForFriends(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map, Mat & im_map);
//void moveRandomly(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map)

float norm_angle(float angle){
	float norm;
	if (angle<PI){
		norm=angle;
		if (angle<-PI){
			norm=2*PI+angle;
		}
	}
	else{
		norm=-(2*PI-angle);
	}
	return norm;
}

Point keepInside(int ax, int ay){
	Point b(0,0);

	if (ax > width) b.x = ax - width;
	else if (ax < 0) b.x = ax + width;
	else b.x = ax;

	if (ay > height) b.y = ay - height;
	else if (ay < 0) b.y = ay + height;
	else b.y = ay;

	return b;
}

float distanceP2P(Point a, Point b){
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void createBoid(Boid & boid_name){
	// Create the  shape
	boid_name.setPointCount(3); //3 to be a triangle
	boid_name.setPoint(0, sf::Vector2f(10.0f, 0.0f)); //This numbers allows the center of the triangle be the same as the sprite center
	boid_name.setPoint(1, sf::Vector2f(-10.0f, 7.5f));
	boid_name.setPoint(2, sf::Vector2f(-10.0f, -7.5f));

	// Give a random color
	int randomR = rand() % 255;
	int randomG = rand() % 255;
	int randomB = rand() % 255;
	sf::Color random_color (randomR, randomG, randomB, 255);
	boid_name.setFillColor(random_color);
	boid_name.setOutlineColor(sf::Color (255, 255, 255, 255));
	boid_name.setOutlineThickness(1);

	// Give a random position
	float randomx = (float)(rand() % width);
	float randomy = (float)(rand() % height);
	boid_name.setPosition(cvRound(randomx), cvRound(randomy));
	boid_name.setRotation((float)(rand() % 360));

	float rotation = boid_name.getRotation() * degree2radian;  //We take into account the rotation to move correctly
	Point2f velocity(cosf(rotation) * mod_vel, sinf(rotation) * mod_vel);
	boid_name.setVelocity(velocity.x, velocity.y);
	cout << "Created boid, vel: " << velocity.x << ", " << velocity.y << endl;
	//	waitKey(0);
}

void createFood(sf::ConvexShape & food_name){
	// Create the  shape
	food_name.setPointCount(4); //4 to be a triangle
	food_name.setPoint(0, sf::Vector2f( 5.0f,  5.0f)); //This numbers allows the center of the triangle be the same as the sprite center
	food_name.setPoint(1, sf::Vector2f(-5.0f,  5.0f));
	food_name.setPoint(2, sf::Vector2f(-5.0f, -5.0f));
	food_name.setPoint(3, sf::Vector2f( 5.0f, -5.0f));

	// Give a color
	sf::Color green (0, 255, 0, 255);
	food_name.setFillColor(green);
	food_name.setOutlineColor(sf::Color (255, 255, 255, 255));
	food_name.setOutlineThickness(1);

	// Give a random position
	float randomx = (float)(rand() % width);
	float randomy = (float)(rand() % height);
	food_name.setPosition(randomx, randomy);
}

void loopedWorld(Boid & boid){
	// Keep the boid in a correct horizontal position
	if (boid.getPosition().x > width) boid.setPosition(boid.getPosition().x - width, boid.getPosition().y);
	if (boid.getPosition().x < 0) boid.setPosition(boid.getPosition().x + width, boid.getPosition().y);

	// Keep the boid in a correct vertical position
	if (boid.getPosition().y > height) boid.setPosition(boid.getPosition().x, boid.getPosition().y - height);
	if (boid.getPosition().y < 0) boid.setPosition(boid.getPosition().x, boid.getPosition().y + height);
}

/*
bool lookForFood(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map){
	bool encontrado = false;
	int x = boid.getPosition().x;
	int y = boid.getPosition().y;
	for (int i = - myopia; i < myopia; i++){
		for (int j = - myopia; j < myopia; j++){
			if (j == 0 && i == 0) break;
				Point pos = keepInside(x + i, y + j);

			if (map[pos.x][pos.y].x == FOOD){
				encontrado = true;

				float angle_boid = norm_angle(boid.getRotation() * degree2radian);
				float angle_wall = norm_angle(atan2(j, i));
				if (norm_angle(angle_boid - angle_wall) < 0 ) boid.rotate(2);
				else boid.rotate(-2);
			}
		}
	}
	return encontrado;
}
 */

/*
void avoidWalls(sf::ConvexShape & boid, std::vector<std::vector<Point> > & map){
	int x = boid.getPosition().x;
	int y = boid.getPosition().y;
	for (int i = - myopia; i < myopia; i++){
		for (int j = - myopia; j < myopia; j++){
			if (j == 0 && i == 0) break;

				Point pos = keepInside(x + i, y + j);

			if (map[posx][posy].x == WALL){
				float angle_boid = norm_angle(boid.getRotation() * degree2radian);
				float angle_food = norm_angle(atan2(j, i));
				if (norm_angle(angle_boid - angle_food) < 0 ) boid.rotate(-5);
				else boid.rotate(5);
			}
		}
	}
}
 */

Point lookForFriends(Boid & boid, std::vector<std::vector<Point> > & map, Mat & im_map){
	int x = boid.getPosition().x;
	int y = boid.getPosition().y;
	Point center (0, 0);
	int found_friends = 0;

	for (int i = - myopia; i < myopia; i++){
		for (int j = - myopia; j < myopia; j++){
			if (j != 0 || i != 0){
				Point pos = keepInside(x+i, y+j);
				if (map[pos.x][pos.y].x == FRIEND){
					printf("Found friend: [%i, %i]", pos.x, pos.y);
					center.x += pos.x;
					center.y += pos.y;
					found_friends ++;
				}
			}
		}
	}
	if (found_friends != 0){
		center.x /= found_friends;
		center.y /= found_friends;
	}
	cout << "Look for friends!" << endl;
	Point dif_pos = keepInside(center.x - x, center.y - y);
	return Point(dif_pos.x / 10.0, dif_pos.y / 10.0);
}

Point giveMeSpace(Boid & boid, std::vector<std::vector<Point> > & map, Mat & im_map){
	int x = boid.getPosition().x;
	int y = boid.getPosition().y;
	Point center (0, 0);
	bool encontrado = false;

	for (int i = - myopia; i < myopia; i++){
		for (int j = - myopia; j < myopia; j++){
			if (j != 0 || i != 0){

				Point pos = keepInside(x+i, y+j);

				if (map[pos.x][pos.y].x != EMPTY && map[pos.x][pos.y].x != FOOD){
					encontrado = true;
					printf("Found intruder: [%i, %i]. I am in [%i, %i]\n", pos.x, pos.y, x, y);
					float dis = distanceP2P(Point(x,y), pos);
					printf("The distance between us is %f\n", dis);
					if (dis < comfort_dis){
						center.x -= (pos.x - x) ;
						center.y -= (pos.y - y) ;
						printf("[%i, %i]", center.x, center.y);
					}
				}
			}
		}
	}
//	waitKey();
	cout << "Give me space!" << endl;
	if (encontrado)
		return keepInside((x - center.x) / 10, (y - center.y) / 10);
	else
		return center;
}

Point uniformVel(vector<Boid> & boids, std::vector<std::vector<Point> > & map, int i){
	Boid boid = boids[i];
	int x = boid.getPosition().x;
	int y = boid.getPosition().y;
	Point velocity = Point(boid.getVelocityX(), boid.getVelocityY());

	int found_friends = 0;
	Point uni_vel(0, 0);

	for (int i = - myopia; i < myopia; i++){
		for (int j = - myopia; j < myopia; j++){
			if (j != 0 || i != 0){

				Point pos = keepInside(x+i, y+j);

				if (map[pos.x][pos.y].x == FRIEND){
					//				std::cout<< "Found Friend " << posx << ", " << posy << std::endl;
					Boid fr = boids[map[pos.x][pos.y].y];
					uni_vel.x += fr.getVelocityX();
					uni_vel.y += fr.getVelocityY();
					found_friends ++;
				}
			}
		}
	}
	if (found_friends != 0){
		velocity.x /= found_friends;
		velocity.y /= found_friends;
	}
	cout << "Uniform velocity" << endl;
	return Point(uni_vel.x - velocity.x, uni_vel.y - velocity.y)/8;
}

/*
 * Given the boid velocity, there is a 70% probability to continue with
 * the same direction there is a 15% proability to turn left and
 * a 15% probability to turn right.
 */
void moveRandomly(Boid & boid, std::vector<std::vector<Point> > & map, int nBoid){
	float rotation = boid.getRotation() * degree2radian;  //We take into account the rotation to move correctly
	sf::Vector2f velocity(cosf(rotation) * mod_vel, sinf(rotation) * mod_vel);

	int keep_mov = (int)(rand() % 100);
	if (keep_mov > 94){
		boid.rotate(1);
	} else if (keep_mov > 97){
		boid.rotate(-1);
	} else {
		map[boid.getPosition().x][boid.getPosition().y] = Point(EMPTY, 0);
		boid.move(velocity.x, velocity.y);
		loopedWorld(boid);
		map[boid.getPosition().x][boid.getPosition().y] = Point(FRIEND, nBoid);
	}
}


#endif /* KOAN_H_ */
