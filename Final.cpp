#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <stdexcept> 
using namespace std;

// Utility function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Class to handle user authentication
class Authentication {
private:
    string username;
    string password;

public:
    bool login() {
        clearScreen(); // Clear screen before login

         cout << "========================================\n";
         cout << "||                                    ||\n";
         cout << "||        WELCOME TO THE SYSTEM       ||\n";
         cout << "||                                    ||\n";
         cout << "========================================\n";
         cout << "Enter Username: ";
         cin >> username;
         cout << "Enter Password: ";
         cin >> password;

        if (username == "admin" && password == "1234") {
            cout << "\n========================================\n";
            cout << "       Login Successful! Welcome        \n";
            cout << "========================================\n\n";

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();

            return true;
        } else {
            cout << "\n========================================\n";
            cout << "    Invalid credentials. Try again.    \n";
            cout << "========================================\n\n";
            clearScreen();
            return false;
        }
    }
};

class Movie {
public:
    string title;
    vector<string> showtimes;
    double price;

    Movie(string t, vector<string> st, double p) : title(t), showtimes(st), price(p) {}
};

class Booking {
public:
    string movieTitle;
    string showtime;
    vector<int> seats;

    Booking(string mt, string st, vector<int> s) : movieTitle(mt), showtime(st), seats(s) {}
};

class MovieTicketSystem {
private:
    vector<Movie> movies;
    vector<Booking> bookings;
    
     void saveData() {
        // Save booking data to file
        ofstream file("bookings.txt");
        for (const auto &booking : bookings) {
            file << booking.movieTitle << "|" << booking.showtime << "|";
            for (size_t j = 0; j < booking.seats.size(); ++j) {
                file << booking.seats[j];
                if (j != booking.seats.size() - 1) file << ",";
            }
            file << endl;
        }
        file.close();
    }

    void saveMovies() {
        ofstream file("movies.txt");
        for (const auto &movie : movies) {
            file << movie.title << "|";
            for (size_t i = 0; i < movie.showtimes.size(); ++i) {
                file << movie.showtimes[i];
                if (i != movie.showtimes.size() - 1) file << ",";
            }
            file << "|" << movie.price << endl;
        }
        file.close();
    }

   void loadMovies() {
    ifstream file("movies.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open movies.txt file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);

        if (pos1 == string::npos || pos2 == string::npos) {
            cout << "Error: Malformed data in movies.txt. Skipping entry.\n";
            continue; // Skip this entry
        }

        string title = line.substr(0, pos1);
        string showtimesStr = line.substr(pos1 + 1, pos2 - pos1 - 1);

        vector<string> showtimes;
        stringstream ss(showtimesStr);
        string time;
        while (getline(ss, time, ',')) {
            showtimes.push_back(time);
        }

        double price = 0.0;
        try {
            price = stod(line.substr(pos2 + 1));
        } catch (const std::invalid_argument& e) { // Corrected syntax for exception handling
            cout << "Error: Invalid price value in movies.txt. Skipping entry.\n";
            continue; // Skip this entry
        }

        movies.push_back(Movie(title, showtimes, price));
    }
    file.close();
}

    void saveBookings() {
        ofstream file("bookings.txt");
        for (const auto &booking : bookings) {
            file << booking.movieTitle << "|" << booking.showtime << "|";
            for (size_t i = 0; i < booking.seats.size(); ++i) {
                file << booking.seats[i];
                if (i != booking.seats.size() - 1) file << ",";
            }
            file << endl;
        }
        file.close();
    }

public:
    MovieTicketSystem() {
        loadMovies();
    }

