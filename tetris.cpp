#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>


// TODO: refactor into proper classes
std::vector< std::vector<int> > fig(4, std::vector<int>(4, 0));
int figX = 0;
int figY = 0;

std::vector< std::vector<int> > field(20, std::vector<int>(10, 0));

void init()
{
    fig[0][0] = 1;
    fig[0][1] = 1;
    fig[0][2] = 1;
    fig[1][1] = 1;

    field[0][1] = 1;
    field[0][2] = 1;
    field[1][1] = 1;
    field[1][2] = 1;

    field[5][5] = 1;
    field[5][5] = 1;
    field[5][6] = 1;
    field[5][6] = 1;
}

void drawMatrix(sf::RenderWindow& window, std::vector< std::vector<int> > matrix, int offsetX = 0, int offsetY = 0)
{
    const int squareSize = 20;
    const int outlineThickness = 2;
    const int matrixPositionX = 50;
    const int matrixPositionY = 100;
    int matrixHeight = squareSize * matrix.size();
    int matrixWidth = squareSize * matrix[0].size();
    const sf::Color color(0, 128, 200);
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j]) {
                sf::RectangleShape shape(sf::Vector2f(squareSize - 2 * outlineThickness, squareSize - 2 * outlineThickness));
                shape.setPosition(matrixPositionX + (j + offsetX) * squareSize, matrixPositionY + matrixHeight - (i + offsetY + 1) * squareSize);
                shape.setFillColor(color);
                shape.setOutlineThickness(outlineThickness);
                window.draw(shape);
            }
        }
    }
    sf::RectangleShape border(sf::Vector2f(matrixWidth, matrixHeight));
    border.setPosition(matrixPositionX, matrixPositionY);
    border.setOutlineThickness(outlineThickness);
    border.setFillColor(sf::Color::Transparent);
    window.draw(border);

}

int main()
{
    init();
    const int windowWidth = 800;
    const int windowHeight = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "My window", sf::Style::Default,  settings);

    while (window.isOpen())
    {
        window.clear();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        drawMatrix(window, field);
        drawMatrix(window, fig, figX, figY);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            figX--;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            figX++;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            figY--;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            figY++;
        }
        window.display();
    }
}
