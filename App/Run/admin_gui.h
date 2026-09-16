#pragma once

#include <QWidget>
#include "service.h"
#include "watchlist.h"

class QTableWidget;
class QLineEdit;
class QPushButton;
class QRadioButton;

class AdminGUI : public QWidget {
    Q_OBJECT
public:
    AdminGUI(MovieService& service, WatchList* watchlist, QWidget* parent = nullptr);

private:
    MovieService& service;
    WatchList* watchlist;

    QTableWidget* tableMovies;
    QLineEdit* editTitle;
    QLineEdit* editGenre;
    QLineEdit* editYear;
    QLineEdit* editLikes;
    QLineEdit* editTrailer;

    QPushButton* buttonAdd;
    QPushButton* buttonUpdate;
    QPushButton* buttonDelete;
    QPushButton* buttonReload;
    QPushButton* buttonBack;
    QPushButton* buttonUndo;
    QPushButton* buttonRedo;

    QRadioButton* radioSortAZ;
    QRadioButton* radioShuffle;

    void reloadTable();
    void loadSelectedRow();

private slots:
    void addMovie();
    void updateMovie();
    void deleteMovie();
    void rowChanged();
    void goBack();

    void sortMovies();
    void shuffleMovies();

    void undoAction();
    void redoAction();
};
