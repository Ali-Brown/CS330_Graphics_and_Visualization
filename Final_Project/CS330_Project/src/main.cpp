#include <iostream>         // cout, cerr
#include <application.h>

int main(int argc, char* argv[]) {
    Application app{ "CS330_OpenGL_Project", 800, 600 };

    app.Run();
    return 0;
}