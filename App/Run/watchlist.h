#pragma once
#include "domain.h"
#include <vector>
#include <string>

class WatchList {
public:
    virtual ~WatchList() = default;

    virtual void save(const std::vector<Movie>& movies) = 0;
    virtual void open() = 0;
};

class WatchListCSV : public WatchList {
private:
    std::string filename;

public:
    WatchListCSV(const std::string& filename);
    void save(const std::vector<Movie>& movies) override;
    void open() override;
};

class WatchListHTML : public WatchList {
private:
    std::string filename;

public:
    WatchListHTML(const std::string& filename);
    void save(const std::vector<Movie>& movies) override;
    void open() override;
};
