# Multi-agent-pathfinding
Pathfinding algorithms to plan paths for multiple agents simultaneously using the Conflict-based Search Algorithm presented by G Sharon, R Stern, A Felner, NR Sturtevant - Artificial Intelligence, 2015. https://doi.org/10.1016/j.artint.2014.11.006

## Situation
A set of N agents (robots) are working in a delivery company's warehouse. Each of them has a product list. They have to look for each product on the list into the warehouse, pick them up, and bring them to the nearest drop-off area. Then, the finished agents have to return to their initial position in order to be ready for the next product lists.
Obviously, the agents have to be given the shortest path possible in order to allow them to reach as soon as possible their final location. The agents have the same speed, width, and height.

## Problem
While moving into the warehouse, the agents must not collide. If an agent is operating an action (picking up or dropping off an item), a moving agent has to either bypass the obstacle or wait for the operating agent to move away. The warehouse can have pillars (static obstacles), traffic rules, N agents moving, M products, and K drop-off areas.

## Solution
C++ engine implemented to compute simultaneously an optimized path for every agent in multiple scenarios (big or small warehouse, many or few obstacles, lots or no traffic rules etc.).
Maps are represented using XML.

## Infos
This work has been the fruit of my work to get my Master in Computer Science in Cork, Ireland

### April - August 2019
Copyright Paul Tortel
