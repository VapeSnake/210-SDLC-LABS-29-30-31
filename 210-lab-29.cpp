// COMSC-210 | Lab 29 | Noel Mier-Luna | Project Mythos
#include <iostream>
#include <random>    // for random number generation for our simulated events.
#include <map>       // To store our parties as keys.
#include <array>     // To store our lists of party members, their loot, and their active quests as values for our map.
#include <list>      // Lists are great for the insertion/deletion of party members, loot items, and quests.
#include <algorithm> // for sorting and other algorithms we might want to use on our lists. Debatable if we will use this but nice for now in case.
#include <fstream>   // for file input/output

using namespace std;

const int NUM_EVENTS = 25; // We will simulate 25 events for our parties. This can be adjusted as needed.

// Prototype functions for this program.
//  File reading function. This function will read party information from a file and populate the parties map.
void loadParties(const string &filename, vector<string> &v);

// Display function.
string displayEvent(const string &eventDescription); // This function will take an event number and return a string describing the event.
// Event functions. These can modify parties' and kingdom's variables based on the event number by reference.
void questEvent(int eventNum, map<string, array<list<string>, 3>> &parties);        // This function simulates a quest event.
void kingdomEvent(int eventNum, int &prosperity, int &safety);                      // This function simulates a kingdom event that modifies prosperity and safety.
void applyEventEffects(int eventNum, map<string, array<list<string>, 3>> &parties); // This function uses event numbers to modify parties and kingdom.
void combat(int eventNum, map<string, array<list<string>, 3>> &parties);            // This function simulates combat which can remove members and

int main()
{

    int prosperity = 50; // Initial prosperity of the kingdom.
    int safety = 50;     // Initial safety of the kingdom.

    // We can store our txt files into vectors of strings to randomly assign them to parties in the program.

    vector<string> partyNames;
    vector<string> partyMembers;
    vector<string> lootItems;
    vector<string> quests;

    /*
    Our first step is to load our world and parties from a file. Our kingdom only has two variables to worry about for now: prosperity and safety.
    By default let's set them to 50 each, and these variables will vary based on events so they will be stored with two ints.
    The parties will be stored in a map, with the key being a string representing the party name, and the value being an array of
    3 lists: one for party members (max party size of 4), one for loot (max loot size of 10), and one for active quests (max quest size of 3).
    We will begin the program with 3 parties, with 3 members, 3 loot items, and 2 quests each. We will use txt files "party_names.txt", "p_member_names.txt",
    "loot.txt", and "quests.txt" to load this information.
    */

    // --- INITIAL SET UP ---
    // -- Create and Initialize the Map with file reading --

    // Our map: key is party name (string), value is an array of 3 lists: [0]: party members, [1]: loot, [2]: active quests
    map<string, array<list<string>, 3>> parties; // Our map to store parties, their members, loot, and quests.

    // For testing, let's read one party's information from files to verify file reading is proper. SUCCESS!

    loadParties("party_names.txt", partyNames);
    loadParties("p_member_names.txt", partyMembers);
    loadParties("loot.txt", lootItems);
    loadParties("quests.txt", quests);

    for (int i = 0; i < 1; i++)
    { // We will only load one party for testing purposes. This can be adjusted to load more parties as needed.
        string name = partyNames[i];
        parties[name] = {list<string>(), list<string>(), list<string>()}; // Creates map with party names and empty lists.
    }

    auto it = parties.begin(); // Get an iterator to the first party in the map.
    if (it != parties.end())
    { // Check if the map is not empty.
        for (int j = 0; j < 3; j++)
        {                                             // Load 3 members, 3 loot items, and 2 quests for the first party.
            it->second[0].push_back(partyMembers[j]); // Add members to the party's member list.
            it->second[1].push_back(lootItems[j]);    // Add loot items to the party's loot list.
            if (j < 2)
            {                                       // Only add 2 quests.
                it->second[2].push_back(quests[j]); // Add quests to the party's quest list.
            }
        }
    }

    // After this, we can use the display function to show all parties,their info, and kingdom status to show initial state of the world.
    // displayEvent(0); -- This will display the initial state of the world before any events occur. We can use event number 0 for initial event. --

    // --- END OF INITIAL SET UP ---

    // LOOP FOR SIMULATION OF EVENTS
    /*
    Our general outline for the simulation loop is as follows:
    For each time period:
        For each party:
        Combat check: If the event is a combat event, we will call the combat function which can remove members and loot from the party based on the event number.
        Quest event: We will call the questEvent function which can add/remove quests, members, and loot.
        Kingdom event: We will call the kingdomEvent function which can modify the kingdom's prosperity and safety based on the event number.
        After processing the events for all parties, we will call the displayEvent function to show the
        current state of the world, including all parties and the kingdom's prosperity and safety.
    */

    return 0;
}
// Function to load party information from a file into a vector of strings.
void loadParties(const string &filename, vector<string> &v)
{
    ifstream inFile(filename);
    if (!inFile)
    { // Check if the file was opened successfully.
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    while (getline(inFile, line))
    {
        v.push_back(line); // Add each line from the file to the vector.
    }
    inFile.close();
}

string displayEvent(const string &eventDescription)
{
    // This function will display the current state of the world, including all parties and the kingdom's prosperity and safety.
    // For now, we will just print the event description. We can expand this later to include more detailed information about parties and kingdom status.
    cout << "Event: " << eventDescription << endl;
    return eventDescription;
}

void combat(int eventNum, map<string, array<list<string> &, 3>> &parties)
{
    /*
     This function will simulate combat based on the event number. It can remove members and loot from parties.
     if random chance triggers
       if party has members
    =        remove a member from the party's member list based on the event number (e.g., eventNum % partyMembers.size())
         displayEvent("Member was lost in combat!"); -- We can expand this later to include the member's name and the party they belonged to.
    */
}

void questEvent(int eventNum, map<string, array<list<string> &, 3>> &parties)
{
    /*

   if party has active quests:
           if success:
               remove quest
               add loot
               displayEvent("Quest completed")
           else:
               remove quest
               displayEvent("Quest failed")

       if random chance:
           add new quest
           displayEvent("New quest acquired")

    */
}

void kingdomEvent(int eventNum, int &prosperity, int &safety)
{
    /*
    This function will simulate a kingdom event that modifies prosperity and safety based on the event number.
    if eventNum % x_value == 0: -- Every x events, we have a prosperity event.
        prosperity += 10; -- Increase prosperity by some value.
        displayEvent("The kingdom's prosperity has increased!"); -- We can expand this later to include more details about the event.

    if eventNum % x_value == 0: -- Every x events, we have a safety event.
        safety -= 10; -- Decrease safety by some value.
        displayEvent("The kingdom's safety has decreased!"); -- We can expand this later to include more details about the event.

    */
}