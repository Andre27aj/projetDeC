# cmake

project(Projet_c C)

set(CMAKE_C_STANDARD 11)

add_executable(Projet_c
        main.c
        cell.c
        fonctions.c
        tarjan.c
        hasse.c
)
# if (WIN32)
#     add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
# endif

configure_file(exemple_invalide.txt exemple_invalide.txt COPYONLY)