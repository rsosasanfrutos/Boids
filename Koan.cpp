#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <tgmath.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>
#include <vector>

void createBoid(sf::ConvexShape & boid_name);
void loopedWorld(sf::ConvexShape & boid);
void moveRandomly(sf::ConvexShape & boid, sf::Vector2f velocity);

using namespace std;

#define width 500
#define height 300
#define degree2radian 0.01745329252

int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "Flocking");
	vector<sf::ConvexShape> boids;

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type){
			// window closed
			case sf::Event::Closed:
				window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				/*
			    // Move or rotate with the arrows
				if (event.key.code == sf::Keyboard::Up)
				{
					velocity_final=velocity;
					boids[i].move(velocity_final.x, velocity_final.y);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					velocity_final = -velocity;
					boids[i].move(velocity_final.x, velocity_final.y);
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					boids[i].rotate(-1);

				}
				if (event.key.code == sf::Keyboard::Right)
				{
					boids[i].rotate(1);
				}
				*/
				if (event.key.code == sf::Keyboard::Space){
					sf::ConvexShape boid;
					createBoid(boid);
					boids.push_back(boid);
				}
				break;

				// we don't process other types of events
			default:
				break;
			}
		}

		for (int i = 0; i < (int)boids.size(); i++){
			// Keep the boid inside de window
			loopedWorld(boids[i]);

			// Make the boid move randomly
			float rotation = boids[i].getRotation() * degree2radian;  //We take into account the rotation to move correctly
			sf::Vector2f velocity(cosf(rotation) * 0.05, sinf(rotation) * 0.05);
			moveRandomly(boids[i], velocity);
		}

		//update the boid position
		window.clear();
		for (int i = 0; i < (int)boids.size(); i++){
			window.draw(boids[i]);
		}
		window.display();
	}
	return 0;
}

void createBoid(sf::ConvexShape & boid_name){
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
	boid_name.setPosition(randomx, randomy);
}

void loopedWorld(sf::ConvexShape & boid){
	// Keep the boid in a correct horizontal position
	if (boid.getPosition().x > width) boid.setPosition(boid.getPosition().x - width, boid.getPosition().y);
	if (boid.getPosition().x < 0) boid.setPosition(boid.getPosition().x + width, boid.getPosition().y);

	// Keep the boid in a correct vertical position
	if (boid.getPosition().y > height) boid.setPosition(boid.getPosition().x, boid.getPosition().y - height);
	if (boid.getPosition().y < 0) boid.setPosition(boid.getPosition().x, boid.getPosition().y + height);
}

/*
 * Given the boid velocity, there is a 70% probability to continue with
 * the same direction there is a 15% proability to turn left and
 * a 15% probability to turn right.
 */
void moveRandomly(sf::ConvexShape & boid, sf::Vector2f velocity){
	int keep_mov = (int)(rand() % 100);
	if (keep_mov > 85){
		boid.rotate(1);
	} else if (keep_mov > 70){
		boid.rotate(-1);
	} else {
		boid.move(velocity.x, velocity.y);
	}
}
