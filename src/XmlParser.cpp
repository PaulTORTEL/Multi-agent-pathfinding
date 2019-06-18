//
// Created by Paul Tortel on 13/05/2019.
//

#include <iostream>
#include "../include/XmlParser.h"
#include "../libs/xml-parser/tinyxml2.h"

Map XmlParser::buildMapFromXml(const char *file, const char* name) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);

    // We define our map with default values
    Map map(name);
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
    std::vector<std::vector<MapSquare>> grid(width, std::vector<MapSquare>(height));
    map.setGrid(grid);

    XmlParser::parsePopulation(properties, map);
}

void XmlParser::parsePopulation(tinyxml2::XMLElement* properties, Map& map) {
    tinyxml2::XMLElement* population = properties->FirstChildElement("population");
    tinyxml2::XMLElement* agents = population->FirstChildElement("agents");

    auto* agent = agents->FirstChildElement("agent");

    while (agent != nullptr) {
        // We get the ID of the agent
        int id = strtol(agent->FindAttribute("id")->Value(), nullptr, 10);

        // We get his start and goal coordinates
        auto* start_coord = agent->FirstChildElement("start")->FirstChildElement("coordinates");
        const int start_x = strtol(start_coord->FindAttribute("x")->Value(), nullptr, 10);
        const int start_y = strtol(start_coord->FindAttribute("y")->Value(), nullptr, 10);

        auto* goal_coord = agent->FirstChildElement("goal")->FirstChildElement("coordinates");
        const int goal_x = strtol(goal_coord->FindAttribute("x")->Value(), nullptr, 10);
        const int goal_y = strtol(goal_coord->FindAttribute("y")->Value(), nullptr, 10);

        // We save the information about the agent
        map.addAgent(Agent(id, start_x, start_y, goal_x, goal_y));

        // We move on to the next agent tage (if doesn't exist, then agent == nullptr)
        agent = agent->NextSiblingElement("agent");
    }
}

void XmlParser::parseTopology(tinyxml2::XMLElement* mapElement, Map& map) {
    tinyxml2::XMLElement* topology = mapElement->FirstChildElement("topology");
    auto* square = topology->FirstChildElement("square");

    // While we have square tags to process
    while (square != nullptr) {
        auto* coord = square->FirstChildElement("coordinates");

        // We get the coordinates from the attributes of the tag "coordinates"
        const int x = strtol(coord->FindAttribute("x")->Value(), nullptr, 10);
        const int y = strtol(coord->FindAttribute("y")->Value(), nullptr, 10);

        const char* type = square->FirstChildElement("type")->GetText();


        // We assign the type to the square and check if something goes wrong
        if (!map.setSquareTypeFromString(type, x, y)) {
            // If here, the SquareType written in the XML for this square is unknown
            std::cerr << "Error: square at [" << x << "," << y << "] has an unknown SquareType" << std::endl;
        }

        // We move on to the next square tag (if doesn't exit, then square == nullptr)
        square = square->NextSiblingElement("square");
    }
}
