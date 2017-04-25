#ifndef PARSERSREGISTRY_H
#define PARSERSREGISTRY_H

#include "canada/montrealparser.h"
#include "france/jcdecauxparser.h"
#include "france/parserlille.h"
#include "france/parsersmoove.h"
#include "france/parserveloway.h"
#include "germany/nextbikeparser.h"
#include "kazakhstan/parserastana.h"
#include "north_america/nabsaparser.h"
#include "russia/parsermoscow.h"
#include "uk/parserlondon.h"

class ParsersRegistry
{
public:
    ParsersRegistry();

    /**
     * @brief Register the parsers using QMetaType mechanism to be able to
     * instantiate them dynamically from their name.
     */
    void registerParsers();
};

#endif // PARSERSREGISTRY_H
