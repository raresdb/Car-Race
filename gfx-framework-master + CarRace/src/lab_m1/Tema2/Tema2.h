#pragma once

#include<vector>
#include "components/simple_scene.h"

#include "camera.h"
#include "Terrain.h"
#include "Car.h"
#include "transf3D.h"

using namespace std;

namespace t2
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        void Init() override;
        void RenderScene();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        // camera related objects
        camera::Camera* camera;
        glm::mat4 projectionMatrix;
        ViewportArea miniViewportArea;

        // camera speeds
        float cameraSpeed = CAR_SPEED * MY_CAR_SPEED_BOOST;
        float rotationSpeed = CAR_TURN_SPEED;

        terrain::Terrain terr;

        // tree data
        vector<pair<int, int>> treePositions = { {-10, -21}, {-5, 11} , {0, 18} , {-18, -11}, {23, -11}, {-11, 14}, {10, 8}, {-8, 32}, {-22, -35}, {0, -10}, {20,20} , {50, 0} , {30, -2} , {48, 51}, {30, -10}, {34, 15}, {48, 17}, {-34, 35}, {-20, 18}, {-25, 13}, {-45, 15}, {30, -14}, {27, -16}, {-30, -15}, {-20, 0}, {-32, 1} , {0, 0}, {3, -2}, {-35, 15}, {40, 0}, {13, -1}, {-28, 22} , {36, -3} , {8, -7} };
        vector<float> treeAngles = { 0.5, 0.8, 1.2, 1.8, 2.3, 2.2, 1.1, 0.3, 1.1, 0.5, 2.8, 1.4, 2.8, 3.1, 2.2, 1.8, 0.2, 0.8, 0.4, 0.2, 1.5, 0.3, 0.8, 1.4, 2.7, 0.5, 1.3, 1.1, 2.2, 1.8, 0.5, 1.2, 0.8, 1.7};

        // car data
        vector<car::Car> cars = { car::Car(glm::vec3(1, 1, 1), "car0"), car::Car(glm::vec3(1, 0, 0), "car1"), car::Car(glm::vec3(0, 1, 0), "car2"), car::Car(glm::vec3(0, 0, 1), "car3"), car::Car(glm::vec3(0.27, 0.29, 0.31), "car4"), car::Car(glm::vec3(0.35, 0.44, 0.63), "car5") };
        vector<unsigned> car_patch_id = {0, 2, 4, 8, 10, 14, 16, 22, 25};
        vector<float> car_dist = { 0.3, -0.5, 0.9, -0.7, 0.2, -0.8, 0.2, -0.5, 0.3};
        vector<float> car_traveled;

        // my car data
        int my_car_patch_id = 20;
        float my_car_angle;
        glm::vec3 my_car_pos;

        // the 2 types of collision -> frontal or rear
        bool car_coll_front = false;
        bool car_coll_back = false;
    };

}   // namespace t2