// Zachary Schupbach
// 11/16/25
// This code reads the log file and outputs to cli
// Run on the server which gets 

#include "logRead.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::string makeFileName(std::string noDirFileName){
    std::string logDirectory = "./logs/";
    logDirectory.append(noDirFileName);
    std::cout << logDirectory << " :After file change\n";
    return logDirectory;
}
void runLogPager(std::string filename, int linesPerPage)
{
    
    std::cout << "test print\n";
    std::ifstream file(makeFileName(filename));
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string buf;
    while (std::getline(file, buf))
        lines.push_back(buf);

    file.close();

    size_t index = 0;
    size_t total = lines.size();

    std::cout << "----- Log Viewer -----\n";
    std::cout << "Press ENTER for next page, 'q' then ENTER to quit.\n\n";

    while (index < total)
    {
        size_t end = index + linesPerPage;
        if (end > total) end = total;

        // print the page
        for (size_t i = index; i < end; i++)
            std::cout << lines[i] << "\n";

        index = end;

        if (index >= total) {
            std::cout << "\n<END OF FILE>\n";
            break;
        }

        std::cout << "\n--- Press ENTER for next page (q to quit) ---";

        std::string input;
        std::getline(std::cin, input);

        if (!input.empty() && (input[0] == 'q' || input[0] == 'Q'))
            break;

        std::cout << "\n";
    }
}
