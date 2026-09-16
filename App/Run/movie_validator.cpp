#include "movie_validator.h"

std::string MovieValidator::validMovie(const Movie& m) {
    if (m.getTitle().empty()) return "Title cannot be empty";
    if (m.getGenre().empty()) return "Genre cannot be empty";
    if (m.getYear() < 1800 || m.getYear() > 2100) return "Invalid year";
    if (m.getLikes() < 0) return "Likes cannot be negative";
    if (m.getTrailer().empty()) return "Trailer cannot be empty";
    return "";
}
