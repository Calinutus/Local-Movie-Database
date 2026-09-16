#include "service.h"

MovieService::MovieService(MovieRepository& repo) : repo(repo) {}

std::string MovieService::addMovie(const std::string& title, const std::string& genre,
                                   int year, int likes, const std::string& trailer) {
    Movie m(title, genre, year, likes, trailer);
    std::string error = MovieValidator::validMovie(m);
    if (!error.empty()) return error;

    if (repo.findMovie(title) != nullptr)
        return "Movie already exists";

    repo.addMovie(m);

    undoStack.push(std::make_unique<UndoAdd>(m, repo));
    while (!redoStack.empty()) redoStack.pop();

    return "OK";
}

bool MovieService::deleteMovie(const std::string& title) {
    Movie* m = repo.findMovie(title);
    if (!m) return false;

    Movie copy = *m;
    repo.removeMovie(title);

    undoStack.push(std::make_unique<UndoRemove>(copy, repo));
    while (!redoStack.empty()) redoStack.pop();

    return true;
}

bool MovieService::updateMovie(const std::string& title, const std::string& genre,
                               int year, int likes, const std::string& trailer) {
    Movie* old = repo.findMovie(title);
    if (!old) return false;

    Movie oldCopy = *old;
    Movie newMovie(title, genre, year, likes, trailer);

    std::string error = MovieValidator::validMovie(newMovie);
    if (!error.empty()) return false;

    repo.updateMovie(newMovie);

    undoStack.push(std::make_unique<UndoUpdate>(oldCopy, newMovie, repo));
    while (!redoStack.empty()) redoStack.pop();

    return true;
}

Movie* MovieService::findMovie(const std::string& title) {
    return repo.findMovie(title);
}

std::vector<Movie>& MovieService::getAllMovies() {
    return repo.getMovies();
}

std::vector<Movie> MovieService::getMovieByGenre(const std::string& genre) {
    std::vector<Movie> result;
    auto& all = repo.getMovies();
    for (auto& m : all) {
        if (genre.empty() || m.getGenre() == genre)
            result.push_back(m);
    }
    return result;
}

bool MovieService::addToWatchList(const Movie& m) {
    if (isInWatchList(m)) return false;
    watchList.push_back(m);
    return true;
}

std::vector<Movie>& MovieService::getWatchList() {
    return watchList;
}

bool MovieService::removeFromWatchList(const std::string& title, bool liked) {
    for (size_t i = 0; i < watchList.size(); i++) {
        if (watchList[i].getTitle() == title) {

            if (liked) {
                Movie* repoMovie = repo.findMovie(title);
                if (repoMovie)
                    repoMovie->setLikes(repoMovie->getLikes() + 1);
            }

            watchList.erase(watchList.begin() + (long long)i);
            return true;
        }
    }
    return false;
}

bool MovieService::isInWatchList(const Movie& movie) {
    for (auto& m : watchList) {
        if (m.getTitle() == movie.getTitle())
            return true;
    }
    return false;
}

bool MovieService::undo() {
    if (undoStack.empty()) return false;

    auto action = std::move(undoStack.top());
    undoStack.pop();

    action->undo();
    redoStack.push(std::move(action));
    return true;
}

bool MovieService::redo() {
    if (redoStack.empty()) return false;

    auto action = std::move(redoStack.top());
    redoStack.pop();

    action->redo();
    undoStack.push(std::move(action));
    return true;
}
