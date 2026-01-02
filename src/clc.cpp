#include "../include/clc.h"
#include <GL/gl.h>
#include <chrono>
#include <cstdlib>
#include <iostream>

namespace fs = std::filesystem;

double saved_time {} , output_time {} , rus_time {};
fs::path home_dir = getenv("HOME");
fs::path saved_time_file_path = home_dir / ".clc" / "lt";
const char font_path[] {"/usr/share/clc/font.ttf"};
std::string t_str = "";

void draw_close_button()
{
    glLineWidth(10);
    
}

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


void save_time()
{
    double total_ms = saved_time;
    if (!fs::exists(saved_time_file_path.parent_path()))
    {
        fs::create_directory(saved_time_file_path.parent_path());
    }
    std::ofstream outFile(saved_time_file_path);
    if (outFile.is_open())
    {
        outFile << total_ms;
        outFile.close();
        printf("clc. massage [alert] : last time got saved");
    }
    else
    {
        printf("clc. massage [error] : can't store last data time\n");
    }

    return ;
}


double load_time()
{
    std::ifstream inFile(saved_time_file_path);
    double previous_time = 0;
    if(inFile.is_open())
    {
        inFile >> previous_time;
        inFile.close();
        return previous_time;
        printf("clc. massage [alert] : last saved time loaded succesfully%f" ,previous_time);
    }
    else
    {
        printf("clc. massage [error] : failed to open ~/.clc/lt file.\nclc could't load your last time_point");
        return 0;
    }
}


int main()
{
    std::atexit(save_time);
    
    bool order_time_stop = true;

    double last_time_time = load_time();
    std::printf("loaded time = %f",load_time());
                       
    uint8_t min = 0 , hour = 0 , sec = 0;

    RGFW_window* RGFW_window_obj = RGFW_createWindow("clc.", 0, 0, 500, 300, RGFW_windowOpenGL | RGFW_windowNoBorder | RGFW_windowNoResize | RGFW_windowCenter);

    RGFW_window_makeCurrentContext_OpenGL(RGFW_window_obj);
    
    glyph_gl_set_opengl_version(3, 3);
    RGFW_window_show(RGFW_window_obj);
    RGFW_window_setExitKey(RGFW_window_obj,RGFW_escape);
    glyph_renderer_t renderer = glyph_renderer_create(font_path, 135.0f,NULL, GLYPH_ENCODING_UTF8,NULL, 0);
    glyph_renderer_set_projection(&renderer, 800, 600);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time; 
    saved_time += last_time_time;
    while(RGFW_window_shouldClose(RGFW_window_obj) == false)
    {
        RGFW_event RGFW_event_obj;

        while(RGFW_window_checkEvent(RGFW_window_obj, &RGFW_event_obj))
        {
//          space
            if(RGFW_event_obj.type == RGFW_keyPressed && RGFW_event_obj.button.value == RGFW_space)    
            {
                /// stop and start timer proc
                if(order_time_stop == true)
                {
                    order_time_stop = false;
                    start_time = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    order_time_stop = true;
                    saved_time += output_time;
                }
            }
//          r
            if (RGFW_event_obj.type == RGFW_keyPressed &&  RGFW_event_obj.button.value == RGFW_r)
            {
                saved_time = 0;
                last_time_time = 0;
                output_time = 0;
            }
            if (RGFW_event_obj.type == RGFW_keyPressed &&  RGFW_event_obj.button.value == RGFW_q)
            {
                std::cout << "this fucking key\n" ; 
                save_time();
                return 0;
            }

        }

//      graphic interface    
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(order_time_stop)
        {
            rus_time = saved_time;
            t_str = t_str_fucn(rus_time);
                               
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_EFFECT_NONE);
        }
        else if (!order_time_stop)
        {    
            auto now_time = std::chrono::high_resolution_clock::now();
            output_time = std::chrono::duration<double>(now_time - start_time).count();

            rus_time = output_time + saved_time;
            t_str = t_str_fucn(rus_time);
       
            glyph_renderer_draw_text(&renderer, t_str.c_str(),230.0f, 350.0f, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_EFFECT_NONE);
        }
// TEST LINE
//      std::cout << "last_time_time :" << last_time_time << '\n' << "output time :"<< output_time << '\n' << "saved_time :" << saved_time << '\n' << "__________\n";
        glyph_renderer_draw_text(&renderer,"clc.", 10, 100, 1.0f, 1.0f, 1.0f, 1.0f, GLYPH_EFFECT_NONE);
        RGFW_window_swapBuffers_OpenGL(RGFW_window_obj);

        drawCircle(200,200,10, 10);

    }
    RGFW_window_close(RGFW_window_obj);

    return 0;

}
