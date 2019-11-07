#include "game_object/GameObject.hpp"
 GameObject::GameObject(Window* aWindow,InputManager* m_input, Time* m_time): 
    m_input(m_input),m_time(m_time),m_window(aWindow){
    this->vertex = new float[32]{
            // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    this->indices = new unsigned int[6]{
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    this->vertex_count = 32;  this->indices_count = 6;

    this->width = this->m_window->GetWidth();
    this->height = this->m_window->GetHeight();
    this->SetInitialMVP();

}

GameObject::GameObject(Window* aWindow,InputManager* m_input, Time* m_time, GLfloat* vertex, unsigned int vertex_count,GLuint* indices, unsigned int indices_count):
m_input(m_input),m_time(m_time),vertex(vertex),indices(indices),vertex_count(vertex_count),indices_count(indices_count),m_window(aWindow){

    this->width = this->m_window->GetWidth();
    this->height = this->m_window->GetHeight();
    this->SetInitialMVP();
    
}

GameObject::GameObject(Window* aWindow,InputManager* m_input, Time* m_time, 
GLfloat* vertex, unsigned int vertex_count,GLuint* indices, unsigned int indices_count, float initial_pos[3]):
m_input(m_input),m_time(m_time),vertex(vertex),indices(indices),vertex_count(vertex_count),indices_count(indices_count),m_window(aWindow){
    this->width = this->m_window->GetWidth();
    this->height = this->m_window->GetHeight();
    this->SetInitialMVP();
    model = glm::translate(model,glm::vec3(initial_pos[0],initial_pos[1],initial_pos[2]));
    
}

//Updates the data and send it to GPU
 void GameObject::UpdateAndBuffer(){
        ///Updaets the vertex data
        this->Update();

        //Tells OpenGl how to intepret the data(?????)
        //glBindVertexArray(this->VAOt);
        this->m_vao->UseVAO();
        
        this->shader->UseShader();

        //Pass position parameters to shader
        this->shader->SetUniformMat4fv("MVP",projection*view*model);

        //Checks foe changes in the aspect ratio given a threshold
        if(this->width > (this->m_window->GetWidth() + 40) || this->height > (this->m_window->GetHeight() + 40) ||
                        this->width < (this->m_window->GetWidth() - 40) || this->height < (this->m_window->GetHeight() - 40)){
            projection = glm::mat4(1.0f); projection = glm::perspective(glm::radians(45.0f),(float)this->m_window->GetWidth()/this->m_window->GetHeight(),0.1f,100.0f);
            this->width = this->m_window->GetWidth();
            this->height = this->m_window->GetHeight();
        }

        //Draws the cube
        glDrawElements(GL_TRIANGLES,this->indices_count,GL_UNSIGNED_INT,0);

 }

 void GameObject::SetUpObject(){

     //Creates the VAO object
     this->m_vao = new VAO();
        //Sets how the atrtibutes should be read, ORER MATTERS
        this->m_vao->SetAttribPoint(3,GL_FLOAT,GL_ARRAY_BUFFER);
        this->m_vao->SetAttribPoint(3,GL_FLOAT,GL_ARRAY_BUFFER);
        this->m_vao->SetAttribPoint(2,GL_FLOAT,GL_ARRAY_BUFFER);
    //Finishes the opbject
    this->m_vao->SetUpObject();
    //Buffer data into it
    this->m_vao->BufferData<GLfloat>(this->vertex,GL_ARRAY_BUFFER,GL_FLOAT,this->vertex_count);
    this->m_vao->BufferData<GLuint>(this->indices,GL_ELEMENT_ARRAY_BUFFER,GL_FLOAT,this->indices_count);

    glBindVertexArray(0);    
    this->CreateShaderObject("shaders/vertex_shaders/MVP_vertex.vert","shaders/fragment_shaders/texture_fragment.frag");
        

 }

 void GameObject::CreateShaderObject(std::string vertex_shader, std::string fragment_shader){
    this->shader = new Shader();
    this->shader->LoadShader(vertex_shader,GL_VERTEX_SHADER);
    this->shader->LoadShader(fragment_shader,GL_FRAGMENT_SHADER);
    this->shader->LinkShaders();
    this->shader->SetTexture("Arrow.png");
 }

 void GameObject::SetInitialMVP(){
    //This is the world space matrix
    this->model = glm::mat4(1.0f); model = glm::rotate(model,glm::radians(-10.0f),glm::vec3(1.0f,0.0f,0.0));
    //How our comera is positioned in the world, it is inverted. If we want to move the camera backward, we move the whole scene forward
    this->view = glm::mat4(1.0f); 
    this->view = glm::rotate(model,glm::radians(40.0f),glm::vec3(1.0f,0.0f,0.0f)); 
    view = glm::translate(view,glm::vec3(0.0f,-5.0f,-8.0f));
    //The prjection matrix
    projection = glm::mat4(1.0f); projection = glm::perspective(glm::radians(45.0f),(float)this->m_window->GetWidth()/this->m_window->GetHeight(),0.1f,100.0f);
 }