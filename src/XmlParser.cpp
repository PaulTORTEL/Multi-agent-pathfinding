//
// Created by Paul on 13/05/2019.
//

#include <iostream>
#include "../include/XmlParser.h"
#include "../libs/xml-parser/tinyxml2.h"

Map XmlParser::buildMapFromXml(const char *file) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);

    // We define our map with default values
    Map map;
    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");

    XmlParser::parseProperties(mapElement, map);
    XmlParser::parseTopology(mapElement, map);

    return map;
}

void XmlParser::parseProperties(tinyxml2::XMLElement* mapElement, Map& map) {
    tinyxml2::XMLElement* properties = mapElement->FirstChildElement("properties");
    tinyxml2::XMLElement* size = properties->FirstChildElement("size");

    // We get the width and the height (char*) and we cast them into integers
    const int32_t width = strtol(size->FirstChildElement("width")->GetText(), nullptr, 10);
    const int32_t height = strtol(size->FirstChildElement("height")->GetText(), nullptr, 10);
    map.setWidth(width);
    map.setHeight(height);

    // We create an empty grid
    std::vector<std::vector<Square>> grid(width, std::vector<Square>(height));
    map.setGrid(grid);

    XmlParser::parsePopulation(properties, map);
}

void XmlParser::parsePopulation(tinyxml2::XMLElement* properties, Map& map) {
    tinyxml2::XMLElement* population = properties->FirstChildElement("population");

    // We get the number of agents on the map and we save it into the "Population" field
    int pop = strtol(population->FirstChildElement("number")->GetText(), nullptr, 10);
    map.setPopulation(pop);
}

void XmlParser::parseTopology(tinyxml2::XMLElement* mapElement, Map& map) {
    tinyxml2::XMLElement* topology = mapElement->FirstChildElement("topology");
    auto* square = topology->FirstChildElement("square");

    while (square != nullptr) {
        auto* coord = square->FirstChildElement("coordinates");
        const int x = strtol(coord->FindAttribute("x")->Value(), nullptr, 10);
        const int y = strtol(coord->FindAttribute("y")->Value(), nullptr, 10);

        const char* type = square->FirstChildElement("type")->GetText();
        const int level = strtol(square->FirstChildElement("level")->GetText(), nullptr, 10);

        map.getGrid()[x][y].level = level;
        if (!map.setSquareTypeFromString(type, map.getGrid()[x][y])) {
            std::cerr << "Error: square at [" << x << "," << y << "] has an unknown SquareType" << std::endl;
        }

        square = square->NextSiblingElement("square");
    }
}
