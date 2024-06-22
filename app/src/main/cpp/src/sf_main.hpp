#pragma once

#include <sf_app.hpp>

sf::App* new_app();

int main() {
    sf::App* app = new_app();
    app->run();
    delete app;
    return 0;
}