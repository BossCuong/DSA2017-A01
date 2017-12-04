/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen, Nguyen Hoang Minh
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "eventLib.h"


/// NOTE: each event will be separated by spaces, or endline character
void loadEvents(char* fName, L1List<ninjaEvent_t> &eList) {
	//TODO
    //Opening event file
    fstream event_file;
    event_file.open(fName);

    //If file does not exist,throw exception
    if(event_file.fail()) throw new DSAException(-1,"Cant open file");

    string line; //Use to storage each line form database   
    

    string event; //Use to storage data part in each line
    //Reding file until end of file
    while (!event_file.eof())
    {
        getline(event_file,line,';');
        if(line.empty()) continue;

        stringstream stream_line(line); //Use to split line

        //Spliting all event in one line to data
        while(!stream_line.eof())
        {
            //Get event code
            getline(stream_line, event,' ');
            //Creating event temp,use to push to data base
            ninjaEvent_t event_temp(event);

            //Push event to database
            eList.push_back(event_temp);
        }
        getline(event_file,line);
    }
    //Closing file
    event_file.close();
}
