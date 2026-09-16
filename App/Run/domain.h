#pragma once
#include <string>
#include <iostream>

class Movie {
private:
    std::string title;
    std::string genre;
    int year;
    int likes;
    std::string trailer;
public:
    Movie();
    Movie(const std::string &title , const std::string &genre, const int &year , const int &likes , const std::string &trailer);

    std::string getTitle() const;
    std::string getGenre() const;
    int getYear() const;
    int getLikes() const;
    std::string getTrailer() const;

    void setTitle(const std::string &newTitle);
    void setGenre(const std::string &newGenre);
    void setYear(const int &newYear);
    void setLikes(const int &newLikes);
    void setTrailer(const std::string &newTrailer);

    friend std::ostream &operator<<(std::ostream &os, const Movie &movie);
    friend std::istream &operator>>(std::istream &is , Movie &movie);
};
