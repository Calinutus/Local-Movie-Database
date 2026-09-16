#pragma once

#include <QWidget>
#include "service.h"
#include "watchlist.h"

class QRadioButton;
class QPushButton;

class StartWindow : public QWidget {
    Q_OBJECT
public:
    explicit StartWindow(MovieService& service, QWidget* parent = nullptr);

private:
    MovieService& service;
    QRadioButton* radioCsv;
    QRadioButton* radioHtml;
    QPushButton* buttonAdmin;
    QPushButton* buttonUser;

    WatchList* createWatchList();

private slots:
    void openAdmin();
    void openUser();
};
