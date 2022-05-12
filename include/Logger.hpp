#pragma once

#include <thread>
#include <chrono>
#include <glob.h>
#include <vector>

#include "State.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include <H5PacketTable.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include "defines.hpp"


#define LOGGING_FREQ 200



class Logger {

private:
    
    StateMachine *machine;
    Relay *relays;
    std::vector<PT*> *pts;
    std::chrono::time_point<std::chrono::system_clock> now, startime;

    herr_t err;     /* Return value from function calls */
    hid_t fid;   /* HDF5 identifier for file */
    hid_t ptable;  // HDF5 identifier for packet table
    hsize_t count;  /* Number of records in table */
    FL_PacketTable *timetable;
    FL_PacketTable *PTtables;
    std::ofstream log_csv;
    //::vector<hid_t*> PTtables;
    

    void loop();

    


public:
    Logger(StateMachine *machine, Relay *relays, std::vector<PT*> *pts);
    bool logging;
    std::thread* thread_obj;
};
