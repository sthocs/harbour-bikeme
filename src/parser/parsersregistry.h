#ifndef PARSERSREGISTRY_H
#define PARSERSREGISTRY_H

#include "france/jcdecauxparser.h"
#include "france/parserlille.h"
#include "france/parsersmoove.h"
#include "france/parserveloway.h"
#include "kazakhstan/parserastana.h"
#include "russia/parsermoscow.h"
#include "uk/parserlondon.h"

class ParsersRegistry
{
public:
    ParsersRegistry();
    void registerParsers();
};

#endif // PARSERSREGISTRY_H
