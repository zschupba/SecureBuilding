#ifndef LOGREAD_HPP
#define LOGREAD_HPP
#include <string>
#include <ctime>
void runLogPager(std::string filename = "logFile.txt",
                 int linesPerPage = 20);

std::string makeFileName(std::string noDirFileName);
#endif
