#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE 
#error  Please enable UNICODE for your compiler! VS: Project Properties -> General -> Advanced\
Character Set -> Use Unicode. Thanks! - Javidx9
#endif
/*
        (MANY OF BASE CONCEPT OF CONSOLE SETTING AND USING IS TAKEN FROM Javidx9)
         
         RESOURCE :
         1. JAVIDX9 OLC 
         2. FTXUI
         3. WINDOWS DOCUMENTATION
         4. ...
                   
                   Color Console Windows
        Author : KADDA Aoues
        Date   : 13 / 03 / 2024

        typedef struct _CHAR_INFO {
            union {
                 WCHAR UnicodeChar;
                 CHAR  AsciiChar;
            } Char;
            WORD Attributes;
        } CHAR_INFO, *PCHAR_INFO;
*/

/*

// Font Families
#define FF_DONTCARE         (0<<4)  // Don't care or don't know.
#define FF_ROMAN            (1<<4)  // Variable stroke width, serifed.
                                    // Times Roman, Century Schoolbook, etc.
#define FF_SWISS            (2<<4)  // Variable stroke width, sans-serifed.
                                    // Helvetica, Swiss, etc.
#define FF_MODERN           (3<<4)  // Constant stroke width, serifed or sans-serifed.
                                    // Pica, Elite, Courier, etc.
#define FF_SCRIPT           (4<<4)  // Cursive, etc.
#define FF_DECORATIVE       (5<<4)  // Old English, etc.

/* Font Weights
#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900


*/
// TODO : add blocking and no blocking pull events :


#include <include/console/ka_windows.hpp>
#include <include/basic/ka_mouse.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <string_view>
#include <bit>
#include <include/geometry/rectangle.hpp>
#include <include/Macros.hpp>





