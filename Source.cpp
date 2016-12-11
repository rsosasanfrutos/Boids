#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 300), "Flocking");
	
 
    // We create the  shape	
	sf::ConvexShape boid;
	boid.setPointCount(3); //3 to be a triangle
	boid.setPoint(0, sf::Vector2f(10.0f, 0.0f)); //This numbers allows the center of the triangle be the same as the sprite center
	boid.setPoint(1, sf::Vector2f(-10.0f, 7.5f));
	boid.setPoint(2, sf::Vector2f(-10.0f, -7.5f));

	boid.setFillColor(sf::Color(0, 0, 255, 255)); 
	boid.setOutlineColor(sf::Color::Red);
	boid.setOutlineThickness(2);
	boid.setPosition(50.0f, 50.0f);

	const float degree2radian = (3.14159f / 180.0f);
	float rotation;
	

	sf::Vector2f velocity, velocity_final;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {


			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				rotation = boid.getRotation();  //We take into account the rotation to move correctly
				velocity.x = std::cosf(rotation * degree2radian); // Vector x component
				velocity.y = std::sinf(rotation * degree2radian); // Vector y component
				// Move or rotate with the arrows
				if (event.key.code == sf::Keyboard::Up)
				{
					velocity_final=velocity;
					boid.move(velocity_final.x, velocity_final.y);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					velocity_final = -velocity;
					boid.move(velocity_final.x, velocity_final.y);
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					boid.rotate(-1);
					
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					boid.rotate(1);
				}
					break;

				// we don't process other types of events
			default:
				break;

			}

	

        }
		
		//update the boid position
        window.clear();
        window.draw(boid);
        window.display();
    }

    return 0;
}
