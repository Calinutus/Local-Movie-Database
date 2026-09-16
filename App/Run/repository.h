#pragma once

#include "domain.h"
#include <string>
#include <vector>



class MovieRepository{
private:
    std::vector<Movie> movies;

    std::string filename;
    void loadFromFile();
    void saveToFile();

public:
    MovieRepository(const std::string& filename);

    void addMovie(const Movie &m);
    void removeMovie(const std::string &title);
    void updateMovie(const Movie& m);
    Movie* findMovie(const std::string &title);
    std::vector<Movie>& getMovies();
    int size() const;


};