#include "Logger.hpp"

Logger::Logger(StateMachine *machine, Relay *relays, std::vector<PT*> *pts) {
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

    char fname [40];
    char date [10];
    char tname[8];
    
    
    // Find run no
    glob_t gstruct;
    strftime(date, 10, "%d-%m-%Y", timeinfo);
    sprintf(fname, "../test-data/%s_*", date);
    glob(fname, GLOB_ERR, NULL, &gstruct);
    
    // Generate filename
    sprintf(fname, "../test-data/%s_%02d.h5", date, (int)gstruct.gl_pathc+1);
    globfree(&gstruct);
    this->fid = H5Fcreate(fname, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    this->timetable = new FL_PacketTable(this->fid, "/T", H5T_NATIVE_INT, 1, H5P_DEFAULT);
    sprintf(tname, "/PT_%d", 1);
    this->PTtables = new FL_PacketTable(this->fid, "/PT_1", H5T_NATIVE_FLOAT, 1, H5P_DEFAULT);
    
    // for (size_t i = 0; i < this->pts->size(); i++) {
        
    //     sprintf(tname, "/PT_%d", (int)i+1);
    //     this->PTtables[i] = H5PTcreate(this->fid, tname, H5T_NATIVE_FLOAT, 1, H5P_DEFAULT);
    // }

    this->startime = std::chrono::system_clock::now();
    this->thread_obj = new std::thread(&Logger::loop,this);
    
}

void Logger::loop() {
    while(this->logging) {
        herr_t err;
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        long diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startime).count();
        err = this->timetable->AppendPacket(&diff);
        for (int i=0; auto *pt: *pts) {
           // err = H5PTappend(this->PTtables[i], 1, pt->pressure());
        }
        std::this_thread::sleep_for(std::chrono::microseconds((const int)1e6/200));
        
    }
    //err = H5PTclose(timetable);
    // for (auto *table: *PTtables) {
    //     err = H5PTclose(table);
    // }
    H5Fclose(fid);
}