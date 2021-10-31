#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <string>
#include <string.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <windows.h>
#include <winuser.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;
const int EXIT_NUM = 0;

/*
    TODO:
    - Have users create tasks
    - List class
        -Order of tasks
        -Displays tasks for the day
        -Displays date
    - Task classes
        -Timer
          -Does it have a timer? 0 - no, 1- ye
        -Restrictions
          -Tells user that they're on restricted app

      1:35p
      TODO:
        Display the tasks under a specified list
        Let user choose what list they want to create the task under
        Debug timer
        implement timer into task


*/
vector<string> blacklistedPrograms = {};



// Class of a "Task"
class Task{
public:
    Task(){
        taskName = "Empty";
        taskDescription = "Empty";
    }
    Task(string name, string desc){
        taskName = name;
        taskDescription = desc;
    }
    void isTimed(bool timedBool)
    {
        timed = timedBool;
        if(timed)
        {
            vector<int> timeDue;
            cout << "Enter the time in which your task is due (Enter in this order: Day> Month> Year> Hour> Minute: " << endl;
            int day;
            cin >> day;
            cin.clear();
            cin.ignore();
            int month;
            cin >> month;
            cin.clear();
            cin.ignore();
            int year;
            cin >> year;
            cin.clear();
            cin.ignore();
            int hour;
            cin >> hour;
            cin.clear();
            cin.ignore();
            int minute;
            cin >> minute;
            cin.clear();
            cin.ignore();

            timeDue.push_back(day);
            timeDue.push_back(month);
            timeDue.push_back(year);
            timeDue.push_back(hour);
            timeDue.push_back(minute);

            getTimeDue(timeDue);

        }

    }
    void getTimeDue(vector<int> timeDue)
    {
        //DAY MONTH YEAR HR MIN AM/PM
        timeUntilDue += timeDue[0]*86400; //Day
        timeUntilDue += timeDue[1]*2629800; //Month
        timeUntilDue += timeDue[2]*31557600; //Year
        timeUntilDue += timeDue[3]*3600; //Hour
        timeUntilDue += timeDue[4]*60; //Min
    }



    void getRemainingTime()
    {
        remainingTime = time(NULL);
        remainingTime = timeUntilDue - remainingTime;
        int year = remainingTime / 31557600;
        remainingTime -= 31557600*year;
        int month = remainingTime / 2629800;
        remainingTime -= 2629800*month;
        int day = remainingTime / 86400;
        remainingTime -= 86400*day;
        int hour = remainingTime / 3600;
        remainingTime -= 3600*hour;
        int minutes = remainingTime / 60;
        remainingTime -= 60*hour;
        cout << "Years:" << year << " Months:" << month << " Days:" << day << " Hours:" << hour << " Minutes:" << minutes << endl;
    }

    // Returns Tasks Name
    string getTaskName(){
        return taskName;
    }

    // Returns Task Info
    string getTaskInfo(){
        return taskDescription;
    }

    // Returns Task's Info
    bool ifTimed(){
        return timed;
    }

    void printTask(){
        cout << "Task name: " << taskName << endl;
        cout << "Task info: " << taskDescription << endl;
        if(timed){
            cout << "This is a timed task" << endl;
            getRemainingTime();
        }
        else
            cout << "This is not a timed task" << endl;
    }

    // Private Member Variables of Tasks
private:
    // Class name
    string taskName;
    // Info about task
    string taskDescription;
    // Is timed
    bool timed = false;
    // number of seconds to complete task
    time_t remainingTime;
    time_t timeUntilDue = time(NULL);

};


// List Class holds all Tasks, also can preform other Task Functions
class List{

public:

    // Default Constructor for List
    List(string title){
        // it gonna be a lil messy
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string month = to_string(1 + ltm->tm_mon);
        string day = to_string(ltm->tm_mday);
        // 10/30 -> title
        titleDate = month + '/' + day;
        listTitle = title;

    }

    void removeTask(string taskName)
    {
        for(unsigned int j = 0; j < allTasks.size(); j++){
            if(allTasks.at(j).getTaskName() == taskName){
                allTasks.erase(allTasks.begin() + j);
                cout << "Task: " << taskName << " has been removed." << endl;
                return;
            }
        }

        cout << "Unable to find the task in: " << listTitle << endl;
    }

