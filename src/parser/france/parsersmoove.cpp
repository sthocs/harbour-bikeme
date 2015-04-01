#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "parsersmoove.h"

QString ParserSmoove::parseCarto(QString xmlCarto) {
    return parseXmlResult(xmlCarto);
}

QString ParserSmoove::parseStationDetails(QString xmlStationDetails, QString url) {
    return xmlStationDetails;
}

QString ParserSmoove::parseAllStationsDetails(QString allStationsDetails) {
    return parseXmlResult(allStationsDetails);
}

QString ParserSmoove::parseXmlResult(QString xmlResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QDomDocument xmlDetails;
    xmlDetails.setContent(xmlResult);
    QDomElement vcs = xmlDetails.documentElement();
    QDomNode sl = vcs.firstChild();
    QDomNode si_node = sl.firstChild();
    while (!si_node.isNull())
    {
        QDomElement si = si_node.toElement();
        QJsonObject jsonStation;
        QJsonObject position;

        jsonStation["number"] = si.attribute("id").toInt();
        jsonStation["name"] = si.attribute("id");
        position["lat"] = si.attribute("la").toFloat();
        position["lng"] = si.attribute("lg").toFloat();
        jsonStation["available_bikes"] = si.attribute("av").toInt();
        jsonStation["available_bike_stands"] = si.attribute("fr").toInt();

        jsonStation["position"] = position;
        jsonStation["last_update"] = -1;
        jsonArray.append(jsonStation);
        si_node = si_node.nextSibling();
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}
