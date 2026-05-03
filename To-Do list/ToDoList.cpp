#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class Task
{
private:
    int id;
    string title;
    bool done;
    vector<int> dependecies;

public:
    Task(int _id_, string _title_); //constructor
    void addDependency(int depId);
    void markDone();

    //getters:
    int getId() { return id; }
    string getTitle() { return title; }
    bool isDone() { return done; }
    vector<int> getDependencies() { return dependecies; }
};

class ToDoList
{
private:
    string name;
    vector<Task> Tasks;
    Task* findTask(int id);

public:
    ToDoList(string _name_) : name(_name_) {} //constructor
    void addTask(int id, string name);
    void addTaskWithDeps(int id, string title, vector<int> deps);
    void markDone(int id);
    void status(int id);
    void printAll();
    void printPending();
    void printReady();
    void removeTask(int _id);
};

void handleCommands(ToDoList& list, bool& stop);

int main()
{
    ToDoList list("default");
    bool stop = false;

    while (!stop)
    {
        handleCommands(list, stop);
    }
}

// Task methods
Task::Task(int _id_, string _title_) : id(_id_), title(_title_), done(false) {}

void Task::addDependency(int depId)
{
    dependecies.push_back(depId);
}

void Task::markDone()
{
    done = true;
}

// ToDoList method
Task* ToDoList::findTask(int id)
{
    for (auto& T : Tasks)
    {
        if (T.getId() == id)
            return &T;
    }
    return nullptr;
}

void ToDoList::addTask(int id, string name)
{
    if (findTask(id) != nullptr)
    {
        cout << "Error: task " << id << " already exists." << endl;
        return;
    }
    Task new_task(id, name);
    Tasks.push_back(new_task);
    cout << "Task " << id << " added." << endl;
}

void ToDoList::addTaskWithDeps(int id, string title, vector<int> deps)
{
    if (findTask(id) != nullptr)
    {
        cout << "Error: task " << id << " already exists." << endl;
        return;
    }

    bool notFound = false;
    for (int i = 0; i < deps.size(); i++)
    {
        if (findTask(deps[i]) == nullptr)
        {
            cout << "Error: dependency " << deps[i] << " not found." << endl;
            notFound = true;
        }
    }
    if (notFound)
        return;

    Task newDepTask(id, title);
    for (int i = 0; i < deps.size(); i++)
    {
        newDepTask.addDependency(deps[i]);
    }
    Tasks.push_back(newDepTask);
    cout << "Task " << id << " added with " << deps.size() << " deps." << endl;
    return;
}

void ToDoList::markDone(int id)
{
    Task* rightNow = findTask(id);

    if (rightNow == nullptr)
    {
        cout << "Error: task " << id << " not found." << endl;
        return;
    }

    vector<int> taskDep = rightNow->getDependencies();

    for (int i = 0; i < taskDep.size(); i++)
    {
        Task* dependentTask = findTask(taskDep[i]);

        if (dependentTask == nullptr || !dependentTask->isDone())
        {
            cout << "Cannot complete task " << id << ": prerequisites not met." << endl;
            return;
        }
    }

    rightNow->markDone();
    cout << "Task " << id << " marked as done." << endl;
}

void ToDoList::status(int id)
{
    Task* statusNow = findTask(id);
    if (statusNow == nullptr)
    {
        cout << "Error: task " << id << " not found." << endl;
        return;
    }

    cout << "Task " << id << ": " << statusNow->getTitle() << endl;
    cout << "Status: " << (statusNow->isDone() ? "done" : "pending") << endl;

    vector<int> taskDep = statusNow->getDependencies();
    sort(taskDep.begin(), taskDep.end());

    int depSize = taskDep.size();
    if (depSize != 0)
        cout << "Dependencies: ";

    for (int i = 0; i < depSize; i++)
    {
        Task* dependentTask = findTask(taskDep[i]);
        cout << dependentTask->getId() << " (" << dependentTask->getTitle() << ")";
        if (i != depSize - 1)
            cout << ", ";
        else
            cout << endl;
    }
}

