#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game_object/GameObject.hpp"
#include "geometry/Shape.hpp"

class aObject : public GameObject{
    public:   
        aObject(Window* aWindow,InputManager* m_input, Time* m_time, Shape* m_shape, float initial_pos[3],
        std::string* vert_shader_path = new std::string("shaders/vertex_shaders/MVP_vertex.vert"),std::string* frag_shader_path = new std::string("shaders/fragment_shaders/basic_fragment.frag"));      
    private:
    void Update() override;
    unsigned int test_speed = 3;

};