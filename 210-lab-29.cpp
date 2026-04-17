// COMSC-210 | Lab 29 | Noel Mier-Luna | Project Mythos
#include <iostream>
#include <random>    // for random number generation for our simulated events.
#include <map>       // To store our parties as keys.
#include <array>     // To store our lists of party members, their loot, and their active quests as values for our map.
#include <list>      // Lists are great for the insertion/deletion of party members, loot items, and quests.
#include <algorithm> // for sorting and other algorithms we might want to use on our lists. Debatable if we will use this but nice for now in case.
#include <fstream>   // for file input/output

using namespace std;

const int NUM_EVENTS = 25; // For alpha, we will just use 3 periods to test change in our world.

// Prototype functions for this program.
//  File reading function. This function will read party information from a file and populate the parties map.
void loadParties(const string &filename, vector<string> &v);

// Display function.
string displayEvent(const int eventNum); // Function receives event number and returns string based on number.
// Event functions. These can modify parties' and kingdom's variables based on the event number by reference.
void questEvent(int eventNum, map<string, array<list<string>, 3>> &parties);        // This function simulates a quest event.
void kingdomEvent(int eventNum, int &prosperity, int &safety);                      // This function simulates a kingdom event that modifies prosperity and safety.
void applyEventEffects(int eventNum, map<string, array<list<string>, 3>> &parties); // This function uses event numbers to modify parties and kingdom.
void combat(int eventNum, map<string, array<list<string>, 3>> &parties);            // This function simulates combat which can remove members and

int main()
{

    int prosperity = 50; // Initial prosperity of the kingdom.
    int safety = 50;     // Initial safety of the kingdom.

    // We can store our txt files into vectors of strings to randomly assign them to parties in the program and to hold display info like events.

    vector<string> partyNames;
    vector<string> partyMembers;
    vector<string> lootItems;
    vector<string> quests;
    vector<string> events;

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
    loadParties("events.txt", events);

    random_device rd;  // Random number generator for assigning party members, loot, and quests randomly.
    mt19937 gen(rd()); // Random number generator.

    for (int i = 0; i < 3; i++)
    { // Load 3 parties into the map with empty lists for members, loot, and quests.
        shuffle(partyNames.begin(), partyNames.end(), gen); // Shuffle the party names to assign them randomly.
        parties[partyNames[i]] = array<list<string>, 3>(); // Initialize the array of lists for each party.
    }
// Initialize first 3 parties with 3 members, 3 loot items, and 2 quests each randomly assigned from the vectors we loaded from files.
    auto it = parties.begin(); // Iterator to loop through the parties in the map.
    // Use a nested for loop. Outer loop for moving through parties, inner loop for assigning members, loot, and quests.
    for (int partyCount = 0; partyCount < 3 && it != parties.end(); partyCount++, ++it)
    {
        // Shuffle the vectors to assign members, loot, and quests randomly to each party.
        shuffle(partyMembers.begin(), partyMembers.end(), gen);
        shuffle(lootItems.begin(), lootItems.end(), gen);
        shuffle(quests.begin(), quests.end(), gen);

        for (int count = 0; count < 3; count++)
        { // Assign 3 members, 3 loot items, and 2 quests to each party.
            it->second[0].push_back(partyMembers[count]); // Add members to the party's member list.
            it->second[1].push_back(lootItems[count]);    // Add loot to the party's loot list.
            if (count < 2)
            { // Only add 2 quests.
                it->second[2].push_back(quests[count]); // Add quests to the party's quest list.
            }
        }
    }

        cout << "Initial party setup complete. Parties, members, loot, and quests have been loaded." << endl;

        if (!parties.empty())
        { // Check if the map is not empty before trying to display.
            cout << "Displaying initial state of the world:" << endl;
            displayEvent(0, prosperity, safety, parties); // Display the initial state of the world.
        }
         else
        {
            cout << "No parties loaded. Cannot display initial state." << endl;
        }
        // Confirm our initial parties of our map are initialized.
        it = parties.begin(); // Reset the iterator to the first party in the map.
        while (it != parties.end())
        { // Loop through the parties in the map and display their information to verify proper loading.
            cout << "Party Name: " << it->first << endl; // Display the party name (key of the map).
            cout << "Members: ";
            for (const auto &member : it->second[0])
            { // Display the members of the party.
                cout << member << " ";
            }
            cout << endl;
            cout << "Loot: ";
            for (const auto &loot : it->second[1])
            { // Display the loot of the party.
                cout << loot << " ";
            }
            cout << endl;
            cout << "Quests: ";
            for (const auto &quest : it->second[2])
            { // Display the quests of the party.
                cout << quest << " ";
            }
            cout << endl
                 << endl;
            ++it; // Move to the next party in the map.
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

string displayEvent(const int eventNum, int &prosperity, int &safety, map<string, array<list<string>, 3>> &parties)
{
    // This function will display the current state of the world, including all parties and the kingdom's prosperity and safety.
    // For now, we will just print the event description. We can expand this later to include more detailed information about parties and kingdom status.
    string eventDescription;
    switch (eventNum)
    {
    case 1:
        eventDescription = "A new adventure has begun!";
        break;
    case 2:
        eventDescription = "The kingdom's garrison is fortified.";
        safety += 10; // Example of modifying safety based on an event.
        break;
    case 3:
        eventDescription = "A famine has struck the land!";
        prosperity -= 5; // Example of modifying prosperity based on an event.
        break;
    default:
        eventDescription = "An unknown event has occurred.";
    }
    cout << "Event: " << eventDescription << endl;
    cout << "Current state of the world:" << endl;
    // Here we would add code to display the current state of the world, including all parties
    // and the kingdom's prosperity and safety. This can be done by iterating through the parties map and printing out the relevant information.

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

//NOTES:
/*
Map properly read info from files and stored it in correct format.
Functions are outlined but still need to be tested and defined further.
Events are simulated based on the event number. Those numbers need to be added and assigned a string description.
Combat function needs to be defined to remove members and add loot based on the event number and random chance.
Quest event function needs to be defined to add/remove quests, members, and loot based on the event number and random chance as well 
as affect kingdom.
Kingdom event function needs to be defined to modify prosperity and safety based on the event number.
Display function needs to be defined to show the current state of the world, including all parties and the kingdom's 
prosperity and safety, as well as the event description.
*/