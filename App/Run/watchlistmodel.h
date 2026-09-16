#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "domain.h"

class WatchlistModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::vector<Movie>& movies;

public:
    explicit WatchlistModel(std::vector<Movie>& movies, QObject* parent = nullptr)
        : QAbstractTableModel(parent), movies(movies) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return (int)movies.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return 5;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return {};

        const Movie& m = movies[(size_t)index.row()];
        switch (index.column()) {
            case 0: return QString::fromStdString(m.getTitle());
            case 1: return QString::fromStdString(m.getGenre());
            case 2: return m.getYear();
            case 3: return m.getLikes();
            case 4: return QString::fromStdString(m.getTrailer());
            default: return {};
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return {};

        switch (section) {
            case 0: return "Title";
            case 1: return "Genre";
            case 2: return "Year";
            case 3: return "Likes";
            case 4: return "Trailer";
            default: return {};
        }
    }

    void refresh() {
        beginResetModel();
        endResetModel();
    }
};
