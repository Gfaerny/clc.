#include <X11/X.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cmath>

#include <string>
#include <sys/types.h>

#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL

#define GLYPHGL_MINIMAL

#include "../include/RGFW/RGFW.h"
#include "../include/glyph/glyph.h"

#include <GL/gl.h>


using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

auto start = high_resolution_clock::now();

double savedTime = 0 , output_time = 0;

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

double loadTime()
{
    ifstream inFile(filePath);
    double previousTime = 0;
    if (inFile.is_open())
    {
        inFile >> previousTime;
        inFile.close();
    }
    else
    {
        std::printf("failed to open ~/.clc/lt file.\nclc could't load your last time_point");
        return 0;
    }
    return previousTime;
}

void on_exit() 
{
    auto now = high_resolution_clock::now();
    if (running) 
    {
        savedTime += duration_cast<milliseconds>(now - start).count();
    }
    std::cerr << savedTime << "\n";
    saveTime(savedTime);
}

int main(void)
{
///  atexit(on_exit);

    double additional_time , last_time = 0;
/// if order_time_stop == true mean's timer has stop counting
    bool order_time_stop = true;
/*
    if(!(loadTime() == 0))
    {
        savedTime = loadTime();
    }
*/

    uint8_t min = 0 , hour = 0 , sec = 0;  
    
    RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
    hints->major = 3;
    hints->minor = 3;
    RGFW_setGlobalHints_OpenGL(hints);

   /// RGFW_window* RGFW_window_obj = RGFW_createWindow("clc.", 0, 0, 500, 300, RGFW_windowNoBorder || RGFW_windowNoResize || RGFW_windowOpenGL);
    RGFW_window* RGFW_window_obj = RGFW_createWindow("clc.", 0, 0, 500, 300, RGFW_windowCenter || RGFW_windowNoResize ||/*RGFW_windowNoBorder ||*/ RGFW_windowHide);
    RGFW_window_createContext_OpenGL(RGFW_window_obj, hints);

    RGFW_window_show(RGFW_window_obj);

    const GLubyte *version = glGetString(GL_VERSION);

    glyph_renderer_t renderer = glyph_renderer_create("font.ttf", 135.0f,NULL, GLYPH_UTF8, NULL, 0);
    glyph_renderer_set_projection(&renderer, 800, 600);
    ///glyph_renderer_create(const char *font_path, float pixel_height, const char *charset, uint32_t char_type, void *effect, int use_sdf)

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string t_str = "";
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time_before_stop;
    std::chrono::time_point<high_resolution_clock> start_time; 
    
    while(RGFW_window_shouldClose(RGFW_window_obj) == false)
    {
        auto time_point_t1 = high_resolution_clock::now();
        RGFW_event RGFW_event_obj;

        while(RGFW_window_checkEvent(RGFW_window_obj, &RGFW_event_obj))
        {
            if(RGFW_event_obj.type == RGFW_keyPressed && RGFW_event_obj.button.value == RGFW_space)    
            {
                /// stop and start clock func's
                if(order_time_stop == true)
                {
                    order_time_stop = false;
                    start_time = high_resolution_clock::now();
                    savedTime = output_time;
                }
                else
                {
                    ///savedTime += duration<double>(now - start).count();
                    order_time_stop = true;
                    savedTime = output_time;
                }
            }
        }

    /// graphic interface    
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if(order_time_stop)
        {
            savedTime = output_time;
            /*
            sec = std::fmod(output_time , 60);
            min = (output_time / 60);
            hour = output_time / 3600;
            /*
            std::cout <<"fmod :" << test<<'\n'  << "output_time solied : " << output_time << '\n';
*//*
            if(hour >= 1)
            {
                t_str = std::to_string(hour);
                t_str += "." + std::to_string(min);
                t_str += "." + std::to_string(sec);
            }

            else if(min >= 1)
            {
                t_str = std::to_string(min);
                t_str += "." +std::to_string(sec);
            }
          */
            t_str = std::to_string(output_time);
            t_str.erase(5);
            
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
            
///         
            last_time_before_stop = high_resolution_clock::now();
        }
        else
        {    
            auto now_time = high_resolution_clock::now();
            output_time = duration<double>(now_time - start_time).count();
            std::cout << output_time + savedTime << '\n' << output_time << savedTime;
            output_time+=savedTime;
            
            t_str = std::to_string(output_time);
            t_str.erase(5);
        
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        }
        glyph_renderer_draw_text(&renderer,"clc.", 10, 100, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        RGFW_window_swapBuffers_OpenGL(RGFW_window_obj);

    }
    RGFW_window_close(RGFW_window_obj);
    return 0;
}
