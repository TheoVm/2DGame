#include <iostream>
#include <./lib/json.hpp>
#include <fstream> 

using json = nlohmann::json;

auto generateMap(std::string path, int dataID){

    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Failed to open json in path " << path << std::endl;
    }

    json json;
    try{
       file >> json; 
    }catch(json::parse_error& e){
        std::cerr << "Error loading info: "<< e.what() << std::endl;
    }

    nlohmann::json_abi_v3_12_0::basic_json<>::value_type data = json["layers"][dataID]["data"];

    return data;    
}