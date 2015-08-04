#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include "utility"
#include "map"
#include "vector"


using namespace std;
//using namespace rapidjson;

/**
 * returns a rapidjson document representation of the given file name
 */

struct a320{
    int max_seats = 180;
    int max_buisness = 30;
    int used_seats = 0;
    int used_buisness = 0;
};


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

/**
 * returns size of a passengergroup as int
 * 1 if single passenger
 */
int getPassengerGroupSize(rapidjson::Value& value){
    if(value.IsArray()){
        return value.Size();
    }else{
        return 1;
    }
}



/**
  * sortArray
  * sorts an Array by groupsize
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

/**
 * copies a Document
 */
static void copyDocument(rapidjson::Document & newDocument, rapidjson::Document & copiedDocument) {
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    newDocument.Accept(writer);
    std::string str = strbuf.GetString();
    copiedDocument.Parse<0>(str.c_str());
}

/**
 * writes a Document to StdOut
 */
void writeDocument(rapidjson::Document doc){
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
}

void writeFirstID(rapidjson::Value& value){
    if(value.IsArray()){
        cout << value[0]["ID"].GetInt();
    }else{
        cout << value["ID"].GetInt();
    }
}

bool compareArraysByID(rapidjson::Document& first_doc, rapidjson::Document& second_doc){
    rapidjson::Value& first = first_doc["passengers"];
    rapidjson::Value& second= second_doc["passengers"];
    if(first.Size()==second.Size()){
        for (int i = 0; i < first.Size(); ++i) {
            if(first[i].IsArray()){
                if(second[i].IsArray()){
                    for (int j = 0; j < first[i].Size(); ++j) {
                        if(!(first[i][j]["ID"].GetInt()==second[i][j]["ID"])){
                            return false;
                        }
                    }
                }else{
                    return false;
                }
            }else if(second[i].IsArray()){
                return false;
            }else if(!(first[i]["ID"].GetInt()==second[i]["ID"])){
                return false;
            }
        }
        return true;
    }else{
        return false;
    }
}

/**
 * returns a Multimap containing
 */
multimap<string,rapidjson::Value&>splitByDestination(rapidjson::Value& passengers){
    multimap<string,rapidjson::Value&> passengers_destination;
    for (int i = 0; i < passengers.Size(); ++i) {
        if(passengers[i].IsArray()){
            // group
            rapidjson::Value& current_passenger = passengers[i];
            string current_destination = current_passenger[0]["destination"].GetString();
            passengers_destination.insert(std::pair<string,rapidjson::Value&>(current_destination, current_passenger));
        }else{
            // single passenger
            string current_destination = passengers[i]["destination"].GetString();
            rapidjson::Value& current_passenger = passengers[i];
            passengers_destination.insert(std::pair<string,rapidjson::Value&>(current_destination, current_passenger));
        }
        map<string,rapidjson::Value> passengers_dest;
    }
    return passengers_destination;
}

int main() {
    //Import
    rapidjson::Document passengers_doc = parseFile("passengers.json");//import passengers file
    rapidjson::Value& passengers = passengers_doc["passengers"];//get passengers array
/*
    rapidjson::Document passengers_sorted_doc = parseFile("passengers.json");
    rapidjson::Value& passengers_sorted = passengers_sorted_doc["passengers"];
    cout << compareArraysByID(passengers_sorted_doc,passengers_doc) << '\n';
    sortPassengersBySize(passengers_sorted);
    cout << compareArraysByID(passengers_sorted_doc,passengers_doc) << '\n';
*/
    //split by destination
    multimap<string,rapidjson::Value&> passengers_destination;
    passengers_destination = splitByDestination(passengers);
    


    return 0;
}
