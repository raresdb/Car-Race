#version 330

// Input
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_coord;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_pos;

// Output
out vec3 frag_color;
out vec3 frag_coord;
out vec3 frag_normal;
out vec3 frag_position;

void main()
{
    vec3 world_position = (Model * vec4(vertex_position, 1)).xyz;

    frag_color = vertex_color;
    frag_coord = vertex_coord;
    frag_normal = vertex_normal;
    frag_position = vec3(world_position.x, world_position.y - pow(length(car_pos - world_position), 2) * 0.01,
        world_position.z);

    gl_Position = Projection * View * vec4(frag_position, 1);
}
