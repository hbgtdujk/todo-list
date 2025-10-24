#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

class ConsoleManager {
public:
    static void clear_screen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    static void pause_console() {
#ifdef _WIN32
        system("pause");
#else
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#endif
    }

    static void show_menu() {
        std::cout << "[1] Add note.\n";
        std::cout << "[2] Show all notes.\n";
        std::cout << "[3] Find note.\n";
        std::cout << "[4] Remove one note.\n";
        std::cout << "[5] Remove all notes.\n";
        std::cout << "[6] Exit.\n";
    }
};

class NotesFile {
private:
    std::string file_path;

public:
    NotesFile(const std::string& path = "todo.txt") : file_path(path) {}

    bool is_open() const {
        std::ifstream file(file_path);
        return file.is_open();
    }

    void append_note(const std::string& note) const {
        std::ofstream file(file_path, std::ios::app);
        if (file) {
            file << note << "\n";
        }
    }

    std::vector<std::string> read_all_notes() const {
        std::vector<std::string> notes;
        std::ifstream file(file_path);
        std::string line;

        while (std::getline(file, line)) {
            notes.push_back(line);
        }
        return notes;
    }

    void write_all_notes(const std::vector<std::string>& notes) const {
        std::ofstream file(file_path, std::ios::trunc);
        for (const auto& note : notes) {
            file << note << "\n";
        }
    }

    void clear_file() const {
        std::ofstream file(file_path, std::ios::trunc);
    }

    std::string get_file_path() const { return file_path; }
};

class NotesManager {
private:
    NotesFile notes_file;

public:
    NotesManager(const std::string& path = "todo.txt") : notes_file(path) {}

    void add_note() const {
        std::string line;
        std::cout << "Enter note: ";
        std::getline(std::cin, line);

        if (notes_file.is_open()) {
            notes_file.append_note(line);
            std::cout << "Note added.\n";
        }
        else {
            std::cout << "Error: can't open file.\n";
        }
    }

    void show_all_notes() const {
        std::vector<std::string> notes = notes_file.read_all_notes();
        int idx = 1;
        bool any = false;

        for (const auto& note : notes) {
            std::cout << idx++ << ": " << note << "\n";
            any = true;
        }

        if (!any) std::cout << "No notes found.\n";
    }

    void find_note() const {
        std::string query;
        std::cout << "Search query: ";
        std::getline(std::cin, query);

        std::vector<std::string> notes = notes_file.read_all_notes();
        bool found = false;

        std::string qlow = query;
        std::transform(qlow.begin(), qlow.end(), qlow.begin(), ::tolower);

        for (size_t i = 0; i < notes.size(); ++i) {
            std::string low = notes[i];
            std::transform(low.begin(), low.end(), low.begin(), ::tolower);
            if (low.find(qlow) != std::string::npos) {
                std::cout << (i + 1) << ": " << notes[i] << "\n";
                found = true;
            }
        }

        if (!found) std::cout << "No matching notes.\n";
    }

    void remove_one_note() const {
        std::vector<std::string> notes = notes_file.read_all_notes();

        if (notes.empty()) {
            std::cout << "No notes to remove.\n";
            return;
        }

        for (size_t i = 0; i < notes.size(); ++i)
            std::cout << (i + 1) << ": " << notes[i] << "\n";

        std::cout << "Enter number to remove: ";
        int num = 0;
        if (!(std::cin >> num)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            std::cout << "Invalid input.\n";
            return;
        }

        std::string dummy;
        std::getline(std::cin, dummy);

        if (num < 1 || num >(int)notes.size()) {
            std::cout << "Number out of range.\n";
            return;
        }

        notes.erase(notes.begin() + (num - 1));
        notes_file.write_all_notes(notes);
        std::cout << "Note removed.\n";
    }

    void remove_all_notes() const {
        notes_file.clear_file();
        std::cout << "All notes removed.\n";
    }
};

class TodoApp {
private:
    NotesManager notes_manager;
    ConsoleManager console;

public:
    void run() {
        int choice = 0;

        while (true) {
            console.clear_screen();
            console.show_menu();

            std::cout << "Your choice: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string dum;
                std::getline(std::cin, dum);
                break;
            }

            std::string dum;
            std::getline(std::cin, dum);

            process_choice(choice);
        }
    }

private:
    void process_choice(int choice) {
        switch (choice) {
        case 1:
            console.clear_screen();
            notes_manager.add_note();
            console.pause_console();
            break;
        case 2:
            console.clear_screen();
            notes_manager.show_all_notes();
            console.pause_console();
            break;
        case 3:
            console.clear_screen();
            notes_manager.find_note();
            console.pause_console();
            break;
        case 4:
            console.clear_screen();
            notes_manager.remove_one_note();
            console.pause_console();
            break;
        case 5:
            console.clear_screen();
            notes_manager.remove_all_notes();
            console.pause_console();
            break;
        case 6:
            exit(0);
        default:
            std::cout << "Invalid choice.\n";
            console.pause_console();
        }
    }
};

int main() {
    TodoApp app;
    app.run();
    return 0;
}
