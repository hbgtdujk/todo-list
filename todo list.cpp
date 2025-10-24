#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif




class TodoList {
private:
    std::string db_path; 

public:
    
    TodoList(const std::string& path = "todo.txt") : db_path(path) {}

    
    void clear_screen() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    
    void pause_console() const {
#ifdef _WIN32
        system("pause");
#else
        std::cout << "Press Enter to continue...";
        std::cin.get();
#endif
    }

    
    void show_menu() const {
        std::cout << "[1] Add note.\n";
        std::cout << "[2] Show all notes.\n";
        std::cout << "[3] Find note.\n";
        std::cout << "[4] Remove one note.\n";
        std::cout << "[5] Remove all notes.\n";
        std::cout << "[6] Exit.\n";
    }

    
    void add_note() const {
        std::string line;
        std::cout << "Enter note: ";
        std::getline(std::cin, line);

        std::ofstream ofs(db_path, std::ios::app);
        if (ofs) {
            ofs << line << "\n";
            std::cout << "Note added.\n";
        } else {
            std::cout << "Error: can't open file.\n";
        }
    }

    
    void show_all() const {
        std::ifstream ifs(db_path);
        std::string line;
        int idx = 1;
        bool any = false;

        while (std::getline(ifs, line)) {
            std::cout << idx++ << ": " << line << "\n";
            any = true;
        }
        if (!any) std::cout << "No notes found.\n";
    }

    
    void find_note() const {
        std::string query;
        std::cout << "Search query: ";
        std::getline(std::cin, query);

        std::ifstream ifs(db_path);
        std::string line;
        int idx = 1;
        bool found = false;

        std::string qlow = query;
        std::transform(qlow.begin(), qlow.end(), qlow.begin(), ::tolower);

        while (std::getline(ifs, line)) {
            std::string low = line;
            std::transform(low.begin(), low.end(), low.begin(), ::tolower);
            if (low.find(qlow) != std::string::npos) {
                std::cout << idx << ": " << line << "\n";
                found = true;
            }
            idx++;
        }

        if (!found) std::cout << "No matching notes.\n";
    }

    
    void remove_one() const {
        std::ifstream ifs(db_path);
        std::vector<std::string> notes;
        std::string line;

        while (std::getline(ifs, line)) {
            notes.push_back(line);
        }

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

        if (num < 1 || num > (int)notes.size()) {
            std::cout << "Number out of range.\n";
            return;
        }

        notes.erase(notes.begin() + (num - 1));

        std::ofstream ofs(db_path, std::ios::trunc);
        for (auto &n : notes)
            ofs << n << "\n";

        std::cout << "Note removed.\n";
    }

    
    void remove_all() const {
        std::ofstream ofs(db_path, std::ios::trunc);
        std::cout << "All notes removed.\n";
    }
};





int main() {
    TodoList todo; 
    int choice = 0;

    while (true) {
        todo.clear_screen();
        todo.show_menu();

        std::cout << "Your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dum;
            std::getline(std::cin, dum);
            break;
        }

        std::string dum;
        std::getline(std::cin, dum); 

        switch (choice) {
            case 1:
                todo.clear_screen();
                todo.add_note();
                todo.pause_console();
                break;
            case 2:
                todo.clear_screen();
                todo.show_all();
                todo.pause_console();
                break;
            case 3:
                todo.clear_screen();
                todo.find_note();
                todo.pause_console();
                break;
            case 4:
                todo.clear_screen();
                todo.remove_one();
                todo.pause_console();
                break;
            case 5:
                todo.clear_screen();
                todo.remove_all();
                todo.pause_console();
                break;
            case 6:
                return 0;
            default:
                std::cout << "Invalid choice.\n";
                todo.pause_console();
        }
    }

    return 0;
}
