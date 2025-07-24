#pragma once

#include <vector>
#include <string>

struct Station {
    std::string line;
    std::string stat_id;
    std::string name;
    bool open;

    Station() : line(""), stat_id(""), name(""), open(true) {}
    Station(std::string line, std::string stat_id, std::string name, bool open) : line(line), stat_id(stat_id), name(name), open(open) {}
};

extern std::vector<Station*> stations;

void initStation();
void batchUpdateStationStatusFromCsv();
void updateStationStatusManually();
void showClosedStations();
void resetAllStationsToInitialState();
void showLineStationsInfo();
void releaseStationMemory();
