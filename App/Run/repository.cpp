#include "repository.h"
#include <fstream>

MovieRepository::MovieRepository(const std::string& filename)
    : filename(filename) {
    loadFromFile();
}

void MovieRepository::loadFromFile() {
    movies.clear();
    std::ifstream fin(filename);
    if (!fin.is_open()) return;

    Movie m;
    while (fin >> m) {
        movies.push_back(m);
    }
}

void MovieRepository::saveToFile() {
    std::ofstream fout(filename);
    for (auto& m : movies) {
        fout << m;
    }
}

void MovieRepository::addMovie(const Movie& m) {
    movies.push_back(m);
    saveToFile();
}

void MovieRepository::removeMovie(const std::string& title) {
    for (size_t i = 0; i < movies.size(); i++) {
        if (movies[i].getTitle() == title) {
            movies.erase(movies.begin() + i);
            saveToFile();
            return;
        }
    }
}

void MovieRepository::updateMovie(const Movie& m) {
    for (auto& movie : movies) {
        if (movie.getTitle() == m.getTitle()) {
            movie = m;
            saveToFile();
            return;
        }
    }
}

Movie* MovieRepository::findMovie(const std::string& title) {
    for (auto& m : movies) {
        if (m.getTitle() == title)
            return &m;
    }
    return nullptr;
}

std::vector<Movie>& MovieRepository::getMovies() {
    return movies;
}

int MovieRepository::size() const {
    return movies.size();
}
