#ifndef MOPIDYMODELS_H
#define MOPIDYMODELS_H

#include <QDate>
#include <QStringList>
#include <QVariant>

namespace mopidy {

enum class PlaybackState
{
    Paused,
    Playing,
    Stopped
};

enum class RefType
{
    Album,
    Artist,
    Directory,
    Playlist,
    Track
};

struct Artist
{
    QString uri;
    QString name;
    QString sortname;
    QString musicbrainz_id;
};
typedef QList<Artist> Artists;

struct Album
{
    QString uri;
    QString name;
    Artists artists;
    int num_tracks;
    int num_discs;
    QDate date;
    QString musicbrainz_id;
};
typedef QList<Album> Albums;

struct Track
{
    QString uri;
    QString name;
    Artists artists;
    Album album;
    Artists composers;
    Artists performers;
    QString genre;
    int track_no;
    int disc_no;
    QDate date;
    int length;
    int bitrate;
    QString comment;
    QString musicbrainz_id;
    QDateTime last_modified;
};
typedef QList<Track> Tracks;

struct Playlist
{
    QString uri;
    QString name;
    Tracks tracks;
    QDateTime last_modified;
};
typedef QList<Playlist> Playlists;

struct SearchResult
{
    QString uri;
    Tracks tracks;
    Albums albums;
    Artists artists;
};

struct TlTrack
{
    int tlid = -1;
    Track track;
};
typedef QList<TlTrack> TlTracks;

struct Ref
{
    QString uri;
    QString name;
    RefType type;
};
typedef QList<Ref> Refs;

struct Image
{
    QString uri;
    int width;
    int height;
};
typedef QList<Image> Images;

typedef QMap<unsigned long, Ref> History;

// QVariant, signals, ...
void registerMopidyTypes();

} // mopidy

//
Q_DECLARE_METATYPE(mopidy::Album)
Q_DECLARE_METATYPE(mopidy::Artist)
Q_DECLARE_METATYPE(mopidy::Image)
Q_DECLARE_METATYPE(mopidy::Playlist)
Q_DECLARE_METATYPE(mopidy::Ref)
Q_DECLARE_METATYPE(mopidy::SearchResult)
Q_DECLARE_METATYPE(mopidy::TlTrack)
Q_DECLARE_METATYPE(mopidy::Track)
Q_DECLARE_METATYPE(mopidy::History)

#endif // MOPIDYMODELS_H
