/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */
#include "eventLib.h"
#include "dbLib.h"
void count_ninja(NinjaInfo_t& ninja,L1List<string>& count)
{
    //Tempo,dont care this
    int x;
    //Tempo string,dont care this
    string ninja_id = ninja.id;
    //If we did not push this data to list,then push it
    if (!count.find(ninja_id,x))
        count.insertHead(ninja_id); 
}

void print_number_Ninja(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << 0;
        return;
    }

    //Make a list to count number of ninja
    L1List<string> count;
    //Push first element of database to count
    string first_ninja_id = nList.get_pHead()->data.id;
    count.insertHead(first_ninja_id);

    //Traverse list and count
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    while(_pRun)
    {
        count_ninja(_pRun->data,count);
        _pRun=_pRun->pNext;
    }
    //count.size is number of ninja
    cout << event.code << ": " << count.getSize();
}
////******************
void print_max_NinjaID(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": empty";
        return;
    }
    //Store max ID
    string max;
    //Use to compare ID with max,if compare > max then max = compare
    string compare;
    //Traverse list
    L1Item<NinjaInfo>* _pRun = nList.get_pHead();
    max = _pRun->data.id;
    while(_pRun)
    {
        compare = _pRun->data.id;
        if(max < compare)
            max = compare;
        _pRun=_pRun->pNext;
    }
    cout << event.code << ": " << max;
}
////
bool isNinjaExist(NinjaInfo_t ninja,L1List<NinjaInfo_t>& nList,L1Item<NinjaInfo>*& _pIndex)
{
    //Check does ninja exist in list
    return nList.find(ninja,_pIndex); //Return _pIndex by reference
}
bool find_first_moving_time(NinjaInfo_t ninja,L1List<NinjaInfo_t>& nList,L1Item<NinjaInfo_t>* &first_moving_time)
{
    L1Item<NinjaInfo_t>* ninja_cur;
    if(!isNinjaExist(ninja,nList,ninja_cur))
        return false;

    //Ninja stading point
    L1Item<NinjaInfo_t>* ninja_stand;


    //Construct a value for longitude and laitude,it is the first point
    ninja_stand = ninja_cur;

    //Distance of two position
    double distance_of_stand_cur = 0;

    //Traverse a List,begin form _pRun
    if(ninja_cur->pNext == NULL)
    {
        first_moving_time = ninja_stand;
        return first_moving_time;
    }
    ninja_cur = ninja_cur->pNext;
    distance_of_stand_cur = distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
    if (distance_of_stand_cur > (double(5)/1000))
    {
        first_moving_time = ninja_stand;
        return first_moving_time;
    }
    while(ninja_cur)
    {
        if(ninja_cur->data == ninja.id)
        {
            //Caculate distance of two position (kilometer)
            distance_of_stand_cur = distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
            //Distance greater than 5 meter
            if (distance_of_stand_cur > (double(5)/1000))
            {
                first_moving_time = ninja_cur;
                return true;
                break;
            }
        }
        ninja_cur = ninja_cur->pNext;
    }
    first_moving_time = NULL;
    return true;    
}
void print_frist_moving_time(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Take ninja ID
    NinjaInfo_t ninja(&event.code[1]);
    //Check does ninja exist,then take _pIndex first time ninja exist
    L1Item<NinjaInfo_t>* first_moving_time;

    if(find_first_moving_time(ninja,nList,first_moving_time)) //Return _pIndex is the first time ninja moving
    {
        if(first_moving_time)
        {
            char des[50];
            strPrintTime(des, first_moving_time->data.timestamp);
            cout << event.code << ": " << des;
        }
        else cout << event.code << ": empty";
    }
    else
        cout << event.code << ": " << -1;
}
////
bool find_last_stopping_time(NinjaInfo_t ninja,L1List<NinjaInfo_t>& nList,L1Item<NinjaInfo_t>*& last_stopping_time)
{
    L1Item<NinjaInfo_t>* ninja_cur;
    if(!isNinjaExist(ninja,nList,ninja_cur))
        return false;
    //Ninja stading point
    L1Item<NinjaInfo_t> *ninja_stand;
 
    //
    L1Item<NinjaInfo_t>* ninja_pre;

    //Construct a value for longitude and laitude,it is the first point
    ninja_stand = ninja_pre = ninja_cur;

    if(ninja_cur->pNext == NULL)
    {
        last_stopping_time = NULL;
        return true;
    }
    
    ninja_cur = ninja_cur->pNext;

    int cnt;
    if(distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude) <= (double(5)/1000))
        cnt = 1;
    //Distance of two position
    double distance_of_two_position = 0;

    bool change_stand = false;
    //Traverse a List,begin form _pRun
    while(ninja_cur)
    {
        if(ninja_cur->data == ninja.id)
        {
            if(!change_stand)
            {
                distance_of_two_position = distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position > (double(5) / 1000))
                    change_stand = true;
            }
            else
            {
                distance_of_two_position = distanceEarth(ninja_pre->data.latitude, ninja_pre->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position <= (double(5) / 1000))
                {
                    change_stand = false;
                    ninja_stand = ninja_pre;
                    ++cnt;
                    continue;
                }
            }
        }
        ninja_pre = ninja_cur;
        ninja_cur = ninja_cur->pNext;
    }

    if(cnt == 0)
    {
        last_stopping_time = NULL;
        return true;
    }
    last_stopping_time = ninja_stand;
    return true;
}
void print_last_stopping_time(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Take ninja ID
    NinjaInfo_t ninja(&event.code[1]);
    //Check if it exist and take index
    L1Item<NinjaInfo>* last_stopping_time = new L1Item<NinjaInfo_t>();

    //Check does ninja exist,then take _pIndex first time ninja exist
    if(find_last_stopping_time(ninja,nList,last_stopping_time)) //return _pIndex is last time ninja stop
    {
        if(last_stopping_time)
        {
            char des[50];
            strPrintTime(des, last_stopping_time->data.timestamp);
            cout << event.code << ": " << des;
        }
        else cout << event.code << ": Non-stop";
    }
    else
        cout << event.code << ": " << -1;
}
////
int get_number_stopping_time(NinjaInfo_t ninja,L1List<NinjaInfo_t>& nList)
{
    L1Item<NinjaInfo_t>* ninja_cur;
    if(!isNinjaExist(ninja,nList,ninja_cur))
        return -1;
    //Ninja stading point
    L1Item<NinjaInfo_t>* ninja_stand;

    //
    L1Item<NinjaInfo_t>* ninja_pre;

    //Construct a value for longitude and laitude,it is the first point
    ninja_stand = ninja_pre = ninja_cur;
    int cnt = 0;
    //***************************
    if(ninja_cur->pNext == NULL)
        return cnt;
    ninja_cur = ninja_cur->pNext;

    if(distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude) <= (double(5)/1000))
        cnt = 1;
    //Distance of two position
    double distance_of_two_position = 0;

    bool change_stand = false;
    //Traverse a List,begin form _pRun
    while(ninja_cur) 
    {
        if(ninja_cur->data == ninja.id)
        {
            if(!change_stand)
            {
                distance_of_two_position = distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position > (double(5) / 1000))
                    change_stand = true;
            }
            else
            {
                distance_of_two_position = distanceEarth(ninja_pre->data.latitude, ninja_pre->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position <= (double(5) / 1000))
                {
                    change_stand = false;
                    ninja_stand = ninja_pre;
                    ++cnt;
                    continue;
                }
            }
        }
        ninja_pre = ninja_cur;
        ninja_cur = ninja_cur->pNext;
    }
    return cnt;
}

