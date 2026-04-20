// COMSC-210 | Lab 29 | Noel Mier-Luna | Project Mythos
#include <iostream>
#include <random>    // for random number generation for our simulated events.
#include <map>       // To store our parties as keys.
#include <array>     // To store our lists of party members, their loot, and their active quests as values for our map.
#include <vector>    // To store our party names, member names, loot items, quests, and events loaded from files for random assignment.
#include <list>      // Lists are great for the insertion/deletion of party members, loot items, and quests.
#include <algorithm> // for sorting and other algorithms we might want to use on our lists. Debatable if we will use this but nice for now in case.
#include <fstream>   // for file input/output

using namespace std;

const int TIME_PERIODS = 25;  // We will simulate 3 time periods in our world.
const int NUM_EVENTS = 5;     // For alpha, we will just have 5 events to test change in our world.
const int MAX_PARTY_SIZE = 4; // Max party size of 4 members.
const int MAX_LOOT_SIZE = 10; // Max loot size of 10 items.
const int MAX_QUEST_SIZE = 3; // Max quest size of 3 quests.
const int MAX_PARTIES = 5;    // Max number of parties in the world at a time. This will allow for new party formation and disbanding of wiped out parties.

// Global vectors so they can be accessed in functions for random assignment of party names, members, loot, and quests.
vector<string> partyNames;
vector<string> partyMembers;
vector<string> lootItems;
vector<string> quests;

// Prototype functions for this program.
//  File reading function. This function will read party information from a file and populate the parties map.
void loadParties(const string &filename, vector<string> &v);

// Display function.
string displayEvent(int eventNum, int &prosperity, int &safety, const map<string, array<list<string>, 3>> &parties); // Function receives event number and returns string based on number.
// Event functions. These can modify parties' and kingdom's variables based on the event number by reference.
string questEvent(int eventNum, const string &partyName, array<list<string>, 3> &party);      // This function simulates a quest event.
void kingdomEvent(int eventNum, int &prosperity, int &safety);                                // This function simulates a kingdom event that modifies prosperity and safety.
void applyEventEffects(int eventNum, const string &partyName, array<list<string>, 3> &party); // This function uses event numbers to modify parties and kingdom.
string combat(int eventNum, const string &partyName, array<list<string>, 3> &party);          // Can remove members and add/delete loot
int randomEvent();                                                                            // This function generates a random event number to simulate the occurrence of events in our world.

