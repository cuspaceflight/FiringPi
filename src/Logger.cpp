#include "Logger.hpp"

Logger::Logger(
        std::shared_ptr <StateMachine> machine,
        std::shared_ptr <Relay> relays,
        std::shared_ptr <std::vector<PT *>> PTs,
        std::shared_ptr <std::vector<LoadCell *>> LCs)
        : machine(machine), relays(relays), PTs(PTs), err(), ptable(), count(), logging(true), LCs(LCs) {

    const std::chrono::time_point <std::chrono::system_clock> now{std::chrono::system_clock::now()};
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char fname[50];
    char date[14];
    char tname[8];


    // Find run no
    glob_t gstruct;
    strftime(date, 14, "%d-%m-%Y", timeinfo);
//    sprintf(fname, "/home/cusf/FiringPi/out/%s_*.h5", date);
    glob(fname, GLOB_ERR, NULL, &gstruct);

    // Generate filename
//    sprintf(fname, "/home/cusf/FiringPi/out/%s_%02d.h5", date, (unsigned int) gstruct.gl_pathc + 1);
    globfree(&gstruct);
//    this->fid = H5Fcreate(fname, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(fname, "/home/cusf/FiringPi/out/%s_%02d.csv", date, (unsigned int) gstruct.gl_pathc + 1);

    // Open files and set up HDF5 packet tables
    this->log_csv.open(fname);
    (this->log_csv) << "T (ms), P0, T0, P1 , T1, P2, T2, P3, T3, LC1, LC2, LC3\n";
//    this->timetable = new FL_PacketTable(this->fid, "/T", H5T_NATIVE_INT, 1, H5P_DEFAULT);
//    sprintf(tname, "/PT_%d", 1);
//    this->PTtables = new FL_PacketTable(this->fid, "/P_1", H5T_NATIVE_FLOAT, 1, H5P_DEFAULT);

    this->startime = std::chrono::system_clock::now();
    this->thread_obj = new std::thread(&Logger::loop, this);

}

void Logger::loop() {
    herr_t err;
    char line[300];
    float data[11];
    while (this->logging) {
        std::chrono::time_point <std::chrono::system_clock> now = std::chrono::system_clock::now();
        long diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startime).count();

        data[0] = (*PTs)[0]->pressure;
        data[1] = (*PTs)[0]->temperature;
        data[2] = (*PTs)[1]->pressure;
        data[3] = (*PTs)[1]->pressure;
        data[4] = (*PTs)[2]->pressure;
        data[5] = (*PTs)[2]->temperature;
        data[6] = (*PTs)[3]->temperature;
        data[7] = (*PTs)[3]->temperature;

        data[8] = (*LCs)[0]->weight;
        data[9] = (*LCs)[1]->weight;
        data[10] = (*LCs)[2]->weight;
//        err = this->timetable->AppendPacket(&diff);
//        err = this->PTtables->AppendPacket(&(data[0]));

        sprintf(line, "%05ld, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n",
                diff, data[0], data[1], data[2], data[3], data[4],
                data[5], data[6], data[7], data[8], data[9], data[10]);

        (this->log_csv) << line;
        std::this_thread::sleep_for(std::chrono::microseconds((const int) 1e6 / SAMPLING_FREQ));

    }
    //err = H5PTclose(timetable);
    // for (auto *table: *PTtables) {
    //     err = H5PTclose(table);
    // }
    (this->log_csv) << "End of file\n";
    this->log_csv.close();
//    err = H5Fclose(this->fid);
//    if (err < 0) {
//        fprintf(stderr, "Failed to close file.\n");
//    }
}
