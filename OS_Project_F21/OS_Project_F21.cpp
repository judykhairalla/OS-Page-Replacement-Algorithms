// OS_Project_F21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <list>
using namespace std;

int vectorSearch(vector<int>, int);
void FIFO(vector<int>, int);
void LRU(vector<int>, int);
void findFarthest(vector<int>&, vector<int>&, vector<int>&, int, int&);
void OPTIMAL(vector<int>, int);



int main()
{
    vector<int> refString;
    int elementInput;
    int frameSize;

    cout << "Input the reference string (stop = -1): \n";
    while (true){
        cin >> elementInput;
        if (elementInput == -1)
            break;
        refString.push_back(elementInput);
    }

    cout << "\nEnter the frame size: ";
    cin >> frameSize;

    //FIFO(refString, frameSize);
    //LRU(refString, frameSize);
    OPTIMAL(refString, frameSize);


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

void FIFO(vector<int> refString, int frameSize)
{
    vector<int>frames;
    queue<int>elementsOrder;
    int pageFaults = 0;
    int hits = 0;

    for (int i = 0; i < refString.size(); i++)
    {
        cout << refString[i] << "\t";
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

            for (int j = 0; j < frames.size(); j++)
                cout << frames[j] << " ";
        }
        else
        {
            hits++;
        }
        cout << endl;
    }

    cout << "Page faults = " << pageFaults << endl;
}

void LRU(vector<int> refString, int frameSize) {

    vector<int> frames;
    unordered_map<int, int> indexRefStr;
    int pageFaults = 0;
    int hits = 0;

    for (int i = 0; i < refString.size(); i++) {

        cout << refString[i] << "\t";
        int lru = INT_MAX;
        int lruIndex = 0;

        //If not found in frames
        if (vectorSearch(frames, refString[i]) == -1)
        {
            for (int j = 0; j < frames.size(); j++)
                cout << frames[j] << " ";

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
        cout << refString[i] << "\t";

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

            for (int j = 0; j < frames.size(); j++)
                cout << frames[j] << " ";

        }
        else
        {
            hits++;
        }

        cout << endl;
    }

    cout << "Page Faults: " << pageFaults;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
