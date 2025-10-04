#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

static const std::string DB = "todo.txt";

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_console() {
#ifdef _WIN32
    system("pause");
#else
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#endif
}

void show_menu() {
    std::cout << "[1] Add note.\n";
    std::cout << "[2] Show all notes.\n";
    std::cout << "[3] Find note.\n";
    std::cout << "[4] Remove one note.\n";
    std::cout << "[5] Remove all notes.\n";
    std::cout << "[6] Exit.\n";
}

void add_note() {
    std::string line;
    std::getline(std::cin, line);
    std::cout << "Enter note: ";
    std::getline(std::cin, line);
    std::ofstream ofs(DB, std::ios::app);
    if (ofs) ofs << line << "\n";
}

void see_all_notes() {
    std::ifstream ifs(DB);
    std::string line;
    int idx = 1;
    bool any = false;
    while (std::getline(ifs, line)) {
        std::cout << idx << ": " << line << "\n";
        idx++;
        any = true;
    }
    if (!any) std::cout << "No notes found.\n";
}

void find_note() {
    std::string query;
    std::getline(std::cin, query);
    std::cout << "Search query: ";
    std::getline(std::cin, query);
    std::ifstream ifs(DB);
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

void remove_one_note() {
    std::ifstream ifs(DB);
    std::vector<std::string> notes;
    std::string line;
    while (std::getline(ifs, line)) {
        notes.push_back(line);
    }
    if (notes.empty()) {
        std::cout << "No notes to remove.\n";
        return;
    }
    for (size_t i = 0; i < notes.size(); ++i) {
        std::cout << (i + 1) << ": " << notes[i] << "\n";
    }
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
    std::ofstream ofs(DB, std::ios::trunc);
    for (auto& n : notes) ofs << n << "\n";
    std::cout << "Removed.\n";
}

void remove_all_notes() {
    std::ofstream ofs(DB, std::ios::trunc);
    std::cout << "All notes removed.\n";
}

int main() {
    int choice = 0;
    while (true) {
        clear_screen();
        show_menu();
        std::cout << "Your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dum;
            std::getline(std::cin, dum);
            break;
        }
        std::string dum;
        std::getline(std::cin, dum);
        if (choice == 1) {
            clear_screen();
            add_note();
            pause_console();
        }
        else if (choice == 2) {
            clear_screen();
            see_all_notes();
            pause_console();
        }
        else if (choice == 3) {
            clear_screen();
            find_note();
            pause_console();
        }
        else if (choice == 4) {
            clear_screen();
            remove_one_note();
            pause_console();
        }
        else if (choice == 5) {
            clear_screen();
            remove_all_notes();
            pause_console();
        }
        else {
            break;
        }
    }
    return 0;
}
