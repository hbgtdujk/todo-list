#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

class ConsoleUtils {
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
};

class Note {
private:
    std::string content_;

public:
    explicit Note(const std::string& content) : content_(content) {}

    const std::string& get_content() const { return content_; }
    void set_content(const std::string& content) { content_ = content; }

    bool contains(const std::string& query) const {
        std::string note_lower = content_;
        std::string query_lower = query;
        std::transform(note_lower.begin(), note_lower.end(), note_lower.begin(), ::tolower);
        std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
        return note_lower.find(query_lower) != std::string::npos;
    }

    friend std::ostream& operator<<(std::ostream& os, const Note& note) {
        os << note.content_;
        return os;
    }
};

class NoteRepository {
private:
    std::string filename_;

public:
    explicit NoteRepository(const std::string& filename = "todo.txt") : filename_(filename) {}

    std::vector<Note> load_all() const {
        std::vector<Note> notes;
        std::ifstream ifs(filename_);
        std::string line;

        while (std::getline(ifs, line)) {
            if (!line.empty()) {
                notes.emplace_back(line);
            }
        }
        return notes;
    }

    void save_all(const std::vector<Note>& notes) const {
        std::ofstream ofs(filename_, std::ios::trunc);
        for (const auto& note : notes) {
            ofs << note.get_content() << "\n";
        }
    }

    void append(const Note& note) const {
        std::ofstream ofs(filename_, std::ios::app);
        if (ofs) {
            ofs << note.get_content() << "\n";
        }
    }

    void clear_all() const {
        std::ofstream ofs(filename_, std::ios::trunc);
    }

    const std::string& get_filename() const { return filename_; }
};

class NoteService {
private:
    std::unique_ptr<NoteRepository> repository_;

public:
    explicit NoteService(std::unique_ptr<NoteRepository> repo)
        : repository_(std::move(repo)) {}

    void add_note(const std::string& content) const {
        if (!content.empty()) {
            repository_->append(Note(content));
            std::cout << "Note added successfully.\n";
        }
    }

    void display_all_notes() const {
        auto notes = repository_->load_all();
        if (notes.empty()) {
            std::cout << "No notes found.\n";
            return;
        }

        std::cout << "All notes:\n";
        for (size_t i = 0; i < notes.size(); ++i) {
            std::cout << (i + 1) << ": " << notes[i] << "\n";
        }
    }

    void search_notes(const std::string& query) const {
        auto notes = repository_->load_all();
        bool found = false;

        std::cout << "Search results:\n";
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].contains(query)) {
                std::cout << (i + 1) << ": " << notes[i] << "\n";
                found = true;
            }
        }

        if (!found) {
            std::cout << "No matching notes found.\n";
        }
    }

    void remove_note_by_index(int index) const {
        auto notes = repository_->load_all();

        if (notes.empty()) {
            std::cout << "No notes to remove.\n";
            return;
        }

        if (index < 1 || index > static_cast<int>(notes.size())) {
            std::cout << "Invalid note number.\n";
            return;
        }

        notes.erase(notes.begin() + (index - 1));
        repository_->save_all(notes);
        std::cout << "Note removed successfully.\n";
    }

    void remove_all_notes() const {
        repository_->clear_all();
        std::cout << "All notes removed.\n";
    }
};

class Menu {
private:
    std::unique_ptr<NoteService> note_service_;

    void show_menu_options() const {
        std::cout << "\n=== Todo List Manager ===\n";
        std::cout << "[1] Add note\n";
        std::cout << "[2] Show all notes\n";
        std::cout << "[3] Find note\n";
        std::cout << "[4] Remove one note\n";
        std::cout << "[5] Remove all notes\n";
        std::cout << "[6] Exit\n";
        std::cout << "Your choice: ";
    }

    int get_user_choice() const {
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            return -1;
        }
        std::string dummy;
        std::getline(std::cin, dummy);
        return choice;
    }

    std::string get_user_input(const std::string& prompt) const {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

    int get_note_index() const {
        int index = 0;
        std::cout << "Enter note number: ";
        if (!(std::cin >> index)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            return -1;
        }
        std::string dummy;
        std::getline(std::cin, dummy);
        return index;
    }

public:
    explicit Menu(std::unique_ptr<NoteService> service)
        : note_service_(std::move(service)) {}

    void run() {
        int choice = 0;

        while (true) {
            ConsoleUtils::clear_screen();
            show_menu_options();
            choice = get_user_choice();

            ConsoleUtils::clear_screen();

            switch (choice) {
            case 1: {
                std::string content = get_user_input("Enter note: ");
                note_service_->add_note(content);
                break;
            }
            case 2:
                note_service_->display_all_notes();
                break;
            case 3: {
                std::string query = get_user_input("Search query: ");
                note_service_->search_notes(query);
                break;
            }
            case 4:
                note_service_->display_all_notes();
                if (auto index = get_note_index(); index != -1) {
                    note_service_->remove_note_by_index(index);
                }
                else {
                    std::cout << "Invalid input.\n";
                }
                break;
            case 5:
                note_service_->remove_all_notes();
                break;
            case 6:
                std::cout << "Goodbye!\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }

            ConsoleUtils::pause_console();
        }
    }
};

int main() {
    auto repository = std::make_unique<NoteRepository>("todo.txt");
    auto service = std::make_unique<NoteService>(std::move(repository));
    auto menu = std::make_unique<Menu>(std::move(service));

    menu->run();
    return 0;
}