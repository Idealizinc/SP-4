#ifndef UTILITY_H
#define UTILITY_H

#include "Mtx44.h"
#include "Vertex.h"

Position operator*(const Mtx44& lhs, const Position& rhs);
bool FindWordInString(const std::string &theWord, const std::string &theStr);
void CapitalizeString(std::string &theString);

#endif
