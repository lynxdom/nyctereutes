#include <iostream>


#include <core/application.h>

int main( int argc, char *argv[] )
{
    Application::app_config config = {};

    config.name = "Test Application";
    config.start_pos_x = 50;
    config.start_pos_y = 50;
    config.start_width = 1200;
    config.start_height = 720;

    Application::application_create( &config );
    Application::application_run();

    return 0;
}