/*
 * Boid.cpp
 *
 *  Created on: 17 dic. 2016
 *      Author: irenerrrd
 */
#include <SFML/Graphics.hpp>

class Boid: public sf::ConvexShape{
public:
	float getVelocityX() const {
		return velocity_x;
	}

	float getVelocityY() const {
		return velocity_y;
	}

	void setVelocity(float vX, float vY) {
		velocity_x = vX;
		velocity_y = vY;
	}

private:
	float velocity_x;
	float velocity_y;


};

