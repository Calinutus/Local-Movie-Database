#include "tests.h"
#include "domain.h"
#include "movie_validator.h"
#include "repository_exception.h"
#include "repository.h"
#include "service.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>



void testMovie() {
    Movie m("A" , "G" , 2000 , 10 , "http://x");
    assert(m.getTitle() == "A");
    assert(m.getGenre() == "G");
    assert(m.getYear() == 2000);
    assert(m.getLikes() == 10);
    assert(m.getTrailer() == "http://x");

    m.setTitle("B");
    m.setGenre("A");
    m.setYear(2001);
    m.setLikes(20);
    m.setTrailer("http://xyz");

    assert(m.getTitle() == "B");
    assert(m.getGenre() == "A");
    assert(m.getYear() == 2001);
    assert(m.getLikes() == 20);
    assert(m.getTrailer() == "http://xyz");
}



void testRepository() {
    std::ofstream("test_repo.txt").close();
    MovieRepository repo("test_repo.txt");

    Movie m("A" , "G" , 2000 , 10 , "http://x");

    // addMovie first time → OK
    repo.addMovie(m);
    assert(repo.size() == 1);
    try {
        repo.addMovie(m);
        assert(false);
    } catch (const RepositoryException& e) {
        assert(std::string(e.what()) == "Movie already exists");
    }
    Movie updated("A" , "D" , 1999 , 5 , "http://y");
    repo.updateMovie(updated);
    assert(repo.findMovie("A")->getGenre() == "D");
    repo.removeMovie("A");
    assert(repo.size() == 0);

    try {
        repo.removeMovie("A");
        assert(false);
    } catch (const RepositoryException& e) {
        assert(std::string(e.what()) == "Movie does not exist");
    }
    try {
        repo.updateMovie(updated);
        assert(false);
    } catch (const RepositoryException& e) {
        assert(std::string(e.what()) == "Movie does not exist");
    }

    assert(repo.findMovie("A") == nullptr);
}




void testService() {
        std::ofstream fout("test_service.txt");
        fout << "Inception|Sci-Fi|2010|2000000|https://youtu.be/YoHD9XEInc0\n";
        fout << "Interstellar|Sci-Fi|2014|1800000|https://youtu.be/zSWdZVtXT7E\n";
        fout << "The Dark Knight|Action|2008|2500000|https://youtu.be/EXeTwQWrcwY\n";
        fout << "Avatar|Fantasy|2009|3000000|https://youtu.be/5PSNL1qE6VY\n";
        fout << "Titanic|Romance|1997|3500000|https://youtu.be/kVrqfYjkTdQ\n";
        fout << "The Matrix|Sci-Fi|1999|2200000|https://youtu.be/vKQi3bBA1y8\n";
        fout << "Gladiator|Action|2000|1500000|https://youtu.be/owK1qxDselE\n";
        fout << "Joker|Drama|2019|2100000|https://youtu.be/zAGVQLHvwOY\n";
        fout << "Pulp Fiction|Crime|1994|2400000|https://youtu.be/s7EdQ4FqbhY\n";
        fout << "Forrest Gump|Drama|1994|2600000|https://youtu.be/bLvqoHBptjg\n";

    fout.close();
    MovieRepository repo("test_service.txt");
    MovieService service(repo);


    assert(repo.size() == 10);
    assert(service.findMovie("Inception") != nullptr);
    assert(service.findMovie("Interstellar") != nullptr);
    assert(service.findMovie("The Dark Knight") != nullptr);
    assert(service.findMovie("Avatar") != nullptr);
    assert(service.findMovie("Titanic") != nullptr);
    assert(service.findMovie("The Matrix") != nullptr);
    assert(service.findMovie("Gladiator") != nullptr);
    assert(service.findMovie("Joker") != nullptr);
    assert(service.findMovie("Pulp Fiction") != nullptr);
    assert(service.findMovie("Forrest Gump") != nullptr);

    service.addMovie("LikedMovie", "G", 2000, 10, "http://x");
    Movie liked("LikedMovie", "G", 2000, 10, "http://x");
    service.addToWatchList(liked);
    int oldLikesLM = service.findMovie("LikedMovie")->getLikes();
    assert(service.removeFromWatchList("LikedMovie", true));
    assert(service.findMovie("LikedMovie")->getLikes() == oldLikesLM + 1);

    assert(service.addMovie("A", "G", 2000, 1, "http://x") == "OK");
    assert(service.addMovie("A", "G", 2000, 1, "http://x") == "Movie already exists");

    assert(service.addMovie("", "G", 2000, 1, "http://x") == "Title cannot be empty");
    assert(service.addMovie("B", "", 2000, 1, "http://x") == "Genre cannot be empty");
    assert(service.addMovie("C", "G", 1800, 1, "http://x") == "Year should be between 1900 and 2100");
    assert(service.addMovie("D", "G", 2000, -1, "http://x") == "Likes should be non-negative");
    assert(service.addMovie("E", "G", 2000, 1, "") == "Trailer cannot be empty");

    assert(service.deleteMovie("A"));
    assert(!service.deleteMovie("A"));

    service.addMovie("B", "G", 2000, 1, "http://x");
    assert(service.updateMovie("B", "Drama", 1999, 5, "http://y"));
    assert(service.findMovie("B")->getGenre() == "Drama");

    assert(!service.updateMovie("B", "", 1999, 5, "http://y"));
    assert(!service.updateMovie("B", "G", 1800, 5, "http://y"));
    assert(!service.updateMovie("ZZZ", "G", 2000, 1, "http://x"));

    assert(service.findMovie("B") != nullptr);
    assert(service.findMovie("ZZZ") == nullptr);

    auto& allMovies = service.getAllMovies();
    assert(!allMovies.empty());

    auto all = service.getMovieByGenre("");
    assert(all.size() == repo.size());

    auto drama = service.getMovieByGenre("Drama");
    assert(!drama.empty());

    auto none = service.getMovieByGenre("NonexistentGenre");
    assert(none.empty());

    auto multi = service.getMovieByGenre("Drama");
    assert(multi.size() >= 2);

    Movie m("C", "G", 2000, 1, "http://x");
    assert(service.addToWatchList(m));
    assert(!service.addToWatchList(m));

    assert(service.isInWatchList(m));
    Movie m2("NotInList", "G", 2000, 1, "http://x");
    assert(!service.isInWatchList(m2));

    auto& wl = service.getWatchList();
    assert(wl.size() == 1);

    int oldLikes = service.findMovie("C") ? service.findMovie("C")->getLikes() : -1;
    assert(service.removeFromWatchList("C", true));
    if (oldLikes != -1)
        assert(service.findMovie("C")->getLikes() == oldLikes + 1);

    assert(!service.removeFromWatchList("C", true));
    assert(!service.removeFromWatchList("NotHere", false));

    Movie m3("X", "G", 2000, 1, "http://x");
    service.addToWatchList(m3);
    assert(service.removeFromWatchList("X", false));

    Movie m4("Y", "G", 2000, 1, "http://x");
    Movie m5("Z", "G", 2000, 1, "http://x");
    service.addToWatchList(m4);
    service.addToWatchList(m5);
    assert(service.removeFromWatchList("Z", false));
}


void run_all_tests() {
    testMovie();
    testRepository();
    testService();
    std::cout << "All tests passed" << std::endl;
}