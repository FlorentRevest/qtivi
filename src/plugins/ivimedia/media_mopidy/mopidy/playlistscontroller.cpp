#include "playlistscontroller.h"
#include "modeltranslator.h"

using namespace mopidy;

PlaylistsController::PlaylistsController()
    : BaseController("core.playlists")
{
}

PlaylistsController::~PlaylistsController()
{
}

void PlaylistsController::uriShemes()
{
    auto rhdl = [&](const QJsonValue &v) {
        QStringList uris;
        for (QJsonValue jv : v.toArray())
            uris << jv.toString();
        emit uriSchemesReceived(uris);
    };

    sendRequest("get_uri_schemes", rhdl);
}

void PlaylistsController::asList()
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit listReceived(refs);
    };

    sendRequest("as_list", rhdl);
}

void PlaylistsController::getItems(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit itemsReceived(refs);
    };

    sendRequest("get_items", rhdl, {{"uri", uri}});
}

void PlaylistsController::lookup(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit lookedup(playlist);
    };

    sendRequest("lookup", rhdl, {{"uri", uri}});
}

void PlaylistsController::refresh(const QString &uri_scheme)
{
    sendNotification("refresh", {{"uri_scheme", uri_scheme}});
}

void PlaylistsController::create(const QString &name, const QString &uri_scheme)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit created(playlist);
    };

    QJsonObject params;
    params.insert("name", name);
    params.insert("uri_scheme", uri_scheme);

    sendRequest("create", rhdl, params);
}

void PlaylistsController::save(const Playlist &pl)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit saved(playlist);
    };

    QJsonObject params;
    params.insert("playlist", ModelTranslator::toJson(pl));

    sendRequest("save", rhdl, params);
}

void PlaylistsController::deletePlaylist(const QString &uri)
{
    sendNotification("delete", {{"uri", uri}});
}
