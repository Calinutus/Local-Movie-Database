#pragma once

#include "watchlistwindow.h"
#include <QWidget>
#include <vector>
#include "service.h"
#include "watchlist.h"

class QLineEdit;
class QLabel;
class QPushButton;

class UserGUI : public QWidget {
    Q_OBJECT
public:
    UserGUI(MovieService& service, WatchList* watchlist, QWidget* parent = nullptr);

private:
    MovieService& service;
    WatchList* watchlist;

    QLineEdit* editGenre;
    QLabel* labelMovieInfo;

    QPushButton* buttonFilter;
    QPushButton* buttonOpenTrailer;
    QPushButton* buttonAddToWatchlist;
    QPushButton* buttonNext;
    QPushButton* buttonViewWatchlist;
    QPushButton* buttonRemoveFromWatchlist;
    QPushButton* buttonOpenWatchlistFile;
    QPushButton* buttonBack;

    std::vector<Movie> currentMovies;
    int currentIndex = 0;

    void showCurrentMovie();

private slots:
    void filterByGenre();
    void openTrailer();
    void addToWatchlist();
    void nextMovie();
    void viewWatchlist();
    void removeFromWatchlist();
    void openWatchlistFile();
    void goBack();
};
