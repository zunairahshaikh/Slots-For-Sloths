#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// --- Utility Functions ---
string toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

template <typename T>
T *findFacilityByName(vector<T *> &facilities, const string &name)
{
    for (auto facility : facilities)
    {
        if (toLower(facility->getName()) == toLower(name))
        {
            return facility;
        }
    }
    return nullptr;
}

const vector<string> VALID_DAYS = {
    "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

bool isValidDay(const string &day)
{
    return find(VALID_DAYS.begin(), VALID_DAYS.end(), toLower(day)) != VALID_DAYS.end();
}

// --- Struct for Booking Information ---
struct BookingInfo
{
    string facilityName;
    string day;
    int slot;
};

// --- Location Base Class ---
class Location
{
protected:
    string name;
    int capacity;
    int openSlot, closeSlot;
    map<string, set<int>> bookings;

    static int facilityCount;

public:
    Location(string n, int cap, int open, int close)
        : name(n), capacity(cap), openSlot(open), closeSlot(close)
    {
        facilityCount++;
    }

    virtual ~Location()
    {
        facilityCount--;
    }

    friend ostream &operator<<(ostream &out, const Location &loc);

    static int getFacilityCount()
    {
        return facilityCount;
    }

    virtual void displayFullDetails() const = 0;
    virtual string getType() const = 0;

    string getName() const { return name; }
    int getOpenSlot() const { return openSlot; }
    int getCloseSlot() const { return closeSlot; }

    void setCapacity(int cap)
    {
        this->capacity = cap; // using this pointer
    }

    bool checkAvailability(const string &day, int slot) const
    {
        string lowercaseDay = toLower(day);
        auto it = bookings.find(lowercaseDay);
        if (it == bookings.end())
            return true;
        return it->second.find(slot) == it->second.end();
    }

    bool bookSlot(const string &day, int slot)
    {
        string lowercaseDay = toLower(day);
        if (!checkAvailability(day, slot))
            return false;
        bookings[lowercaseDay].insert(slot);
        return true;
    }

    bool unbookSlot(const string &day, int slot)
    {
        string lowercaseDay = toLower(day);
        auto it = bookings.find(lowercaseDay);
        if (it != bookings.end())
        {
            if (it->second.erase(slot))
            {
                if (it->second.empty())
                {
                    bookings.erase(it);
                }
                return true;
            }
        }
        return false;
    }

    void displayAvailability(const string &day) const
    {
        cout << "\nAvailability for " << name << " on " << day << ":\n";
        for (int slot = openSlot; slot <= closeSlot; slot++)
        {
            cout << "Slot " << slot << ": "
                 << (checkAvailability(day, slot) ? "Available" : "Booked") << endl;
        }
    }

    void saveBookings(ofstream &outFile) const
    {
        for (const auto &dayEntry : bookings)
        {
            string day = dayEntry.first;
            for (int slot : dayEntry.second)
            {
                outFile << name << "," << day << "," << slot << "\n";
            }
        }
    }

    void loadBooking(const string &day, int slot)
    {
        bookings[toLower(day)].insert(slot);
    }

    map<string, set<int>> getBookings() const
    {
        return bookings;
    }
};

int Location::facilityCount = 0;

ostream &operator<<(ostream &out, const Location &loc)
{
    out << loc.name << " (" << loc.getType() << ") - Capacity: " << loc.capacity;
    return out;
}

// --- Derived Classes ---
class Library : public virtual Location
{
    int studyCarrels;

public:
    Library(string n, int cap, int carrels, int open, int close)
        : Location(n, cap, open, close), studyCarrels(carrels) {}

    void displayFullDetails() const override
    {
        cout << *this << "\nStudy Carrels: " << studyCarrels << endl;
    }

    string getType() const override { return "Library"; }
};

class Cafe : public virtual Location
{
    string menuType;

public:
    Cafe(string n, int cap, string menu, int open, int close)
        : Location(n, cap, open, close), menuType(menu) {}

    void displayFullDetails() const override
    {
        cout << *this << "\nMenu: " << menuType << endl;
    }

    string getType() const override { return "Cafe"; }
};

class SportsFacility : public virtual Location
{
    string sportType;

public:
    SportsFacility(string n, int cap, string sport, int open, int close)
        : Location(n, cap, open, close), sportType(sport) {}

    void displayFullDetails() const override
    {
        cout << *this << "\nSport: " << sportType << endl;
    }

    string getType() const override { return sportType + " Facility"; }
};

class SportsCafe : public SportsFacility, public Cafe
{
public:
    SportsCafe(string n, int cap, string sport, string menu, int open, int close)
        : Location(n, cap, open, close),
          SportsFacility(n, cap, sport, open, close),
          Cafe(n, cap, menu, open, close) {}

    void displayFullDetails() const override
    {
        cout << name << " (Sports Cafe)\n"
             << "Sport: " << SportsFacility::getType() << "\n"
             << "Menu: " << Cafe::getType() << endl;
    }

    string getType() const override { return "Sports Cafe"; }
};

// --- Booking System ---
class BookingSystem
{
    vector<Location *> locations;

    void clearInputBuffer()
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int getIntInputCancelable(const string &prompt, int min, int max)
    {
        int input;
        do
        {
            cout << prompt;
            if (cin >> input)
            {
                if (input == 0)
                    return 0;
                if (input >= min && input <= max)
                    break;
                else
                    cout << "Invalid input, try again!\n";
            }
            else
            {
                cout << "Invalid input, try again!\n";
                clearInputBuffer();
            }
        } while (true);
        return input;
    }

    string getStringInputCancelable(const string &prompt)
    {
        clearInputBuffer();
        string input;
        cout << prompt;
        getline(cin, input);
        if (input == "0")
            return "CANCEL";
        return input;
    }

public:
    ~BookingSystem()
    {
        for (auto loc : locations)
            delete loc;
    }

    void addLocation(Location *loc)
    {
        locations.push_back(loc);
    }

    void initializeLocations()
    {
        addLocation(new Library("Main Library", 100, 30, 1, 9));
        addLocation(new Cafe("Student Cafe", 50, "Coffee & Snacks", 1, 7));
        addLocation(new SportsFacility("Basketball Court", 30, "Basketball", 1, 8));
        addLocation(new SportsCafe("Sports Hub", 40, "Table Tennis", "Healthy Snacks", 1, 8));
    }

    void loadBookingsFromFile()
    {
        ifstream inFile("bookings.txt");
        if (!inFile)
        {
            cout << "No previous bookings found.\n";
            return;
        }

        string facilityName, day;
        int slot;
        while (getline(inFile, facilityName, ',') &&
               getline(inFile, day, ',') &&
               inFile >> slot)
        {
            inFile.ignore();
            Location *loc = findFacilityByName(locations, facilityName);
            if (loc)
            {
                loc->loadBooking(day, slot);
            }
        }
        cout << "Previous bookings loaded!\n";
    }

    void saveBookingsToFile()
    {
        ofstream outFile("bookings.txt");
        for (auto loc : locations)
            loc->saveBookings(outFile);
    }

    void displayFacilities()
    {
        cout << "\n==== Available Facilities ====\n";
        for (size_t i = 0; i < locations.size(); i++)
        {
            cout << i + 1 << ". " << *locations[i] << endl;
        }
        cout << "Total facilities: " << Location::getFacilityCount() << endl;
    }

    void bookFacility()
    {
        displayFacilities();
        int index = getIntInputCancelable("\nEnter facility number (or 0 to cancel): ", 1, locations.size());
        if (index == 0)
            return;

        Location *facility = locations[index - 1];

        string day;
        do
        {
            day = getStringInputCancelable("Enter day (or 0 to cancel): ");
            if (day == "CANCEL")
                return;
        } while (!isValidDay(day));

        facility->displayAvailability(day);

        int slot = getIntInputCancelable("Enter slot number (or 0 to cancel): ", facility->getOpenSlot(), facility->getCloseSlot());
        if (slot == 0)
            return;

        if (facility->bookSlot(day, slot))
        {
            cout << "Booking confirmed!\n";
            saveBookingsToFile();
        }
        else
        {
            cout << "Slot already booked!\n";
        }
    }

    void unbookFacility()
    {
        displayFacilities();
        int index = getIntInputCancelable("\nEnter facility number (or 0 to cancel): ", 1, locations.size());
        if (index == 0)
            return;

        Location *facility = locations[index - 1];

        string day;
        do
        {
            day = getStringInputCancelable("Enter day (or 0 to cancel): ");
            if (day == "CANCEL")
                return;
        } while (!isValidDay(day));

        facility->displayAvailability(day);

        int slot = getIntInputCancelable("Enter slot number to unbook (or 0 to cancel): ", facility->getOpenSlot(), facility->getCloseSlot());
        if (slot == 0)
            return;

        if (facility->unbookSlot(day, slot))
        {
            cout << "Slot unbooked successfully!\n";
            saveBookingsToFile();
        }
        else
        {
            cout << "Slot was not booked!\n";
        }
    }

    void editBooking()
    {
        displayFacilities();
        int index = getIntInputCancelable("\nEnter facility number (or 0 to cancel): ", 1, locations.size());
        if (index == 0)
            return;

        Location *facility = locations[index - 1];

        string oldDay;
        do
        {
            oldDay = getStringInputCancelable("Enter current booked day (or 0 to cancel): ");
            if (oldDay == "CANCEL")
                return;
        } while (!isValidDay(oldDay));

        int oldSlot = getIntInputCancelable("Enter current booked slot number (or 0 to cancel): ", facility->getOpenSlot(), facility->getCloseSlot());
        if (oldSlot == 0)
            return;

        if (facility->unbookSlot(oldDay, oldSlot))
        {
            cout << "Old booking removed.\n";

            string newDay;
            do
            {
                newDay = getStringInputCancelable("Enter new day for booking (or 0 to cancel): ");
                if (newDay == "CANCEL")
                    return;
            } while (!isValidDay(newDay));

            int newSlot = getIntInputCancelable("Enter new slot number (or 0 to cancel): ", facility->getOpenSlot(), facility->getCloseSlot());
            if (newSlot == 0)
                return;

            if (facility->bookSlot(newDay, newSlot))
            {
                cout << "Booking updated successfully!\n";
                saveBookingsToFile();
            }
            else
            {
                cout << "New slot not available! Re-book the old slot manually if needed.\n";
            }
        }
        else
        {
            cout << "No such booking found to edit!\n";
        }
    }

    void viewFacilityDetails()
    {
        displayFacilities();
        int choice = getIntInputCancelable("\nEnter facility number (or 0 to cancel): ", 1, locations.size());
        if (choice == 0)
            return;

        locations[choice - 1]->displayFullDetails();
    }

    void viewAllBookingsForFacility()
    {
        displayFacilities();
        int index = getIntInputCancelable("\nEnter facility number (or 0 to cancel): ", 1, locations.size());
        if (index == 0)
            return;

        Location *facility = locations[index - 1];

        cout << "\n==== BOOKINGS FOR " << facility->getName() << " ====\n";

        bool anyBooking = false;
        for (const auto &dayEntry : facility->getBookings())
        {
            cout << dayEntry.first << ": ";
            for (int slot : dayEntry.second)
            {
                cout << slot << " ";
            }
            cout << endl;
            anyBooking = true;
        }

        if (!anyBooking)
        {
            cout << "No bookings found for this facility.\n";
        }
    }

    void run()
    {
        initializeLocations();
        loadBookingsFromFile();

        int choice;
        do
        {
            cout << "\n=== CAMPUS FACILITY BOOKING SYSTEM ===\n";
            cout << "1. View Facilities\n";
            cout << "2. Book Facility\n";
            cout << "3. Unbook Facility\n";
            cout << "4. Edit a Booking\n";
            cout << "5. View Facility Details\n";
            cout << "6. View All Bookings for a Facility\n";
            cout << "7. Exit\n";
            choice = getIntInputCancelable("Enter choice (1-7): ", 1, 7);

            switch (choice)
            {
            case 1:
                displayFacilities();
                break;
            case 2:
                bookFacility();
                break;
            case 3:
                unbookFacility();
                break;
            case 4:
                editBooking();
                break;
            case 5:
                viewFacilityDetails();
                break;
            case 6:
                viewAllBookingsForFacility();
                break;
            case 7:
                cout << "Goodbye!\n";
                break;
            }
        } while (choice != 7);
    }
};
int main()
{
    BookingSystem system;
    system.run();
    return 0;
}


