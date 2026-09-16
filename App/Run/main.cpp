#include <QApplication>
#include "repository.h"
#include "service.h"
#include "watchlist.h"
#include "startwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MovieRepository repo("movies.txt");
    MovieService service(repo);

    StartWindow w(service);
    w.show();

    return app.exec();
}