    // Prints listTitle
    void getName(){
        cout << "List: "<< listTitle<<endl;
    }

    // Returns the string of ListTitle
    string returnListTitle(){
        return listTitle;
    }

    // Prints all the tasks in the list
    void printTasks(){

        // Print all tasks
        for(unsigned int i = 0; i < allTasks.size(); i++){
            cout << "Task " << i << " :" << allTasks.at(i).getTaskName();
            cout << "\n\t" << allTasks.at(i).getTaskInfo() << "\n";

            if( allTasks.at(i).ifTimed()== true){
                cout << "\tThis task is timed\n";
            }

            else{
                cout << "\tThis task is NOT timed\n";
            }


        }

    }

    // Returns if there is an active task
    bool isActiveTask(){
        if(activeTask)
            return true;
        else
            return false;
    }

    // Sets Active Task
    void setActiveTask(string taskName){

        // Go through all tasks
        for(unsigned int i = 0; i < allTasks.size(); i++){

            // First task name that matches
            if (taskName == allTasks.at(i).getTaskName()){
                activeTask = new Task(allTasks.at(i).getTaskName(),allTasks.at(i).getTaskInfo());
                return;
            }

        }

        // Display error saying the active task could not be found
        cout << "There was no task found that was called: " << taskName << "\n";
        return;

    }

    // Adds task to allTasks vector
    void addTask(Task newTask){
        allTasks.push_back(newTask);
    }

    // Removes Active Task
    void removeActiveTask(){

        // There is no active task
        if(!activeTask){
            return;
        }

        // delete pointer
        delete activeTask;
        activeTask = NULL;

        return;

    }

    void displayActiveTask(){

        if(activeTask)
            activeTask->printTask();

        else
            cout << listTitle << " has no active task\n";

    }

    Task returnTask(int location){
        if (unsigned(location)> allTasks.size()){
            cout << "return task is gonna error" << endl;
        }
        return allTasks.at(location);
    }

    // Returns the number of tasks
    int numberOfTasks(){

        int j = 0;

        for(unsigned int i = 0; i < allTasks.size(); i++){
            j++;
        }

        return j;

    }


private:
    vector <Task> allTasks;
    Task *activeTask = NULL;
    string titleDate;
    string listTitle;
    // idk what the hell this is


};

class OnTask{
public:

    // Default Constructor
    OnTask(){
        title();
    }
    // Prints Title Screen
    void title(){
        cout << "  OOOOO   N     N TTTTTTTTT   A       SSSSSS   K      K "<<endl;
        cout << " O     O  NN    N     T      A A     S         K     K  "<<endl;
        cout << "O       O N N   N     T     A   A     S        K    K   "<<endl;
        cout << "O       O N  N  N     T    AAAAAAA     SSS     KKKK    "<<endl;
        cout << "O       O N   N N     T   A       A        S   K    K  "<<endl;
        cout << " O     O  N    NN     T  A         A        S  K     K "<<endl;
        cout << "  OOOOO   N     N     T A           A SSSSSS   K      K"<< "\n\n\n" << endl;
    }

    void loadSettings()
    {
        ifstream file;
        file.open("settings.txt");

        string linecheck;
        while(getline(file, linecheck))
        {
            blacklistedPrograms.push_back(linecheck);
        }
        file.close();

    }
    void storeSettings()
    {
        ofstream file;
        file.open("settings.txt");
        for(unsigned int i = 0; i < blacklistedPrograms.size(); i++)
        {
            file << blacklistedPrograms[i] << endl;
        }
        file.close();
    }

    // Runs a loop for the "COMMANDS" that users can use

    void start(){

        string listToCheck; // Name of list to check

        // Runs menu print statement
        int userNum = mainMenu();

        // Free all active tasks, if quiting
        if(userNum == EXIT_NUM){
            for(unsigned int i = 0; i < allLists.size(); i++){
                allLists.at(i).removeActiveTask();
            }
        }

        // runs the application
        while (userNum != EXIT_NUM)
        {
            switch (userNum)
            {
                case 0:
                    break;

                case 1: {
                    // creates a task here
                    createATask();
                    break;}

                case 2: {
                    // Creates a list
                    createAList();
                    break;}

                case 3: {
                    // Display List and tasks under than
                    displayAll();
                    break;}

                case 4:{
                    // Set task to finished
                    setTaskCompleted();
                    break;}

                case 5:{
                    // Display all completed tasks
                    printCompletedTasks();
                    break;}

                case 6:{
                    // Set new active task
                    setActiveTask();
                    break;}

                case 7:
                    // Display Active Tasks
                    displayAllActiveTasks();
                    break;

                case 8:
                    // Clear Active Task
                    removeActiveTask();
                    break;

                case 10:
                    settingMenu();
                    break;

                default:
                    cout << "Invalid number" <<endl;
            }

            // Reloop to get the next number
            userNum = mainMenu();
        }

    }

