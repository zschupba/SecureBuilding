// Zachary Schupbach
// 11/16/25
// This code appends logs to the logFile
// 

#include "logAppend.h"
#include "logRead.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>


using namespace std;

vector<string> getAdjacentRooms(const string& room)
{

    vector<string> neighbors;
    
    if (room == "R001"){ neighbors.push_back("R002");
    }else if (room == "R002"){ neighbors.push_back("R001"); neighbors.push_back("R003");
    }else if (room == "R003"){ neighbors.push_back("R002"); neighbors.push_back("R104");
    }else if (room == "R104"){ neighbors.push_back("R003"); neighbors.push_back("R105");
    }else if (room == "R105"){ neighbors.push_back("R104"); neighbors.push_back("R106");
    }else if (room == "R106"){ neighbors.push_back("R207"); neighbors.push_back("R105");
    }else if (room == "R207"){ neighbors.push_back("R208"); neighbors.push_back("R106");
    }else if (room == "R208"){ neighbors.push_back("R209"); neighbors.push_back("R207");
    }else if (room == "R209"){ neighbors.push_back("R208");}
    // std::cout << neighbors.size() << " :neighbors size\n";
    return neighbors;
}


string getTimestamp()
{
    using namespace chrono;

    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);

    time_t timestamp_seconds = time(NULL); 
    //std::cout << "Current timestamp in seconds: " << timestamp_seconds << std::endl;


    tm localTm;
    localtime_r(&tt, &localTm);
    int hour12 = localTm.tm_hour % 12;
    if (hour12 == 0) hour12 = 12;

    string ampm = (localTm.tm_hour >= 12 ? "pm" : "am");

    int month = localTm.tm_mon + 1;
    int day   = localTm.tm_mday;
    int year2 = (localTm.tm_year + 1900) % 100;
    // std::cout << localTm.tm_year << " :year\n";

    int secondsInMin = timestamp_seconds %60;
    //int secondsInMin = timestamp_seconds - ((((localTm.tm_year-70)*365.25*24*60*60))+((localTm.tm_mon)*30*24*60*60)+ (localTm.tm_mday+3)*24*60*60 + localTm.tm_hour*60*60);

    stringstream ss;
    ss << month << "/" << day << "/" << year2 << " "
       << hour12 << ":" << setw(2) << setfill('0') << localTm.tm_min << ":" << secondsInMin
       << ampm;

    return ss.str();
}

//void makeLog() {
// int main() {
//     makeLog();
// }
void makeLog(int sleepTime, int renditions, std::string fileName){
    unordered_map<string, vector<string>> roomMap;
    unordered_map<string,string> personRoom;
    //std::cout << "test\n";
    srand(time(nullptr));
    
    ofstream log(makeFileName(fileName));
    if (!log.is_open()) {
        std::cout << "didnt open yo";
        cerr << "Couldnt open logFile.txt\n";
        return;
    }

    vector<string> allRooms = {
            "R001","R002","R003","R104","R105","R106","R207","R208","R209"
        };

    vector<string> people = {
        "AH1","BK1","CJ1","CJ2","JB1","JE1",
        "JW1","MK1","RC1","RJ1","ZS1","ZS2"
    };


    for (const auto& p : people)
        personRoom[p] = "R105";  // Everyone enters into R105
    for (auto& pr : personRoom)
            roomMap[pr.second].push_back(pr.first);

    // Header
    log << "People in Gallery - ";
    for (size_t i = 0; i < people.size(); ++i) {
        log << people[i];
        if (i < people.size() - 1) log << ", ";
    }
    log << "\n";



    // Should use a while loop but im testing
    for (int t = 0; t < renditions; t++)
    {
        log << getTimestamp() << "\n";
        // Moves people to adjacent room
        roomMap.clear();
        for (auto& entry : personRoom){
            // std::cout << entry.second << ": entry second \n";
            vector<string> adjacentRooms = getAdjacentRooms(entry.second);
            if((rand()%100)>50){
                adjacentRooms.push_back(entry.second); 
            }
            if (!adjacentRooms.empty())


                //for(int i=0; i<adjacentRooms.size(); i++)
                 //   std::cout << adjacentRooms[rand() % adjacentRooms.size()] << ", " ;
                //std::cout << "\n";
                // entry.second = adjacentRooms[rand() % adjacentRooms.size()];
                //std::cout << rand() % adjacentRooms.size()<< " :adjacent room size \n" ;
                roomMap[adjacentRooms[rand() % adjacentRooms.size()]].push_back(entry.first);
                //roomMap["R106"].push_back(entry.first);
        }
        
        personRoom.clear();
        for (const auto& p : roomMap){
            for (int i=0; i < p.second.size(); ++i) {
                //std::cout << p.second[i] << " left " << p.first << "\n";
                personRoom[p.second[i]] = p.first;  
            }
        }

        
            
        std::cout << "log: " << t << " made\n";
        // Log each room in order
        for (const auto& roomNum : allRooms)
        {
            log << roomNum << ": ";

            auto it = roomMap.find(roomNum); // check if anyone is in this room
            if (it != roomMap.end()) {
                for (size_t i = 0; i < it->second.size(); ++i) {
                    log << it->second[i];
                    if (i < it->second.size() - 1) log << ", ";
                }
            }

            log << "\n";
        }
        log << "\n"; // blank line between snapshots


        log << "\n"; // break between snapshots

        sleep(sleepTime);   //Time between log entries
    }

    log.close();
}
