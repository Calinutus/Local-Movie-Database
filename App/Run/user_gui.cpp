#include "user_gui.h"
#include "startwindow.h"
#include "watchlistwindow.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>

UserGUI::UserGUI(MovieService& service, WatchList* watchlist, QWidget* parent)
    : QWidget(parent), service(service), watchlist(watchlist)
{
    auto* mainLayout = new QVBoxLayout{this};

    auto* topLayout = new QHBoxLayout{};
    editGenre = new QLineEdit{};
    buttonFilter = new QPushButton{"Filter"};
    topLayout->addWidget(new QLabel{"Genre (empty = all):"});
    topLayout->addWidget(editGenre);
    topLayout->addWidget(buttonFilter);

    labelMovieInfo = new QLabel{"No movie selected"};
    labelMovieInfo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelMovieInfo->setMinimumHeight(120);

    buttonOpenTrailer         = new QPushButton{"Open Trailer"};
    buttonAddToWatchlist      = new QPushButton{"Add to Watchlist"};
    buttonNext                = new QPushButton{"Next"};
    buttonViewWatchlist       = new QPushButton{"View Watchlist"};
    buttonRemoveFromWatchlist = new QPushButton{"Remove from Watchlist"};
    buttonOpenWatchlistFile   = new QPushButton{"Open Watchlist File"};
    buttonBack                = new QPushButton{"Back"};

    auto* midButtons = new QHBoxLayout{};
    midButtons->addWidget(buttonOpenTrailer);
    midButtons->addWidget(buttonAddToWatchlist);
    midButtons->addWidget(buttonNext);

    auto* bottomButtons = new QHBoxLayout{};
    bottomButtons->addWidget(buttonViewWatchlist);
    bottomButtons->addWidget(buttonRemoveFromWatchlist);
    bottomButtons->addWidget(buttonOpenWatchlistFile);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(labelMovieInfo);
    mainLayout->addLayout(midButtons);
    mainLayout->addLayout(bottomButtons);
    mainLayout->addWidget(buttonBack);

    connect(buttonFilter,            &QPushButton::clicked, this, &UserGUI::filterByGenre);
    connect(buttonOpenTrailer,       &QPushButton::clicked, this, &UserGUI::openTrailer);
    connect(buttonAddToWatchlist,    &QPushButton::clicked, this, &UserGUI::addToWatchlist);
    connect(buttonNext,              &QPushButton::clicked, this, &UserGUI::nextMovie);
    connect(buttonViewWatchlist,     &QPushButton::clicked, this, &UserGUI::viewWatchlist);
    connect(buttonRemoveFromWatchlist,&QPushButton::clicked, this, &UserGUI::removeFromWatchlist);
    connect(buttonOpenWatchlistFile, &QPushButton::clicked, this, &UserGUI::openWatchlistFile);
    connect(buttonBack,              &QPushButton::clicked, this, &UserGUI::goBack);

    setWindowTitle("User - Movies");
    resize(700, 400);
}

void UserGUI::showCurrentMovie() {
    if (currentMovies.empty()) {
        labelMovieInfo->setText("No movies found");
        return;
    }
    const Movie& m = currentMovies[currentIndex];
    QString text =
        "Title: "   + QString::fromStdString(m.getTitle())  + "\n" +
        "Genre: "   + QString::fromStdString(m.getGenre())  + "\n" +
        "Year: "    + QString::number(m.getYear())          + "\n" +
        "Likes: "   + QString::number(m.getLikes())         + "\n" +
        "Trailer: " + QString::fromStdString(m.getTrailer());
    labelMovieInfo->setText(text);

    QDesktopServices::openUrl(QUrl(QString::fromStdString(m.getTrailer())));
}

void UserGUI::filterByGenre() {
    QString g = editGenre->text();
    currentMovies = service.getMovieByGenre(g.toStdString());
    currentIndex = 0;
    showCurrentMovie();
}

void UserGUI::openTrailer() {
    if (currentMovies.empty()) return;
    const Movie& m = currentMovies[currentIndex];
    QDesktopServices::openUrl(QUrl(QString::fromStdString(m.getTrailer())));
}

void UserGUI::addToWatchlist() {
    if (currentMovies.empty()) return;
    const Movie& m = currentMovies[currentIndex];
    if (!service.addToWatchList(m)) {
        QMessageBox::information(this, "Watchlist", "Movie already in watchlist");
    } else {
        QMessageBox::information(this, "Watchlist", "Movie added");
        watchlist->save(service.getWatchList());
    }
}

void UserGUI::nextMovie() {
    if (currentMovies.empty()) return;
    currentIndex = (currentIndex + 1) % currentMovies.size();
    showCurrentMovie();
}

void UserGUI::viewWatchlist() {
    auto& wl = service.getWatchList();
    if (wl.empty()) {
        QMessageBox::information(this, "Watchlist", "Watchlist is empty");
        return;
    }

    auto* win = new WatchlistWindow(service);
    win->show();
}

void UserGUI::removeFromWatchlist() {
    bool ok;
    QString title = QInputDialog::getText(this, "Remove", "Title:", QLineEdit::Normal, "", &ok);
    if (!ok || title.isEmpty()) return;

    auto reply = QMessageBox::question(this, "Like", "Did you like it?", QMessageBox::Yes | QMessageBox::No);
    bool liked = (reply == QMessageBox::Yes);

    if (service.removeFromWatchList(title.toStdString(), liked)) {
        QMessageBox::information(this, "Watchlist", "Removed");
        watchlist->save(service.getWatchList());
    } else {
        QMessageBox::warning(this, "Watchlist", "Movie not found in watchlist");
    }
}

void UserGUI::openWatchlistFile() {
    watchlist->save(service.getWatchList());
    watchlist->open();
}

void UserGUI::goBack() {
    auto* start = new StartWindow(service);
    start->show();
    close();
}