    // Case #6, Set active Task
    void setActiveTask(){
        string listToCheck;
        string taskName;

        cout << "Name the list to set active task: ";
        cin.clear();
        cin.ignore();
        getline(cin,listToCheck);

        // For loop to go through all lists
        for (auto list_a = allLists.begin(); list_a != allLists.end(); list_a++) {

            // Compare each list name
            if(list_a->returnListTitle() == listToCheck){

                list_a->printTasks();

                cout << "Name the task to set Active: ";
                cin.clear();
                //cin.ignore();
                getline(cin,taskName);

                // Call function to remove speficied task name
                list_a->setActiveTask(taskName);

                return;
            } // End of if statement to check list name

        } // end of for loop togo through whole list

        cout << "The list you specified for does not exist" << endl;
    }

    // Case #8, Remove active Task
    void removeActiveTask(){
        string listToCheck;
        string taskName;

        cout << "Name the list to remove active task: ";
        cin.clear();
        cin.ignore();
        getline(cin,listToCheck);

        // For loop to go through all lists
        for (auto list_a = allLists.begin(); list_a != allLists.end(); list_a++) {

            // Compare each list name
            if(list_a->returnListTitle() == listToCheck){

                list_a->removeActiveTask();
                cout << "\nThe active task has been removed" << endl;
                return;

            } // End of if statement to check list name

        } // end of for loop togo through whole list

        cout << "The list you specified for does not exist" << endl;

    }

    // Case #7, Display All Active Tasks
    void displayAllActiveTasks(){
        for (auto list_a = allLists.begin(); list_a != allLists.end(); list_a++) {
            cout << "List: " << list_a->returnListTitle() << endl;
            list_a->displayActiveTask();
            }
        cout << "\n\n";
    }

    // Case #4, set a task to finish
    void setTaskCompleted(){
        string listToCheck;
        string taskName;
        int location = 0;

        cout << "Name the list that the task is at: ";
        cin.clear();
        cin.ignore();
        getline(cin,listToCheck);

        // For loop to go through all lists
        for (auto list_a = allLists.begin(); list_a != allLists.end(); list_a++) {

            // Compare each list name
            if(list_a->returnListTitle() == listToCheck){

                list_a->printTasks();

                cout << "Name the task to complete: ";
                cin.clear();
                getline(cin,taskName);

                // Call function to remove speficied task name
                completedTasks.addTask(list_a->returnTask(location));
                list_a->removeTask(taskName);

                return;
            } // End of if statement to check list name

            location++;

        } // end of for loop togo through whole list

        cout << "The list you specified for does not exist" << endl;
    }

    // Displays all List and their contents
    void displayAll()
    {
        // Displays the Lists and all
        for (unsigned int i = 0; i < allLists.size(); i++)
        {
            cout << i+1<<". ";
            allLists.at(i).getName();
            allLists.at(i).printTasks();
            //TODO: Display tasks in list
        }
    }

    // Creates a new List
    void createAList()
    {
        cout << "List Name" << endl;
        string listName;
        cin.clear();
        cin.ignore();
        getline(cin, listName);
        List newList(listName);
        allLists.push_back(newList);
    }

