#include "Tema2.h"

using namespace t2;

void Tema2::Init()
{
    // setting the camera behind the desired start position
    camera = new camera::Camera();
    camera->Set(glm::vec3(terr.points[my_car_patch_id].first + 2, 1, terr.points[my_car_patch_id].second),
        glm::vec3(terr.points[my_car_patch_id].first, 0, terr.points[my_car_patch_id].second),
        glm::vec3(-0.66, 0.33, 0));

    // positioning the enemy cars on margins of the start patches
    car_traveled = vector<float>(car_patch_id.size(), 0.0);

    // loading the car meshes
    for (auto i = 0; i < cars.size(); i++) {
        meshes[cars[i].car->GetMeshID()] = cars[i].car;
    }

    // loading the terrain meshes
    meshes[terr.road->GetMeshID()] = terr.road;
    meshes[terr.grass->GetMeshID()] = terr.grass;
    meshes[terr.tree->GetMeshID()] = terr.tree;
    meshes[terr.sky->GetMeshID()] = terr.sky;

    // setting the minimap dimensions
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.y / 3.f, resolution.y / 3.f);

    // adding the shader that creates the illusion of a curved terrain
    {
        Shader* shader = new Shader("CurveShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"),
            GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"),
            GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    car_coll_front = false;
    car_coll_back = false;

    // parse enemy cars for updates
    for (auto i = 0; i < car_patch_id.size(); i++) {
        // points of the patch
        glm::vec3 p1 = glm::vec3(terr.points[car_patch_id[i]].first, 0, terr.points[car_patch_id[i]].second);
        glm::vec3 p2 = glm::vec3(terr.points[car_patch_id[i] + 1].first, 0, terr.points[car_patch_id[i] + 1].second);
        // direction of the road vector (normalized)
        glm::vec3 d = glm::normalize(p2 - p1);
        // normalized vector perpendicular on the direction
        glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));

        // getting the angle of the car using the direction of the road
        float angle = glm::acos(glm::dot(d, glm::vec3(1, 0, 0)));
        if (d.z <= 0)
            angle = angle;
        else
            angle = -angle;

        // next position of the car
        glm::vec3 next_car_pos = p1 + p * (car_dist[i]) + d * (glm::length(p2 - p1) - (car_traveled[i] + CAR_SPEED *
            deltaTimeSeconds));
        // next positions my car will have based on button pressed (W, S)
        glm::vec3 my_next_front_pos = camera->GetTargetPosition() + glm::normalize(
            glm::vec3(camera->forward.x, 0, camera->forward.z)) * cameraSpeed * deltaTimeSeconds;
        glm::vec3 my_next_back_pos = camera->GetTargetPosition() + glm::normalize(
            glm::vec3(camera->forward.x, 0, camera->forward.z)) * (-cameraSpeed * deltaTimeSeconds);

        // signal a collision if the enemy car's next position is too close to my car's next position
        if (glm::length(next_car_pos - my_next_front_pos) <= CAR_COLLISION_RANGE)
            car_coll_front = true;
        if (glm::length(next_car_pos - my_next_back_pos) <= CAR_COLLISION_RANGE)
            car_coll_back = true;

        // move the car if there's no danger of collision
        if(!(glm::length(next_car_pos - my_next_front_pos) <= CAR_COLLISION_RANGE) &&
            !(glm::length(next_car_pos - my_next_back_pos) <= CAR_COLLISION_RANGE))
            car_traveled[i] += CAR_SPEED * deltaTimeSeconds;

        // update the patch of the car if needed
        if (car_traveled[i] > glm::length(p2 - p1)) {
            if (!car_patch_id[i])
                car_patch_id[i] = terr.points.size() - 2;
            else
                car_patch_id[i]--;
            car_traveled[i] = 0;
        }
    }


    my_car_pos = glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z);
    my_car_angle = acos(glm::dot(glm::normalize(glm::vec2(camera->forward.x, camera->forward.z)), glm::vec2(1, 0)));

    // render main scene
    projectionMatrix =  glm::perspective(RADIANS(60.f), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene();

    // saving old camera parameters
    glm::vec3 position = camera->position;
    glm::vec3 center = camera->GetTargetPosition();
    glm::vec3 up = camera->up;

    // render minimap
    projectionMatrix = glm::ortho(-7.f, 7.f, -7.f, 7.f, -10.0f, 10.0f);
    camera->Set(camera->GetTargetPosition() + glm::vec3(0, 1, 0), camera->GetTargetPosition(), glm::vec3(-1, 0, 0));    
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);   
    RenderScene();

    // set camera to old parameters
    camera->Set(position, center, up);
}

