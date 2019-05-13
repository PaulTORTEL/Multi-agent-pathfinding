#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Map map = XmlParser::BuildMapFromXml("../maps/map_1.xml");
    std::cout << map << std::endl;


    return 0;
}