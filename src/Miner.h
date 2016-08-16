#ifndef MINER_H
#define MINER_H

#include "json.h"

using json = nlohmann::json;

class Miner{
public:

    Miner(){}

    Miner(json j){
        ip = j["ip"];
        link = j["link"];
        description = j["description"];
        time = j["time"];
    }

    json getJson(){
        json j;
        j["ip"] = ip;
        j["link"] = link;
        j["description"] = description;
        j["time"] = time;
        return j;
    }

private:
    std::string ip;
    std::string link;
    std::string description;
    unsigned long long time;

};

#endif // MINER_H
