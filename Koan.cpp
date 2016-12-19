
#include "Koan.h"
//#include "Boid.cpp"

int main(){
	sf::RenderWindow window(sf::VideoMode(width, height), "Flocking");
	std::vector<Boid> boids;
	std::vector<sf::ConvexShape> foods;
	std::vector<std::vector<Point> > map (width + 1, std::vector<Point>(height + 1, Point(EMPTY, 0)));
	for(int i = 0; i < width; i++){
		map[i][0] = Point(WALL, 0);
		map[i][height] = Point(WALL,0);
	}
	for(int j = 0; j < height; j++){
		map[0][j] = Point(WALL,0);
		map[width][j] = Point(WALL,0);
	}

	while (window.isOpen()){
		Mat img_map(height, width, CV_8UC3, Scalar(0,0,0));
		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type){
			// window closed
			case sf::Event::Closed:
				window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Up){
					Boid boid;
					createBoid(boid);
					boids.push_back(boid);
				} else if (event.key.code == sf::Keyboard::Left){
					sf::ConvexShape food;
					createFood(food);
					foods.push_back(food);
					map[food.getPosition().x][food.getPosition().y] = Point(FOOD,0);
				} else if (event.key.code == sf::Keyboard::Down){
					boids.pop_back();
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
			Point v1 = lookForFriends(boids[i], map, img_map);
			Point v2 = giveMeSpace(boids[i], map, img_map);
			Point v3 = uniformVel(boids, map, i);
			cout << v1 << ", " << v2 << ", " << v3 << endl;
			Point nVelocity (boids[i].getVelocityX() + v1.x + v2.x + v3.x,
							boids[i].getVelocityY() + v1.y + v2.y + v3.y);
			boids[i].setVelocity(nVelocity.x / 2.0, nVelocity.y / 2.0);
			float rot = atan2(nVelocity.y, nVelocity.x) * radian2degree;
			boids[i].setRotation(rot);
			map[cvRound(boids[i].getPosition().x)][cvRound(boids[i].getPosition().y)] = Point(EMPTY, 0);
			boids[i].move(nVelocity.x / 2.0, nVelocity.y / 2.0);
			boids[i].setPosition((float)cvRound(boids[i].getPosition().x), (float)cvRound(boids[i].getPosition().y));
			cout << nVelocity << endl;
			printf("Pos: [%f, %f]",  boids[i].getPosition().x,  boids[i].getPosition().y);
//			cout << "Pos: [" << boids[i].getPosition().x << ", " << boids[i].getPosition().y << "]" << endl;
			loopedWorld(boids[i]);
			map[cvRound(boids[i].getPosition().x)][cvRound(boids[i].getPosition().y)] = Point(FRIEND, i);

		}

		//update the boid position
		window.clear();
		for (int i = 0; i < (int)boids.size(); i++){
			window.draw(boids[i]);
		}
		for (int i = 0; i < (int)foods.size(); i++){
			if (map[foods[i].getPosition().x][foods[i].getPosition().y].x != FOOD){
				foods.erase(foods.begin() + i);
			}
			window.draw(foods[i]);
		}
		//		for (int i = 0; i < width; i++){
		//			for (int j = 0; j < height; j++){
		//				if (map[i][j] != EMPTY){
		//					std::cout << "Cell " << i << ", " << j << ": " << map[i][j] << std::endl;
		//				} // else std::cout << ".";
		//			}
		////			std::cout << std::endl;
		//		}
		window.display();
		imshow("Mapa", img_map);
//		waitKey(1);
		//		char a;
		//		std::cin >> a;
		cout << "Fin" << endl << endl << endl;
		waitKey(500);
	}
	return 0;
}


