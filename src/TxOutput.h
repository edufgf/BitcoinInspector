#ifndef TXOUTPUT_H
#define TXOUTPUT_H

#include <string>
#include "json.h"

using json = nlohmann::json;

class TxOutput{
public:
    TxOutput(){};

    TxOutput(json j){
        spent = j["spent"];
        tx_index = j["tx_index"];
        type = j["type"];
        n = j["n"];
        value = j["value"];
        if (j.find("addr") != j.end()) address = j["addr"];
        script = j["script"];
    }

    json getJson(){
        json j;
        j["spent"] = spent;
        j["tx_index"] = tx_index;
        j["type"] = type;
        j["n"] = n;
        j["value"] = value;
        if (j.find("addr") != j.end()) j["addr"] = address;
        j["script"] = script;
        return j;
    }

private:
    bool spent;
    unsigned int tx_index;
    unsigned int type;
    unsigned int n;
    unsigned long long value;
    std::string address;
    std::string script;
};

#endif // TXOUTPUT_H
