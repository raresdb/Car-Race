#pragma once

#include<vector>
#include "components/simple_scene.h" 

#define ROAD_WIDTH 3.f
#define GRASS_PATCH_DIMENSION 1.f
#define GRASS_FULL_DIMENSION 120.f
#define ROAD_COLOR glm::vec3(0.1, 0.1, 0.1)
#define GRASS_COLOR glm::vec3(0.49, 0.85, 0.31)
#define TREE_TRUNK_COLOR glm::vec3(0.5, 0.44, 0.33)
#define TREE_TRUNK_WIDTH 0.25f
#define TREE_TRUNK_HEIGHT 1.0f
#define TREE_CROWN_COLOR glm::vec3(0.17, 0.37, 0.04)
#define TREE_CROWN_WIDTH 2.f
#define TREE_CROWN_HEIGHT 1.0f
#define SKY_COLOR glm::vec3(0.18, 0.42, 0.95)
#define SKY_HEIGHT 30.f
#define INTERP_POINTS 50

using namespace std;

namespace terrain
{
    class Tree {
    public:
        Tree() {
            vector<VertexFormat> vertices = {
                // trunk vertices
                VertexFormat(glm::vec3(-TREE_TRUNK_WIDTH / 2 , -0.1, -TREE_TRUNK_WIDTH / 2), TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(-TREE_TRUNK_WIDTH / 2, -0.1, TREE_TRUNK_WIDTH / 2), TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(TREE_TRUNK_WIDTH / 2, -0.1, TREE_TRUNK_WIDTH / 2), TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(TREE_TRUNK_WIDTH / 2, -0.1, -TREE_TRUNK_WIDTH / 2), TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(-TREE_TRUNK_WIDTH / 2, TREE_TRUNK_HEIGHT, -TREE_TRUNK_WIDTH / 2),
                TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(-TREE_TRUNK_WIDTH / 2, TREE_TRUNK_HEIGHT, TREE_TRUNK_WIDTH / 2),
                TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(TREE_TRUNK_WIDTH / 2, TREE_TRUNK_HEIGHT, TREE_TRUNK_WIDTH / 2),
                TREE_TRUNK_COLOR),
                VertexFormat(glm::vec3(TREE_TRUNK_WIDTH / 2, TREE_TRUNK_HEIGHT, -TREE_TRUNK_WIDTH / 2),
                TREE_TRUNK_COLOR),

                // crown vertices
                VertexFormat(glm::vec3(-TREE_CROWN_WIDTH / 2 , TREE_TRUNK_HEIGHT, -TREE_CROWN_WIDTH / 2),
                TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(-TREE_CROWN_WIDTH / 2, TREE_TRUNK_HEIGHT, TREE_CROWN_WIDTH / 2),
                TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(TREE_CROWN_WIDTH / 2, TREE_TRUNK_HEIGHT, TREE_CROWN_WIDTH / 2),
                TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(TREE_CROWN_WIDTH / 2, TREE_TRUNK_HEIGHT, -TREE_CROWN_WIDTH / 2),
                TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(-TREE_CROWN_WIDTH / 2, TREE_CROWN_HEIGHT + TREE_TRUNK_HEIGHT,
                -TREE_CROWN_WIDTH / 2), TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(-TREE_CROWN_WIDTH / 2, TREE_CROWN_HEIGHT + TREE_TRUNK_HEIGHT,
                TREE_CROWN_WIDTH / 2), TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(TREE_CROWN_WIDTH / 2, TREE_CROWN_HEIGHT + TREE_TRUNK_HEIGHT,
                TREE_CROWN_WIDTH / 2), TREE_CROWN_COLOR),
                VertexFormat(glm::vec3(TREE_CROWN_WIDTH / 2, TREE_CROWN_HEIGHT + TREE_TRUNK_HEIGHT,
                -TREE_CROWN_WIDTH / 2), TREE_CROWN_COLOR)
            };

            vector<unsigned> indices = {0,1,5,4,0,5,0,3,7,4,0,7,2,3,7,6,2,7,1,2,6,5,1,6,8,9,13,12,8,13,8,11,15,12,8,15,
                10,11,15,14,10,15,9,10,14,13,9,14,4,5,6,7,4,6,12,13,14,15,12,14};

            tree = new Mesh("tree");
            tree->InitFromData(vertices, indices);
        }