    // Creates a new task
    void createATask()
    {

        string listName;
        bool loop = false;
        int length;

        // Add Task name
        cout << "Add a task name\n";
        string name;
        cin.clear();
        cin.ignore();
        getline(cin, name);

        // Add Task description
        cout << "Add a task description\n";
        string desc;
        cin.clear();
        //cin.ignore();
        getline(cin, desc);
        Task task = Task(name, desc);

        // Ask if the task is timed
        cout << "Is this task timed? (Y/N)" << endl;
        string taskTimed;
        cin.clear();
        //cin.ignore();
        getline(cin, taskTimed);


        // Convert taskTimed to upper
        length = taskTimed.length();
        for (int x=0; x<length; x++){
            taskTimed[x] = toupper(taskTimed[x]);}

        // Check to see is the task is timed
        if(taskTimed == "Y" || taskTimed == "YES")
        {
            task.isTimed(true);
            task.getRemainingTime();
            alarm();
        }

        // Loop to Get accurate info about it is task Timed
        if (taskTimed != "Y" && taskTimed != "YES" && taskTimed != "N" && taskTimed != "NO")
        {
            loop = true;
        }

        // Loop to get accurate info on wether this is timed
        while (loop)
        {
            cin.clear();
            cout << "Is this task timed? (Y/N)" << endl;
            cin >> taskTimed;

            // Convert taskTimed to upper
            length = taskTimed.length();
            for (int x=0; x<length; x++){
                taskTimed[x] = toupper(taskTimed[x]);
            }

            if(taskTimed == "Y" || taskTimed == "YES")
            {
                task.isTimed(true);
                task.getRemainingTime();

                alarm();

                break;
            }
            else if(taskTimed == "N" || taskTimed == "NO")
            {
                alarm();
            }

            // Check to se if you need to loop
            if (taskTimed != "Y" && taskTimed != "YES" && taskTimed != "N" && taskTimed != "NO")
            {
                loop = true;
            }
            else
            {
                loop = false;
            }

        }

        // If for some reason there is 0 LISTS
        if(allLists.size() == 0){

            // Make default list, because there are no lists
            List newList("Default");
            allLists.push_back(newList);

            // At default, add task into that list
            allLists.at(0).addTask(task);

            // We are done, so return
            return;
        }


            // Else there are lists avaivable to insert
        else
        {
            while (true)
            {
                // Display Plossible Lists
                displayAll();
                cout << "\nPossible Lists, please select one:\n"<<endl;
                cin.clear();
                cin.ignore();
                cin >> listName;

                // For loop to run through all lists
                for(unsigned int i = 0; i < allLists.size(); i++)
                {
                    // If listTitle in ALLLISTS matches userInput
                    if(allLists.at(i).returnListTitle() == listName)
                    {
                        allLists.at(i).addTask(task);
                        return;
                    }
                }
                cout << "That list does not exist, please enter a current list:" << endl;
            } // End of While loop
        } // End of else statement
    }

    void alarm()
    {
        
        int numprogs = blacklistedPrograms.size();
            for(int i = 0; i < numprogs; i++)
            {
                //wstring wide_string = wstring(blacklistedPrograms[i].begin(), blacklistedPrograms[i].end());
                //const char *thing = blacklistedPrograms[i].c_str();
                cout << "closed: " << blacklistedPrograms[i] << endl;
                killProcessByName(blacklistedPrograms[i]);
            }

        
    }

    // Only displays text about the main menu
    int mainMenu()
    {
        int inputNum;
        cout << endl;
        cout << "1. Create Tasks" << endl;
        cout << "2. Create List" <<endl;
        cout << "3. Display Current Tasks" << endl;
        cout << "4. Set task to Finished" << endl;
        cout << "5. Display Finished Tasks" << endl;
        cout << "6. Set New Active Task" << endl;
        cout << "7. Display Active Task" << endl;
        cout << "8. Clear Active Task" << endl;
        cout << "10. Settings" << endl;
        cout << "0. Quit" << endl;
        cin >> inputNum;

        while (cin.fail())
        {
            cout << "Please enter a valid number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> inputNum;
        }

        cin.clear();
        cout << "\n\n";

        return inputNum;
    }

    void settingMenu(){
        // custom blacklist options
        // on/off closing applications
        // something
        int inputNum;
        cout << "1. Customize blacklist Applications" << endl;
        cout << "2. Turn on/off closing application" <<endl;
        cout <<"0. Quit" << endl;
        cin >> inputNum;

        while (cin.fail()) {
            cout << "Please enter a valid number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> inputNum;
        }
        while (inputNum != EXIT_NUM){

            switch(inputNum){
                case 1:
                    editToBlacklist();
                    break;
                case 2:
                    break;
                case 0:
                    start();
                default:
                    cout << "Not valid Option"<<endl;
            }
            cout << "1. Customize blacklist Applications" << endl;
            cout << "2. Turn on/off closing application" <<endl;
            cout <<"0. Quit" << endl;
            cin >> inputNum;

            while (cin.fail())
            {
                cout << "Please enter a valid number" << endl;
                cin.clear();
                cin.ignore(256, '\n');
                cin >> inputNum;
            }
        }
        cout <<endl<< endl;

    }

