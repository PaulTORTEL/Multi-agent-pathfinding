//
// Created by Paul Tortel on 13/05/2019.
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

    /**
     * Parse the "Topology" tag of a map XML and setup the squares of the world
     * @param mapElement : the map tag (XMLElement)
     * @param map : the Map reference
     */
    static void parseTopology(tinyxml2::XMLElement *mapElement, Map &map);

public:

    /**
     * Create, populate the map with an XML file, and return the map
     * @param file : the path to the XML file
     * @param name : the name of the Map
     * @return the newly created map
     */
    static Map buildMapFromXml(const char *file, const char* name);
};


#endif //PATHFINDING_PROJECT_XMLPARSER_H
