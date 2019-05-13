#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"

int main() {
    Map map = XmlParser::buildMapFromXml("../maps/map_1.xml", "map_1");
    std::cout << map << std::endl;

    return 0;
}