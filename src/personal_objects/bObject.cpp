#include "personal_objects/bObject.hpp"
#include <sys/time.h> 
#include <type_traits>
#include <typeinfo>

bObject::bObject(BasicsBlock* basic_block,Camera* m_camera,Model* model,float initial_pos[3],Shader* m_shader):GameObject
(basic_block,m_camera,model,initial_pos,m_shader){
    m_deque_test = new std::deque<char*>();
    didExit = false;
    /*===============GUI=====================*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void bObject::Ready(){
    obj_iterator = bb->all_objs->begin();
}

void bObject::Update(){
    #ifdef __unix__
    //BE CAREFULL WHEN USING THIS, IT SIMULATES MEMORY LEAK
    if(m_input->ProcessInput(GLFW_KEY_9,GLFW_PRESS)){
        m_deque_test->push_back((char*) malloc (1000000) );
    }
    //THIS CLEANS THE WASTED MEMORY
    if(m_input->ProcessInput(GLFW_KEY_0,GLFW_PRESS)){
        for(int i = 0; i < m_deque_test->size(); i++){
            delete m_deque_test->at(i);
        }
        m_deque_test->clear();
    }
    #endif
    
    if(show_cursor == GLFW_CURSOR_NORMAL) RenderGUI();
    if(m_input->ProcessInput(GLFW_KEY_F1) && f1KeyRealeased){
        
        show_cursor = show_cursor == GLFW_CURSOR_DISABLED? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(m_window->GetWindow(),GLFW_CURSOR,show_cursor);
        f1KeyRealeased = false;
    
    }else if (m_input->ProcessInput(GLFW_KEY_F1,GLFW_RELEASE)){
        f1KeyRealeased = true;
    }

    if(m_input->ProcessInput(GLFW_KEY_1)){
        bb->global_data.fill_type = GL_FILL;
    }else if(m_input->ProcessInput(GLFW_KEY_2)){
        bb->global_data.fill_type = GL_LINE;
    }

}

void bObject::RenderGUI(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        static float vec_pos[3];
        static float *vec_color;
        ImGui::Begin("Game Object Manipulator");
        ImGui::Text("Select Object Name: %s", (*obj_iterator)->object_name.c_str());
        if(ImGui::Button("<<")){
            if(obj_iterator != bb->all_objs->begin()){
                obj_iterator--;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(">>")){
            if(obj_iterator != --bb->all_objs->end()){
                obj_iterator++;
            }
        }
        (*obj_iterator)->isSelected = true;
        vec_pos[0] = (*obj_iterator)->model_mat[3][0];
        vec_pos[1] = (*obj_iterator)->model_mat[3][1];
        vec_pos[2] = (*obj_iterator)->model_mat[3][2];
        ImGui::SliderFloat3("Object position", vec_pos,-5.0,5.0);
        (*obj_iterator)->model_mat[3][0] = vec_pos[0];
        (*obj_iterator)->model_mat[3][1] = vec_pos[1];
        (*obj_iterator)->model_mat[3][2] = vec_pos[2];

        if ((*obj_iterator)->m_material != nullptr){
            vec_color = glm::value_ptr((*obj_iterator)->m_material->diffuse_color);
            ImGui::ColorEdit3("Material Color", vec_color);
            (*obj_iterator)->m_material->ambient_color[0] = (*obj_iterator)->m_material->diffuse_color[0];
            (*obj_iterator)->m_material->ambient_color[1] = (*obj_iterator)->m_material->diffuse_color[1];
            (*obj_iterator)->m_material->ambient_color[2] = (*obj_iterator)->m_material->diffuse_color[2];
           
        }

        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        Light* light;
        light = dynamic_cast<Light*>(*obj_iterator);
        if(light != NULL){
            vec_color = glm::value_ptr(light->light_color);
            ImGui::ColorEdit3("Light Color", vec_color);
            ImGui::SliderFloat("Light Intensity", &light->light_intensity,0.0f,1.0f);
            ImGui::NewLine();
        }
        ImGui::Text("FPS: %f", 1/m_time->delta_time);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
