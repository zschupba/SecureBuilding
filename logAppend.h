#ifndef LOGAPPEND_HPP
#define LOGAPPEND_HPP

#include <string>
#include <vector>
#include <unordered_map>



std::vector<std::string> getAdjacentRooms(const std::string& room);

std::string getTimestamp();


void movePeople(std::unordered_map<std::string, std::string>& personRoom);

void writeSnapshot(std::ofstream& log,
                   const std::unordered_map<std::string, std::string>& personRoom);

void makeLog(int sleepTime =1, int renditions = 10, std::string filePath = "logFile.txt"); // main function

#endif