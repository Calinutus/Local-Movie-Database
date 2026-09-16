#pragma once

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>
#include "service.h"
#include "watchlistmodel.h"

class WatchlistWindow : public QWidget {
    Q_OBJECT
public:
    explicit WatchlistWindow(MovieService& service, QWidget* parent = nullptr)
        : QWidget(parent), service(service) {
        auto* layout = new QVBoxLayout{this};

        table = new QTableView{};
        model = new WatchlistModel(service.getWatchList(), this);
        table->setModel(model);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        layout->addWidget(table);

        setWindowTitle("Watchlist");
        resize(700, 400);
    }

    void refresh() {
        model->refresh();
    }

private:
    MovieService& service;
    QTableView* table;
    WatchlistModel* model;
};
