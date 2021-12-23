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


void FIFO(vector<int>, int);
void LRU(vector<int>, int);
void OPTIMAL(vector<int>, int);

int vectorSearch(vector<int>, int);
void findFarthest(vector<int>&, vector<int>&, vector<int>&, int, int&);
void printFrames(int, vector<int>);

void setConsoleSize();
void printTitle();
void printRefString(vector<int> refString);
void adjustLineSpacing(int, vector<int>, int);
void set_cursor(int, int);
void printAlgorithmName(string, vector<int>);
void changeColor(int);


int lineSpacing = 4;


int main()
{
    bool inputValid = true;
    setConsoleSize();
    printTitle();
  
    // An infinite loop to keep getting back to the main menu and test new algorithms or reference strings
    while(true){
        vector<int> refString;
        int elementInput;
        int frameSize;
        int choice;
        lineSpacing = 4;
        cout << "\nMENU:\n";
        cout << "1- FIFO\n2- LRU\n3- Optimal\n4- All Algorithms\n\n";


        //Take Algorithm Choice Input until it's valid
        do{
            inputValid = true;
            cout << "Choose the desired Algorithm: ";
            cin >> choice;
            if (!cin || choice > 4) {
                cout << "Invalid Input" << endl;
                inputValid = false;
                cin.clear();
                cin.ignore();
            }
        }
        while(inputValid==false);


        //Take Reference String Input until it's valid
        do{
            inputValid = true;
            refString.clear();
            cout << "\nInput the reference string (stop = -1): \n";

            //Accepting Reference String Elements until user inputs -1
            while (true){
                cin >> elementInput;
                if (!cin) {
                    cout << "Invalid Input" << endl;
                    inputValid = false;
                    cin.clear();
                    cin.ignore();
                }

                if (elementInput == -1)
                    break;

                refString.push_back(elementInput);
            }

        } while (inputValid == false);


        //Take Frame Size input until it's valid
        do {
            inputValid = true;
            cout << "\nEnter the frame size: ";
            cin >> frameSize;

            if (!cin) {
                cout << "Invalid Input" << endl;
                inputValid = false;
                cin.clear();
                cin.ignore();
            }

        } while (inputValid == false);
      

        system("cls");

        // According to the user's choice, apply the desired algorithm(s)
        // Each time, print the algorithm's name followed by the reference string 
        // before printing the frames at their different states
        switch (choice) {

        // FIFO 
        case 1:
            printAlgorithmName("FIFO", refString);
            printRefString(refString);
            FIFO(refString, frameSize);
            break;

        // LRU 
        case 2:
            printAlgorithmName("LRU", refString);
            printRefString(refString);
            LRU(refString, frameSize);
            break;

        // Optimal 
        case 3:
            printAlgorithmName("OPTIMAL", refString);
            printRefString(refString);
            OPTIMAL(refString, frameSize);
            break;

        // All Algorithms
        case 4:
            // FIFO
            printAlgorithmName("FIFO", refString);
            printRefString(refString);
            FIFO(refString, frameSize);
            
            // LRU, Adjust the line spacing to start printing below the FIFO algorithm
            adjustLineSpacing(frameSize, refString, 1);
            printAlgorithmName("LRU", refString);
            printRefString(refString);
            lineSpacing += 4;
            LRU(refString, frameSize);

            // Optimal, Adjust the line spacing to start printing below the LRU algorithm
            adjustLineSpacing(frameSize, refString, 2);
            printAlgorithmName("OPTIMAL", refString);
            printRefString(refString);
            lineSpacing += 4;
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

//---------------------------------- PAGE REPLACEMENT ALGORITHMS ----------------------------------
void FIFO(vector<int> refString, int frameSize)
{
    vector<int>frames;
    queue<int>elementsOrder;
    int pageFaults = 0;
    int hits = 0;

    //Iterate through each element in Reference String
    for (int i = 0; i < refString.size(); i++)
    {
        // Check if the current refString Element is in Frames, if not then a Page fault occurs
        if (vectorSearch(frames, refString[i]) == -1)
        {
            pageFaults++;

            // If Frames are full, we will replace a frame
            if (frames.size() == frameSize)
            {
                int toPop = elementsOrder.front();
                frames[vectorSearch(frames, toPop)] = refString[i]; //get the index of the frame to be replaced and update its value
                elementsOrder.pop(); 
                elementsOrder.push(refString[i]);
            }
            //If Frames aren't full then push the current element into frames and elementsOrder queue
            else
            {
                frames.push_back(refString[i]);
                elementsOrder.push(refString[i]);
            }
            
            printFrames(i, frames);
        }
        else
        {
            hits++;
        }
    }

    changeColor(12);
    cout << "\n\nPage faults = " << pageFaults;
    changeColor(10);
    cout << "\t \tHits = " << hits << endl;
    changeColor(15);
}


void LRU(vector<int> refString, int frameSize) {
    vector<int> frames;
    unordered_map<int, int> indexRefStr;
    int pageFaults = 0;
    int hits = 0;

    // Traverse the reference string, find the Least Recently used element to substitute with the current one 
    for (int i = 0; i < refString.size(); i++) {
        int lru = INT_MAX;
        int lruIndex = 0;

        //If not found in frames
        if (vectorSearch(frames, refString[i]) == -1)
        {
            pageFaults++;
            //If frames are full, replace frame
            if (frames.size() == frameSize) {
                //check LRU element to be replaced
                for (int j = 0; j < frames.size(); j++) {
                    //We traverse the frames and check which frames hasn't been called 
                    //For the longest time using its index in the Reference String (indexRefStr)
                    //We store the refString index of frames[j] if it's less than the current lru value.
                    //After the loop ends, we will have the LRU index 
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
            printFrames(i,frames);
        }
        else {
            hits++;
        }
        indexRefStr[refString[i]] = i;
    }
    
    changeColor(12);
    cout << "\n\nPage faults = " << pageFaults;
    changeColor(10);
    cout << "\t \tHits = " << hits << endl;
    changeColor(15);
}



void OPTIMAL(vector<int> refString, int frameSize)
{
    vector<int> frames;
    list<int> framesOrder;
    int pageFaults = 0;
    int hits = 0;

    // Traverse the reference string
    for (int i = 0; i < refString.size(); i++)
    {
        // If the current element doesn't exist in the frames, a page fault occurs
        if (vectorSearch(frames, refString[i]) == -1)
        {
            pageFaults++;

            // if the frames are full, find the farthest element to substitute with the current one
            if (frames.size() == frameSize)
            {
                vector<int> noFutureVisits;
                int index = -1;
                findFarthest(frames, refString, noFutureVisits, i, index);

                // If there are elements that won't be visited in the future, apply the FIFO algorithm
                // to select the element to be removed
                if (!noFutureVisits.empty())
                {
                    // A linked list (framesOrder) maintains the order of input pages in the frames.
                    // The front page is the page that was first inserted in the frames,
                    // if it was also never visited, then it is the page to be removed.
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
                    // Remove the farthest page and replace it with the new page from the reference string
                    framesOrder.remove(frames[index]);
                    frames[index] = refString[i];
                }
            }
            else
            {
                // If there is room for more pages, insert the current page to the frames
                frames.push_back(refString[i]);
            }

            framesOrder.push_back(refString[i]);

            printFrames(i, frames);

        }
        else
        {
            hits++;
        }

    }

    changeColor(12);
    cout << "\n\nPage faults = " << pageFaults;
    changeColor(10);
    cout << "\t \tHits = " << hits << endl;
    changeColor(15);
}


//----------------------------------------- HELPING FUNCTIONS -----------------------------------------
int vectorSearch(vector<int> vec, int key)
{
    // Iterate through the input vector
    vector<int>::iterator iterator;
    iterator = find(vec.begin(), vec.end(), key);

    // If the key element is found, return its index
    if (iterator != vec.end())
    {
        return iterator - vec.begin();
    }
    // If the key element was not found, return -1 
    else
        return -1;
}

void findFarthest(vector<int>& frames, vector<int>& refString, vector<int>& noFutureVisits, int i, int& index)
{
    int farthest = -1;
    bool visited;

    // Traverse the frames 
    for (int f = 0; f < frames.size(); f++)
    {
        visited = false;
        // If an element from the frames is repeated in the future pages,
        // check whether its occurence is the farthest and accordingly update
        // the variable "farthest" and update its index
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

        // If the element will never be visited in the future, insert it to the respective vector
        if (!visited)
        {
            noFutureVisits.push_back(frames[f]);
        }
    }
}

void printFrames(int i, vector<int> frames)
{
    // Print the frames vector after the previous frames in the x axis (x = i*8)
    // and below the previous algorithms and frames (y= j + lineSpacing)
    for (int j = 0; j < frames.size(); j++) {
        set_cursor(i * 8, j + lineSpacing);
        cout << frames[j];
    }
}

//------------------------------------------- LAYOUT FUNCTIONS -------------------------------------------
void setConsoleSize() {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1200, 800, TRUE);
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

void changeColor(int color)
{

    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);

}

void adjustLineSpacing(int frameSize, vector<int> refString, int factor)
{
    lineSpacing = (frameSize * factor) + (10 * factor);
    set_cursor(0, lineSpacing);
}

void printTitle() {
    cout << "  ____       _       ____   _____     ____    _____   ____    _          _       ____   _____   __  __   _____   _   _   _____ \n";
    cout << " |  _ \\     / \\     / ___| | ____|   |  _ \\  | ____| |  _ \\  | |        / \\     / ___| | ____| |  \\/  | | ____| | \\ | | |_   _|\n";
    cout << " | |_) |   / _ \\   | |  _  |  _|     | |_) | |  _|   | |_) | | |       / _ \\   | |     |  _|   | |\\/| | |  _|   |  \\| |   | |  \n";
    cout << " |  __/   / ___ \\  | |_| | | |___    |  _ <  | |___  |  __/  | |___   / ___ \\  | |___  | |___  | |  | | | |___  | |\\  |   | |  \n";
    cout << " |_|     /_/   \\_\\  \\____| |_____|   |_| \\_\\ |_____| |_|     |_____| /_/   \\_\\  \\____| |_____| |_|  |_| |_____| |_| \\_|   |_|  \n";
}


void printRefString(vector<int> refString)
{
    for (int i = 0; i < refString.size(); i++) {
        cout << refString[i];
        if (i != refString.size() - 1) {
            cout << setw(4);
            cout << "|";
            cout << setw(4);
        }
    }
    cout << endl;
    for (int i = 0; i < refString.size() * 8; i++) {
        cout << "_";
    }
    cout << endl;
}


void printAlgorithmName(string algo, vector<int> refString)
{
    changeColor(11);

    for (int i = 0; i < refString.size() * 8 / 2; i++)
        cout << "-";

    cout << " " << algo << " ";

    for (int i = 0; i < refString.size() * 8 / 2; i++)
        cout << "-";
    
    cout << endl;

    changeColor(15);
}



