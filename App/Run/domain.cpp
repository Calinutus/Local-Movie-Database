#include "domain.h"
#include <sstream>


Movie::Movie() {
    this->title = "";
    this->genre = "";
    this->year = 0;
    this->likes = 0;
    this->trailer = "";
}

Movie::Movie(const std::string &title, const std::string &genre, const int &year, const int &likes, const std::string &trailer) {
    this->title = title;
    this->genre = genre;
    this->year = year;
    this->likes = likes;
    this->trailer = trailer;
}

std::string Movie::getTitle() const {return this->title;}
std::string Movie::getGenre() const {return this->genre;}
int Movie::getYear() const {return this->year;}
int Movie::getLikes() const {return this->likes;}
std::string Movie::getTrailer() const {return this->trailer;}

void Movie::setTitle(const std::string &newTitle) {this->title = newTitle;}
void Movie::setGenre(const std::string &newGenre) {this->genre = newGenre;}
void Movie::setYear(const int &newYear) {this->year = newYear;}
void Movie::setLikes(const int &newLikes) {this->likes = newLikes;}
void Movie::setTrailer(const std::string &newTrailer) {this->trailer = newTrailer;}


std::ostream& operator<<(std::ostream &os, const Movie &movie) {
    os << movie.title << "|"
        << movie.genre << "|"
        << movie.year << "|"
        << movie.likes << "|"
        << movie.trailer << '\n';

    return os;
}

std::istream& operator>>(std::istream &is, Movie &movie) {
    std::string line;
    if (!std::getline(is, line))
        return is;

    if (line.empty())
        return is;

    std::stringstream ss(line);
    std::string title , genre , yearStr , likeStr , trailer;

    std::getline(ss , title , '|');
    std::getline(ss , genre , '|');
    std::getline(ss , yearStr , '|');
    std::getline(ss , likeStr , '|');
    std::getline(ss , trailer , '\n');

    auto trim = [](std::string &s) {
        while (!s.empty() && std::isspace((unsigned char)s.front()))
            s.erase(0  ,1);
        while (!s.empty() && std::isspace((unsigned char)s.back()))
            s.pop_back();
    };
    trim(title);
    trim(genre);
    trim(yearStr);
    trim(likeStr);
    trim(trailer);

    if (title.empty())
        return is;

    movie.setTitle(title);
    movie.setGenre(genre);
    movie.setYear(std::stoi(yearStr));
    movie.setLikes(std::stoi(likeStr));
    movie.setTrailer(trailer);

    return is;
}
