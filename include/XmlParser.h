//
// Created by Paul on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_XMLPARSER_H
#define PATHFINDING_PROJECT_XMLPARSER_H


#include "Map.h"
#include "../libs/xml-parser/tinyxml2.h"

class XmlParser {

private:
    /**
     * Parse the "Properties" tag of a map XML and populate the corresponding fields in the Map instance
     * @param mapElement : the map tag (XMLElement)
     * @param map : the Map reference
     */
    static void parseProperties(tinyxml2::XMLElement *mapElement, Map &map);

    /**
     * Parse the "Population" tag of a map XML and populate the corresponding fields in the Map instance
     * @param properties : the properties tag (XMLElement)
     * @param map : the Map reference
     */
    static void parsePopulation(tinyxml2::XMLElement *properties, Map &map);

    static void parseTopology(tinyxml2::XMLElement *mapElement, Map &map);

public:

    static Map BuildMapFromXml(const char* file);
};


#endif //PATHFINDING_PROJECT_XMLPARSER_H
