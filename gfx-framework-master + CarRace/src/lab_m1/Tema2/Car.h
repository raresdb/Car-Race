#pragma once

#include "components/simple_scene.h"

#define CAR_WIDTH 0.25
#define CAR_LENGTH 0.5
#define CAR_HEIGHT 0.3
#define CAR_SPEED 7
#define CAR_TURN_SPEED 1.5
#define CAR_COLLISION_RANGE 0.45
#define MY_CAR_SPEED_BOOST 1.1

using namespace std;

namespace car {
	class Car {
	public:
		Car(glm::vec3 color, string name) {
            vector<VertexFormat> vertices = {
                VertexFormat(glm::vec3(-CAR_LENGTH / 2 , 0, -CAR_WIDTH / 2), color * glm::vec3(0, 0, 0)),
                VertexFormat(glm::vec3(-CAR_LENGTH / 2, 0, CAR_WIDTH / 2), color * glm::vec3(0, 0, 0)),
                VertexFormat(glm::vec3(CAR_LENGTH / 2, 0, CAR_WIDTH / 2), color * glm::vec3(0, 0, 0)),
                VertexFormat(glm::vec3(CAR_LENGTH / 2, 0, -CAR_WIDTH / 2), color * glm::vec3(0, 0, 0)),
                VertexFormat(glm::vec3(-CAR_LENGTH / 2, CAR_HEIGHT, -CAR_WIDTH / 2), color),
                VertexFormat(glm::vec3(-CAR_LENGTH / 2, CAR_HEIGHT, CAR_WIDTH / 2), color),
                VertexFormat(glm::vec3(CAR_LENGTH / 2, CAR_HEIGHT, CAR_WIDTH / 2), color),
                VertexFormat(glm::vec3(CAR_LENGTH / 2, CAR_HEIGHT, -CAR_WIDTH / 2), color)
            };

            vector<unsigned> indices = {0,1,5,4,0,5,0,3,7,4,0,7,2,3,7,6,2,7,1,2,6,5,1,6,4,5,6,7,4,6};

            car = new Mesh(name);
            car->InitFromData(vertices, indices);
		}
		
		Mesh* car;
	};

}	// namespace car