void print_number_stopping_time(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Take ninja ID
    NinjaInfo_t ninja(&event.code[1]);

    //Store a number of stopping time
    int cnt = 0;

    //Check does ninja exist,then take _pIndex first time ninja exist

    cnt = get_number_stopping_time(ninja,nList); //parameter _pIndex get form isNinjaExist func
    if(cnt != -1)
    {
        cout << event.code << ": " << cnt;
    }
    else
        cout << event.code << ": " << -1;
}
////
double get_sum_of_distance(NinjaInfo_t ninja,L1List<NinjaInfo_t>& nList)
{
    //Store a sum of distance
    double sum_of_distance = 0;

    L1Item<NinjaInfo>* _pRun;
    if(!isNinjaExist(ninja,nList,_pRun))
        return -1;


    NinjaInfo_t ninja_pre;

    //Current longitude and latitude of ninja
    NinjaInfo_t ninja_cur;

    //Construct a value for longitude and laitude,it is the first point
    ninja_cur = _pRun->data;

    //Distance of two position
    double distance_of_two_position = 0;

    bool change_stand = false;
    //Traverse a List,begin form _pRun
    while(_pRun)
    {
        if(_pRun->data == ninja.id)
        {
            ninja_pre.longitude = ninja_cur.longitude;
            ninja_pre.latitude  = ninja_cur.latitude;

            ninja_cur.longitude = _pRun->data.longitude;
            ninja_cur.latitude  = _pRun->data.latitude;


            distance_of_two_position = distanceEarth(ninja_pre.latitude, ninja_pre.longitude, ninja_cur.latitude, ninja_cur.longitude);

            sum_of_distance += distance_of_two_position;

        }
        _pRun = _pRun->pNext;
    }
    return sum_of_distance;
}
void print_sum_of_distance(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Take ninja ID
    NinjaInfo_t ninja(&event.code[1]);

    //Store a sum of distance
    double sum_of_distance;

    //Check does ninja exist,then take _pIndex first time ninja exist
    sum_of_distance = get_sum_of_distance(ninja,nList); //parameter _pIndex get form isNinjaExist func
    if(sum_of_distance != -1)
        cout << event.code << ": " << sum_of_distance;
    else
        cout << event.code << ": " << -1;
}
////

