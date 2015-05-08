#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>

class Game
{
public:
    Game()
      : FIELD_WIDTH(10)
      , FIELD_HEIGHT(20)
      , field(FIELD_HEIGHT, std::vector<int>(FIELD_WIDTH, 0))
      , fig(4, std::vector<int>(4, 0))
      , figX(0)
      , figY(FIELD_HEIGHT)
    {
        // TODO ugly, handle it i a better way
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

    void DrawField(sf::RenderWindow& window)
    {
        int fieldHeight = squareSize * field.size();
        int fieldWidth = squareSize * field[0].size();
        DrawMatrix(window, field);
        sf::RectangleShape border(sf::Vector2f(fieldWidth, fieldHeight));
        border.setPosition(fieldPositionX, fieldPositionY);
        border.setOutlineThickness(outlineThickness);
        border.setFillColor(sf::Color::Transparent);
        window.draw(border);

    }

    void DrawMatrix(sf::RenderWindow& window, std::vector< std::vector<int> > matrix, int offsetX = 0, int offsetY = 0)
    {
        int matrixHeight = squareSize * field.size();
        int matrixWidth = squareSize * field[0].size();
        const sf::Color color(0, 128, 200);
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                if (matrix[i][j]) {
                    sf::RectangleShape shape(sf::Vector2f(squareSize - 2 * outlineThickness, squareSize - 2 * outlineThickness));
                    shape.setPosition(fieldPositionX + (j + offsetX) * squareSize, fieldPositionY + matrixHeight - (i + offsetY + 1) * squareSize);
                    shape.setFillColor(color);
                    shape.setOutlineThickness(outlineThickness);
                    window.draw(shape);
                }
            }
        }
    }

    void Draw(sf::RenderWindow& window)
    {
        DrawField(window);
        DrawMatrix(window, fig, figX, figY);
    }

    void MoveFigureRight()
    {
        if (figX < FIELD_WIDTH - 1)
            figX++;
    }

    void MoveFigureLeft()
    {
        if (figX > 0)
            figX--;
    }

    void MoveFigureDown()
    {
        if (figY > 0)
            figY--;
    }
private:
    const int FIELD_WIDTH;
    const int FIELD_HEIGHT;
    std::vector< std::vector<int> > field;
    std::vector< std::vector<int> > fig;
    int figX;
    int figY;
    enum {
        fieldPositionX = 50,
        fieldPositionY = 100,
    };
    enum {
        squareSize = 20,
        outlineThickness = 2,
        matrixPositionX = 50,
        matrixPositionY = 100,
    };
};

void init()
{
}


int main()
{
    Game game;

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            game.MoveFigureLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            game.MoveFigureRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            game.MoveFigureDown();
        }
        game.Draw(window);
        window.display();
    }
}
