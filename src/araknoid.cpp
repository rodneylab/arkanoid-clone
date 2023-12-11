#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

constexpr unsigned int kWINDOW_WIDTH{800U}, kWINDOW_HEIGHT{600U},
    kFRAMERATE_LIMIT{60U};

int main()
{
    sf::RenderWindow window{{kWINDOW_WIDTH, kWINDOW_HEIGHT}, "Araknoid - 1"};
    window.setFramerateLimit(kFRAMERATE_LIMIT);

    while (true)
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            break;
        }
        window.clear(sf::Color::Black);
        window.display();
    }
}
