#include "platform.h"

#ifdef __PLATFORM_WINDOWS__

#include <windows.h>
#include <thread>

namespace Platform
{

    static std::chrono::time_point<std::chrono::steady_clock> g_start_time;
    static struct platform_state *g_state_ptr; 

    typedef struct internal_state
    {
        HINSTANCE h_instance;
        HWND hwnd;
    } internal_state;

    typedef struct window_rect
    {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
    } window_rect;

    LRESULT CALLBACK win32_process_message(HWND hwnd, 
                                           uint32_t msg, 
                                           WPARAM w_param, 
                                           LPARAM l_param);

    int8_t platform_system_startup(platform_state *platform_state_,
                                   const char *application_name_,
                                   uint32_t x_,
                                   uint32_t y_,
                                   uint32_t width_,
                                   uint32_t height_)
    {
        g_state_ptr = platform_state_;
        auto h_instance = GetModuleHandleA(0);

        platform_state_->internal_state = std::make_shared<struct internal_state>();
        auto local_internal_state = std::static_pointer_cast<struct internal_state>( platform_state_->internal_state );
        local_internal_state->h_instance = h_instance;

        // Setup and register window class.
        HICON icon = LoadIcon( h_instance, IDI_APPLICATION);
        WNDCLASSA wc = {};

        // evidence there is no god
        wc.style = CS_DBLCLKS;  // Get double-clicks
        wc.lpfnWndProc = win32_process_message;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = h_instance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
        wc.hbrBackground = NULL;                   // Transparent
        wc.lpszClassName = "viverrinus_window_class";

        if( !RegisterClassA( &wc ) )
        {
            MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        uint32_t client_x = x_;
        uint32_t client_y = y_;
        uint32_t client_width = width_;
        uint32_t client_height = height_;

        uint32_t window_x = client_x;
        uint32_t window_y = client_y;
        uint32_t window_width = client_width;
        uint32_t window_height = client_height;

        uint32_t window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        uint32_t window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_THICKFRAME;

        // Obtain the size of the border.
        RECT border_rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        // In this case, the border rectangle is negative.
        window_x += border_rect.left;
        window_y += border_rect.top;

            // Grow by the size of the OS border.
        window_width += border_rect.right - border_rect.left;
        window_height += border_rect.bottom - border_rect.top;

        HWND handle = CreateWindowExA(window_ex_style, 
                                    "viverrinus_window_class", 
                                    application_name_,
                                    window_style, 
                                    window_x, 
                                    window_y, 
                                    window_width, 
                                    window_height,
                                    0, 0, h_instance, 0);

        if (handle == 0) 
        {
            MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            LOG_FATAL("Window creation failed!");
            return false;
        } 
        else 
        {
            local_internal_state->hwnd = handle;
        }

        // Show the window
        bool should_activate = 1;  // TODO: if the window should not accept input, this should be false.
        int32_t show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
        // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
        ShowWindow(local_internal_state->hwnd, show_window_command_flags);

        g_start_time = std::chrono::steady_clock::now();

        return true;
    }

    void platform_sleep( uint64_t milliseconds_ )
    {
        std::chrono::milliseconds milliseconds( milliseconds_ );
        std::this_thread::sleep_for( milliseconds );
    }

    std::chrono::steady_clock::duration get_absolute_time()
    {
        auto current_time = std::chrono::steady_clock::now();
        return current_time - g_start_time;
    }

    void platform_system_shutdown() 
    {
        auto local_internal_state = std::static_pointer_cast<struct internal_state>( g_state_ptr->internal_state );

        if ( !local_internal_state && local_internal_state->hwnd) 
        {
            DestroyWindow( local_internal_state->hwnd);
            local_internal_state->hwnd = NULL;
        }
    }

    int8_t platform_pump_messages() 
    {
        if (g_state_ptr) {
            MSG message;
            while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) 
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
        }
        return true;
    }

    void console_write( HANDLE output_handle_,
                        const char *message_,
                        uint8_t color_)
    {
        // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
        static uint8_t levels[6] = {64, 4, 6, 2, 1, 8};
        SetConsoleTextAttribute(output_handle_, levels[ color_ ]);
        OutputDebugStringA( message_ );
        size_t length = strlen( message_ );
        LPDWORD number_written = 0;
        WriteConsoleA( GetStdHandle(STD_OUTPUT_HANDLE), 
                       message_, 
                       static_cast<DWORD>( length ), 
                       number_written, 0);
    }

    void platform_console_write( const char *message_, 
                                 uint8_t color_) 
    {
        console_write( GetStdHandle(STD_OUTPUT_HANDLE),
                       message_,
                       color_ );
    }

    void platform_console_write_erro( const char *message_, 
                                      uint8_t color_)
    {
        console_write( GetStdHandle(STD_ERROR_HANDLE),
                       message_,
                       color_ );
    }

    LRESULT CALLBACK win32_process_message(HWND hwnd, uint32_t msg, WPARAM w_param, LPARAM l_param) 
    {
    
        LRESULT ret = 0;

        switch (msg) 
        {
            case WM_ERASEBKGND:
            {
                // Notify the OS that erasing will be handled by the application to prevent flicker.
                return 1;
            }
            break;

            case WM_CLOSE:
            {
                // TODO: Fire an event for the application to quit.
           //     event_context data = {};
          //      event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
          //      return 0;
            }
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }

            case WM_SIZE: 
            {
                // Get the updated size.
                RECT r;
                GetClientRect(hwnd, &r);

                uint32_t width = r.right - r.left;
                uint32_t height = r.bottom - r.top;

                return 0;
            } 
            break;

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: 
            {
 

                // Return 0 to prevent default window behaviour for some keypresses, such as alt.
                return 0;
            }

            case WM_MOUSEMOVE: 
            {
               return 0;
            } 
            break;

            case WM_MOUSEWHEEL: 
            {
                 return 0;
            } 
            break;

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: 
            {
                uint8_t pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            } break;
        }

        return DefWindowProcA(hwnd, msg, w_param, l_param);
    }
}

#endif