int main()
{

    int prosperity = 50; // Initial prosperity of the kingdom.
    int safety = 50;     // Initial safety of the kingdom.

    // We can store our txt files into vectors of strings to randomly assign them to parties in the program and to hold display info like events.

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
    // Not necessary yet. In future will hold event descriptions.
    /*loadParties("events.txt", events);*/

    if (partyNames.empty() || partyMembers.empty() || lootItems.empty() || quests.empty())
    {
        cerr << "Error: One or more files failed to load. Please check the file paths and contents." << endl;
        return 1; // Exit with an error code if any file failed to load.
    }

    random_device rd;  // Random number generator for event numbers.
    mt19937 gen(rd()); // Random number generator.

    for (int i = 0; i < 3; i++)
    {                                                       // Load 3 parties into the map with empty lists for members, loot, and quests.
        shuffle(partyNames.begin(), partyNames.end(), gen); // Shuffle the party names to assign them randomly.
        parties[partyNames[i]] = array<list<string>, 3>();  // Initialize the array of lists for each party.
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
        {                                                 // Assign 3 members, 3 loot items, and 2 quests to each party.
            it->second[0].push_back(partyMembers[count]); // Add members to the party's member list.
            it->second[1].push_back(lootItems[count]);    // Add loot to the party's loot list.
            if (count < 2)
            {                                           // Only add 2 quests.
                it->second[2].push_back(quests[count]); // Add quests to the party's quest list.
            }
        }
    }

    cout << "Initial party setup complete. Parties, members, loot, and quests have been loaded." << endl;

    if (!parties.empty())
    { // Check if the map is not empty before trying to display.
        cout << "---Displaying initial state of the world---\n"
             << endl;
        displayEvent(1, prosperity, safety, parties); // Display the initial state of the world.
    }
    else
    {
        cout << "No parties loaded. Cannot display initial state." << endl;
    }
    // Confirm our initial parties of our map are initialized.
    it = parties.begin(); // Reset the iterator to the first party in the map.
    while (it != parties.end())
    {                                                // Loop through the parties in the map and display their information to verify proper loading.
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

    cout << "START OF PROGRAM SIMULATION\n"
         << endl;
    for (int time = 0; time < TIME_PERIODS; time++)
    {
        cout << "Time Period " << time + 1 << ":\n"
             << endl;                                                // Display the current time period in the simulation.
        int kingdomEventNum = randomEvent();                         // Generate a random event number for the kingdom event to simulate the occurrence of an event in the kingdom.
        cout << "Kingdom Event#: " << kingdomEventNum << endl; // Display the generated event number for testing purposes.
        // Test kingdom event function by simulating the generated event.
        kingdomEvent(kingdomEventNum, prosperity, safety);
        // Test combat and quest event functions by simulating the generated event.

        for (auto it = parties.begin(); it != parties.end();)
        {
            const string &partyName = it->first;
            auto &partyData = it->second;

            int partyEvent = randomEvent();

            applyEventEffects(partyEvent, partyName, partyData);

            // Disband check moved to main loop to ensure it is checked after all events have been applied to the party.
            if (partyData[0].empty())
            {
                cout << "[Disbanded] " << partyName
                     << " has been wiped out and disbanded.\n";
                it = parties.erase(it);
            }
            else
            {
                ++it;
            }
        }
        // New party formation check. 20% chance for a new party to form each time period, but only if we are below the max party limit.
        if (parties.size() < MAX_PARTIES && rand() % 5 == 0)
        {
            string newPartyName;
            do {
                newPartyName = partyNames[rand() % partyNames.size()]; // Randomly select a new party name from the partyNames vector.
            } while (parties.find(newPartyName) != parties.end());     // Ensure the new party name is not already in use to avoid duplicates.
            if (parties.find(newPartyName) == parties.end())
            { // Check if the new party name is not already in use to avoid duplicates.
                parties[newPartyName] = array<list<string>, 3>(); // Add a new party to the map with empty lists for members, loot, and quests.
                parties[newPartyName][0].push_back(partyMembers[rand() % partyMembers.size()]); // Add a random member to the new party.
                parties[newPartyName][0].push_back(partyMembers[rand() % partyMembers.size()]); // Add a second random member to the new party.
                parties[newPartyName][2].push_back(quests[rand() % quests.size()]); // Add a random quest to the new party.
                cout << "[New Party] " << newPartyName << " has formed!\n";
            }
        }
        // Display the state of the world after the kingdom event to verify the changes.
        displayEvent(kingdomEventNum, prosperity, safety, parties);
    } // End of simulation loop.

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

string displayEvent(int eventNum, int &prosperity, int &safety, const map<string, array<list<string>, 3>> &parties)
{
    // This function will display the current state of the world, including all parties and the kingdom's prosperity and safety.
    // All changes to safety and prosperity will come from kingdom and quest events. This will ONLY store descriptions.
    string eventDescription;
    switch (eventNum)
    {
    case 1:
        eventDescription = "A new adventure has begun!";
        break;
    case 2:
        eventDescription = "The kingdom's garrison is fortified.\n Safety has increased!";
        break;
    case 3:
        eventDescription = "A famine has struck the land!\n Prosperity has decreased!";
        break;
    case 4:
        eventDescription = "A band of marauders is terrorizing the countryside!\n Safety and prosperity have decreased!";
        break;
    case 5:
        eventDescription = "A bountiful harvest has increased the kingdom's wealth!\n Prosperity has increased!";
        break;
    default:
        eventDescription = "Nothing significant has occurred.";
    }
    cout << "Event: " << eventDescription << endl;
    cout << "---Current state of the world---" << endl;
    cout << "Kingdom Prosperity: " << prosperity << endl;
    cout << "Kingdom Safety: " << safety << endl;
    cout << "---Parties---" << endl;
    for (const auto &party : parties)
    {
        cout << "Party Name: " << party.first << endl;
        cout << "Members: ";
        for (const auto &member : party.second[0])
        {
            cout << member << " ";
        }
        cout << endl;
        cout << "Loot: ";
        for (const auto &loot : party.second[1])
        {
            cout << loot << " ";
        }
        cout << endl;
        cout << "Quests: ";
        for (const auto &quest : party.second[2])
        {
            cout << quest << " ";
        }
        cout << "\n---------- " << endl
             << endl;
    }

    return eventDescription;
}

string combat(int eventNum, const string &partyName, array<list<string>, 3> &party)
{
    // This function will return a string that informs players if a member of a party is lost.
    if (eventNum < 2 || eventNum > 5)
    {
        return "[Combat] " + partyName + "had a peaceful period. No combat occurred."; // Statement for each party.
    }
    if (party[0].empty())
    {                                                                               // Check if the party's member list is empty before trying to remove a member.
        return "[Combat] " + partyName + " has no members left to lose in combat!"; // Statement for each party.
    }

    string combatDescription;
    switch (eventNum)
    {
    case 2:
    {
        combatDescription = "[Combat] " + partyName + " lost a member to a swarm of goblins.";
        break;
    }
    case 3:
    {
        combatDescription = "[Combat] " + partyName + " lost a member to a vampire.";
        break;
    }
    case 4:
    {
        combatDescription = "[Combat] " + partyName + " had a member caught in a trap set by bandits";
        break;
    }
    case 5:
    {
        combatDescription = "[Combat] " + partyName + " was ambushed by a gnolls, but they managed to escape without any losses.";
        return combatDescription; // Return early since no members are lost in this event.
    }
    }
    party[0].pop_back();      // Remove a member from the party's member list to simulate the loss of a member in combat.
    return combatDescription; // Now this will return the combat description after removing a member to simulate the consequences of combat.
}
// This function simulates a quest event that can add/remove quests, members, and loot based on the event number and random chance.
string questEvent(int eventNum, const string &partyName, array<list<string>, 3> &party)
{
    if (eventNum < 2 || eventNum > 5)
    {
        return "[Quest] " + partyName + " has no new quests have arisen during this time period.";
    }
    if (party[0].empty())
    { // Check if the map is empty before trying to access it.
        return "[Quest] " + partyName + " has no one left to give quests to! All members have been wiped out.";
    }

    string questDescription;
    switch (eventNum)
    {
    case 2:
    {
        questDescription = "[Quest] " + partyName + " has a new quest: Clear the Goblin Cave.";
        if (party[2].size() < MAX_QUEST_SIZE) // Check if the party's quest list is not already at max capacity before adding a new quest.
        {
            party[2].push_back("Clear the Goblin Cave"); // Add a new quest to the party's quest list to simulate a new quest arising.
        }
        else // Adds a comment that party cannot take on the new quest if their quest list is already at max capacity.
        {
            questDescription += " However, " + partyName + "'s quest list is already at max capacity, so they cannot take on this new quest.";
        }
        return questDescription;
        break;
    }
    case 3:
    {
        questDescription = "[Quest] " + partyName + " has a new quest: Gather Herbs for the Villager.";
        if (party[2].size() < MAX_QUEST_SIZE) // Check if the party's quest list is not already at max capacity before adding a new quest.
        {
            party[2].push_back("Gather Herbs for the Villager"); // Add a new quest to the party's quest list to simulate a new quest arising.
            return questDescription;
        }
        else // Adds a comment that party cannot take on the new quest if their quest list is already at max capacity.
        {
            questDescription += " However, " + partyName + "'s quest list is already at max capacity, so they cannot take on this new quest.";
            return questDescription;
        }
        break;
    }
    // For events 4 and 5, we will simulate the completion of a quest, which can remove a quest from the party's quest
    // list and add loot to the party's loot list based on the event number.
    case 4:
    {
        // Check if the party's quest list is not empty before trying to remove a quest.
        if (!party[2].empty())
        {
            questDescription = "[Quest] " + partyName + " has completed a quest!";
            party[2].pop_back();                 // Remove a quest from the party's quest list to simulate the completion of a quest.
            if (party[1].size() < MAX_LOOT_SIZE) // Check if the party's loot list is not already at max capacity before adding new loot.
            {
                party[1].push_back("Healing Potion"); // Add new loot to the party's loot list to simulate the reward for completing a quest.
            }
            else // Adds a comment that party cannot receive the new loot if their loot list is already at max capacity.
            {
                questDescription += " However, " + partyName + "'s loot list is already at max capacity, so they cannot receive this new loot reward.";
                return questDescription;
            }
            return questDescription;
        }
        break;
    }
    case 5:
    {
        if (!party[2].empty())
        {
            questDescription = "[Quest] " + partyName + " has completed a quest!";
            party[2].pop_back();                 // Remove a quest from the party's quest list to simulate the completion of a quest.
            if (party[1].size() < MAX_LOOT_SIZE) // Check if the party's loot list is not already at max capacity before adding new loot.
            {
                party[1].push_back("Magic Sword"); // Add new loot to the party's loot list to simulate the reward for completing a quest.
            }
            else // Adds a comment that party cannot receive the new loot if their loot list is already at max capacity.
            {
                questDescription += " However, " + partyName + "'s loot list is already at max capacity, so they cannot receive this new loot reward.";
                return questDescription;
            }
            return questDescription;
        }
        break;
    }
    }
    return "[Quest] No new quests have arisen during this time period."; // Fix to ensure a return statement is reached if no quests are added or completed.
}
void kingdomEvent(int eventNum, int &prosperity, int &safety)
{

    switch (eventNum)
    {
    case 2:           // Garrison fortified event.
        safety += 10; // Example of an event that increases safety.
        break;
    case 3:               // Famine event.
        prosperity -= 15; // Example of an event that decreases prosperity.
        break;
    case 4:               // Marauders event.
        prosperity -= 10; // Example of an event that decreases prosperity.
        safety -= 5;      // Example of an event that decreases safety.
        break;
    case 5:               // Bountiful harvest event.
        prosperity += 15; // Example of an event that increases prosperity.
        break;
    default:
        // No change to prosperity or safety for other events.
        break;
    }
}
// This function will help keep our main loop cleaner by calling combat and quest events.
void applyEventEffects(int eventNum, const string &partyName, array<list<string>, 3> &party)
{
    cout << combat(eventNum, partyName, party) << endl;     // Apply combat event effects to the party based on the event number.
    cout << questEvent(eventNum, partyName, party) << endl; // Apply quest event effects to the party based on the event number.
    // Recruitment check. --------------------
    if (party[0].size() < MAX_PARTY_SIZE && rand() % 5 == 0) // 20% chance for new member to join.
    {
        string newMember = partyMembers[rand() % partyMembers.size()]; // Randomly select a new member from the partyMembers vector to add to the party.
        party[0].push_back(newMember);                                 // Add the new member to the party.
        cout << "[Recruitment] " << newMember << " has joined " << partyName << "!" << endl;
    }
}

int randomEvent()
{
    // This function will generate a random event number between 2 and NUM_EVENTS. Event 1 will always be used for initial event at start of program.
    int eventNum = rand() % (NUM_EVENTS - 1) + 2; // Generate a random event number between 2 and NUM_EVENTS.
    return eventNum;
}

// NOTES:
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