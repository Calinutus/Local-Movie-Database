#pragma once

#include "movie_validator.h"
#include "repository.h"
#include "undo.h"
#include <vector>
#include <algorithm>
#include <stack>
#include <memory>

class MovieService {
private:
    MovieRepository& repo;
    std::vector<Movie> watchList;

    std::stack<std::unique_ptr<UndoAction>> undoStack;
    std::stack<std::unique_ptr<UndoAction>> redoStack;

public:
    MovieService(MovieRepository& repo);

    std::string addMovie(const std::string &title , const std::string &genre ,
                         int year , int likes , const std::string &trailer);
    bool deleteMovie(const std::string &title);
    bool updateMovie(const std::string &title , const std::string &genre ,
                     int year , int likes , const std::string &trailer);

    Movie* findMovie(const std::string &title);
    std::vector<Movie>& getAllMovies();

    std::vector<Movie> getMovieByGenre(const std::string &genre);
    bool addToWatchList(const Movie& m);
    std::vector<Movie>& getWatchList();
    bool removeFromWatchList(const std::string& title, bool liked);
    bool isInWatchList(const Movie& movie);

    bool undo();
    bool redo();
};
