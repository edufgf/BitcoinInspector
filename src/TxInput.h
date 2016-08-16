#ifndef TXINPUT_H
#define TXINPUT_H

#include "TxOutput.h"
#include "json.h"

using json = nlohmann::json;

class TxInput{
public:
    TxInput(json j){
        sequence = j["sequence"];
        script = j["script"];
        TxOutput tx(j["prev_out"]);
        prev_output = tx;
    }

    json getJson(){
        json j;
        j["sequence"] = sequence;
        j["script"] = script;
        json tx = prev_output.getJson();
        j["prev_out"] = tx;
        return j;
    }

private:
    unsigned int sequence;
    TxOutput prev_output;
    std::string script;
};


#endif // TXINPUT_H
