#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

#include <sys/types.h>

#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL

#include "../include/RGFW.h"
#include "../include/glyph.h"

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

auto start = high_resolution_clock::now();
long long savedTime = 0;
bool running = false;
fs::path homeDir = getenv("HOME");
fs::path filePath = homeDir / ".clc" / "lt";


void saveTime(long long total_ms)
{
    if (!fs::exists(filePath.parent_path()))
    {
        fs::create_directory(filePath.parent_path());
    }
    ofstream outFile(filePath);
    if (outFile.is_open())
    {
        outFile << total_ms;
        outFile.close();
    }
    else
    {
        cerr << total_ms << filePath << "\n";
        cerr << "clc error : can't store last data time" << endl;
    }
}

long long loadTime()
{
    ifstream inFile(filePath);
    long long previousTime = 0;
    if (inFile.is_open())
    {
        inFile >> previousTime;
        inFile.close();
    }
    return previousTime;
}


void onExit() 
{
    auto now = high_resolution_clock::now();
    if (running) 
    {
        savedTime += duration_cast<milliseconds>(now - start).count();
    }
    std::cerr << savedTime << "\n";
    saveTime(savedTime);
}

int main()
{
    atexit(onExit);
    
    savedTime = loadTime();
    start = high_resolution_clock::now();
    
    RGFW_window* RGFW_window_obj = RGFW_createWindow("clc.", 0, 0, 500, 300, RGFW_windowNoBorder || RGFW_windowNoResize);

    bool dragging = false;

    glyph_renderer_t renderer = glyph_renderer_create("font.ttf", 64.0f,NULL, GLYPH_UTF8, NULL, 0);
    glyph_renderer_set_projection(&renderer, 800, 600);

    while(RGFW_window_shouldClose(RGFW_window_obj) == false)
    {
    
        auto time = high_resolution_clock::now();
        RGFW_event RGFW_event_obj;

        while(RGFW_window_checkEvent(RGFW_window_obj, &RGFW_event_obj))
        {
            if(RGFW_event_obj.type == RGFW_keyPressed && RGFW_event_obj.button.value == RGFW_space)    
            {
                /// stop and start clock func's
                
                            
            }
        }

    /// graphic interface
    
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render text
        glyph_renderer_draw_text(&renderer, "clc test",
                                50.0f, 300.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
    
        RGFW_window_swapBuffers_OpenGL(RGFW_window_obj);
        glFlush();        

    }
        
    return 0;
}
