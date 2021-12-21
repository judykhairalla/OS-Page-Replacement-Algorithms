// OS_Project_F21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <list>
#include <iomanip>
#include <windows.h>
using namespace std;

int vectorSearch(vector<int>, int);
void FIFO(vector<int>, int);
void LRU(vector<int>, int);
void findFarthest(vector<int>&, vector<int>&, vector<int>&, int, int&);
void OPTIMAL(vector<int>, int);



int main()
{
    while(true){
        vector<int> refString;
        int elementInput;
        int frameSize;
        int choice;

        cout << "1- FIFO\n2- LRU\n3- Optimal\n4-Chaos\n";
        cout << "Please choose the desired Algorithm: ";
        cin >> choice;

        cout << "Input the reference string (stop = -1): \n";
        while (true){
            cin >> elementInput;
            if (elementInput == -1)
                break;
            refString.push_back(elementInput);
        }

        cout << "\nEnter the frame size: ";
        cin >> frameSize;
        system("cls");
        cout << "REFERENCE STRING: " << endl;
        for (int i = 0; i < refString.size(); i++) {
            cout << refString[i];
            if (i != refString.size() - 1){
                cout << setw(4);
                cout << "|";
                cout <<setw(4);
            }
        }
        cout << endl;
        for (int i = 0; i < refString.size() * 8; i++) {
            cout << "_";
        }
        cout << endl;
        switch (choice) {
        case 1:
            FIFO(refString, frameSize);
            break;
        case 2:
            LRU(refString, frameSize);
            break;
        case 3:
            OPTIMAL(refString, frameSize);
        case 4:
            FIFO(refString, frameSize);
            LRU(refString, frameSize);
            OPTIMAL(refString, frameSize);
            break;
        default:
            cout << "Invalid Choice";
        }

        cout << endl << endl;
        system("pause");
        system("cls");
    }
}


int vectorSearch(vector<int> vec, int key)
{
    vector<int>::iterator iterator;
    iterator = find(vec.begin(), vec.end(), key);
    if (iterator != vec.end())
    {
        return iterator - vec.begin();
    }
    else
        return -1;
}

void set_cursor(int x = 0, int y = 0)
{
    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(handle, coordinates);
}

void FIFO(vector<int> refString, int frameSize)
{
    vector<int>frames;
    queue<int>elementsOrder;
    int pageFaults = 0;
    int hits = 0;

    for (int i = 0; i < refString.size(); i++)
    {
     
        // Page fault
        if (vectorSearch(frames, refString[i]) == -1)
        {
            pageFaults++;

            // Frames are full
            if (frames.size() == frameSize)
            {
                int toPop = elementsOrder.front();
                frames[vectorSearch(frames, toPop)] = refString[i];
                elementsOrder.pop();
                elementsOrder.push(refString[i]);
            }
            else
            {
                frames.push_back(refString[i]);
                elementsOrder.push(refString[i]);
            }
           
            for (int j = 0; j < frames.size(); j++){
                    set_cursor(i*8, j+4);
                    cout  << frames[j];
                }
              }
        else
        {
            hits++;
        } 
    }  
  
    cout << endl<<endl;
    cout << "Page faults = " << pageFaults << endl;
}




void LRU(vector<int> refString, int frameSize) {

    vector<int> frames;
    unordered_map<int, int> indexRefStr;
    int pageFaults = 0;
    int hits = 0;

    for (int i = 0; i < refString.size(); i++) {
        int lru = INT_MAX;
        int lruIndex = 0;

        //If not found in frames
        if (vectorSearch(frames, refString[i]) == -1)
        {
 
            pageFaults++;
            if (frames.size() == frameSize) {
                //check LRU
                for (int j = 0; j < frames.size(); j++) {
                    if (indexRefStr[frames[j]] < lru) {
                        lru = indexRefStr[frames[j]];
                        lruIndex = j;
                    }
                }
                frames[lruIndex] = refString[i];
            }
            else {
                frames.push_back(refString[i]);
            }
            for (int j = 0; j < frames.size(); j++) {
                set_cursor(i * 8, j + 10);
                cout << frames[j];
            }
        }
        else {
            hits++;
        }
        cout << endl;
        indexRefStr[refString[i]] = i;
    }
    cout << "Page Faults: " << pageFaults;
}

void findFarthest(vector<int>& frames, vector<int>& refString, vector<int>& noFutureVisits, int i, int& index)
{
    int farthest = -1;
    bool visited;
    for (int f = 0; f < frames.size(); f++)
    {
        visited = false;
        for (int j = i + 1; j < refString.size(); j++)
        {
            if (frames[f] == refString[j])
            {
                visited = true;
                if (j > farthest)
                {
                    farthest = j;
                    index = f;
                }
                break;
            }
        }

        if (!visited)
        {
            noFutureVisits.push_back(frames[f]);
        }
    }
}

void OPTIMAL(vector<int> refString, int frameSize)
{
    vector<int> frames;
    list<int> framesOrder;
    int pageFaults = 0;

    int hits = 0;

    for (int i = 0; i < refString.size(); i++)
    {
       

        // Page fault
        if (vectorSearch(frames, refString[i]) == -1)
        {
            pageFaults++;
            if (frames.size() == frameSize)
            {
                vector<int> noFutureVisits;
                int index = -1;
                findFarthest(frames, refString, noFutureVisits, i, index);

                if (!noFutureVisits.empty())
                {
                    for (auto itr = framesOrder.begin(); itr != framesOrder.end(); itr++) {

                        if (vectorSearch(noFutureVisits, *itr) != -1)
                        {
                            frames[vectorSearch(frames, *itr)] = refString[i];
                            framesOrder.remove(*itr);
                            break;
                        }
                    }
                }
                else
                {
                    framesOrder.remove(frames[index]);
                    frames[index] = refString[i];
                }
            }
            else
            {
                frames.push_back(refString[i]);
            }

            framesOrder.push_back(refString[i]);

            for (int j = 0; j < frames.size(); j++) {
                set_cursor(i * 8, j + 16);
                cout << frames[j];
            }

        }
        else
        {
            hits++;
        }

        cout << endl;
    }

    cout << "Page Faults: " << pageFaults;
}