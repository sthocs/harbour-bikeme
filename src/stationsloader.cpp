#include "stationsloader.h"

#include "parser/bikedataparser.h"

StationsLoader::StationsLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
}

void StationsLoader::setProvider(QString providerName)
{
    _providerName = providerName;
}

void StationsLoader::fetchAllStationsDetails(QUrl allStationsDetailsUrl)
{
    QNetworkRequest request(allStationsDetailsUrl);
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(allStationsDetailsFetched()));
}

void StationsLoader::allStationsDetailsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    //TODO handle redirections
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    int id = QMetaType::type(_providerName.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<Station*> stations = parser->parseAllStationsDetails2(QString::fromUtf8(reply->readAll()));
        delete parser;
        emit allStationsDetailsFetched(stations);
    }
    reply->deleteLater();
}
