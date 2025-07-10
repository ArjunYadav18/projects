#include <windows.h>
#include <string>

#define BTN_0 100
#define BTN_1 101
#define BTN_2 102
#define BTN_3 103
#define BTN_4 104
#define BTN_5 105
#define BTN_6 106
#define BTN_7 107
#define BTN_8 108
#define BTN_9 109
#define BTN_ADD 110
#define BTN_SUB 111
#define BTN_MUL 112
#define BTN_DIV 113
#define BTN_EQUAL 114
#define BTN_CLEAR 115

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
char displayText[50] = "";
HWND hwndDisplay;

void AppendText(const char* text) {
    strcat_s(displayText, text);
    SetWindowText(hwndDisplay, displayText);
}

void ClearText() {
    strcpy_s(displayText, "");
    SetWindowText(hwndDisplay, displayText);
}

void CalculateResult() {
    double num1, num2, result;
    char op;
    sscanf_s(displayText, "%lf %c %lf", &num1, &op, 1, &num2);
    
    switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if (num2 != 0)
                result = num1 / num2;
            else {
                MessageBox(NULL, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                ClearText();
                return;
            }
            break;
        default: return;
    }

    sprintf_s(displayText, "%.2lf", result);
    SetWindowText(hwndDisplay, displayText);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "CalculatorClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc))
        return -1;

    CreateWindow("CalculatorClass", "Windows Calculator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        300, 200, 250, 350, NULL, NULL, hInst, NULL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hwndDisplay = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT, 
                20, 20, 200, 30, hWnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "7", WS_VISIBLE | WS_CHILD, 20, 60, 50, 50, hWnd, (HMENU)BTN_7, NULL, NULL);
            CreateWindow("BUTTON", "8", WS_VISIBLE | WS_CHILD, 70, 60, 50, 50, hWnd, (HMENU)BTN_8, NULL, NULL);
            CreateWindow("BUTTON", "9", WS_VISIBLE | WS_CHILD, 120, 60, 50, 50, hWnd, (HMENU)BTN_9, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD, 170, 60, 50, 50, hWnd, (HMENU)BTN_DIV, NULL, NULL);

            CreateWindow("BUTTON", "4", WS_VISIBLE | WS_CHILD, 20, 110, 50, 50, hWnd, (HMENU)BTN_4, NULL, NULL);
            CreateWindow("BUTTON", "5", WS_VISIBLE | WS_CHILD, 70, 110, 50, 50, hWnd, (HMENU)BTN_5, NULL, NULL);
            CreateWindow("BUTTON", "6", WS_VISIBLE | WS_CHILD, 120, 110, 50, 50, hWnd, (HMENU)BTN_6, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD, 170, 110, 50, 50, hWnd, (HMENU)BTN_MUL, NULL, NULL);

            CreateWindow("BUTTON", "1", WS_VISIBLE | WS_CHILD, 20, 160, 50, 50, hWnd, (HMENU)BTN_1, NULL, NULL);
            CreateWindow("BUTTON", "2", WS_VISIBLE | WS_CHILD, 70, 160, 50, 50, hWnd, (HMENU)BTN_2, NULL, NULL);
            CreateWindow("BUTTON", "3", WS_VISIBLE | WS_CHILD, 120, 160, 50, 50, hWnd, (HMENU)BTN_3, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD, 170, 160, 50, 50, hWnd, (HMENU)BTN_SUB, NULL, NULL);

            CreateWindow("BUTTON", "0", WS_VISIBLE | WS_CHILD, 20, 210, 50, 50, hWnd, (HMENU)BTN_0, NULL, NULL);
            CreateWindow("BUTTON", "C", WS_VISIBLE | WS_CHILD, 70, 210, 50, 50, hWnd, (HMENU)BTN_CLEAR, NULL, NULL);
            CreateWindow("BUTTON", "=", WS_VISIBLE | WS_CHILD, 120, 210, 50, 50, hWnd, (HMENU)BTN_EQUAL, NULL, NULL);
            CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD, 170, 210, 50, 50, hWnd, (HMENU)BTN_ADD, NULL, NULL);
            break;

        case WM_COMMAND:
            switch (wp) {
                case BTN_0: AppendText("0"); break;
                case BTN_1: AppendText("1"); break;
                case BTN_2: AppendText("2"); break;
                case BTN_3: AppendText("3"); break;
                case BTN_4: AppendText("4"); break;
                case BTN_5: AppendText("5"); break;
                case BTN_6: AppendText("6"); break;
                case BTN_7: AppendText("7"); break;
                case BTN_8: AppendText("8"); break;
                case BTN_9: AppendText("9"); break;
                case BTN_ADD: AppendText(" + "); break;
                case BTN_SUB: AppendText(" - "); break;
                case BTN_MUL: AppendText(" * "); break;
                case BTN_DIV: AppendText(" / "); break;
                case BTN_EQUAL: CalculateResult(); break;
                case BTN_CLEAR: ClearText(); break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}
