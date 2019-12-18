#include "game_object/GameObject.hpp"

 GameObject::GameObject(BasicsBlock* basic_block, Camera* m_camera,float initial_pos[3]): 
    m_input(basic_block->m_input),m_time(basic_block->m_time),m_window(basic_block->m_window), m_camera(m_camera){
   this->color = glm::vec3(0);
   this->shader = nullptr;this->m_vao = nullptr;
   //this->MVP_string = new std::string("MVP");
   model = glm::translate(model,glm::vec3(initial_pos[0],initial_pos[1],initial_pos[2]));
   this->SetInitialMVP();
   this->m_light = nullptr;

   this->Model_string = new std::string("Model");
   this->View_string = new std::string("View");
   this->Projection_string = new std::string("Projection");
   
   this->Mat_ambient = new std::string("material.ambient");
   this->Mat_diffuse = new std::string("material.diffuse");
   this->Mat_specular = new std::string("material.specular");
   this->Mat_shininess = new std::string("material.shininess");

   this->Light_ambient = new std::string("light.ambient");
   this->Light_diffuse = new std::string("light.difusse");
   this->Light_specular = new std::string("light.specular");
   this->Light_pos = new std::string("light.position");

}

GameObject::GameObject(BasicsBlock* basic_block, Camera* m_camera,Shape* m_shape,float initial_pos[3], 
std::string* vert_shader_path,std::string* frag_shader_path):
m_window(basic_block->m_window) ,m_input(basic_block->m_input),m_time(basic_block->m_time), m_camera(m_camera),
vertex_shader_path(vert_shader_path), fragment_shader_path(frag_shader_path),m_shape(m_shape){
   this->shader = nullptr; this->m_vao = nullptr;
   this->SetInitialMVP();
   this->color = glm::vec3(0);
   model = glm::translate(model,glm::vec3(initial_pos[0],initial_pos[1],initial_pos[2]));
   this->m_light = nullptr;
   this->Model_string = new std::string("Model");
   this->View_string = new std::string("View");
   this->Projection_string = new std::string("Projection");

   this->Mat_ambient = new std::string("material.ambient");
   this->Mat_diffuse = new std::string("material.diffuse");
   this->Mat_specular = new std::string("material.specular");
   this->Mat_shininess = new std::string("material.shininess");

   this->Light_ambient = new std::string("light.ambient");
   this->Light_diffuse = new std::string("light.diffuse");
   this->Light_specular = new std::string("light.specular");
   this->Light_pos = new std::string("light.position");
    
}

//Updates the data and send it to GPU
 void GameObject::UpdateAndBuffer(){
    if(isLight){
       this->m_light->light_pos = glm::vec3(model[3][0],model[3][1],model[3][2]);
    }
    if(this->m_vao != nullptr && this->shader != nullptr && this->m_camera != nullptr){this->shader->UseShader();}
    //Updaets the vertex data
    this->Update();
    if(this->m_vao != nullptr && this->shader != nullptr && this->m_camera != nullptr){

      //Binds VAO
      this->m_vao->UseVAO();
      //Applies color to the object
      this->shader->SetUniformVec3f(this->Mat_ambient,this->color * 0.3f);
      this->shader->SetUniformVec3f(this->Mat_diffuse,this->color);
      this->shader->SetUniformVec3f(this->Mat_specular,glm::vec3(0.5f,0.5f,0.5f));
      this->shader->SetUniform1f(this->Mat_shininess,32.0f);
      //Pass uniforms
      if(this->m_light != nullptr){
         shader->SetUniformVec3f(this->Light_ambient,this->m_light->light_color * this->m_light->light_intensity);
         shader->SetUniformVec3f(this->Light_diffuse,this->m_light->light_color * this->m_light->light_intensity);
         shader->SetUniformVec3f(this->Light_specular,this->m_light->light_color * this->m_light->light_intensity);
         shader->SetUniformVec3f(this->Light_pos,this->m_light->light_pos);
      }
      this->shader->SetUniformMat4f(this->Model_string,this->model);
      this->shader->SetUniformMat4f(this->View_string,this->m_camera->GetView());
      this->shader->SetUniformMat4f(this->Projection_string,this->m_camera->GetProjection());
      if(this->m_shape->indices_count > 1){
         glDrawElements(GL_TRIANGLE_STRIP,this->m_shape->indices_count,GL_UNSIGNED_INT,0);
      }else{
         glDrawArrays(GL_TRIANGLES,0,this->m_shape->vertex_count);
      }
    }

    glBindVertexArray(0);
 }

 void GameObject::SetUpVertex(){
   if (this->m_shape != nullptr){
      //Creates the VAO object
      this->m_vao = new VAO(GL_FLOAT);
         //Sets how the atrtibutes should be read, ORDER MATTERS
         this->m_vao->SetAttribPoint(3);
         this->m_vao->SetAttribPoint(3);
      //Finishes the opbject
      this->m_vao->SetUpObject();
      //Buffer data into it
      this->m_vao->BufferData<GLfloat>(this->m_shape->vertex,GL_ARRAY_BUFFER,GL_FLOAT,this->m_shape->att_count);
      if(this->m_shape->indices_count > 1){
         this->m_vao->BufferData<GLuint>(this->m_shape->indices,GL_ELEMENT_ARRAY_BUFFER,GL_UNSIGNED_INT,this->m_shape->indices_count);
      }
   
      this->CreateShaderObject(this->vertex_shader_path,this->fragment_shader_path);
   }
 }

 void GameObject::SetUpVertex(VAO* aVAO){
   this->m_vao = aVAO;
   this->m_vao->UseVAO();
   this->m_vao->BufferData<GLfloat>(this->m_shape->vertex,GL_ARRAY_BUFFER,GL_FLOAT,this->m_shape->att_count);
   if(this->m_shape->indices_count > 1){
      this->m_vao->BufferData<GLuint>(this->m_shape->indices,GL_ELEMENT_ARRAY_BUFFER,GL_UNSIGNED_INT,this->m_shape->indices_count);
   }
   this->CreateShaderObject(this->vertex_shader_path,this->fragment_shader_path);
 }

 void GameObject::CreateShaderObject(std::string* vertex_shader, std::string* fragment_shader){
   this->shader = new Shader();
   this->shader->LoadShader(vertex_shader,GL_VERTEX_SHADER);
   this->shader->LoadShader(fragment_shader,GL_FRAGMENT_SHADER);
   this->shader->LinkShaders();
    
 }

void GameObject::SetTexture(std::string* tex_path){
   if(this->shader != nullptr){
      this->shader->SetTexture(tex_path);
   }
}

 void GameObject::SetInitialMVP(){
   //This is the world space matrix
   this->model = glm::mat4(1.0f);
 }

void GameObject::MakeLight(){
   if(this->m_light){
      isLight = true;
   }
}

 void GameObject::GiveLight(Light* light){
    this->m_light = light;
 }