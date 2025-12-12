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
#include <print>

#define GLYPH_MINIMAL
#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
extern "C" 
{
#include "../include/RGFW/RGFW.h"
#include "../include/glyph/glyph.h"
};
#include <GL/gl.h>

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

double saved_time = 0 , output_time = 0;
fs::path home_dir = getenv("HOME");
fs::path saved_time_file_path = home_dir / ".clc" / "lt";


static std::string t_str_fucn (double &time)
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


void save_time(double total_ms)
{
    if (!fs::exists(saved_time_file_path.parent_path()))
    {
        fs::create_directory(saved_time_file_path.parent_path());
    }
    ofstream outFile(saved_time_file_path);
    if (outFile.is_open())
    {
        outFile << total_ms;
        outFile.close();
    }
    else
    {
        cerr << total_ms << saved_time_file_path<< "\n";
        cerr << "clc. massage [error] : can't store last data time" << endl;
    }
}

double load_time()
{
    ifstream inFile("/home/gfaerny/.clc/lt");
    double previous_time = 0;
    if (inFile.is_open())
    {
        inFile >> previous_time;
        inFile.close();
        return previous_time;
        std::printf("clc. massage [alert] : last saved time loaded succesfully%f" ,previous_time);
    }
    else
    {
        std::printf("clc. massage [error] : failed to open ~/.clc/lt file.\nclc could't load your last time_point");
        return 0;
    }
}


int main()
{   
    double additional_time , last_time = 0;
    bool order_time_stop = true;

    double last_time_time = load_time();
    std::printf("loaded time = %f",saved_time);
                       
    uint8_t min = 0 , hour = 0 , sec = 0;  
        
    RGFW_window* RGFW_window_obj = RGFW_createWindow("hi", 0, 0, 500, 300, RGFW_windowOpenGL | RGFW_windowNoBorder | RGFW_windowNoResize);

    RGFW_window_makeCurrentContext_OpenGL(RGFW_window_obj);
    

    glyph_gl_set_opengl_version(3, 3);
    RGFW_window_show(RGFW_window_obj);
    RGFW_window_setExitKey(RGFW_window_obj,RGFW_escape);
    glyph_renderer_t renderer = glyph_renderer_create("font.ttf", 135.0f,NULL, GLYPH_UTF8,NULL, 0);
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
                /// stop and start timer proc
                if(order_time_stop == true)
                {
                    order_time_stop = false;
                    start_time = high_resolution_clock::now();
                    saved_time = output_time;
                }
                else
                {
                    order_time_stop = true;
                    saved_time = output_time;
                }
            }
        }

    /// graphic interface    
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(order_time_stop)
        {
            saved_time = output_time ;
            
            t_str = t_str_fucn(output_time);
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
            
            last_time_before_stop = high_resolution_clock::now();
        }
        else
        {    
            auto now_time = high_resolution_clock::now();
            output_time = duration<double>(now_time - start_time).count();
            output_time+=saved_time ;
            
        
            t_str = t_str_fucn(output_time);
       
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        }
        glyph_renderer_draw_text(&renderer,"clc.", 10, 100, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_NONE);
        RGFW_window_swapBuffers_OpenGL(RGFW_window_obj);

    }
    RGFW_window_close(RGFW_window_obj);
    return 0;
}
