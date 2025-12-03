// Zachary Schupbach
// 11/16/25
// Sanitizes user input and authenticates user accounts and grants access 
// after getting commands if they can append or logRead or logAppend then does that

#include "serverInputHandler.h"
#include "logAppend.h"
#include "logRead.h"
#include <string>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <unistd.h>
#include <termios.h>
#include <fstream>

// This should be handled with encryption and gitignore
const std::string usernames[] = {"admin", "user1", "user2"};
const std::string passwords[] = {"admin123", "user1pass", "user2pass"};
const int NUM_USERS = 3;
const int MAX_ATTEMPTS = 5;


int main() {
//int serverInputHandler(){
    bool validInput = false;
    bool validFileName = false;
    bool validCommand = false;
    int renditions = 10;
    int sleepTime = 1;
    std::string fileName = "logFile.txt";
    std::string input;

    int validLogin; // 0 for no access, 4 for read, 7 for read and write access
    validLogin = getUserCredentials();
    if (validLogin==0) {
        std::cout << "How you get here? you really have no access\n";
    }else if(validLogin==4){
        std::cout << "You logged into read some logs!!\n";
        validInput= false;
        while(!validInput){
            std::cout << "Enter 'readLog' in order to read the logs: ";
            input = getLineButSecure(validInput);
            if (input == "readLog") {
                runLogPager();
                std::cout << "Conclusion of log file\n";
            } else {
                std::cout << "You dont have permissions for that buddy\n";
                validInput = false;
            }
        }
    }
    else if(validLogin==7) 
    {
        validInput = false;
        std::cout << "You can read or write whatever you want...\n";
        while(!validCommand){
                    std::cout << "Do you want to choose the name of your logFile? 'Y' or 'N': ";
                    input = getLineButSecure(validCommand);
                    if (input == "y" || input == "Y"){
                        while(!validFileName){   
                            std::cout << "What do you want the file name to be: ";
                            input = getLineButSecure(validFileName);    // !#!#!#!#!!#!# i dont trust this one bit for users for some reason #!#!#!#!#!#!#!#!
                            if(input.find("..") == std::string::npos) { //  elimnates moving directories and locks them to log directory only 
                                std::cout << "Dont try to move directories yo\n" ;
                            } else if(validFileName) {
                                fileName = input;
                            }
                        }
                    }
                }
        while(!validInput){
            
            std::cout << "Enter readLog or appendLog commands: " ;
            input = getLineButSecure(validInput);
            if (input == "readLog"){
                runLogPager();
                std::cout << "Conclusion of log file\n";
            } else if (input == "appendLog"){
                validCommand = false;
                while(!validCommand){
                    std::cout << "Do you want change the log output settings? (Y) or (N): ";
                    input = getLineButSecure(validCommand);
                    validInput = false;
                    if(input == "y" || input == "Y"){
                        while(!validInput){
                            std::cout<< "How long should it wait between log entries: " ;
                            input = getLineButSecure(validInput);
                            try{
                                sleepTime = std::stoi(input);
                            }catch (const std::invalid_argument& e) {
                                std::cerr << "thats not a number" << e.what() << std::endl;
                                validInput=false;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "that too big " << e.what() << std::endl;
                                validInput=false;
                            }
                        }
                        validInput = false;
                        while(!validInput){
                            std::cout<< "How many entries should it record: " ;
                            input = getLineButSecure(validInput);
                            try{
                                renditions = std::stoi(input);
                            }catch (const std::invalid_argument& e) {
                                std::cerr << "thats not a number" << e.what() << std::endl;
                                validInput=false;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "that too big " << e.what() << std::endl;
                                validInput=false;
                            }
                        }
                    }
                }
                makeLog(sleepTime,renditions);
                std::cout << "log has finished making, everyone left\n";

            } else {
                std::cout << "no shot you got here, good job\n";
            }
        }
    }
    return 0;
}

// ###################### Stores in gitignore  ################################
// Gets user login info
std::unordered_map<std::string, std::string> load_env(const std::string& filename = "passwords.env") {  
    std::unordered_map<std::string, std::string> env_vars;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            size_t equals_pos = line.find('=');
            if (equals_pos != std::string::npos) {
                std::string key = line.substr(0, equals_pos);
                std::string value = line.substr(equals_pos + 1);
                env_vars[key] = value;
            }
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open .env file: " << filename << std::endl;
    }
    return env_vars;
}

// ################### Input Validatino and buffer overflow ###########################
std::string getLineButSecure(bool& validInput){
    std::string input;
    std::getline(std::cin, input);
    if (input.size() > 26) {
        validInput = false;
        std::cout << "That input is too long yo\n";
        return "";
    }
    
    for (int i = 0; i < int(input.size()); i++) {  
        if (!((input[i] >= 'A' && input[i] <= 'Z') ||
              (input[i] >= 'a' && input[i] <= 'z') ||
              (input[i] >= '0' && input[i] <= '9') ||
              input[i] == '!' || input[i] == '@' || input[i] == '#' ||
              input[i] == '$' || input[i] == '&' || input[i] == '*' || 
              input[i] == ' ' || input[i] == '.'))
        {
            validInput = false;
            std::cout << "Invalid input only alphanumeric characters and '.!@#$&* ', try again\n";
            return "";
        }
    }
    validInput = true;
    return input;
}

int getUserCredentials(){
    int failedAttempts = 0;
    bool validInput = false;
    std::string username;
    std::string password;
    // ###################### Max attempts to prevent brute force ############################
    while(failedAttempts<=5){
        while(!validInput){
            std::cout << "Give me Your Username: " ;
            username = getLineButSecure(validInput);
        } 
        validInput = false;
        while(!validInput){
            std::cout << "Give me Your password: " ;
            setStdinEcho(false);      // ################# Turns off echo for the password ###################
            password = getLineButSecure(validInput);
            setStdinEcho(true);
            
        }
        int permLevel = authenticateCredentials(username,password);     // ########## Different permission levels  ##################
        if(permLevel>0){
            std::cout << "You've authenticated into the server\n";
            return permLevel;
        }
        else {
            failedAttempts++;
            std::cout << "Invalid username or password, try again, Attempts left: " << 
                (MAX_ATTEMPTS - failedAttempts)<< "\n";
            validInput = false;
            std::cout << "Pausing for 1 sec...\n";
            sleep(1);     // ################# Pause between attempts (would do longer in production) ###################
        } 
        if(failedAttempts>=5){
            std::cout << "max failed attempts reached, good luck next time\n";
            break;
        }
    }
    return 0;
}
int authenticateCredentials(std::string user, std::string password){
    const std::unordered_map<std::string, std::string> creds = load_env("passwords.env");
    auto it = creds.find(user);
    if (it == creds.end()) {
        return 0;
    }
    if (it->second == password){
        if(user == "admin"){
            return 7;
        } else if (user == "user1" || user== "user2"){
            return 4;
        }
    }
    return 0;
}

void setStdinEcho(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) {
        tty.c_lflag |= ECHO;
    } else {
        tty.c_lflag &= ~ECHO;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// Accesses the programs with given permission level
std::string programAccess(int permission){

    return "";
}

std::string passStringToServer(){
    
    return "";
}
