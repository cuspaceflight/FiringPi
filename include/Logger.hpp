#pragma once

#include <thread>
#include <chrono>
#include <glob.h>
#include <vector>

#include "State.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "LoadCell.hpp"
#include <H5PacketTable.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include "defines.hpp"


#define LOGGING_FREQ 200



class Logger {

private:
    
    std::shared_ptr<StateMachine> machine;
    std::shared_ptr<Relay> relays;
    std::shared_ptr<std::vector<PT*>> PTs;
    std::shared_ptr<std::vector<LoadCell *>> LCs;
    std::chrono::time_point<std::chrono::system_clock> now, starttime;

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
    Logger(std::shared_ptr<StateMachine> machine,
           std::shared_ptr<Relay> relays,
           std::shared_ptr<std::vector<PT*>> PTs,
           std::shared_ptr<std::vector<LoadCell*>> LCs
    );
    bool logging;
    std::thread* thread_obj;
};