// For sorting
bool compareTasksById(Task& a, Task& b)
{
    return a.getId() < b.getId();
}

void ToDoList::printAll()
{
    vector<Task> tempTask = Tasks;
    sort(tempTask.begin(), tempTask.end(), compareTasksById);

    for (int i = 0; i < tempTask.size(); i++)
    {
        cout << (tempTask[i].isDone() ? "[done]    " : "[pending] ");
        cout << tempTask[i].getId() << ": " << tempTask[i].getTitle() << endl;
    }
}

void ToDoList::printPending()
{
    vector<Task> tempTask = Tasks;
    sort(tempTask.begin(), tempTask.end(), compareTasksById);

    vector<Task> pendings;
    for (int i = 0; i < tempTask.size(); i++)
    {
        if (!tempTask[i].isDone())
            pendings.push_back(tempTask[i]);
    }

    for (int i = 0; i < pendings.size(); i++)
    {
        if (i == 0)
            cout << "Pending tasks: ";
        cout << pendings[i].getId() << " (" << pendings[i].getTitle() << ")";
        if (i != pendings.size() - 1)
            cout << ", ";
    }
    cout << endl;
}

void ToDoList::printReady()
{
    vector<int> readyIDs;
    for (auto& T : Tasks)
    {
        if (T.isDone())
            continue;

        bool allDone = true;
        for (int depId : T.getDependencies())
        {
            Task* depTask = findTask(depId);
            if (depTask == nullptr || !depTask->isDone())
            {
                allDone = false;
                break;
            }
        }
        if (allDone)
            readyIDs.push_back(T.getId());
    }

    if (readyIDs.empty())
    {
        cout << endl;
        return;
    }

    sort(readyIDs.begin(), readyIDs.end());

    cout << "Ready tasks: ";
    for (int id : readyIDs)
    {
        cout << id << ' ';
    }
    cout << endl;
}

void ToDoList::removeTask(int _id)
{
    Task* willRemove = findTask(_id);
    if (willRemove == nullptr)
    {
        cout << "Error: task " << _id << " not found." << endl;
        return;
    }

    for (auto& T : Tasks)
    {
        for (int dependentId : T.getDependencies())
        {
            if (dependentId == _id)
            {
                cout << "Cannot remove task " << _id << ": other tasks depend on it." << endl;
                return;
            }
        }
    }

    for (int i = 0; i < Tasks.size(); i++)
    {
        if (Tasks[i].getId() == _id)
        {
            Tasks.erase(Tasks.begin() + i);
            cout << "Task " << _id << " removed." << endl;
            return;
        }
    }
}

void handleCommands(ToDoList& list, bool& stop)
{
    string line;
    getline(cin, line);

    if (line.empty()) return;

    stringstream ss(line);
    string command;
    ss >> command;

    if (command == "EXIT")
    {
        stop = true;
        return;
    }
    else if (command == "NEW_LIST")
    {
        string name;
        ss >> name;
        list = ToDoList(name);
        cout << "List '" << name << "' created." << endl;
    }
    else if (command == "ADD")
    {
        int id;
        string title;
        ss >> id >> title;
        list.addTask(id, title);
    }
    else if (command == "ADD_DEP")
    {
        vector<int> deps;
        int id;
        string title;
        ss >> id >> title;
        int depID;
        while (ss >> depID)
        {
            deps.push_back(depID);
        }
        list.addTaskWithDeps(id, title, deps);
    }
    else if (command == "DONE")
    {
        int id;
        ss >> id;
        list.markDone(id);
    }
    else if (command == "STATUS")
    {
        int id;
        ss >> id;
        list.status(id);
    }
    else if (command == "PRINT_ALL")
    {
        list.printAll();
    }
    else if (command == "PENDING")
    {
        list.printPending();
    }
    else if (command == "READY")
    {
        list.printReady();
    }
    else if (command == "REMOVE")
    {
        int id;
        ss >> id;
        list.removeTask(id);
    }
}
