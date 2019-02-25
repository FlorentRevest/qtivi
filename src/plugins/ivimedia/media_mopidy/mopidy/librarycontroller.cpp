#include "librarycontroller.h"
#include "modeltranslator.h"

#include <QDebug>

using namespace mopidy;

LibraryController::LibraryController()
    : BaseController("core.library")
{
}

LibraryController::~LibraryController()
{
}

void LibraryController::browse(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit browsed(refs);
    };

    QJsonObject params;
    params["uri"] = uri.isEmpty() ? QJsonValue() : uri;

    sendRequest("browse", rhdl, params);
}

void LibraryController::search(const QHash<QString, QStringList> &query, const QStringList &uris, bool exact, const QUuid &identifier)
{
    const auto rhdl = [&](const QJsonValue &v) {
        SearchResult sr;
        ModelTranslator::fromJson(v.toArray().at(0).toObject(), sr);
        emit searchResult(identifier, sr);
    };

    QJsonObject params;

    QJsonObject joQuery;
    for (auto it = query.cbegin(); it != query.cend(); ++it)
        joQuery.insert(it.key(), QJsonArray::fromStringList(it.value()));
    params["query"] = joQuery;
    params["uris"] = QJsonArray::fromStringList(uris);
    params["exact"] = exact;

    sendRequest("search", rhdl, params);
}

void LibraryController::lookup(const QStringList &uris)
{
    auto rhdl = [&](const QJsonValue &v) {
        Tracks tracks = ModelTranslator::fromJsonArray<Track>(v.toArray());
        emit lookedup(tracks);
    };

    QJsonObject params;
    if (uris.size() == 1)
        params["uri"] = uris.first();
    else if (uris.size() > 1)
        params["uris"] = QJsonArray::fromStringList(uris);

    sendRequest("lookup", rhdl, params);
}

void LibraryController::refresh(const QString &uri)
{
    QJsonObject params;
    params["uri"] = uri.isEmpty() ? QJsonValue() : uri;
    sendNotification("refresh", params);
}

void LibraryController::getImages(const QStringList &uris)
{
    auto rhdl = [&](const QJsonValue &v) {
        QMap<QString, mopidy::Images> uri_images;
        const QJsonObject joUriImages = v.toObject();
        for (auto uri : joUriImages.keys())
        {
            Images images = ModelTranslator::fromJsonArray<Image>(joUriImages.value(uri).toArray());
            uri_images.insert(uri, images);
        }
        emit imagesReceived(uri_images);
    };

    QJsonObject params;
    params["uris"] = QJsonArray::fromStringList(uris);

    sendRequest("get_images", rhdl, params);
}
