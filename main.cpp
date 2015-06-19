#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace std;
//using namespace rapidjson;

rapidjson::Document parseFile(string file) {
    rapidjson::Document d;
    ifstream json_file;
    json_file.open(file);
    if (json_file.is_open()) {
        string json_string;//string containing full json file
        string line;//current line in file
        while (getline(json_file, line)) {
            json_string.append(line).append("\n");//append current line to full string
        }
        json_file.close();
        char *json_charArray = const_cast<char *>(json_string.c_str());
        d.Parse(json_charArray);
        /*
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        */
        return d;
    }
    else {
        cout << "Unable to open file\n";
        cout << json_file.rdstate();
        return d;
    }
}


int getPassengerGroupSize(rapidjson::Value& value){
    if(value.IsArray()){
        return value.Size();
    }else{
        return 1;
    }
}



/** sortArray
  * sorts an Array by groupsize
  *
  */

rapidjson::Value& sortPassengersBySize(rapidjson::Value& array){
    assert(array.IsArray());
    int array_size = array.Size();
    for (int i = 0; i < array_size; ++i) {
        int group_size;
        if(array[i].IsArray()) {
            group_size = array[i].Size();
        }else{
            group_size = 1;
        }
        int min = i;
        for (int k = 0; k < array_size ; ++k) {
            rapidjson::Value& newValue = array[k];
            rapidjson::Value& oldValue = array[min];
            if(getPassengerGroupSize(newValue) < getPassengerGroupSize(oldValue))
                min = k;
        }
        array[i].Swap(array[min]);
    }
    return array;
}

int main() {
    rapidjson::Document passengers_json = parseFile("passengers.json");//import passengers file
    rapidjson::Value& passengers = passengers_json["passengers"];//get passengers array
    assert(passengers.IsArray());

    rapidjson::Value& passengers_sorted = sortPassengersBySize(passengers);cout << '\n';

    

    return 0;
}
