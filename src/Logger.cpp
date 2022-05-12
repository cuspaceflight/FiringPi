#include "Logger.hpp"

Logger::Logger(StateMachine *machine, Relay *relays, std::vector<PT *> *pts) {
    this->machine = machine;
    this->relays = relays;
    this->pts = pts;
    this->logging = true;
    //this->nPTs = this->pts.size();

    const std::chrono::time_point<std::chrono::system_clock> now{std::chrono::system_clock::now()};
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char fname [20];
    char date [10];
    char tname[4];
    
    
    // Find run no
    glob_t gstruct;
    strftime(date, 10, "%d-%m-%Y", timeinfo);
    sprintf(fname, "../test-data/%s_*", date);
    glob(fname, GLOB_ERR, NULL, &gstruct);
    
    // Generate filename
    sprintf(fname, "../test-data/%s_%02d.h5", date, gstrcut.gl_matchc+1);
    globfree(&gstruct);
    this->fid = H5Fcreate(fname, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    this->timetable = H5PTcreate(this->fid, "T", H5T_NATIVE_LONG, 1);
    
    for (size_t i = 0; i < pts->size(); i++) {
        
        sprintf(tname, "PT_%d", i+1);
        this->PTtables[i] = H5PTcreate(fid, tname, H5T_NATIVE_FLOAT, 1);
    }

    this->startime = std::chrono::system_clock::now();
    this->thread_obj = new std::thread(&Logger::loop,this);
    
}

void Logger::loop() {
    while(this->logging) {
        herr_t err;
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        long diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startime).count();
        err = H5PTappend(this->timetable, 1, diff);
        for int i=0; auto *pt: *pts) {
            err = H5PTappend(PTtables[i], 1, pt->pressure());
        }
        std::this_thread::sleep_for(std::chrono::microseconds((const int)1e6/200));
        
    }
    err = H5PTclose(timetable)
    for (auto *table: *PTtables) {
        err = H5PTclose(table);
    }
    H5Fclose(fid);
}