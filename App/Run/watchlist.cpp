#include "watchlist.h"
#include <fstream>
#include <cstdlib>

WatchListCSV::WatchListCSV(const std::string& filename)
    : filename(filename) {}

void WatchListCSV::save(const std::vector<Movie>& movies) {
    std::ofstream fout(filename);
    if (!fout.is_open())
        return;

    for (const auto& m : movies) {
        fout << m.getTitle() << ","
             << m.getGenre() << ","
             << m.getYear() << ","
             << m.getLikes() << ","
             << m.getTrailer() << "\n";
    }
}

void WatchListCSV::open() {
    std::string cmd = "explorer.exe \"" + filename + "\"";
    system(cmd.c_str());
}

WatchListHTML::WatchListHTML(const std::string& filename)
    : filename(filename) {}

void WatchListHTML::save(const std::vector<Movie>& movies) {
    std::ofstream fout(filename);
    if (!fout.is_open())
        return;

    fout << "<!DOCTYPE html>\n<html>\n<head>\n<title>Watchlist</title>\n</head>\n<body>\n";
    fout << "<table border=\"1\">\n<tr>"
         << "<td>Title</td><td>Genre</td><td>Year</td><td>Likes</td><td>Trailer</td>"
         << "</tr>\n";

    for (const auto& m : movies) {
        fout << "<tr><td>" << m.getTitle() << "</td>"
             << "<td>" << m.getGenre() << "</td>"
             << "<td>" << m.getYear() << "</td>"
             << "<td>" << m.getLikes() << "</td>"
             << "<td>" << m.getTrailer() << "</td></tr>\n";
    }

    fout << "</table>\n</body>\n</html>\n";
}

void WatchListHTML::open() {
    std::string cmd = "explorer.exe \"" + filename + "\"";
    system(cmd.c_str());
}