        Mesh* tree;
    };

	class Terrain {
	public:
        // list of points for the center of the road -> it repeats the first point
        vector<pair<double, double>> points = {
            {-32.5, 11.1}, {-29.4, 17.8}, {-26.7, 20.0}, {-20.1, 21.4}, {-10.0, 20.0}, {-4.8, 17.3}, {-0.1, 12.7},
            {0.5, 6.9}, {2.9, 2.8}, {8.2, 0.4}, {12.6, 1.3}, {17.2, 4.6}, {19.5, 7.7}, {23.1, 9.3}, {28.7, 9.5},
            {33.3, 8.2}, {34.4, 7}, {35.6, 4.8}, {34.4, -1.0}, {31.2, -3.6}, {25.0, -5.0}, {20.0, -5.0}, {-15.0, -5.0},
            {-21.2, -4.0}, {-27.8, -0.3}, {-32.4, 4.7}, {-33.0, 7.7}, {-32.5, 11.1}
        };

        // same list but with points added between each pair of consecutive original points
        vector<pair<double, double>> interp_points;

        vector<VertexFormat> road_vertices;
        vector<unsigned int> road_indices;

		Terrain() {
            vector<unsigned int> grass_indices;
            vector<VertexFormat> grass_vertices;

            // grass
            
            for (auto i = -GRASS_FULL_DIMENSION / 2; i < GRASS_FULL_DIMENSION / 2; i += GRASS_PATCH_DIMENSION)
                for (auto j = -GRASS_FULL_DIMENSION / 2; j < GRASS_FULL_DIMENSION / 2; j += GRASS_PATCH_DIMENSION)
                {
                    auto idx_i = (i + GRASS_FULL_DIMENSION / 2) / GRASS_PATCH_DIMENSION;
                    auto idx_j = (j + GRASS_FULL_DIMENSION / 2) / GRASS_PATCH_DIMENSION;
                    grass_vertices.push_back(VertexFormat(glm::vec3(i, -0.1, GRASS_PATCH_DIMENSION + j), GRASS_COLOR));
                    grass_vertices.push_back(VertexFormat(glm::vec3(i, -0.1, j), GRASS_COLOR));
                    grass_vertices.push_back(VertexFormat(glm::vec3(GRASS_PATCH_DIMENSION + i, -0.1, j), GRASS_COLOR));
                    grass_vertices.push_back(VertexFormat(glm::vec3(GRASS_PATCH_DIMENSION + i, -0.1,
                        GRASS_PATCH_DIMENSION + j), GRASS_COLOR));
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4);
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4 + 1);
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4 + 2);
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4 + 3);
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4);
                    grass_indices.push_back(4 * idx_i * GRASS_FULL_DIMENSION / GRASS_PATCH_DIMENSION + idx_j * 4 + 2);
                }

            grass = new Mesh("grass");
            grass->InitFromData(grass_vertices, grass_indices);

            // sky

            vector<unsigned int> sky_indices = { 0, 1, 5, 4, 0, 5, 0, 3, 7, 4, 0, 7, 2, 3, 7, 6, 2, 7, 1, 2, 6, 5, 1, 6 };
            vector<VertexFormat> sky_vertices =
            {
                VertexFormat(glm::vec3(-GRASS_FULL_DIMENSION / 2, -SKY_HEIGHT, -GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(GRASS_FULL_DIMENSION / 2, -SKY_HEIGHT, -GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(GRASS_FULL_DIMENSION / 2, -SKY_HEIGHT, GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(-GRASS_FULL_DIMENSION / 2, -SKY_HEIGHT, GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(-GRASS_FULL_DIMENSION / 2, SKY_HEIGHT, -GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(GRASS_FULL_DIMENSION / 2, SKY_HEIGHT, -GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(GRASS_FULL_DIMENSION / 2, SKY_HEIGHT, GRASS_FULL_DIMENSION / 2), SKY_COLOR),
                VertexFormat(glm::vec3(-GRASS_FULL_DIMENSION / 2, SKY_HEIGHT, GRASS_FULL_DIMENSION / 2), SKY_COLOR)
            }
            ;

            sky = new Mesh ("sky");
            sky->InitFromData(sky_vertices, sky_indices);

            // road

            // linear interpolation for creating intermediary points
            for (auto i = 0; i < points.size() - 1; i++) {
                for (auto j = 0; j < INTERP_POINTS; j++) {
                    interp_points.push_back(pair<double, double>(points[i].first + j *
                        (points[i + 1].first - points[i].first) / INTERP_POINTS,
                        points[i].second + j *(points[i + 1].second - points[i].second) / INTERP_POINTS));
                }
            }

            // add the first point again
            interp_points.push_back(interp_points[0]);

            for (auto i = 0; i < interp_points.size() - 1; i++) {
                glm::vec3 p1 = glm::vec3(interp_points[i].first, 0, interp_points[i].second);
                glm::vec3 p2 = glm::vec3(interp_points[i + 1].first, 0, interp_points[i + 1].second);
                glm::vec3 d = p2 - p1;
                glm::vec3 p = glm::normalize(glm::cross(d, glm::vec3(0, 1, 0)));
                road_vertices.push_back(VertexFormat(p1 + p * (ROAD_WIDTH / 2), ROAD_COLOR));
                road_vertices.push_back(VertexFormat(p1 - ROAD_WIDTH / 2 * p, ROAD_COLOR));
                road_indices.push_back(2 * i);
                road_indices.push_back(2 * i + 1);
                road_indices.push_back(2 * i + 3);
                road_indices.push_back(2 * i);
                road_indices.push_back(2 * i + 2);
                road_indices.push_back(2 * i + 3);
            }

            // add the first vertices again
            road_vertices.push_back(road_vertices[0]);
            road_vertices.push_back(road_vertices[1]);

            road = new Mesh("road");
            road->InitFromData(road_vertices, road_indices);

            // tree

            Tree tree_obj;
            tree = tree_obj.tree;
        }

        bool inside_patch(glm::vec3 point, unsigned patch_id) {
            bool inside = false;

            // first triangle
            {
                bool inside_trg = true;

                // triangle vertices
                glm::vec3 p1 = road_vertices[patch_id * 2 * INTERP_POINTS + 0].position;
                glm::vec3 p2 = road_vertices[patch_id * 2 * INTERP_POINTS + 1].position;
                glm::vec3 p3 = road_vertices[(patch_id + 1) * 2 * INTERP_POINTS + 0].position;

                // checking that the point is on the right side of each triangle line
                if (glm::cross(p2 - p1, p3 - p1).y * glm::cross(p2 - p1, point - p1).y < 0)
                    inside_trg = false;
                if (glm::cross(p3 - p1, p2 - p1).y * glm::cross(p3 - p1, point - p1).y < 0)
                    inside_trg = false;
                if (glm::cross(p3 - p2, p1 - p2).y * glm::cross(p3 - p2, point - p2).y < 0)
                    inside_trg = false;

                if (inside_trg)
                    inside = true;
            }

            // second triangle
            {
                bool inside_trg = true;
                glm::vec3 p1 = road_vertices[patch_id * 2 * INTERP_POINTS + 1].position;
                glm::vec3 p2 = road_vertices[(patch_id + 1) * 2 * INTERP_POINTS + 0].position;
                glm::vec3 p3 = road_vertices[(patch_id + 1) * 2 * INTERP_POINTS + 1].position;

                if (glm::cross(p2 - p1, p3 - p1).y * glm::cross(p2 - p1, point - p1).y < 0)
                    inside_trg = false;
                if (glm::cross(p3 - p1, p2 - p1).y * glm::cross(p3 - p1, point - p1).y < 0)
                    inside_trg = false;
                if (glm::cross(p3 - p2, p1 - p2).y * glm::cross(p3 - p2, point - p2).y < 0)
                    inside_trg = false;

                if (inside_trg)
                    inside = true;
            }

            return inside;
        }

		Mesh* road;
		Mesh* grass;
        Mesh* tree;
        Mesh* sky;
	};


}	//namespace terrain