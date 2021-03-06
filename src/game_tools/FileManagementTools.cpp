#include "game_tools/FileManagementTools.hpp"

char* FileManagementTools::GetFilesContents(const char *filename,int* lenght)
{
  std::FILE *fp = std::fopen(filename, "rb");
  char* output;
  int size;
  if (fp)
  {
    std::string contents;
    //Sets the position of the indicator to the end
    std::fseek(fp, 0, SEEK_END);
    size = std::ftell(fp);
    //Resize the string into the size of the position of the indicator
    contents.resize(size);
    //Moves the position of the indicator to the beggining 
    std::rewind(fp);
    //reads object given count, the adress of the beginning of an object and the object to be read (Also the size of each object wich is one)
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    //contents.append('\0');
    output = (char*) malloc(sizeof(char)*size);
    *lenght = size/sizeof(char);
    //std::cout<<"Size of file: "<<*lenght<<" bytes" <<std::endl;
    memcpy(output,contents.data(),size);

    return output;
  }
  throw(errno);
}

std::vector<std::string> FileManagementTools::ParseLine(std::string line, std::string delimeter){
  int pos = 0;
  std::string token;
  std::vector<std::string> tokens;
  while((pos = line.find(delimeter)) != std::string::npos){
    
    token = line.substr(0,pos);
    if(token.length()>0)
      tokens.push_back(token);

    
    line.erase(0,pos+delimeter.length());
  }
  tokens.push_back(line);
  return tokens;
}

void FileManagementTools::RemoveFromString(std::string &line,const char* to_remove){
  for ( unsigned int i = 0; i < strlen(to_remove); ++i ) {
    line.erase( std::remove(line.begin(), line.end(), to_remove[i]), line.end() );
  }
}