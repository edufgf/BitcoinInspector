#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <json.h>
#include <QString>
#include <string>
#include <vector>
#include <QDebug>
#include <Miner.h>

#include <chrono>
#include <ctime>

using json = nlohmann::json;

class Block{
public:

    Block(){};

    Block(const Block& b) : Block(b.getRawData()) {
        this->setTimeStamp(b.getTimeStamp());
    }

    Block(QString raw_tx_data){
        this->raw_data = raw_tx_data;
        json bx = json::parse(raw_tx_data.toUtf8().data());

        // op = block, useless
        //std::string op = bx["op"];

        json bx_obj = bx["x"];
        reward = bx_obj["reward"];
        totalBTCSent = bx_obj["totalBTCSent"];
        estimatedBTCSent = bx_obj["estimatedBTCSent"];
        difficulty_bits = bx_obj["bits"];
        version = bx_obj["version"];
        nonce = bx_obj["nonce"];
        prevBlockIndex = bx_obj["prevBlockIndex"];
        nTx = bx_obj["nTx"];
        blockIndex = bx_obj["blockIndex"];
        size = bx_obj["size"];
        time = bx_obj["time"];
        mrklRoot = bx_obj["mrklRoot"];
        hash = bx_obj["hash"];
        height = bx_obj["height"];

        Miner m(bx_obj["foundBy"]);
        mined_by = m;

        json tx_ind = bx_obj["txIndexes"];
        for (json::iterator it = tx_ind.begin(); it != tx_ind.end(); ++it)
            tx_indexes.push_back(it.value());
    }

    QString parse_to_raw(){
        json j;
        j["op"] = "block";

        json bx_obj;

        bx_obj["reward"] = reward;
        bx_obj["totalBTCSent"] = totalBTCSent;
        bx_obj["estimatedBTCSent"] = estimatedBTCSent;
        bx_obj["bits"] = difficulty_bits;
        bx_obj["version"] = version;
        bx_obj["nonce"] = nonce;
        bx_obj["prevBlockIndex"] = prevBlockIndex;
        bx_obj["nTx"] = nTx;
        bx_obj["blockIndex"] = blockIndex;
        bx_obj["size"] = size;
        bx_obj["time"] = time;
        bx_obj["mrklRoot"] = mrklRoot;
        bx_obj["hash"] = hash;
        bx_obj["height"] = height;

        bx_obj["foundBy"] = mined_by.getJson();

        json indexes;
        for (unsigned int & ind : tx_indexes){
            indexes.push_back(ind);
        }
        bx_obj["txIndexes"] = indexes;

        j["x"] = bx_obj;

        std::string str = j.dump(3);
        return QString::fromStdString(str);
    }

    QString getRawData() const{
        return this->raw_data;
    }


    std::chrono::time_point<std::chrono::system_clock> getTimeStamp() const{
       return this->time_stamp;
    }

    void setTimeStamp(std::chrono::time_point<std::chrono::system_clock> t){
        this->time_stamp = t;
    }


    /*  tm_struct details

        tm_sec	int	seconds after the minute	0-61*
        tm_min	int	minutes after the hour	0-59
        tm_hour	int	hours since midnight	0-23
        tm_mday	int	day of the month	1-31
        tm_mon	int	months since January	0-11
        tm_year	int	years since 1900
        tm_wday	int	days since Sunday	0-6
        tm_yday	int	days since January 1	0-365
        tm_isdst	int	Daylight Saving Time flag
    */
    QString time_stamp_string(){
        time_t t = std::chrono::system_clock::to_time_t(this->time_stamp);
        struct tm * ts = localtime(&t);

        std::string time_str;
        time_str += std::to_string(ts->tm_year + 1900) + "-" + std::to_string(ts->tm_mon + 1) + "-" + std::to_string(ts->tm_mday) + " ";
        time_str += std::to_string(ts->tm_hour) + ":" + std::to_string(ts->tm_min) + ":" + std::to_string(ts->tm_sec) + " ";
        // Daylight Saving Time
        time_str += std::to_string(ts->tm_isdst) + "\n";

        return QString::fromStdString(time_str);
    }

    QString dump_log(){
        QString dump;
        dump += "### BLOCK LOG START ###\n";
        dump += raw_data + "\n";
        dump += "### BLOCK LOG TS ###\n";
        dump += time_stamp_string();
        dump += "### BLOCK LOG END ###\n";
        return dump;
    }

    unsigned long long reward;
    unsigned long long totalBTCSent;
    unsigned long long estimatedBTCSent;
    unsigned int difficulty_bits;
    unsigned int version;
    unsigned int nonce;
    unsigned int prevBlockIndex;
    unsigned long long nTx;
    unsigned int blockIndex;
    unsigned int size;
    unsigned long long time;
    std::string mrklRoot;
    std::string hash;
    unsigned int height;
    Miner mined_by;
    std::vector<unsigned int> tx_indexes;

    QString raw_data;
    std::chrono::time_point<std::chrono::system_clock> time_stamp;

   private:
};

/*{ "op":"block",
 * "x":{ "
 * txIndexes":[ 164215898, 164215659, 164215666, 164215637, 164215703, 164215671, 164215693, 164215723, 164215719, 164215667, 164215690, 164215691, 164215673, 164215707, 164215678, 164215680, 164215708, 164215639, 164215648, 164215641, 164215670, 164215643, 164215722, 164215721, 164215724, 164201951, 164199025 ],
 * "reward":1269552705,
 * "totalBTCSent":807300090190,
 * "estimatedBTCSent":80021134976,
 * "bits":402990697,
 * "version":536870912,
 * "nonce":3185283909,
 * "foundBy":{
 *          "ip":"52.21.250.237",
 *          "link":"https:\/\/bw.com\/",
 *          "description":"BW.COM",
 *          "time":1469682363 },
 * "prevBlockIndex":1132497,
 * "nTx":929,
 * "blockIndex":1132498,
 * "size":585104,
 * "time":1469682363,
 * "mrklRoot":"7c40e4c4781d1e4c2b4c7d350381ba00b02d5da0869b2de199f0e2ba4e7997f6",
 * "hash":"000000000000000004a55b7c97770548f99153ec09b3622ffea4893759424cbd",
 * "height":422557
 * }
 * }
 * */

#endif // BLOCK_H