////
void print_max_distance_ID(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Store list of ninja ID (unique)
    L1List<NinjaInfo_t> ID_List;
    //Use to pass to func,dont care this
    int dont_care;

    //Traverse to first time data exist in data base
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    ID_List.push_back(_pRun->data);
    while (_pRun)
    {
        //Add ID to ID_List,if it exist in ID_List,skip it
        if(!ID_List.find(_pRun->data,dont_care))
            ID_List.insertHead(_pRun->data);

        _pRun = _pRun->pNext;
    }

    //Store maximum distance
    double max_distance = 0;
    //Temporary distance
    double temp_distance;
    //Store ninja ID have maximum distance
    string Ninja_ID_have_max_distance;

    //Traverse ID_List
    _pRun = ID_List.get_pHead();
    Ninja_ID_have_max_distance = _pRun->data.id;
    while(_pRun)
    {
        //Get distance of each ID in ID_List
        temp_distance = get_sum_of_distance(_pRun->data,nList);
        // Compare with max,if max < temp then max = temp
        if(max_distance < temp_distance)
        {
            max_distance = temp_distance;
            Ninja_ID_have_max_distance = _pRun->data.id;
        }
        _pRun = _pRun->pNext;
    }

    cout << event.code << ": " << Ninja_ID_have_max_distance;

}
time_t get_moving_time(NinjaInfo_t& ninja,L1List<NinjaInfo_t>& nList)
{
    //Store sum of moving time
    time_t sum_of_moving_time = 0;
    //Current ninja point
    L1Item<NinjaInfo_t>* ninja_cur;
    //Check does ninja exist,if exist return pos of ninja in database by ninja_cur
    if(!isNinjaExist(ninja,nList,ninja_cur))
        return -1;

    //
    L1Item<NinjaInfo_t>* ninja_pre;

    //Ninja stading point
    L1Item<NinjaInfo_t>* ninja_stand;

    //Construct a value for longitude and laitude,it is the first point
    ninja_stand = ninja_pre = ninja_cur;

    //Distance of two position
    double distance_of_two_position = 0;

    bool change_stand = false;
    //Traverse a List,begin form _pRun
    while(ninja_cur)
    {
        if(ninja_cur->data == ninja.id)
        {
            if(!change_stand)
            {
                distance_of_two_position = distanceEarth(ninja_stand->data.latitude, ninja_stand->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position > (double(5) / 1000))
                {
                    sum_of_moving_time += difftime(ninja_cur->data.timestamp,ninja_pre->data.timestamp);
                    change_stand = true;
                }
            }
            else
            {
                distance_of_two_position = distanceEarth(ninja_pre->data.latitude, ninja_pre->data.longitude, ninja_cur->data.latitude, ninja_cur->data.longitude);
                if (distance_of_two_position <= (double(5) / 1000))
                {
                    change_stand = false;
                    ninja_stand  = ninja_pre;
                    continue; //Continue to get p_pre = p_Stand,pcur = pstand->next
                }
                else sum_of_moving_time += difftime(ninja_cur->data.timestamp,ninja_pre->data.timestamp);
            }
        }
        ninja_pre = ninja_cur;
        ninja_cur = ninja_cur->pNext;
    }
    return sum_of_moving_time;
}

