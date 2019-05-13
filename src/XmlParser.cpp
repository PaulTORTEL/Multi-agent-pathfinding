//
// Created by Paul on 13/05/2019.
//

#include "../include/XmlParser.h"
#include "../libs/xml-parser/tinyxml2.h"

Map XmlParser::BuildMapFromXml(const char *file) {
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
    int width = strtol(size->FirstChildElement("width")->GetText(), nullptr, 10);
    int height = strtol(size->FirstChildElement("height")->GetText(), nullptr, 10);
    map.setWidth(width);
    map.setHeight(height);

    XmlParser::parsePopulation(properties, map);
}

void XmlParser::parsePopulation(tinyxml2::XMLElement* properties, Map& map) {
    tinyxml2::XMLElement* population = properties->FirstChildElement("population");

    // We get the number of agents on the map and we save it into the "Population" field
    int pop = strtol(population->FirstChildElement("number")->GetText(), nullptr, 10);
    map.setPopulation(pop);
}

void XmlParser::parseTopology(tinyxml2::XMLElement* mapElement, Map& map) {

}
