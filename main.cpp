#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace std;

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

int main() {
    parseFile("input.json");
    return 0;
}
