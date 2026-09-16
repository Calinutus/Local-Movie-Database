#include "admin_gui.h"
#include "startwindow.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QShortcut>
#include <QKeySequence>

#include <algorithm>
#include <random>

AdminGUI::AdminGUI(MovieService& service, WatchList* watchlist, QWidget* parent)
    : QWidget(parent), service(service), watchlist(watchlist)
{
    auto* mainLayout = new QHBoxLayout{this};

    tableMovies = new QTableWidget{};
    tableMovies->setColumnCount(5);
    QStringList headers{"Title","Genre","Year","Likes","Trailer"};
    tableMovies->setHorizontalHeaderLabels(headers);
    tableMovies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto* rightLayout = new QVBoxLayout{};
    auto* form = new QFormLayout{};
    editTitle   = new QLineEdit{};
    editGenre   = new QLineEdit{};
    editYear    = new QLineEdit{};
    editLikes   = new QLineEdit{};
    editTrailer = new QLineEdit{};
    form->addRow("Title:",   editTitle);
    form->addRow("Genre:",   editGenre);
    form->addRow("Year:",    editYear);
    form->addRow("Likes:",   editLikes);
    form->addRow("Trailer:", editTrailer);

    auto* buttonsLayout = new QHBoxLayout{};
    buttonAdd    = new QPushButton{"Add"};
    buttonUpdate = new QPushButton{"Update"};
    buttonDelete = new QPushButton{"Delete"};
    buttonReload = new QPushButton{"Reload"};
    buttonsLayout->addWidget(buttonAdd);
    buttonsLayout->addWidget(buttonUpdate);
    buttonsLayout->addWidget(buttonDelete);
    buttonsLayout->addWidget(buttonReload);

    radioSortAZ = new QRadioButton{"Sort A→Z"};
    radioShuffle = new QRadioButton{"Shuffle"};
    radioSortAZ->setChecked(true);

    auto* radioLayout = new QHBoxLayout{};
    radioLayout->addWidget(radioSortAZ);
    radioLayout->addWidget(radioShuffle);

    buttonUndo = new QPushButton{"Undo"};
    buttonRedo = new QPushButton{"Redo"};
    buttonBack = new QPushButton{"Back"};

    rightLayout->addLayout(form);
    rightLayout->addLayout(buttonsLayout);
    rightLayout->addLayout(radioLayout);
    rightLayout->addWidget(buttonUndo);
    rightLayout->addWidget(buttonRedo);
    rightLayout->addWidget(buttonBack);
    rightLayout->addStretch();

    mainLayout->addWidget(tableMovies);
    mainLayout->addLayout(rightLayout);

    connect(buttonAdd,    &QPushButton::clicked, this, &AdminGUI::addMovie);
    connect(buttonUpdate, &QPushButton::clicked, this, &AdminGUI::updateMovie);
    connect(buttonDelete, &QPushButton::clicked, this, &AdminGUI::deleteMovie);
    connect(buttonReload, &QPushButton::clicked, this, &AdminGUI::reloadTable);
    connect(tableMovies,  &QTableWidget::currentCellChanged, this, &AdminGUI::rowChanged);
    connect(buttonBack,   &QPushButton::clicked, this, &AdminGUI::goBack);

    connect(buttonUndo,   &QPushButton::clicked, this, &AdminGUI::undoAction);
    connect(buttonRedo,   &QPushButton::clicked, this, &AdminGUI::redoAction);


    connect(radioSortAZ,  &QRadioButton::clicked, this, &AdminGUI::sortMovies);
    connect(radioShuffle, &QRadioButton::clicked, this, &AdminGUI::shuffleMovies);

    new QShortcut(QKeySequence("Ctrl+Z"), this, SLOT(undoAction()));
    new QShortcut(QKeySequence("Ctrl+Y"), this, SLOT(redoAction()));

    setWindowTitle("Administrator - Movies");
    resize(900, 500);
    reloadTable();
}

void AdminGUI::reloadTable() {
    auto& movies = service.getAllMovies();
    tableMovies->setRowCount((int)movies.size());

    int row = 0;
    for (const auto& m : movies) {
        tableMovies->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(m.getTitle())));
        tableMovies->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(m.getGenre())));
        tableMovies->setItem(row, 2, new QTableWidgetItem(QString::number(m.getYear())));
        tableMovies->setItem(row, 3, new QTableWidgetItem(QString::number(m.getLikes())));
        tableMovies->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(m.getTrailer())));
        row++;
    }
}

void AdminGUI::loadSelectedRow() {
    int row = tableMovies->currentRow();
    if (row < 0) return;
    editTitle->setText(tableMovies->item(row, 0)->text());
    editGenre->setText(tableMovies->item(row, 1)->text());
    editYear->setText(tableMovies->item(row, 2)->text());
    editLikes->setText(tableMovies->item(row, 3)->text());
    editTrailer->setText(tableMovies->item(row, 4)->text());
}

void AdminGUI::rowChanged() {
    loadSelectedRow();
}

void AdminGUI::addMovie() {
    std::string title   = editTitle->text().toStdString();
    std::string genre   = editGenre->text().toStdString();
    int year            = editYear->text().toInt();
    int likes           = editLikes->text().toInt();
    std::string trailer = editTrailer->text().toStdString();

    QString result = QString::fromStdString(service.addMovie(title, genre, year, likes, trailer));
    QMessageBox::information(this, "Add movie", result);
    reloadTable();
}

void AdminGUI::updateMovie() {
    std::string title   = editTitle->text().toStdString();
    std::string genre   = editGenre->text().toStdString();
    int year            = editYear->text().toInt();
    int likes           = editLikes->text().toInt();
    std::string trailer = editTrailer->text().toStdString();

    bool ok = service.updateMovie(title, genre, year, likes, trailer);
    if (!ok)
        QMessageBox::warning(this, "Update movie", "Update failed");
    reloadTable();
}

void AdminGUI::deleteMovie() {
    std::string title = editTitle->text().toStdString();
    bool ok = service.deleteMovie(title);
    if (!ok)
        QMessageBox::warning(this, "Delete movie", "Movie not found");
    reloadTable();
}

void AdminGUI::goBack() {
    auto* start = new StartWindow(service);
    start->show();
    close();
}

void AdminGUI::sortMovies() {
    auto& movies = service.getAllMovies();
    std::sort(movies.begin(), movies.end(),
              [](const Movie& a, const Movie& b) {
                  return a.getTitle() < b.getTitle();
              });
    reloadTable();
}

void AdminGUI::shuffleMovies() {
    auto& movies = service.getAllMovies();
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::shuffle(movies.begin(), movies.end(), engine);
    reloadTable();
}

void AdminGUI::undoAction() {
    if (!service.undo())
        QMessageBox::information(this, "Undo", "Nothing to undo");
    reloadTable();
}

void AdminGUI::redoAction() {
    if (!service.redo())
        QMessageBox::information(this, "Redo", "Nothing to redo");
    reloadTable();
}