    void editToBlacklist(){
        // adding to globalblacklist
        cout << "Current items: ";
        for (unsigned int i = 0; i< blacklistedPrograms.size(); i++)
        {
            cout << blacklistedPrograms.at(i) << ", ";
        }
        cout << endl;

        // user choose remove or add
        cout << "1. Add to blacklist" <<endl;
        cout << "2. Remove to blacklist" <<endl;
        cout << "0. Back to Settings" <<endl;

        int inputNum;
        cin >> inputNum;
        while (cin.fail())
        {
            cout << "Please enter a valid number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> inputNum;
        }
        while (inputNum != EXIT_NUM)
        {
            switch(inputNum)
            {
                case 1:
                {
                    cout << "Enter Application Name (no .exe needed): ";
                    string appName;
                    cin >> appName;
                    appName.append(".exe");
                    blacklistedPrograms.push_back(appName);
                    storeSettings();

                    break;
                }
                    //string appName;
                    //cin >> appName;
                    //appName.append(".exe");
                    //const char *charName = appName.c_str();
                    //blacklistedPrograms.push_back(charName);
                    //strcat(appName, ".exe");
                    //blacklistedPrograms.push_back(appName);
                case 2:
                {
                    // if their are no black listed programs, dont do anything
                    if (blacklistedPrograms.size())
                    {
                        cout << "Enter Name of the program (no .exe): ";
                        string findProgram;
                        cin >> findProgram;
                        findProgram.append(".exe");
                        bool ifFound = false;
                        // find program
                        for (unsigned int i = 0; i < blacklistedPrograms. size(); i++)
                        {
                            if (blacklistedPrograms.at(i) == findProgram)
                            {
                                // idk if this works lol could break
                                blacklistedPrograms.erase(blacklistedPrograms.begin() + i);
                                ifFound = true;
                                storeSettings();
                            }
                        }
                        if (!ifFound){
                            cout << "Couldn't find '"<< findProgram <<"' "<<endl;
                        }
                    }
                    else
                    {
                        cout << "There are no Blacklisted programs" <<endl;
                    }
                    break;
                }

                case 0:
                    settingMenu();

                default:
                    cout << "Enter Valid Number" <<endl;

            }
            cout << "Current items: ";
            for (unsigned int i = 0; i< blacklistedPrograms.size(); i++)
            {
                cout << blacklistedPrograms.at(i) << ", ";
            }
            cout << endl;

            cout << "1. Add to blacklist" <<endl;
            cout << "2. Remove to blacklist" <<endl;
            cout << "0. Back to Settings" <<endl;
            cin >> inputNum;
            while (cin.fail())
            {
                cout << "Please enter a valid number" << endl;
                cin.clear();
                cin.ignore(256, '\n');
                cin >> inputNum;

            }
        }
    }

    // Prints the number of completed tasks
    void printCompletedTasks()
    {

        // If the number of tasks are 0, say theyre no completed tasks

        if(completedTasks.numberOfTasks() == 0)
        {
            cout << "There are no completed tasks" <<endl;
            return;
        }

        completedTasks.printTasks();
        return;
    }  // End of printCompletedTasks
    /*
    void kill(string filename) {
        // Sazhelle implementation
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes) {
            if (strcmp((const char*)pEntry.szExeFile, filename.c_str())) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
    }
    */

    void killProcessByName(string filename)
    {
        string temp = "taskkill /f /im " + filename;
        system(temp.c_str());
        /*
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes)
        {
            if (strcmp((const char*)pEntry.szExeFile, filename) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                    (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL)
                {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);*/
    }
    

private:
    vector <List> allLists; // Starts from the first date, then makes a second one from the next day and bring previous tasks to the next.
    List completedTasks = List("Completed Task"); // All completed tasks

};

int main() {

    // Idk Sazhelle messed me up
    OnTask MainList;
    //MainList.settingOptions();
    // Mainmenu returns an error code
    MainList.loadSettings();
    MainList.start();
    MainList.storeSettings();

    return 0;
}
