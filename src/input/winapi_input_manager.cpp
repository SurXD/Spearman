#include "winapi_input_manager.h"

int winapi_input_manager::key_to_winapi_key(key k)
{
    switch (k) 
    {
        case key::LEFT: return VK_LEFT;
        case key::RIGHT: return VK_RIGHT;
        case key::UP: return VK_UP;
        case key::DOWN: return VK_DOWN;

        case key::A: return 'A';
        case key::B: return 'B';
        case key::C: return 'C';
        case key::D: return 'D';
        case key::E: return 'E';
        case key::F: return 'F';
        case key::G: return 'G';
        case key::H: return 'H';
        case key::I: return 'I';
        case key::J: return 'J';
        case key::K: return 'K';
        case key::L: return 'L';
        case key::M: return 'M';
        case key::N: return 'N';
        case key::O: return 'O';
        case key::P: return 'P';
        case key::Q: return 'Q';
        case key::R: return 'R';
        case key::S: return 'S';
        case key::T: return 'T';
        case key::U: return 'U';
        case key::V: return 'V';
        case key::W: return 'W';
        case key::X: return 'X';
        case key::Y: return 'Y';
        case key::Z: return 'Z';

        case key::NUM0: return '0';
        case key::NUM1: return '1';
        case key::NUM2: return '2';
        case key::NUM3: return '3';
        case key::NUM4: return '4';
        case key::NUM5: return '5';
        case key::NUM6: return '6';
        case key::NUM7: return '7';
        case key::NUM8: return '8';
        case key::NUM9: return '9';

        case key::SPACE: return VK_SPACE;
        case key::ENTER: return VK_RETURN;
        case key::ESCAPE: return VK_ESCAPE;
        case key::SHIFT: return VK_SHIFT;
        case key::CONTROL: return VK_CONTROL;
        case key::TAB: return VK_TAB;

        case key::F1: return VK_F1;
        case key::F2: return VK_F2;
        case key::F3: return VK_F3;
        case key::F4: return VK_F4;
        case key::F5: return VK_F5;
        case key::F6: return VK_F6;
        case key::F7: return VK_F7;
        case key::F8: return VK_F8;
        case key::F9: return VK_F9;
        case key::F10: return VK_F10;
        case key::F11: return VK_F11;
        case key::F12: return VK_F12;

        default: return 0;
    }
}

winapi_input_manager::winapi_input_manager()
{
}

bool winapi_input_manager::is_pressed(key k)
{
    return keys[static_cast<size_t>(k)];
}

void winapi_input_manager::pull_events()
{
    for(size_t i = 0; i < KEYS_COUNT; i++)
    {
        int winapi_key = key_to_winapi_key(static_cast<key>(i));
        keys[i] = (GetAsyncKeyState(winapi_key) & 0x8000) != 0;
    }
}
