#include "game_object/Model.hpp"

void Model::Draw(Shader* shader){
    for(unsigned int i = 0; i < meshes.size() ; i++){
        meshes[i]->Draw(shader);
    }
}

void Model::LoadModel(std::string path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout<<"ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    }
    directory = path.substr(0, path.find_last_of("/"));
    ProcessNode(scene->mRootNode, scene);
}