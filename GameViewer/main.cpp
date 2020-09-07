#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Version/version.h>

int main() {
    std::cout << "Hello, World!" << Version::DATE << std::endl;


    return 0;
}
