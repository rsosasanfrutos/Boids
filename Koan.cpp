
#include "Koan.h"
//#include "Boid.cpp"

int main(){
	sf::RenderWindow window(sf::VideoMode(width, height), "Flocking");
	std::vector<Boid> boids;
	std::vector<std::vector<Point> > map (width + 1, std::vector<Point>(height + 1, Point(EMPTY, 0)));
	std::vector<sf::CircleShape> comidas;
	std::vector<sf::CircleShape> depredadores;

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
				else if (event.key.code == sf::Keyboard::Left) {
					sf::CircleShape comida;
						createFood(comida);
						comidas.push_back(comida);
						map[comida.getPosition().x][comida.getPosition().y] = Point(FOOD, 0);
				}
				else if (event.key.code == sf::Keyboard::Right) {
					sf::CircleShape last_comida = comidas.back();
					map[last_comida.getPosition().x][last_comida.getPosition().y].x = EMPTY;
					comidas.pop_back();
				}
				else if (event.key.code == sf::Keyboard::D) {
					sf::CircleShape depredador;
					createenemy(depredador);
					depredadores.push_back(depredador);
				}
				else if (event.key.code == sf::Keyboard::S) {
					sf::CircleShape last_depredador = depredadores.back();
					map[last_depredador.getPosition().x][last_depredador.getPosition().y].x = EMPTY;
					depredadores.pop_back();
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
			Point2f v4 = lookForFood(boids[i], map, img_map);
			Point2f v5 = lookForEnemies(boids[i], map, img_map);
//			cout << rand << ", " << v1 << ", " /*<< v2 << ", "*/ << v3 << endl;

			sf::Vector2f posicion_vieja;
			posicion_vieja = boids[i].getPosition();
			float rotacion_vieja;
			rotacion_vieja = boids[i].getRotation();

			Point2f nVelocity (rand.x + v1.x + v2.x + v3.x + v4.x +v5.x, rand.y + v1.y + v2.y + v3.y + v4.y + v5.y);
			boids[i].setVelocity(nVelocity.x, nVelocity.y);
			float rot = atan2(nVelocity.y, nVelocity.x) * radian2degree;
			boids[i].move(nVelocity.x, nVelocity.y);
			boids[i].setRotation(rot);
			boids[i].setPosition(cvRound(boids[i].getPosition().x), cvRound(boids[i].getPosition().y));
			loopedWorld(boids[i]);
			map[boids[i].getPosition().x][boids[i].getPosition().y] = Point(FRIEND, 0);
			map[posicion_vieja.x][posicion_vieja.y] = Point(EMPTY, 0);
			

		}

		for (int i = 0; i < (int)depredadores.size(); i++) {
			Point2f rand = moveRandomlyEnemy(depredadores[i]);
			Point2f nVelocity(rand.x , rand.y );
			map[depredadores[i].getPosition().x][depredadores[i].getPosition().y] = Point(EMPTY, 0);
			float rot = atan2(nVelocity.y, nVelocity.x) * radian2degree;
			depredadores[i].move(nVelocity.x, nVelocity.y);
			depredadores[i].setRotation(rot);
			depredadores[i].setPosition(cvRound(depredadores[i].getPosition().x), cvRound(depredadores[i].getPosition().y));
			loopedWorldEnemy(depredadores[i]);
			map[depredadores[i].getPosition().x][depredadores[i].getPosition().y] = Point(ENEMY, 0);

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

		for (int i = 0; i < (int)depredadores.size(); i++) {
			window.draw(depredadores[i]);
		}
		
		for (int k = 0; k < (int)comidas.size(); k++) {
			int encontrado=0;
			for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if(comidas[k].getPosition().x + i <= width &&comidas[k].getPosition().x + i >0 && comidas[k].getPosition().y + j>0 && comidas[k].getPosition().y +j<= height && encontrado==0)
					{ 
						if (map[comidas[k].getPosition().x + i][comidas[k].getPosition().y + j].x == FRIEND) {

							encontrado = 1;
							break;

						}
					}
				}
			}

			if (encontrado == 1) {
				map[comidas[k].getPosition().x][comidas[k].getPosition().y] = Point(EMPTY, 0);
				comidas.erase(comidas.begin() + k);
				encontrado = 0;
			}
			
		}
		for (int k = 0; k < (int)comidas.size(); k++) {
			window.draw(comidas[k]);
		}
		window.display();
	
	}
	return 0;
}


