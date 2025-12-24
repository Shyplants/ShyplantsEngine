#include "Engine/PCH/EnginePCH.h"

#include "Engine/Platform/Windows/WindowsInput.h"
#include "Engine/Platform/Input/KeyboardDevice.h"

// -------------------------------------------------
// Key translation
// -------------------------------------------------

KeyCode WindowsInput::TranslateKey(WPARAM wParam)
{
    switch (wParam)
    {
        // -----------------------------
        // Alphabet
        // -----------------------------
        case 'A': return KeyCode::A;
        case 'B': return KeyCode::B;
        case 'C': return KeyCode::C;
        case 'D': return KeyCode::D;
        case 'E': return KeyCode::E;
        case 'F': return KeyCode::F;
        case 'G': return KeyCode::G;
        case 'H': return KeyCode::H;
        case 'I': return KeyCode::I;
        case 'J': return KeyCode::J;
        case 'K': return KeyCode::K;
        case 'L': return KeyCode::L;
        case 'M': return KeyCode::M;
        case 'N': return KeyCode::N;
        case 'O': return KeyCode::O;
        case 'P': return KeyCode::P;
        case 'Q': return KeyCode::Q;
        case 'R': return KeyCode::R;
        case 'S': return KeyCode::S;
        case 'T': return KeyCode::T;
        case 'U': return KeyCode::U;
        case 'V': return KeyCode::V;
        case 'W': return KeyCode::W;
        case 'X': return KeyCode::X;
        case 'Y': return KeyCode::Y;
        case 'Z': return KeyCode::Z;

        // -----------------------------
        // Numbers (Top row)
        // -----------------------------
        case '0': return KeyCode::Num0;
        case '1': return KeyCode::Num1;
        case '2': return KeyCode::Num2;
        case '3': return KeyCode::Num3;
        case '4': return KeyCode::Num4;
        case '5': return KeyCode::Num5;
        case '6': return KeyCode::Num6;
        case '7': return KeyCode::Num7;
        case '8': return KeyCode::Num8;
        case '9': return KeyCode::Num9;

        // -----------------------------
        // Function keys
        // -----------------------------
        case VK_F1:  return KeyCode::F1;
        case VK_F2:  return KeyCode::F2;
        case VK_F3:  return KeyCode::F3;
        case VK_F4:  return KeyCode::F4;
        case VK_F5:  return KeyCode::F5;
        case VK_F6:  return KeyCode::F6;
        case VK_F7:  return KeyCode::F7;
        case VK_F8:  return KeyCode::F8;
        case VK_F9:  return KeyCode::F9;
        case VK_F10: return KeyCode::F10;
        case VK_F11: return KeyCode::F11;
        case VK_F12: return KeyCode::F12;

        // -----------------------------
        // Modifiers
        // -----------------------------
        case VK_LSHIFT: return KeyCode::LeftShift;
        case VK_RSHIFT: return KeyCode::RightShift;
        case VK_LCONTROL: return KeyCode::LeftCtrl;
        case VK_RCONTROL: return KeyCode::RightCtrl;
        case VK_LMENU: return KeyCode::LeftAlt;
        case VK_RMENU: return KeyCode::RightAlt;

        // -----------------------------
        // Navigation
        // -----------------------------
        case VK_LEFT:  return KeyCode::ArrowLeft;
        case VK_RIGHT: return KeyCode::ArrowRight;
        case VK_UP:    return KeyCode::ArrowUp;
        case VK_DOWN:  return KeyCode::ArrowDown;

        case VK_HOME:  return KeyCode::Home;
        case VK_END:   return KeyCode::End;
        case VK_PRIOR: return KeyCode::PageUp;
        case VK_NEXT:  return KeyCode::PageDown;

        // -----------------------------
        // Special
        // -----------------------------
        case VK_SPACE:  return KeyCode::Space;
        case VK_RETURN: return KeyCode::Enter;
        case VK_ESCAPE:return KeyCode::Escape;
        case VK_TAB:   return KeyCode::Tab;
        case VK_BACK:  return KeyCode::Backspace;
        case VK_INSERT:return KeyCode::Insert;
        case VK_DELETE:return KeyCode::Delete;

        default:
            return KeyCode::Unknown;
    }
}


// -------------------------------------------------
// Message handler
// -------------------------------------------------

void WindowsInput::HandleMessage(
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if (!(lParam & (1 << 30))) // 이전 상태가 up일 때만
        {
            KeyCode key = TranslateKey(wParam);
            if (key != KeyCode::Unknown)
                KeyboardDevice::SetKeyDown(static_cast<uint32>(key), true);
        }
        break;
    }

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        KeyCode key = TranslateKey(wParam);
        if (key != KeyCode::Unknown)
            KeyboardDevice::SetKeyDown(static_cast<uint32>(key), false);
        break;
    }

    case WM_LBUTTONDOWN:
        KeyboardDevice::SetKeyDown(static_cast<uint32>(KeyCode::MouseLeft), true);
        break;

    case WM_LBUTTONUP:
        KeyboardDevice::SetKeyDown(static_cast<uint32>(KeyCode::MouseLeft), false);
        break;

    case WM_RBUTTONDOWN:
        KeyboardDevice::SetKeyDown(static_cast<uint32>(KeyCode::MouseRight), true);
        break;

    case WM_RBUTTONUP:
        KeyboardDevice::SetKeyDown(static_cast<uint32>(KeyCode::MouseRight), false);
        break;
    }
}
