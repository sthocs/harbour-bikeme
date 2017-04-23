#ifndef NABSAPARSER_H
#define NABSAPARSER_H

#include "../bikedataparser.h"

class NabsaParser : public BikeDataParser
{
public:
    virtual QList<City*> parseCities(QString cities, ProviderInfo& providerInfo);
    virtual QList<Station*> parseAllStations(QString allStations, bool withDetails);
    virtual void parseCityUrls(QString cityUrls, City* city);
    virtual QList<Station*> parseStationsList(QString stationsList);
    virtual void parseStationsRealTimeData(QString realTimeData , QList<Station*> stations);

private:
    static const QString CSV_DELIMITER;

    void updateCsvDelimiters(QString& csvData);
};
Q_DECLARE_METATYPE(NabsaParser)

#endif // NABSAPARSER_H
