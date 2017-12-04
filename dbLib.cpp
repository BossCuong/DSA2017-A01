/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen, Nguyen Hoang Minh
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

void    strPrintTime(char* des, time_t& t) {
    tm *pTime = gmtime(&t);
    strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}

void loadNinjaDB(char* fName, L1List<NinjaInfo_t> &db) {
	// TODO: write code to load information from file into db
    // TODO: write code to load information from file into db
    //Skip this
    L1Item<NinjaInfo_t>* _pRun;
    L1Item<NinjaInfo_t>* _pNew;

    //Opening data file
    fstream data_file;
    data_file.open(fName);

    //Creating ninja temp,use to push to data base
    NinjaInfo_t ninja_temp;

    //If file does not exist,throw exception
    if(data_file.fail()) return;

    string line; //Use to storage each line form database
    stringstream stream_line; //Use to split line

    string string_data; //Use to storage data part in each line
    char* time_char = new char[50];
    string time_string;
    getline(data_file, line); // Skip first line
    //Reding file until end of file
    while (!data_file.eof())
    {
        //Read each line,if line is empty then skip
        getline(data_file,line);
        if(line.empty()) continue;

        //Add line to stream
        stream_line.str(line);

        //Skip do not use data
        getline(stream_line, string_data, ',');

        //Get report_time
        getline(stream_line, string_data, ',');
        stringstream ninja_timestamp(string_data);
        time_string = ninja_timestamp.str();
        strcpy(time_char,time_string.c_str());
        struct tm tm;
        strptime(time_char,"%d/%m/%Y %H:%M:%S", &tm);
        time_t time_stamp = timegm(&tm);
        ninja_temp.timestamp = time_stamp;  

        //Get ninja_tag
        getline(stream_line, string_data, ',');
        stringstream ninja_tag;
        ninja_tag << setfill('0') << setw(4) << string_data; //Format string with leading zero
        ninja_tag >> ninja_temp.id;

        //Get longitude
        getline(stream_line, string_data, ',');
        stringstream ninja_longitude(string_data);
        ninja_longitude >> ninja_temp.longitude ;

        //Get latitude
        getline(stream_line, string_data, ',');
        stringstream ninja_latitude(string_data);
        ninja_latitude >> ninja_temp.latitude;

        if(db.get_pHead() == NULL)
        {
            db._pHead = new L1Item<NinjaInfo_t>(ninja_temp);
            _pRun = db.get_pHead();
            db._pTail = _pRun;
            db.increasingSize();
        }
        else
        {
            _pNew = new L1Item<NinjaInfo_t>(ninja_temp);
            _pRun->pNext = _pNew;
            db._pTail = _pNew;
            _pRun = _pRun->pNext;
            db.increasingSize();
        }
    }
    delete[] time_char;
    //Closing file
    data_file.close();

}

bool parseNinjaInfo(char* pBuf, NinjaInfo_t& nInfo) {
    // TODO: write code to parse information from a string buffer, ignore if you don't use it
    return true;
}


void process(L1List<ninjaEvent_t>& eventList, L1List<NinjaInfo_t>& bList) {
    void*   pGData = NULL;
    initNinjaGlobalData(&pGData);

    L1Item<ninjaEvent_t>* pRunEventList = eventList.get_pHead();
    string frist_save_ninja_id;
    string last_save_ninja_id;
    if(bList.getSize() == 0)
        frist_save_ninja_id = last_save_ninja_id = "empty";
    else
    {
        frist_save_ninja_id = bList.get_pHead()->data.id;
        last_save_ninja_id = bList.get_pTail()->data.id;
    }
    
    ninjaEvent_t event;
    while (pRunEventList)
    {
        event = pRunEventList->data;
        if(!processEvent(event, bList, pGData))
        {
            cout << event.code << " is an invalid event\n";
            pRunEventList = pRunEventList->pNext;
        }
        else if (strcmp(event.code,"0") == 0)
        {
            cout << "0:";
            L1Item<ninjaEvent_t>* pRunEventTemp = eventList.get_pHead();
            while(pRunEventTemp)
            {
                cout << " " << pRunEventTemp->data.code;
                pRunEventTemp = pRunEventTemp->pNext;
            }
            cout << endl;
        }
        else if(strcmp(event.code,"1") == 0)
        {
            cout << "1: " << frist_save_ninja_id << endl;
        }
        else if(strcmp(event.code,"2") == 0)
        {
            cout << "2: " << last_save_ninja_id << endl;
        }
        pRunEventList = pRunEventList->pNext;
    }
    releaseNinjaGlobalData(pGData);
}


bool initNinjaGlobalData(void** pGData) {
    /// TODO: You should define this function if you would like to use some extra data
    /// the data should be allocated and pass the address into pGData
    return true;
}

void releaseNinjaGlobalData(void* pGData) {
    /// TODO: You should define this function if you allocated extra data at initialization stage
    /// The data pointed by pGData should be released
}


void printNinjaInfo(NinjaInfo_t& b) {
    printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}

/// This function converts decimal degrees to radians
double deg2rad(double deg) {
    return (deg * pi / 180);
}

///  This function converts radians to decimal degrees
double rad2deg(double rad) {
    return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}