#define KA_CLAMP_CONDITION(x,y,rect) \
(x >= rect##.x && x < rect##.x + rect##.dx && y >= rect##.y && y < rect##.y +rect##.dy)


namespace console {


    typedef    CHAR_INFO   gchar;

    // Colour type 
    enum COLOUR
    {
        FG_BLACK = 0x0000,
        FG_DARK_BLUE = 0x0001,
        FG_DARK_GREEN = 0x0002,
        FG_DARK_CYAN = 0x0003,
        FG_DARK_RED = 0x0004,
        FG_DARK_MAGENTA = 0x0005,
        FG_DARK_YELLOW = 0x0006,
        FG_GREY = 0x0007, // Thanks MS :-/
        FG_DARK_GREY = 0x0008,
        FG_BLUE = 0x0009,
        FG_GREEN = 0x000A,
        FG_CYAN = 0x000B,
        FG_RED = 0x000C,
        FG_MAGENTA = 0x000D,
        FG_YELLOW = 0x000E,
        FG_WHITE = 0x000F,

        BG_BLACK = 0x0000,
        BG_DARK_BLUE = 0x0010,
        BG_DARK_GREEN = 0x0020,
        BG_DARK_CYAN = 0x0030,
        BG_DARK_RED = 0x0040,
        BG_DARK_MAGENTA = 0x0050,
        BG_DARK_YELLOW = 0x0060,
        BG_GREY = 0x0070,
        BG_DARK_GREY = 0x0080,
        BG_BLUE = 0x0090,
        BG_GREEN = 0x00A0,
        BG_CYAN = 0x00B0,
        BG_RED = 0x00C0,
        BG_MAGENTA = 0x00D0,
        BG_YELLOW = 0x00E0,
        BG_WHITE = 0x00F0,
    };

    // solid box
    enum W_4SOLID
    {
        SOLID = 0x2588,
        THREEQUARTERS = 0x2593,
        HALF = 0x2592,
        QUARTER = 0x2591,
    };

    // another definition
    enum W_SLD
    {
        S1P1 = AK_SOLID(88),
        S3P4 = AK_SOLID(93),
        S1P2 = AK_SOLID(92),
        S1P4 = AK_SOLID(91)
    };

    enum W_SOLID
    {
        SLD0 = 0x2580,
        SLD1,
        SLD2,
        SLD3,
        SLD4,
        SLD5,
        SLD6,
        SLD7,
        SLD8,
        SLD9,
        SLDA,
        SLDB,
        SLDC,
        SLDD,
        SLDE,
        SLDF
    };


    // Pricipale Class Colored Console
    class Console {

    public:

        Console()
            :hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) }
            , hConsoleIn{ GetStdHandle(STD_INPUT_HANDLE) }
            , m_bg_color(COLOUR::BG_BLACK)
            , m_bOpen{ true }
            , m_Ascii_close_char(KA_CTRL_C)
            , m_vkey(VK_ESCAPE)
            , m_fontw(14)
            , m_fonth(8)
            , m_DrawWindow{}
        {
            if (!GetConsoleMode(hConsole, &m_saveOldMode)) {
                std::cerr << "Error GetConsoleMode";
            }

            // Lunch thread to stop console
            std::thread thrStop([=] {
                while (m_bOpen) {
                    if (GetAsyncKeyState(m_vkey) & 0x8000) {
                        m_bOpen = false;
                    }
                }
                });

            thrStop.detach();
            
        }

        int construct_console(int width, int hight, int fontw, int fonth) {

            m_fontw = fontw;
            m_fonth = fonth;

            // 1. Check valid handle to output standard console
            if (hConsole == INVALID_HANDLE_VALUE) {
                return Error(L"Bad Handle");
            }

            m_lx = width;
            m_ly = hight;

            // 2. set as small possible the size of windows before resize windows buffer 
            m_rectWindow = { 0,0,1,1 };
            SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);

            // 3. set the size of screen buffer
            COORD  coord = { short(m_lx), short(m_ly) };
            if (!SetConsoleScreenBufferSize(hConsole, coord)) {
                return Error(L"SetConsoleScreenBufferSize");
            }


            // 4. Assign screen buffer to the console
            if (!SetConsoleActiveScreenBuffer(hConsole)) {
                Error(L"SetConsoleActiveScreenBuffer");
            }

            // 5. Set font size of screen buffer
            CONSOLE_FONT_INFOEX  cfi;
            cfi.cbSize = sizeof(cfi);
            cfi.nFont = 0;
            cfi.dwFontSize.X = fontw;
            cfi.dwFontSize.Y = fonth;
            cfi.FontFamily = FF_DONTCARE;
            cfi.FontWeight = FW_NORMAL;
            wcscpy_s(cfi.FaceName, L"Consolas");

            if (!SetCurrentConsoleFontEx(hConsole, false, &cfi)) {
                return Error(L"SetCurrentConsoleFontEx");
            }

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                return Error(L"GetConsoleScreenBufferInfo");
            }
            if (m_ly > csbi.dwMaximumWindowSize.Y)
                return Error(L"Screen Height / Font Height Too Big");
            if (m_lx > csbi.dwMaximumWindowSize.X)
                return Error(L"Screen Width / Font Width Too Big");

            // 6. Set physical Console Window Size
            m_rectWindow = { 0,0, (short)m_lx - 1, (short)m_ly - 1 };
            if (!SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow))
                return Error(L"SetConsoleWindowInfo");

            // Set flags to allow mouse input
            if (!SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
                return Error(L"SetConsoleMode");

            // 7. Allocate memory for screen buffer
            screen = new CHAR_INFO[m_lx * m_ly];

            memset(screen, 0, sizeof(CHAR_INFO) * m_lx * m_ly);
            // Set default draw Window
            m_DrawWindow.x = 0.f;
            m_DrawWindow.y = 0.f;
            m_DrawWindow.dx = float(m_lx);
            m_DrawWindow.dy = float(m_ly);

            // set Original window
            m_OriginWindow.x = 0.f;
            m_OriginWindow.y = 0.f;
            m_OriginWindow.dx = float(m_lx);
            m_OriginWindow.dy = float(m_ly);

            // Hide Cursor
            setCursorVisibility(0);


            return 1;
        }
        
        // Don't use this externally
        bool flush_InputBuffer() {
           return FlushConsoleInputBuffer(hConsoleIn);
        }

        HANDLE get_stdin() const {
            return hConsoleIn;
        }

        HANDLE get_stdout() const {
            return hConsole;
        }

        void set_drawWindows(float x, float y, float dx, float dy) {
            m_DrawWindow.x = x ;
            m_DrawWindow.y = y ;
            m_DrawWindow.dx = dx ;
            m_DrawWindow.dy = dy ;
        }

        fRect get_drawWindows() const {
            return m_DrawWindow;
        }

        void display() {
            WriteConsoleOutput(hConsole, screen, { (short)m_lx, (short)m_ly }, { 0,0 },
                (SMALL_RECT*)&m_rectWindow);

            // update mouse coordinate
           // pull_event();
        }

        fVec2 get_font_size() const {
            return fVec2(float(m_fontw), float(m_fonth));
        }

        int getX() const {
            return m_MouseX;
        }

        int getY() const {
            return m_MouseY;
        }

        COORD get_mouseXY() const {
            return { short(m_MouseX), short(m_MouseY) };
        }

        // Mouse Event 
        Mouse get_mouse() const {
            return m_Mouse;
        }

        bool keyboard_On() const {
            return m_KeyEvent;
        }

        bool key_released() {
            return m_KeyReleased;
        }

        bool key_pressed() {
            return m_KeyPressed;
        }

        bool mouse_move() const {
            return m_MouseMove;
        }

        void set_background_color(int bg_color) {
            m_bg_color = bg_color;
        }

        constexpr int get_background_color() const {
            return m_bg_color;
        }

        void clear(COLOUR  bg_color) {
            for (int i = 0; i < m_lx; ++i)
                for (int j = 0; j < m_ly; ++j)
                    draw(i, j, L' ', bg_color);

        }

        void clear() {
            // memset(screen, 0 , sizeof(CHAR_INFO) * m_lx * m_ly);
            clear((COLOUR)m_bg_color);
        }

        // Main draw functions
        void draw(int x, int y, short c = 0x2588, short color = 0x000F) {

            if (KA_CLAMP_CONDITION(x,y,m_OriginWindow)) {
                screen[y * m_lx + x].Char.UnicodeChar = c;
                screen[y * m_lx + x].Attributes = color;
            }
        }

        void text_at(int x, int y, std::wstring_view text, short _color = 0x000F) {

            for (int i = 0; i < text.size(); ++i)
                draw(x + i, y, (short)text[i], _color);
        }

        int  text_lines_at(int x, int y, const std::wstring& text, short color = 0x000F)
        {
            std::wstringstream ss(text);
            std::wstring str;
            int i{};
            while (std::getline(ss, str)) {   // get line after /r and /n;
                text_at(x, y + i, str, color);
                ++i;
            }

            return i;
        }

        void draw(int x, int y, int dx, int dy, short c, short color = COLOUR::BG_BLACK) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    draw(x + i, y + j, c, color);
        }

        

        ~Console() {
            delete[] screen;
            if (!SetConsoleMode(hConsole, m_saveOldMode)) {
                std::cerr << "Error SetConsoleMode";
            }
            CloseHandle(hConsole);
            CloseHandle(hConsoleIn);
        }

        void setCursorVisibility(char visible) {
            CONSOLE_CURSOR_INFO      structCursorInfo;
            GetConsoleCursorInfo(hConsole, &structCursorInfo);  // Get current cursor size
            structCursorInfo.bVisible = (visible ? TRUE : FALSE);
            SetConsoleCursorInfo(hConsole, &structCursorInfo);
        }

        void setCursorVolume(int x) {
            CONSOLE_CURSOR_INFO cursor;

            GetConsoleCursorInfo(hConsole, &cursor);

            cursor.bVisible = TRUE;
            cursor.dwSize = 100;

            SetConsoleCursorInfo(hConsole, &cursor);

        }

        void setCursorXY(int x, int y) {

        }

        constexpr POINT get_size() const {
            return POINT{ .x = m_lx, .y = m_ly };
        }

        constexpr int width() const {
            return m_lx;
        }

        constexpr int hight() const {
            return m_ly;
        }

        constexpr bool is_open() const {
            return m_bOpen;
        }

        bool pull_event() const {
            INPUT_RECORD ir{};
            DWORD Nevents{};
            ReadConsoleInput(hConsoleIn, &ir, 1, &Nevents);

            return m_bOpen;
        }

        void close() {
            m_bOpen = false;
        }

        wchar_t get_AsciiKey() const {
            return m_Ascii;
        }

        bool pull_event() {
            INPUT_RECORD irb[32];
            DWORD events{};

            GetNumberOfConsoleInputEvents(hConsoleIn, &events);
            if (events > 0) {
                ReadConsoleInput(hConsoleIn, irb, events, &events);
            }
            else {
                m_KeyEvent = false;
                m_MouseEvent = false;
                m_MouseMove = false;
                m_KeyReleased = false;
                m_KeyPressed = false;
                return false;
            }

            for (DWORD i = 0; i < events; i++) {
                switch (irb[i].EventType)
                {
                case MOUSE_EVENT:
                    m_MouseEvent = true;
                    m_MouseX = irb[i].Event.MouseEvent.dwMousePosition.X;
                    m_MouseY = irb[i].Event.MouseEvent.dwMousePosition.Y;
                    m_MouseMove = irb[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED;
                    console::process_mouse_event(m_Mouse, irb[i].Event.MouseEvent);
                    return true;
                    
                case KEY_EVENT:
                    m_KeyEvent = true;
                    m_Ascii = irb[i].Event.KeyEvent.uChar.UnicodeChar;
                    m_KeyReleased = irb[i].Event.KeyEvent.bKeyDown == FALSE;
                    m_KeyPressed = irb[i].Event.KeyEvent.bKeyDown == TRUE;
                    return true;
                }
            }
        }

    protected:

        void clip(int& x, int& y) {

            if (x < 0) x = 0;
            if (x >= m_lx) x = m_lx;
            if (y < 0) y = 0;
            if (y >= m_ly) y = m_ly;

        }

        int Error(const wchar_t* msg)
        {
            wchar_t buf[256];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
            SetConsoleActiveScreenBuffer(hOriginalConsole);
            wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
            return 0;
        }


    private:
        int                m_lx;
        int                m_ly;
        int                m_bg_color;
        bool               m_bOpen;
        char               m_Ascii_close_char;
        short              m_vkey;
        wchar_t            m_Ascii;

        int                m_MouseX;
        int                m_MouseY;
        bool               m_KeyEvent{ false };
        bool               m_MouseEvent{ false };
        bool               m_MouseMove{false};
        bool               m_KeyReleased{ false };
        bool               m_KeyPressed{ false };
        int                m_fontw;
        int                m_fonth;

        SMALL_RECT         m_rectWindow;
        fRect              m_DrawWindow;
        fRect              m_OriginWindow;
        DWORD              m_saveOldMode;
        HANDLE             hConsole;
        HANDLE             hOriginalConsole;
        HANDLE             hConsoleIn;

        Mouse              m_Mouse;

        CHAR_INFO*         screen;

    };


    template<typename Drawable>
    void draw(Drawable* obj) {   // should be have draw function
        obj->draw();
    }
}

using COLOR = console::COLOUR;
#undef KA_CLAMP_CONDITION