#ifndef PARSERSREGISTRY_H
#define PARSERSREGISTRY_H

#include "france/jcdecauxparser.h"
#include "france/parserlille.h"
#include "france/parsersmoove.h"
#include "russia/parsermoscow.h"

class ParsersRegistry
{
public:
    ParsersRegistry();
    void registerParsers();
};

#endif // PARSERSREGISTRY_H
