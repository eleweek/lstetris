#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>


class Game
{
public:
    class Square
    {
    public:
        Square(bool isOccupied=false, sf::Color color=sf::Color(0, 128, 200))
          : IsOccupied(isOccupied)
          , Color(color)
        {}

        void Draw(sf::RenderWindow& window, int x, int y)
        {
            if (IsOccupied) {
                int fieldHeight = squareSize * FIELD_HEIGHT;
                int fieldWidth = squareSize * FIELD_WIDTH;
                sf::RectangleShape shape(sf::Vector2f(squareSize - 2 * outlineThickness, squareSize - 2 * outlineThickness));
                shape.setPosition(fieldPositionX + x * squareSize, fieldPositionY + fieldHeight - (y + 1) * squareSize);
                shape.setFillColor(Color);
                shape.setOutlineThickness(outlineThickness);
                window.draw(shape);
            }
        }

        bool IsOccupied;
        sf::Color Color;
    };

    class Figure
    {
    public:
        Figure()
          : CurrentStateIndex(0)
        {
            int states[2][4][4] = {
                {{0, 0, 1, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}},

                {{0, 1, 1, 0},
                {0, 0, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}}
            };

            // TODO: refactor out of this class
            // TODO: remove constants
            for (int s = 0; s < 2; ++s) {
                States.push_back(std::vector< std::vector<Square> >(4, std::vector<Square>(4)));
                for (int i = 0; i < 4; ++i)
                    for(int j = 0; j < 4; ++j)
                        States[s][i][j] = states[s][i][j];
            }
        }

        void Rotate()
        {
            CurrentStateIndex = (CurrentStateIndex + 1) % States.size();
        }

        Square GetSquare(int i, int j)
        {
            return States[CurrentStateIndex][i][j];
        }

        std::vector< std::vector<Square> > GetCurrentState()
        {
            return States[CurrentStateIndex];
        }
    private:
        std::vector< std::vector< std::vector<Square> > > States;
        int CurrentStateIndex;
    };

public:
    Game()
      : field(FIELD_WIDTH, std::vector<Square>(FIELD_HEIGHT, Square()))
      , figX(0)
      , figY(FIELD_HEIGHT - 4)
    {
        // TODO ugly, handle it i a better way
        field[0][0] = true;
        field[0][1] = true;
        field[1][0] = true;
        field[1][1] = true;

        field[5][5] = true;
        field[5][5] = true;
        field[5][6] = true;
        field[5][6] = true;
    }

    void DrawField(sf::RenderWindow& window)
    {
        int fieldHeight = squareSize * FIELD_HEIGHT;
        int fieldWidth = squareSize * FIELD_WIDTH;
        DrawMatrix(window, field);
        sf::RectangleShape border(sf::Vector2f(fieldWidth, fieldHeight));
        border.setPosition(fieldPositionX, fieldPositionY);
        border.setOutlineThickness(outlineThickness);
        border.setFillColor(sf::Color::Transparent);
        window.draw(border);
    }

    void DrawMatrix(sf::RenderWindow& window, std::vector< std::vector<Square> > matrix, int offsetX = 0, int offsetY = 0)
    {
        int matrixHeight = squareSize * field.size();
        int matrixWidth = squareSize * field[0].size();
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                matrix[i][j].Draw(window, i + offsetX, j + offsetY);
            }
        }
    }

    void Draw(sf::RenderWindow& window)
    {
        DrawField(window);
        DrawMatrix(window, fig.GetCurrentState(), figX, figY);
    }

    void MoveFigureRight()
    {
        if (IsCorrectFigurePosition(figX + 1, figY))
            figX++;
    }

    void MoveFigureLeft()
    {
        if (IsCorrectFigurePosition(figX - 1, figY))
            figX--;
    }

    void MoveFigureDown()
    {
        if (IsCorrectFigurePosition(figX, figY - 1))
            figY--;
    }

    void ForceFigureDown()
    {
        MoveFigureDown();
    }

    void RotateFigure()
    {
        /* TODO: collision checking */
        fig.Rotate();
    }

    bool IsCorrectFigurePosition(int x, int y)
    {
        for (int i = 0; i < FIG_SIZE; ++i) {
            for (int j = 0; j < FIG_SIZE; ++j) {
                if (fig.GetSquare(i, j).IsOccupied && ((x + i) >= FIELD_WIDTH || (x + i) < 0 || (y+j) >= FIELD_HEIGHT || (y + j) < 0 || field[x + i][y + j].IsOccupied)) {
                    return false;
                }
            }
        }

        return true;
    }

private:
    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 20;
    static const int FIG_SIZE = 4;
    std::vector< std::vector<Square> > field;
    Figure fig;
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

    sf::Clock clock;
    const int tickSize = 1000;
    while (window.isOpen())
    {
        window.clear();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Left:
                            game.MoveFigureLeft();
                            break;
                        case sf::Keyboard::Right:
                            game.MoveFigureRight();
                            break;
                        case sf::Keyboard::Down:
                            game.MoveFigureDown();
                            break;
                        case sf::Keyboard::Space:
                            game.RotateFigure();
                            break;
                        default:
                            break;
                    };
                default:
                    break;
            };
        }
        if (clock.getElapsedTime().asMilliseconds() > tickSize) {
            game.ForceFigureDown();
            clock.restart();
        }
        game.Draw(window);
        window.display();
    }
}
