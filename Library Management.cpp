

/*CLARIZE L. IBARRETA*/
/*BSIT - 2D*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

/* Book Class */
class Book 
{
	private:
    	string title;
    	string author;
    	string isbn;
    	bool isAvailable;

	public:
    	Book(string t, string a, string i) 
        	: title(t), author(a), isbn(i), isAvailable(true) {}

    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getISBN() { return isbn; }
    bool getAvailability() { return isAvailable; }

    void setAvailability(bool status) { isAvailable = status; }

    void display() 
	{
        cout << "Title: " << title 
             << " | Author: " << author 
             << " | ISBN: " << isbn 
             << " | Status: " << (isAvailable ? "Available" : "Borrowed") 
             << endl << endl;
    }

    void save(ofstream &out) 
	{
        out << title << "," << author << "," << isbn << "," << isAvailable << endl;
    }

    static Book load(string line) 
	{
        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);
        size_t pos3 = line.find(",", pos2 + 1);

        string t = line.substr(0, pos1);
        string a = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string i = line.substr(pos2 + 1, pos3 - pos2 - 1);
        bool avail = stoi(line.substr(pos3 + 1));

        Book b(t, a, i);
        b.setAvailability(avail);
        return b;
    }
};

/* Library User Class */
class LibraryUser 
{
	private:
    	string userID;
    	string name;
    	vector<string> borrowedBooks;

	public:
    	LibraryUser(string id, string n) : userID(id), name(n) {}

    	string getUserID() { return userID; }
    	string getName() { return name; }

    void borrowBook(string isbn) 
	{
        borrowedBooks.push_back(isbn);
    }

    void returnBook(string isbn) 
	{
        borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), isbn), borrowedBooks.end());
    }

    void displayBorrowed() 
	{
        cout << endl << name << " has borrowed: ";
        if (borrowedBooks.empty()) 
		{
            cout << "No books";
        } 
		else 
		{
            for (string isbn : borrowedBooks) cout << isbn << " ";
        }
    }

    void display() 
	{
        cout << "User ID: " << userID << " | Name: " << name << endl;
    }

    void save(ofstream &out) 
	{
        out << userID << "," << name;
        for (string isbn : borrowedBooks) out << "," << isbn;
        out << endl;
    }

    static LibraryUser load(string line) 
	{
        size_t pos1 = line.find(",");
        string id = line.substr(0, pos1);
        size_t pos2 = line.find(",", pos1 + 1);
        string n = line.substr(pos1 + 1, pos2 - pos1 - 1);

        LibraryUser u(id, n);

        size_t start = pos2 + 1;
        while (start < line.size()) {
            size_t pos = line.find(",", start);
            if (pos == string::npos) pos = line.size();
            u.borrowBook(line.substr(start, pos - start));
            start = pos + 1;
        }
        return u;
    }
};

/* Library Class */
class Library 
{
	private:
    	vector<Book> books;
    	vector<LibraryUser> users;

	public:
    	~Library() 
		{
        	saveBooks();
        	saveUsers();
    	}

    void addBook(string title, string author, string isbn) 
	{
        books.push_back(Book(title, author, isbn));
    }

    void removeBook(string isbn) 
	{
        books.erase(remove_if(books.begin(), books.end(),
                              [&](Book &b){ return b.getISBN() == isbn; }),
                    books.end());
    }

    void displayAllBooks() 
	{
        cout << "\n   ~ Books in Library ~   \n";
        for (auto &b : books) b.display();
    }

    void registerUser(string id, string name) 
	{
		users.push_back(LibraryUser(id, name));	  	
    }

    void removeUser(string id) 
	{
        users.erase(remove_if(users.begin(), users.end(),
                              [&](LibraryUser &u){ return u.getUserID() == id; }),
                    users.end());
    }

    void displayAllUsers() 
	{
        cout << "\n    Registered Users    \n";
        for (auto &u : users) u.display();
        cout << endl;
    }

    void borrowBook(string isbn, string userID) 
	{	
        for (auto &b : books) 
		{
            if (b.getISBN() == isbn && b.getAvailability()) 
			{
                for (auto &u : users) 
				{
                    if (u.getUserID() == userID) 
					{
                        b.setAvailability(false);
                        u.borrowBook(isbn);
                        cout << u.getName() << " borrowed " << b.getTitle() << endl;
                        return;
                    }
                }
            }
        }
        cout << "Book not available or User not found.\n";
    }

    void returnBook(string isbn, string userID) 
	{
        for (auto &u : users)
		{
            if (u.getUserID() == userID)
			{
                u.returnBook(isbn);
                for (auto &b : books) 
				{
                    if (b.getISBN() == isbn) 
					{
                        b.setAvailability(true);
                        cout << u.getName() << " returned: " << b.getTitle() << endl;
                        return;
                    }
                }
            }
        }
        cout << "Return failed.\n";
    }

    void displayBorrowedBooks(string userID) 
	{
        for (auto &u : users) 
		{
            if (u.getUserID() == userID) 
			{
                u.displayBorrowed();
                return;
            }
        }
        cout << "User not found.\n";
    }

/* File Handling */
    void saveBooks() 
	{
        ofstream out("books.txt");
        for (auto &b : books) b.save(out);
        out.close();
    }

    void saveUsers() 
	{
        ofstream out("users.txt");
        for (auto &u : users) u.save(out);
        out.close();
    }
    
    void loadBooks() 
	{
		books.clear();
        ifstream in("books.txt");
        string line;
        while (getline(in, line)) books.push_back(Book::load(line));
        in.close();
    }

    void loadUsers() 
	{
		users.clear();
        ifstream in("users.txt");
        string line;
        while (getline(in, line)) users.push_back(LibraryUser::load(line));
        in.close();
    }

};

/* Main Function */
int main() 
{
    Library library;
    library.loadBooks();
    library.loadUsers();

    int choice;
    do {
        cout << "\n--- Library Management System ---\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Register User\n";
        cout << "4. Display All Users\n";
        cout << "5. Borrow Book\n";
        cout << "6. Return Book\n";
        cout << "7. Show Borrowed Books (per user)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, author, isbn;
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            library.addBook(title, author, isbn);
            cout << "Book added successfully!\n";

        } else if (choice == 2) {
            library.displayAllBooks();

        } else if (choice == 3) {
            string id, name;
            cout << "Enter User ID: ";
            getline(cin, id);
            cout << "Enter Name: ";
            getline(cin, name);
            library.registerUser(id, name);
            cout << "User registered successfully!\n";

        } else if (choice == 4) {
            library.displayAllUsers();

        } else if (choice == 5) {
            string isbn, userID;
            cout << "Enter ISBN of book to borrow: ";
            getline(cin, isbn);
            cout << "Enter User ID: ";
            getline(cin, userID);
            library.borrowBook(isbn, userID);

        } else if (choice == 6) {
            string isbn, userID;
            cout << "Enter ISBN of book to return: ";
            getline(cin, isbn);
            cout << "Enter User ID: ";
            getline(cin, userID);
            library.returnBook(isbn, userID);

        } else if (choice == 7) {
            string userID;
            cout << "Enter User ID: ";
            getline(cin, userID);
            library.displayBorrowedBooks(userID);

        } else if (choice == 0) {
            cout << "Exiting program... Saving data.\n";

        } else {
            cout << "Invalid choice, try again.\n";
        }

    } while (choice != 0);

    return 0;
}