void print_max_moving_time(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Store list of ninja ID (unique)
    L1List<NinjaInfo_t> ID_List;
    //Use to pass to func,dont care this
    int dont_care;

    //Traverse list
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    ID_List.push_back(_pRun->data);
    while (_pRun)
    {
        //Add ID to ID_List,if it exist in ID_List,skip it
        if(!ID_List.find(_pRun->data,dont_care))
            ID_List.insertHead(_pRun->data);

        _pRun = _pRun->pNext;
    }

    //Store maximum ninja moving time
    time_t max_moving_time = 0;
    //Store temporary ninja moving time
    time_t temp_moving_time;
    //Store ninja ID have maximum moving time
    string Ninja_ID_have_max_moving_time;

    //Traverse ID_List
    _pRun = ID_List.get_pHead();
    Ninja_ID_have_max_moving_time = _pRun->data.id;
    while(_pRun)
    {
        //Get ninja moving time of each ID in ID_List
        temp_moving_time = get_moving_time(_pRun->data,nList);

        //Compare with maximum ninja moving time,if max < temp then max = temp
        if(max_moving_time < temp_moving_time)
        {
            max_moving_time = temp_moving_time;
            Ninja_ID_have_max_moving_time = _pRun->data.id;
        }
        _pRun = _pRun->pNext;
    }

    cout << event.code << ": " << Ninja_ID_have_max_moving_time;
}
////
void print_eliminated_ninja_ID(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }

    string ID(&event.code[2]);

    string max;
    string temp;
    //Traverse list
    L1Item<NinjaInfo>* ninja_cur = nList.get_pHead();
    max = ninja_cur->data.id;
    while(ninja_cur)
    {
        temp = ninja_cur->data.id;
        if(temp > max && temp < ID)
            max = temp;
        ninja_cur = ninja_cur->pNext;
    }
    if(max >= ID)
    {
        cout << event.code << ": " << -1;
    }
    else
    {
        cout << event.code << ": " << max;
        char remove_id[10];
        strcpy(remove_id,max.c_str());
        while(true)
        {
            if(nList.getSize() == 0)
            {
                return;
            }
            
            L1Item<NinjaInfo_t>* p;
            if(nList.get_pHead()->data == remove_id)
            {
                p = nList.get_pHead();
                nList._pHead = nList._pHead->pNext;
                delete p;
                nList._size--;
            }
            else 
            {
                ninja_cur = nList.get_pHead()->pNext;
                L1Item<NinjaInfo_t>* ninja_pre = ninja_cur;
                while(ninja_cur)
                {          
                     if(ninja_cur->data == remove_id)
                     {
                         p = ninja_cur;
                         ninja_pre->pNext = ninja_cur->pNext;
                         ninja_cur = ninja_cur->pNext;
                         delete p;
                         nList._size--;
                         continue;
                     }
                     ninja_pre = ninja_cur;
                     ninja_cur = ninja_cur->pNext;
                }
                return;
             }
        }
    }
}
////
time_t get_stopping_time(NinjaInfo_t& ninja,L1List<NinjaInfo_t>& nList)
{
    time_t sum_of_time = 0;
    L1Item<NinjaInfo_t>* _pRun;
    if(!isNinjaExist(ninja,nList,_pRun))
        return -1;

    //Ninja stading point
    NinjaInfo_t ninja_stand;

    //
    NinjaInfo_t ninja_pre;

    //Current longitude and latitude of ninja
    NinjaInfo_t ninja_cur;

    //Construct a value for longitude and laitude,it is the first point
    ninja_stand = ninja_cur = _pRun->data;

    //Distance of two position
    double distance_of_two_position = 0;

    bool change_stand = false;

    //Traverse a List,begin form _pRun
    while(_pRun)
    {
        if(_pRun->data == ninja.id)
        {
            ninja_pre.timestamp = ninja_cur.timestamp;
            ninja_pre.longitude = ninja_cur.longitude;
            ninja_pre.latitude  = ninja_cur.latitude;

            ninja_cur.timestamp = _pRun->data.timestamp;
            ninja_cur.longitude = _pRun->data.longitude;
            ninja_cur.latitude  = _pRun->data.latitude;

            if(!change_stand)
            {
                distance_of_two_position = distanceEarth(ninja_stand.latitude, ninja_stand.longitude, ninja_cur.latitude, ninja_cur.longitude);
                if (distance_of_two_position > (double(5) / 1000))
                {
                    change_stand = true; 
                    sum_of_time += ninja_pre.timestamp - ninja_stand.timestamp;
                }
            }
            else
            {
                distance_of_two_position = distanceEarth(ninja_pre.latitude, ninja_pre.longitude, ninja_cur.latitude, ninja_cur.longitude);
                if (distance_of_two_position <= (double(5) / 1000))
                {
                    change_stand = false;
                    ninja_stand.timestamp = ninja_pre.timestamp;
                    ninja_stand.longitude = ninja_pre.longitude;
                    ninja_stand.latitude  = ninja_pre.latitude;
                    continue;
                }
            }
        }
        _pRun = _pRun->pNext;
    }
    return sum_of_time;
}
void print_max_stopping_time_ID(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Store list of ninja ID (unique)
    L1List<NinjaInfo_t> ID_List;
    //Use to pass to func,dont care this
    int dont_care;

    //Traverse list
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    ID_List.push_back(_pRun->data);
    while (_pRun)
    {
        //Add ID to ID_List,if it exist in ID_List,skip it
        if(!ID_List.find(_pRun->data,dont_care))
            ID_List.push_back(_pRun->data);

        _pRun = _pRun->pNext;
    }

    //Store maximum ninja stopping time
    time_t max_moving_time = 0;
    //Store temporary ninja stopping time
    time_t temp_moving_time;
    //Store ninja ID have maximum stopping time
    string Ninja_ID_have_max_stopping_time;

    //Traverse ID_List
    _pRun = ID_List.get_pHead();
    Ninja_ID_have_max_stopping_time = _pRun->data.id;
    while(_pRun)
    {
        //Get ninja stopping time of each ID in ID_List
        temp_moving_time = get_stopping_time(_pRun->data,nList);
        //Compare with maximum ninja stopping time,if max < temp then max = temp
        if(max_moving_time < temp_moving_time)
        {
            max_moving_time = temp_moving_time;
            Ninja_ID_have_max_stopping_time = _pRun->data.id;
        }
        _pRun = _pRun->pNext;
    }

    cout << event.code << ": " << Ninja_ID_have_max_stopping_time;
}
////

