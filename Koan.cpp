
#include "Koan.h"
//#include "Boid.cpp"

int main(){
	sf::RenderWindow window(sf::VideoMode(width, height), "Flocking");
	std::vector<Boid> boids;
	std::vector<std::vector<Point> > map (width + 1, std::vector<Point>(height + 1, Point(EMPTY, 0)));

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
				if (event.key.code == sf::Keyboard::P){
					cin.get();
				}
				if (event.key.code == sf::Keyboard::Up){
					Boid boid;
					createBoid(boid);
					boids.push_back(boid);
				} else if (event.key.code == sf::Keyboard::Down){
					Boid last_boid = boids.back();
					map[last_boid.getPosition().x][last_boid.getPosition().y].x = EMPTY;
					boids.pop_back();
				}
				break;
			default:
				break;
			}
		}

		for (int i = 0; i < (int)boids.size(); i++){
			// Keep the boid inside de window
			loopedWorld(boids[i]);

			// Make the boid move randomly
			Point2f rand = moveRandomly(boids[i]);
			Point2f v1 = lookForFriends(boids[i], map, img_map);
			Point2f v2 = giveMeSpace(boids[i], map, img_map);
			Point2f v3 = uniformVel(boids, map, i);
//			cout << rand << ", " << v1 << ", " /*<< v2 << ", "*/ << v3 << endl;

			map[boids[i].getPosition().x][boids[i].getPosition().y] = Point(EMPTY, 0);

			Point2f nVelocity (rand.x + v1.x + v2.x + v3.x, rand.y + v1.y + v2.y + v3.y);
			boids[i].setVelocity(nVelocity.x, nVelocity.y);
			float rot = atan2(nVelocity.y, nVelocity.x) * radian2degree;
			boids[i].setRotation(rot);
			boids[i].move(nVelocity.x, nVelocity.y);

			boids[i].setPosition(cvRound(boids[i].getPosition().x), cvRound(boids[i].getPosition().y));
			loopedWorld(boids[i]);
			map[boids[i].getPosition().x][boids[i].getPosition().y] = Point(FRIEND, i);
		}

//		for(int i = 0; i < width; i++){
//			for (int j = 0; j < height; j++){
//				if (map[i][j].x == FRIEND){
//					cout << "FRIEND: [" << i << ", " << j << ". Num: " << map[i][j].y << endl;
//				}
//			}
//		}
//		cout << endl;

		//update the boid position
		window.clear();
		for (int i = 0; i < (int)boids.size(); i++){
			window.draw(boids[i]);
		}

		window.display();
	}
	return 0;
}


