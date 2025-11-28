#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <sys/types.h>
#include <string>

#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG
#define RGFW_OPENGL

#include "../include/RGFW/RGFW.h"
#include "../include/glyph/glyph.h"

#include <GL/gl.h>

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

double savedTime = 0 , output_time = 0;
fs::path homeDir = getenv("HOME");
fs::path filePath = homeDir / ".clc" / "lt";


static  std::string t_str_fucn (double &time)
{
    std::string function_result = "";    

    if(time >= 3600)
    {
        int hour = time / 3600;
        function_result += std::to_string(hour);
        function_result += ".";

        int min = std::fmod(time , 3600) / 60;

        function_result +=std::to_string(min);
        function_result += ".";        
    }
    else if (time >= 60)
    {
        int min = std::fmod(time,3600) / 60;
        function_result += std::to_string(min);
        function_result += ".";   
    }
    
        double sec = std::fmod(time , 60);
        function_result +=std::to_string(sec);

        function_result.erase(6);    
    return function_result;
}


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


int main(void)
{

    double additional_time , last_time = 0;

    bool order_time_stop = true;

    if(!(loadTime() == 0))
    {
        savedTime = loadTime();
    }

        
    uint8_t min = 0 , hour = 0 , sec = 0;  
    
    RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
    hints->major = 3;
    hints->minor = 3;
    RGFW_setGlobalHints_OpenGL(hints);

   
    RGFW_window* RGFW_window_obj = RGFW_createWindow("clc.", 0, 0, 500, 300, RGFW_windowCenter || RGFW_windowNoResize || RGFW_windowNoBorder || RGFW_windowOpenGL);
    RGFW_window_createContext_OpenGL(RGFW_window_obj, hints);

    RGFW_window_show(RGFW_window_obj);

    glyph_renderer_t renderer = glyph_renderer_create("font.ttf", 135.0f,NULL, GLYPH_UTF8, NULL, 0);
    glyph_renderer_set_projection(&renderer, 800, 600);
    
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

            t_str = t_str_fucn(output_time);
 
            std::cout << output_time << '\n';
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
            
            last_time_before_stop = high_resolution_clock::now();
        }

        else
        {    
            auto now_time = high_resolution_clock::now();
            output_time = duration<double>(now_time - start_time).count();
            output_time+=savedTime;
            
            std::cout << output_time << '\n';
            t_str = t_str_fucn(output_time);
       
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        }
        glyph_renderer_draw_text(&renderer,"clc.", 10, 100, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        RGFW_window_swapBuffers_OpenGL(RGFW_window_obj);

    }
    RGFW_window_close(RGFW_window_obj);
    return 0;
}
