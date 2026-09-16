#pragma once

#include <memory>
#include "domain.h"
#include "repository.h"

class UndoAction {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual ~UndoAction() = default;
};

class UndoAdd : public UndoAction {
private:
    Movie movie;
    MovieRepository& repo;

public:
    UndoAdd(const Movie& m, MovieRepository& r)
        : movie(m), repo(r) {}

    void undo() override {
        repo.removeMovie(movie.getTitle());
    }

    void redo() override {
        repo.addMovie(movie);
    }
};

class UndoRemove : public UndoAction {
private:
    Movie movie;
    MovieRepository& repo;

public:
    UndoRemove(const Movie& m, MovieRepository& r)
        : movie(m), repo(r) {}

    void undo() override {
        repo.addMovie(movie);
    }

    void redo() override {
        repo.removeMovie(movie.getTitle());
    }
};

class UndoUpdate : public UndoAction {
private:
    Movie oldMovie;
    Movie newMovie;
    MovieRepository& repo;

public:
    UndoUpdate(const Movie& oldM, const Movie& newM, MovieRepository& r)
        : oldMovie(oldM), newMovie(newM), repo(r) {}

    void undo() override {
        repo.updateMovie(oldMovie);
    }

    void redo() override {
        repo.updateMovie(newMovie);
    }
};
