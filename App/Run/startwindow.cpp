#include "startwindow.h"
#include "admin_gui.h"
#include "user_gui.h"

#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>

StartWindow::StartWindow(MovieService& service, QWidget* parent)
    : QWidget(parent), service(service)
{
    auto* mainLayout = new QVBoxLayout{this};

    auto* typeBox = new QGroupBox{"Watchlist type"};
    auto* typeLayout = new QVBoxLayout{typeBox};
    radioCsv = new QRadioButton{"CSV"};
    radioHtml = new QRadioButton{"HTML"};
    radioCsv->setChecked(true);
    typeLayout->addWidget(radioCsv);
    typeLayout->addWidget(radioHtml);

    auto* modeBox = new QGroupBox{"Mode"};
    auto* modeLayout = new QVBoxLayout{modeBox};
    buttonAdmin = new QPushButton{"Administrator"};
    buttonUser  = new QPushButton{"User"};
    modeLayout->addWidget(buttonAdmin);
    modeLayout->addWidget(buttonUser);

    mainLayout->addWidget(typeBox);
    mainLayout->addWidget(modeBox);

    connect(buttonAdmin, &QPushButton::clicked, this, &StartWindow::openAdmin);
    connect(buttonUser,  &QPushButton::clicked, this, &StartWindow::openUser);

    setWindowTitle("Movie Manager - Start");
    resize(320, 220);
}

WatchList* StartWindow::createWatchList() {
    if (radioCsv->isChecked())
        return new WatchListCSV("watchlist.csv");
    else
        return new WatchListHTML("watchlist.html");
}

void StartWindow::openAdmin() {
    WatchList* wl = createWatchList();
    auto* win = new AdminGUI(service, wl);
    win->show();
    close();
}

void StartWindow::openUser() {
    WatchList* wl = createWatchList();
    auto* win = new UserGUI(service, wl);
    win->show();
    close();
}