void Tema2::RenderScene() {
    // render main terrain
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["road"], shaders["CurveShader"], modelMatrix);
    RenderMesh(meshes["grass"], shaders["CurveShader"], modelMatrix);
    RenderMesh(meshes["sky"], shaders["VertexColor"], modelMatrix);

    // render trees
    for (auto i = 0; i < treePositions.size(); i++)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= t3D::Translate(treePositions[i].first, 0, treePositions[i].second);
        modelMatrix *= t3D::RotateOY(treeAngles[i]);
        RenderMesh(meshes["tree"], shaders["CurveShader"], modelMatrix);
    }

    // render enemy cars
    for (auto i = 0; i < car_patch_id.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);

        // points on margins of the patch
        glm::vec3 p1 = glm::vec3(terr.points[car_patch_id[i]].first, 0, terr.points[car_patch_id[i]].second);
        glm::vec3 p2 = glm::vec3(terr.points[car_patch_id[i] + 1].first, 0, terr.points[car_patch_id[i] + 1].second);
        // distance and its perpendicular vector normalized
        glm::vec3 d = glm::normalize(p2 - p1);
        glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));

        // angle of the car
        float angle = glm::acos(glm::dot(d, glm::vec3(1, 0, 0)));
        if (d.z <= 0)
            angle = angle;
        else
            angle = -angle;

        modelMatrix *= glm::translate(modelMatrix, p1 + p * (car_dist[i]) + d * (glm::length(p2 - p1) -
            car_traveled[i]));
        modelMatrix *= t3D::RotateOY(angle);
        RenderMesh(meshes["car" + to_string(i % cars.size())], shaders["CurveShader"], modelMatrix);
    }

    // render my car
    {
        if (camera->forward.z <= 0)
            my_car_angle = my_car_angle;
        else
            my_car_angle = -my_car_angle;

        glm::mat4 modelMatrix = t3D::Translate(my_car_pos.x, my_car_pos.y, my_car_pos.z);
        modelMatrix *= t3D::RotateOY(my_car_angle);
        RenderMesh(meshes["car3"], shaders["VertexColor"], modelMatrix);
    }

}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set uniforms for the shader
    int location = shader->GetUniformLocation("Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    location = shader->GetUniformLocation("View");
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    location = shader->GetUniformLocation("Projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    location = shader->GetUniformLocation("car_pos");
    glUniform3f(location, camera->GetTargetPosition().x, camera->GetTargetPosition().y, camera->GetTargetPosition().z);
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move forward
    if (window->KeyHold(GLFW_KEY_W)) {
        glm::vec3 next_position = camera->GetTargetPosition() + glm::normalize(
            glm::vec3(camera->forward.x, 0, camera->forward.z)) * cameraSpeed * deltaTime;

        // surrounding patches
        unsigned next_car_patch_id = (my_car_patch_id == terr.points.size() - 2) ? 0 : my_car_patch_id + 1;
        unsigned prev_car_patch_id = (my_car_patch_id == 0) ? terr.points.size() - 2 : my_car_patch_id - 1;

        // if still on road and no collision detected move the car
        if (terr.inside_patch(next_position, prev_car_patch_id) ||
            terr.inside_patch(next_position, my_car_patch_id) ||
            terr.inside_patch(next_position, next_car_patch_id))
            if (!car_coll_front)
                camera->MoveForward(cameraSpeed * deltaTime);

        // update patch if needed
        if (!terr.inside_patch(camera->GetTargetPosition(), my_car_patch_id)); {
            if (terr.inside_patch(camera->GetTargetPosition(), next_car_patch_id))
                my_car_patch_id = next_car_patch_id;

            if (terr.inside_patch(camera->GetTargetPosition(), prev_car_patch_id))
                my_car_patch_id = prev_car_patch_id;
        }
    }

    // turn left
    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateThirdPerson_OY(rotationSpeed * deltaTime);
    }

    // move backwards
    if (window->KeyHold(GLFW_KEY_S)) {
        glm::vec3 next_position = camera->GetTargetPosition() + glm::normalize(
            glm::vec3(camera->forward.x, 0, camera->forward.z)) * (-cameraSpeed * deltaTime);

        unsigned next_car_patch_id = (my_car_patch_id == terr.points.size() - 2) ? 0 : my_car_patch_id + 1;
        unsigned prev_car_patch_id = (my_car_patch_id == 0) ? terr.points.size() - 2 : my_car_patch_id - 1;

        if (terr.inside_patch(next_position, prev_car_patch_id) ||
            terr.inside_patch(next_position, my_car_patch_id) ||
            terr.inside_patch(next_position, next_car_patch_id))
            if (!car_coll_back)
                camera->MoveForward(-cameraSpeed * deltaTime);

        if (!terr.inside_patch(camera->GetTargetPosition(), my_car_patch_id)); {
            if (terr.inside_patch(camera->GetTargetPosition(), next_car_patch_id))
                my_car_patch_id = next_car_patch_id;

            if (terr.inside_patch(camera->GetTargetPosition(), prev_car_patch_id))
                my_car_patch_id = prev_car_patch_id;
        }
    }

    // turn right
    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateThirdPerson_OY(-rotationSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}