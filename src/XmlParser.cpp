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
    XmlParser::parseRules(mapElement, map);

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


        Agent new_agent = Agent(id, start_x, start_y);

        auto* pickup_list = agent->FirstChildElement("pickup_list");
        auto* product = pickup_list->FirstChildElement("product");

        while (product != nullptr) {
            const int product_id = strtol(product->FindAttribute("id")->Value(), nullptr, 10);
            new_agent.addProductToPickupList(product_id);

            product = product->NextSiblingElement("product");
        }
        new_agent.computeItemsToPickup();
        map.addAgent(new_agent);


        // We move on to the next agent tage (if doesn't exist, then agent == nullptr)
        agent = agent->NextSiblingElement("agent");
    }
}

void XmlParser::parseTopology(tinyxml2::XMLElement *map_element, Map &map) {
    tinyxml2::XMLElement* topology = map_element->FirstChildElement("topology");
    auto* square = topology->FirstChildElement("square");

    // While we have square tags to process
    while (square != nullptr) {
        auto* coord = square->FirstChildElement("coordinates");

        // We get the coordinates from the attributes of the tag "coordinates"
        const int x = strtol(coord->FindAttribute("x")->Value(), nullptr, 10);
        const int y = strtol(coord->FindAttribute("y")->Value(), nullptr, 10);

        const char* type = square->FirstChildElement("type")->GetText();

        if (strcmp(type, "PRODUCT") == 0) {
            const int id = strtol(square->FirstChildElement("id")->GetText(), nullptr, 10);
            map.addProduct(x, y, id);
        } else if (strcmp(type, "DROP_OFF_POINT") == 0) {
            const int id = strtol(square->FirstChildElement("id")->GetText(), nullptr, 10);
            map.addDropOffPoint(x, y, id);
        } else if (strcmp(type, "REPAIR_POINT") == 0) {
            const int id = strtol(square->FirstChildElement("id")->GetText(), nullptr, 10);
            map.addRepairPoint(x, y, id);
        } else {
            // We assign the type to the square and check if something goes wrong
            if (!map.setSquareTypeFromString(type, x, y)) {
                // If here, the SquareType written in the XML for this square is unknown
                std::cerr << "Error: square at [" << x << "," << y << "] has an unknown SquareType" << std::endl;
            }
        }

        // We move on to the next square tag (if doesn't exit, then square == nullptr)
        square = square->NextSiblingElement("square");
    }
}

void XmlParser::parseRules(tinyxml2::XMLElement *map_element, Map &map) {
    tinyxml2::XMLElement* rules = map_element->FirstChildElement("rules");
    auto* rule = rules->FirstChildElement("rule");

    // While we have square tags to process
    while (rule != nullptr) {
        auto* square = rule->FirstChildElement("square");
        const int x = strtol(square->FindAttribute("x")->Value(), nullptr, 10);
        const int y = strtol(square->FindAttribute("y")->Value(), nullptr, 10);

        auto* accesses = rule->FirstChildElement("forbidden_accesses");
        auto* access = accesses->FirstChildElement("access");

        while (access != nullptr) {
            const char* access_text = access->GetText();
            map.addForbiddenAccess(x, y, access_text);
            access = access->NextSiblingElement("access");
        }

        auto* exits = rule->FirstChildElement("forbidden_exits");
        auto* exit = exits->FirstChildElement("exit");

        while (exit != nullptr) {
            const char* exit_text = exit->GetText();
            map.addForbiddenExit(x, y, exit_text);
            exit = exit->NextSiblingElement("exit");
        }

        rule = rule->NextSiblingElement("rule");
    }
}
