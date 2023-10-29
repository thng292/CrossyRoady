#include "Entity.h"

int Entity::getVelocity() { return velocity; }

ConsoleGame::Vec2 Entity::getSize() { return size; }

ConsoleGame::Vec2 Entity::getCoord() { return coord; }

void Entity::setVelocity(int Velocity) { velocity = Velocity; }

void Entity::setSize(ConsoleGame::Vec2 Size) { size = Size; }

void Entity::setCoord(ConsoleGame::Vec2 Coord) { coord = Coord; }
