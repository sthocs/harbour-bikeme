#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

#include "../city.h"
#include "../station.h"

class BikeDataParser
{
public:
    virtual ~BikeDataParser() {}

    /**
     * @brief Parse the cities of a given provider
     * @return
     */
    virtual QList<City*> parseCities(QString /* cities */, ProviderInfo& /* providerInfo */) { return QList<City*>(); }

    /**
     * Some systems provide, for each city, a master url returning several urls
     * to access various data. This function will parse them and setup the city
     * @brief Parse urls of a city
     * @param cityUrls The urls of the city
     * @param city The city to setup with these urls
     */
    virtual void parseCityUrls(QString /* cityUrls */, City* /* city */) {}

    /**
     * @brief Parse the stations list, with or without the real-time data
     * @param stationsList
     * @param withDetails If true, will include the real-time data in the
     * results, if available in the stationsList.
     * @return
     */
    virtual QList<Station*> parseStationsList(QString /* stationsList */, bool /* withDetails */) { return QList<Station*>(); }

    /**
     * @brief Parse real-time data of all stations
     * @param realTimeData The updated real-time data
     * @param stations The stations list which will be updated
     */
    virtual void parseStationsRealTimeData(QString /* realTimeData */, QList<Station*> /* stations */) {}

    /**
     * @brief Parse real-time data for a single station
     * @param stationDetails The updated station data
     * @param station The station object to update
     */
    virtual void parseStationDetails(QString /* stationDetails */, Station* /* station */) {}
};

#endif // MASTERPARSER_H