    void addMovie() {
        string title;
        int showtimeCount;
        vector<string> showtimes;
        double price;

        cout << "========================================\n";
        cout << "            Add a New Movie             \n";
        cout << "========================================\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        cout << "Enter movie title: ";
        getline(cin, title);

        cout << "Enter number of showtimes: ";
        cin >> showtimeCount;

        for (int i = 0; i < showtimeCount; i++) {
            string time;
            cout << "Enter showtime " << i + 1 << ": ";
            cin >> time;
            showtimes.push_back(time);
        }

        do {
            cout << "Enter ticket price: ";
            cin >> price;
            if (price <= 0) {
                cout << "Ticket price must be greater than zero. Please try again.\n";
            }
        } while (price <= 0);

        movies.push_back(Movie(title, showtimes, price));
        saveMovies();

        cout << "\n========================================\n";
        cout << "       Movie added successfully!        \n";
        cout << "========================================\n";
        cout << "\nPress Enter to return to the menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    
    void deleteMovie() {
    displayMovies();

    int movieChoice;
    cout << "Select a movie to delete by number: ";
    cin >> movieChoice;

    if (movieChoice < 1 || static_cast<size_t>(movieChoice) > movies.size()) {
        cout << "\n========================================\n";
        cout << "           Invalid choice!             \n";
        cout << "========================================\n\n";
        return;
    }

    movies.erase(movies.begin() + movieChoice - 1); // Remove the selected movie
    saveMovies(); // Save the updated list to file

    cout << "\n========================================\n";
    cout << "       Movie deleted successfully!     \n";
    cout << "========================================\n\n";

    // Pause to allow the user to see the success message
    cout << "Press Enter to return to the menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    cin.get();
}


    void displayMovies() {
        cout << "========================================\n";
        cout << "          Available Movies              \n";
        cout << "========================================\n";
        cout << "No. | Movie Title               | Showtimes | Price\n";
        cout << "----------------------------------------------\n";

        for (size_t i = 0; i < movies.size(); ++i) {
            cout << i + 1 << "  | " << movies[i].title;
            cout << "  | ";
            for (size_t j = 0; j < movies[i].showtimes.size(); ++j) {
                cout << movies[i].showtimes[j];
                if (j != movies[i].showtimes.size() - 1) cout << ", ";
            }
            cout << " | NPR " << movies[i].price << endl;
        }
        cout << "----------------------------------------------\n";
    }
    
     void searchMovies() {
        string query;
        cout << "Enter movie title to search: ";
        cin.ignore();
        getline(cin, query);

        bool found = false;
        for (size_t i = 0; i < movies.size(); ++i) {
            if (movies[i].title.find(query) != string::npos) {
                cout << i + 1 << "  | " << movies[i].title;
                cout << "  | ";
                for (size_t j = 0; j < movies[i].showtimes.size(); ++j) {
                    cout << movies[i].showtimes[j];
                    if (j != movies[i].showtimes.size() - 1) cout << ", ";
                }
                cout << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No movies found with the title: " << query << endl;
        }
    }
    
     void viewBookings() {
        cout << "========================================\n";
        cout << "           Booking History             \n";
        cout << "========================================\n";
        cout << "No. | Movie Title          | Showtime  | Seats\n";
        cout << "---------------------------------------------\n";

        if (bookings.empty()) {
            cout << "No bookings available.\n";
        } else {
            for (size_t i = 0; i < bookings.size(); ++i) {
                cout << i + 1 << "  | " << bookings[i].movieTitle;
                cout << "  | " << bookings[i].showtime << "  | ";
                for (size_t j = 0; j < bookings[i].seats.size(); ++j) {
                    cout << bookings[i].seats[j];
                    if (j != bookings[i].seats.size() - 1) cout << ", ";
                }
                cout << endl;
            }
        }
        cout << "---------------------------------------------\n";
    }

    void cancelBooking() {
        viewBookings();

        int bookingChoice;
        cout << "Select a booking to cancel by number: ";
        cin >> bookingChoice;

        if (bookingChoice < 1 || static_cast<size_t>(bookingChoice) > bookings.size()) {
            cout << "\n========================================\n";
            cout << "           Invalid choice!             \n";
            cout << "========================================\n\n";
            return;
        }

        bookings.erase(bookings.begin() + bookingChoice - 1);
        saveData();
        cout << "\n========================================\n";
        cout << "       Booking cancelled successfully!   \n";
        cout << "========================================\n\n";
    }

    void bookTickets() {
        displayMovies();

        int movieChoice;
        cout << "Select a movie by number: ";
        cin >> movieChoice;
        if (movieChoice < 1 || static_cast<size_t>(movieChoice) > movies.size()) {
            cout << "\n========================================\n";
            cout << "           Invalid choice!             \n";
            cout << "========================================\n\n";
            return;
        }

        Movie &selectedMovie = movies[movieChoice - 1];

        cout << "\nAvailable Showtimes:\n";
        for (size_t i = 0; i < selectedMovie.showtimes.size(); ++i) {
            cout << i + 1 << ". " << selectedMovie.showtimes[i] << "\n";
        }

        int showtimeChoice;
        cout << "Select a showtime by number: ";
        cin >> showtimeChoice;
        if (showtimeChoice < 1 || static_cast<size_t>(showtimeChoice) > selectedMovie.showtimes.size()) {
            cout << "\n========================================\n";
            cout << "           Invalid choice!             \n";
            cout << "========================================\n\n";
            return;
        }

        string selectedShowtime = selectedMovie.showtimes[showtimeChoice - 1];

        const int rows = 5, cols = 10;
    vector<vector<char>> seats(rows, vector<char>(cols, 'A'));

    // Mark already booked seats
    for (const auto &booking : bookings) {
        if (booking.movieTitle == selectedMovie.title && booking.showtime == selectedShowtime) {
            for (int seat : booking.seats) {
                int row = (seat - 1) / cols;
                int col = (seat - 1) % cols;
                seats[row][col] = 'B';
            }
        }
    }

    // Display the seating arrangement with row and column labels
    cout << "\n========================================\n";
    cout << "         Seating Arrangement           \n";
    cout << "========================================\n";

    // Column headers
    cout << "     "; // For row number space
    for (int c = 0; c < cols; ++c) {
        cout << char('A' + c) << " ";  // Column labels (A, B, C, ...)
    }
    cout << endl;

    // Display seating with row numbers
    for (int i = 0; i < rows; ++i) {
        cout << "Row " << i + 1 << " ";  // Row label (Row 1, Row 2, ...)
        for (int j = 0; j < cols; ++j) {
            cout << seats[i][j] << " ";  // Seat status (A: Available, B: Booked)
        }
        cout << endl;
    }
    cout << "\n(A: Available, B: Booked)\n";

    int seatCount;
    cout << "Enter the number of seats to book: ";
    cin >> seatCount;

    vector<pair<int, int>> seatsToBook;  // Vector to store the row and column of booked seats
    for (int i = 0; i < seatCount; i++) {
        int row, col;
        char colChar;
        cout << "Enter seat for booking (row number and column letter, e.g., 1 A): ";
        cin >> row >> colChar;

        // Convert column letter to index
        col = colChar - 'A';

        if (row < 1 || row > rows || col < 0 || col >= cols || seats[row - 1][col] == 'B') {
            cout << "Seat " << row << " " << colChar << " is invalid or already booked. Try again.\n";
            --i; // Retry this iteration
        } else {
            seats[row - 1][col] = 'B';  // Mark the seat as booked
            seatsToBook.push_back({row - 1, col});  // Store the seat coordinates
        }
    }

    // Convert row and column coordinates back to seat numbers for booking
    vector<int> seatNumbers;
    for (const auto &seat : seatsToBook) {
        int seatNumber = seat.first * cols + seat.second + 1;
        seatNumbers.push_back(seatNumber);
    }

    bookings.push_back(Booking(selectedMovie.title, selectedShowtime, seatNumbers));
    saveData();
    cout << "\n========================================\n";
    cout << "      Tickets booked successfully!      \n";
    cout << "========================================\n\n";

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void seatingArrangement() {
    const int rows = 5;
    const int cols = 10;
    vector<vector<char>> seats(rows, vector<char>(cols, 'A'));

    // Mark already booked seats
    for (const auto &booking : bookings) {
        for (int seat : booking.seats) {
            int row = (seat - 1) / cols;
            int col = (seat - 1) % cols;
            if (row < rows && col < cols) {
                seats[row][col] = 'B';
            }
        }
    }

    // Display row labels
    cout << "\n========================================\n";
    cout << "         Seating Arrangement            \n";
    cout << "========================================\n";

    // Column headers
    cout << "     "; // For row number space
    for (int c = 0; c < cols; ++c) {
        cout << char('A' + c) << " ";  // Column labels (A, B, C, ...)
    }
    cout << endl;

    // Display seating with row numbers
    for (int i = 0; i < rows; ++i) {
        cout << "Row " << i + 1 << " ";  // Row label (Row 1, Row 2, ...)
        for (int j = 0; j < cols; ++j) {
            cout << seats[i][j] << " ";  // Seat status (A: Available, B: Booked)
        }
        cout << endl;
    }
    cout << "\n(A: Available, B: Booked)\n";

    // Clear the input buffer and wait for user to press Enter
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nPress Enter to return to the menu...";
    cin.get();
}
void menu() {
        int choice;
        do {
            clearScreen(); // Clear the screen at the beginning of the menu

            cout << "========================================\n";
            cout << "      Movie Ticket Booking System       \n";
            cout << "========================================\n";
            cout << "1. Add Movie\n";
            cout << "2. Display Movies\n";
            cout << "3. Book Tickets\n";
            cout << "4. View Bookings\n";
            cout << "5. Cancel Booking\n";
            cout << "6. Search Movies\n";
            cout << "7. Delete Movie\n";
            cout << "8. View Seating Arrangement\n";
            cout << "9. Exit\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            // Clear input buffer to handle leftover characters
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            clearScreen(); // Clear the screen before performing any action

            switch (choice) {
                case 1:
                    addMovie();
                    break;
                case 2:
                    displayMovies();
                    break;
                case 3:
                    bookTickets();
                    break;
                case 4:
                    viewBookings();
                    break;
                case 5:
                    cancelBooking();
                    break;
                case 6:
                    searchMovies();
                    break;
                case 7:
                    deleteMovie();
                    break;
                case 8:
                    seatingArrangement();
                    break;
                case 9:
                   cout << "\n****************************************\n";
                   cout << "*                                      *\n";
                   cout << "*          Exiting... Goodbye!         *\n";
                   cout << "*                                      *\n";
                   cout << "****************************************\n";
                   cout << "\n";
                   cout << "****************************************\n";
                   cout << "*            Created By:               *\n";
                   cout << "*  Dineh Rimal, Kusal Rimal,           *\n";
                   cout << "*  and Anish Kumar Goit                *\n";
                   cout << "****************************************\n\n";

                    break;
                default:
                    cout << "\n========================================\n";
                    cout << "       Invalid choice. Try again.       \n";
                    cout << "========================================\n\n";
            }

            if (choice != 9) {
                cout << "\nPress Enter to continue...";
                cin.get(); // Pause before returning to the menu
            }

        } while (choice != 9);
    }
};

// Main function
int main() {
    Authentication auth;
    if (!auth.login()) {
        cout << "\n========================================\n";
        cout << " Exiting the system due to failed login. \n";
        cout << "========================================\n\n";
        return 0;
    }
    MovieTicketSystem mts;
    mts.menu();
    return 0;
}
