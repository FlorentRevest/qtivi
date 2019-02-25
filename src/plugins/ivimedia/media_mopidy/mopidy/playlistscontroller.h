#ifndef PLAYLISTSCONTROLLER_H
#define PLAYLISTSCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

namespace mopidy {

class PlaylistsController : public BaseController
{
    Q_OBJECT

public:
    explicit PlaylistsController();
    ~PlaylistsController();

    void uriShemes();

    // Fetching
    void asList();
    void getItems(const QString &uri);
    void lookup(const QString &uri);
    void refresh(const QString &uri_scheme = "");

    // Manipulating
    void create(const QString &name, const QString &uri_scheme = "");
    void save(const mopidy::Playlist &pl);
    void deletePlaylist(const QString &uri);

signals:
    void uriSchemesReceived(const QStringList &schemes);
    void listReceived(const mopidy::Refs &refs);
    void itemsReceived(const mopidy::Refs &refs);
    void lookedup(const mopidy::Playlist &playlist);
    void created(const mopidy::Playlist &playlist);
    void saved(const mopidy::Playlist playlist);
};

} // mopidy

#endif //PLAYLISTSCONTROLLER_H
