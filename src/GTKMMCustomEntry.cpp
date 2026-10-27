#include "GTKMMCustomEntry.h"

#include <cassert>
#include <glibmm/main.h>

GTKMMCustomEntry::GTKMMCustomEntry() {
    current_type_ = Type::Characters;
    set_input_purpose(Gtk::InputPurpose::ALPHA);
    signal_changed().connect(sigc::mem_fun(*this, &GTKMMCustomEntry::ChangeAction));
}

GTKMMCustomEntry::GTKMMCustomEntry(int min, int max) {
    current_type_ = Type::Numeric;
    min_ = min < max ? min : max;
    max_ = max > min ? max : min;
    assert(min != max);
    set_input_purpose(Gtk::InputPurpose::DIGITS);
    set_max_length(std::to_string(max).length());
    set_placeholder_text(std::to_string(min) + "-" + std::to_string(max));
    signal_changed().connect(sigc::mem_fun(*this, &GTKMMCustomEntry::ChangeAction));
}

std::string GTKMMCustomEntry::GetVersion() { return GTKMMCUSTOMENTRY_VERSION; }

void GTKMMCustomEntry::ChangeAction() {
    if (is_updating_) return;
    is_updating_ = true;
    Glib::signal_idle().connect_once(sigc::mem_fun(*this, &GTKMMCustomEntry::IdleAction));
}
void GTKMMCustomEntry::IdleAction() {
    std::string current_text = get_text();
    std::string new_text;
    bool modified = false;

    for (char c: current_text) {
        if (current_type_ == Type::Numeric && std::isdigit(c)) {
            new_text += c;
        } else if (current_type_ == Type::Characters && ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))) {
            new_text += c;
        } else {
            modified = true;
        }
    }

    if (modified) {
        int cursor_pos = get_position();
        set_text(new_text);
        set_position(std::max(0, cursor_pos - 1));
    }

    if (current_type_ == Type::Numeric && !get_text().empty() &&
        (std::stoi(get_text()) > max_ || std::stoi(get_text()) < min_)) {
        set_text("");
        set_position(std::max(0, 0));
    }

    is_updating_ = false;
}
