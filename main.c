// --- main.c (remplacement complet) ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "fonctions.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdarg.h>

static void setup_console_utf8(void) {
    setlocale(LC_ALL, ".UTF-8");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

static void print_utf8(const char* s) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE && GetFileType(h) == FILE_TYPE_CHAR) {
        int wlen = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
        if (wlen > 0) {
            wchar_t* wbuf = (wchar_t*)malloc((size_t)wlen * sizeof(wchar_t));
            if (wbuf) {
                MultiByteToWideChar(CP_UTF8, 0, s, -1, wbuf, wlen);
                DWORD written;
                WriteConsoleW(h, wbuf, (DWORD)(wlen - 1), &written, NULL);
                free(wbuf);
            }
        }
        return;
    }
#endif
    fputs(s, stdout);
}

static void print_utf8f(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0) return;
    char* buf = (char*)malloc((size_t)n + 1);
    if (!buf) return;
    va_start(ap, fmt);
    vsnprintf(buf, (size_t)n + 1, fmt, ap);
    va_end(ap);
    print_utf8(buf);
    free(buf);
}

static const char* ask_input_file(void) {
    int choice = 0;
    printf("Selectionnez le fichier d'entree:\n");
    printf("  1) exemple1.txt\n");
    printf("  2) exemple_invalide.txt\n");
    printf("  3) exemple2.txt\n");
    printf("  4) exemple_invalide2.txt\n");
    printf("Votre choix: ");
    fflush(stdout);
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Entree invalide, defaut: exemple1.txt\n");
        return "exemple1.txt";
    }
    switch (choice) {
        case 2: return "exemple_invalide.txt";
        case 3: return "exemple2.txt";
        case 4: return "exemple_invalide2.txt";
        default: return "exemple1.txt";
    }
}

int main(int argc, char** argv) {
    setup_console_utf8();

    int force_menu = (argc >= 2 && strcmp(argv[1], "--menu") == 0);

    const char* in  = (!force_menu && argc >= 2) ? argv[1] : ask_input_file();
    const char* out = (argc >= 3) ? argv[2] : "partie_1.txt";

    AdjList g = readGraph(in);

    print_adj(&g);
    bool ok = verify_markov(&g);

    if (ok) {
        export_mermaid(&g, out);
    } else {
        printf("Graphe invalide: aucun export vers `%s`.\n", out);
    }

    adj_free(&g);
    return ok ? 0 : 2;
}