void print_saving_ninja_form_trap_list(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    return;
}
////
bool isClosed(NinjaInfo_t a,NinjaInfo_t b)
{
    return ((a.latitude == b.latitude) && (a.longitude == b.longitude));
}
bool isNinjaLost(NinjaInfo_t& ninja,L1List<NinjaInfo_t>& nList)
{
    //List to store all ninja database have same ID
    L1List<NinjaInfo_t> check_list;
    //Insert all database have same ID to check_list
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    while (_pRun)
    {
        if(_pRun->data == ninja)
            check_list.push_back(_pRun->data);
        _pRun = _pRun->pNext;
    }

    //Never happen if just have 3 point
    if(check_list.getSize() <= 3) return false;

    //
    L1Item<NinjaInfo_t>* ninja_data_src_ptr = check_list.get_pHead();
    //
    L1Item<NinjaInfo_t>* ninja_data_temp_ptr = ninja_data_src_ptr->pNext->pNext->pNext;
    if(check_list.getSize() == 4)
        return isClosed(ninja_data_src_ptr->data, ninja_data_temp_ptr->data);

    while (ninja_data_temp_ptr->pNext)
    {
        if(isClosed(ninja_data_src_ptr->data,ninja_data_temp_ptr->data)) return true;
        ninja_data_temp_ptr = ninja_data_temp_ptr->pNext;
    }

    ninja_data_src_ptr = check_list.get_pHead()->pNext;
    while (ninja_data_src_ptr->pNext->pNext->pNext)
    {
        ninja_data_temp_ptr = ninja_data_src_ptr->pNext->pNext->pNext;
        while(ninja_data_temp_ptr)
        {
            if(isClosed(ninja_data_src_ptr->data,ninja_data_temp_ptr->data)) return true;
            ninja_data_temp_ptr = ninja_data_temp_ptr->pNext;
        }
        ninja_data_src_ptr = ninja_data_src_ptr->pNext;
    }

    return false;
}
void print_lost_ninja_list(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList)
{
    if(nList.getSize() == 0)
    {
        cout << event.code << ": " << -1;
        return;
    }
    //Store list of ninja ID (unique)
    L1List<NinjaInfo_t> ID_List;
    //Use to pass to func,dont care this
    int dont_care;

    //Traverse list
    L1Item<NinjaInfo_t>* _pRun = nList.get_pHead();
    ID_List.push_back(_pRun->data);
    while (_pRun)
    {
        //Add ID to ID_List,if it exist in ID_List,skip it
        if(!ID_List.find(_pRun->data,dont_care))
            ID_List.push_back(_pRun->data);

        _pRun = _pRun->pNext;
    }

    L1List<NinjaInfo_t> Lost_List;
    _pRun = ID_List.get_pHead();
    while (_pRun)
    {
        if(isNinjaLost(_pRun->data,nList))
            Lost_List.push_back(_pRun->data);
        _pRun = _pRun->pNext;
    }

    if(Lost_List.getSize() == 0)
        cout << event.code << ": " << -1;
    else
    {
        cout << event.code << ":";
        _pRun = Lost_List.get_pHead();
        while (_pRun) {
            cout << " " << _pRun->data.id;
            _pRun = _pRun->pNext;
        }
    }
}
bool processEvent(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList, void* pGData) {
    // TODO: Your code comes here
    string code = event.code;

    string EventCode = "0";
    string FristNinja_ID = "1";
    string LastNinja_ID = "2";
    string NumberOfNinja = "3";
    string MaxID = "4";
    string FristMovingTime = "5";
    string LastStoppingTime = "6";
    string NumberOfStoppingTime= "7";
    string SumOfDistance = "8";
    string ID_MaxSumOfDistance = "9";
    string ID_MaxTimeMoving = "10";
    string ID_EliminatedNinja = "11";
    string NarutoID = "12";
    string SaveNinja_ID ="13";
    string LostNinja_ID = "14";

    if(code.length() == 2 || code.length() == 1 || code.length() == 6 || code.length() == 18) {
        code = code.substr(0,2);
        if (code == EventCode)                return true;
        else if (code == FristNinja_ID)       return true;
        else if (code == LastNinja_ID)        return true;
        else if (code == NumberOfNinja)       print_number_Ninja(event, nList);
        else if (code == MaxID)               print_max_NinjaID(event, nList);
        else if (code == ID_MaxSumOfDistance) print_max_distance_ID(event, nList);
        else if (code == ID_MaxTimeMoving)    print_max_moving_time(event,nList);
        else if (code == ID_EliminatedNinja)  print_eliminated_ninja_ID(event,nList);
        else if (code == NarutoID)            print_max_stopping_time_ID(event,nList);
        else if (code == LostNinja_ID)        print_lost_ninja_list(event,nList);
        else if (code == SaveNinja_ID)        return true;
        else return false;
    }
    else if(code.length() == 5) {
        code = code.substr(0,1);
        if (code == FristMovingTime)          print_frist_moving_time(event, nList);
        else if (code == LastStoppingTime)    print_last_stopping_time(event, nList);
        else if (code == NumberOfStoppingTime)print_number_stopping_time(event, nList);
        else if (code == SumOfDistance)       print_sum_of_distance(event, nList);
        else return false;
    }
    else return false;
    cout << endl;
    /// NOTE: The output of the event will be printed on one line
    /// end by the endline character.
    return true;
}

