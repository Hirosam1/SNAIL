#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include<thread>
#include <chrono>

#include "personal_objects/aObject.hpp"
#include "personal_objects/bObject.hpp"
#include "personal_objects/cObject.hpp"
#include "personal_objects/movnigObject.hpp"

#include "game_managing/BasicsBlock.hpp"
#include"game_object/GameObject.hpp"
#include "game_object/Camera.hpp"
#include "game_tools/Debugging.hpp"
#include "game_managing/InputManager.hpp"
#include "geometry/Shape.hpp"
#include "game_managing/Time.hpp"
#include "graphics/Window.hpp"
#include "game_object/Light.hpp"


class GameManager{
    private:
        //------------------------------------- Variables ---------------------------------------------------------------

        //The main window of the application
        Window* main_window;
        //default width and height of game window
        int width;
        int height;
        //A block of data containing all indexes of all of the most important objects
        BasicsBlock* basic_block;

        //Number of cores
        unsigned int supported_concurrency;
        //Threads in program
        std::thread* threads = nullptr; 
        //Thread specifically for input
        std::thread input_thread;
        //the current width and height of game window
        int current_width;
        int current_height;

        //is game engine ready to star rendering?
        bool ready_to_start;

        //The input manager
        InputManager* main_input;
    
        //name of the game
        std::string game_name;

        //All objects to be rendered/prossessed on the scene
        std::vector<GameObject*>* all_objs;

        //Global time manager
        Time* main_time;

        //------------------------------------- Methods ----------------------------------------------------------------
        
        //Error handler of GLFW
        static void ErrorCallback(int error, const char* description);

        //Callback when Buffer size changes
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

        //Function that handles the input in a separate thread
        static void CheckInput(Window* window_to_check);

        //Sets up the game Objects
        void SetUpObjects();
        
        //Terminate Engine
        void TerminateEngine();

    public:
        int MAX_FRAMERATE = 60;
        GameManager(std::string game_name,int width, int height);
        //Initiate the game engine, and become ready to start
        void EngineInit();
        //Starts the main loop of the game engine
        void EngnieStart();
};