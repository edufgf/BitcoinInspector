#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <json.h>
#include <QString>
#include <string>
#include <vector>
#include "TxInput.h"
#include "TxOutput.h"
#include <QDebug>

#include <chrono>
#include <ctime>

using json = nlohmann::json;

/* Raw transaction format reference: https://bitcoin.org/en/developer-reference#raw-transaction-format */

class Transaction{
public:

    Transaction(){};

    Transaction(const Transaction& t) : Transaction(t.getRawData()) {
        this->setTimeStamp(t.getTimeStamp());
    }

    Transaction(QString raw_tx_data){
        this->raw_data = raw_tx_data;
        json tx = json::parse(raw_tx_data.toUtf8().data());

        std::string op = tx["op"];
        unconfirmed = (op == "utx") ? true : false;
        json tx_obj = tx["x"];
        lock_time = tx_obj["lock_time"];
        version = tx_obj["ver"];
        size = tx_obj["size"];
        time = tx_obj["time"];
        tx_index = tx_obj["tx_index"];
        tx_in_count = tx_obj["vin_sz"];
        tx_out_count = tx_obj["vout_sz"];
        relayed_by = tx_obj["relayed_by"];
        tx_hash = tx_obj["hash"];
        json in_obj = tx_obj["inputs"];
        for (json::iterator it = in_obj.begin(); it != in_obj.end(); ++it)
            inputs.push_back(TxInput(it.value()));

        json out_obj = tx_obj["out"];
        for (json::iterator it = out_obj.begin(); it != out_obj.end(); ++it)
            outputs.push_back(TxOutput(it.value()));
    }

    QString parse_to_raw(){
        json j;
        if (unconfirmed) j["op"] = "utx";
        else j["op"] = "not utx";

        json tx_obj;
        tx_obj["lock_time"] = lock_time;
        tx_obj["ver"] = version;
        tx_obj["size"] = size;
        tx_obj["time"] = time;
        tx_obj["tx_index"] = tx_index;
        tx_obj["vin_sz"] = tx_in_count;
        tx_obj["vout_sz"] = tx_out_count;
        tx_obj["relayed_by"] = relayed_by;
        tx_obj["hash"] = tx_hash;

        json in_obj;
        for (TxInput & x : inputs){
            in_obj.push_back(x.getJson());
        }
        tx_obj["inputs"] = in_obj;

        json out_obj;
        for (TxOutput & x : outputs){
            out_obj.push_back(x.getJson());
        }
        tx_obj["out"] = out_obj;

        j["x"] = tx_obj;

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
        dump += "### TRANSACTION LOG START ###\n";
        dump += raw_data + "\n";
        dump += "### TRANSACTION LOG TS ###\n";
        dump += time_stamp_string();
        dump += "### TRANSACTION LOG END ###\n";
        return dump;
    }

    std::chrono::time_point<std::chrono::system_clock> time_stamp;
private:
    bool unconfirmed;
    unsigned int lock_time;
    unsigned int version;
    unsigned int size;
    unsigned long long time;
    unsigned int tx_index;
    unsigned int  tx_in_count;
    unsigned int  tx_out_count;
    std::string tx_hash;
    std::string relayed_by;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;

    QString raw_data;
};


/*
 *
 * Bytes / Name / Data Type / Description
 4 	version 	uint32_t 	Transaction version number; currently version 1. Programs creating transactions using newer consensus rules may use higher version numbers.
Varies 	tx_in count 	compactSize uint 	Number of inputs in this transaction.
Varies 	tx_in 	txIn 	Transaction inputs. See description of txIn below.
Varies 	tx_out count 	compactSize uint 	Number of outputs in this transaction.
Varies 	tx_out 	txOut 	Transaction outputs. See description of txOut below.
4 	lock_time 	uint32_t 	A time (Unix epoch time) or block number. See the locktime parsing rules. */
/*
"x":{
   "lock_time":0,
   "ver":1,
   "size":258,
   "inputs":[
      {
         "sequence":4294967295,
         "prev_out":{
            "spent":true,
            "tx_index":162122315,
            "type":0,
            "addr":"1LVmURVUD3naLKAgzkbZKRYEXNwKrpadb1",
            "value":11696428,
            "n":1,
            "script":"76a914d5db292d0d4ec1c4d79be9ccd42831ae3d21b8e388ac"
         },
         "script":"483045022100d3550a31b7d433c991b9ecfc090425233dea601d5ce0b4577d002fb5560a92e8022061a5875917db8538cbe664026b9afa614e82b5abdf6447a2fc1a7e83e4d4a2cf014104947923f80f0b9a0067bf6e36651a5ffa9c4f824f51c7423e7390daa3b602dfd4ceb22e3ad8c816f05df3a0f9e262f0dab43a4835d49be3627bafe956f2284df9"
      }
   ],
   "time":1468874415,
   "tx_index":162149578,
   "vin_sz":1,
   "hash":"96cda976e5b3f0c94645a23393ab4432ba3f93bc2bf82e2e730d59b30aafd5c1",
   "vout_sz":2,
   "relayed_by":"127.0.0.1",
   "out":[
      {
         "spent":false,
         "tx_index":162149578,
         "type":0,
         "addr":"19XVKLzNp1s7QvypgUyDEf9qvNjCSN3dUY",
         "value":3426443,
         "n":0,
         "script":"76a9145d851f53a1b1c33dc562cca6283641c884dd48ed88ac"
      },
      {
         "spent":false,
         "tx_index":162149578,
         "type":0,
         "addr":"1LVmURVUD3naLKAgzkbZKRYEXNwKrpadb1",
         "value":8259985,
         "n":1,
         "script":"76a914d5db292d0d4ec1c4d79be9ccd42831ae3d21b8e388ac"
      }
   ]
}
*/

#endif // TRANSACTION_H
