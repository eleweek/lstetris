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
        {}

        Figure(int states[][4][4], int numStates, sf::Color color = sf::Color(0, 128, 200))
          : CurrentStateIndex(0)
        {
            for (int s = 0; s < numStates; ++s) {
                States.push_back(std::vector< std::vector<Square> >(4, std::vector<Square>(4)));
                for (int i = 0; i < 4; ++i)
                    for(int j = 0; j < 4; ++j)
                        States[s][i][j] = Square(states[s][i][j], color);
            }
        }

        void Rotate()
        {
            CurrentStateIndex = (CurrentStateIndex + 1) % States.size();
        }

        void RotateBack()
        {
            CurrentStateIndex = (CurrentStateIndex - 1) % States.size();
        }

        Square GetSquare(int row, int col)
        {
            return States[CurrentStateIndex][row][col];
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
      : field(FIELD_HEIGHT, std::vector<Square>(FIELD_WIDTH, Square()))
    {
        InitAvailableFigures();
        GenerateNewFigure();
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

    void DrawMatrix(sf::RenderWindow& window, std::vector< std::vector<Square> > matrix, int offsetCol = 0, int offsetRow = 0)
    {
        int matrixHeight = squareSize * field[0].size();
        int matrixWidth = squareSize * field.size();
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                matrix[i][j].Draw(window, j + offsetCol, i + offsetRow);
            }
        }
    }

    void Draw(sf::RenderWindow& window)
    {
        DrawField(window);
        DrawMatrix(window, Fig.GetCurrentState(), figCol, figRow);
    }

    void MoveFigureRight()
    {
        if (IsCorrectFigurePosition(figCol + 1, figRow))
            figCol++;
    }

    void MoveFigureLeft()
    {
        if (IsCorrectFigurePosition(figCol - 1, figRow))
            figCol--;
    }

    void MoveFigureDown()
    {
        if (IsCorrectFigurePosition(figCol, figRow - 1))
            figRow--;
    }

    void ForceFigureDown()
    {
        if (IsCorrectFigurePosition(figCol, figRow - 1)) {
            MoveFigureDown();
        } else {
            AddFigureToField();
            ClearLines();
            GenerateNewFigure();
        }
    }

    void AddFigureToField()
    {
        for (int r = 0; r < FIG_SIZE; ++r) {
            for (int c = 0; c < FIG_SIZE; ++c) {
                if (Fig.GetSquare(r, c).IsOccupied){
                    field[figRow + r][figCol + c] = Fig.GetSquare(r, c);
                }
            }
        }
    }

    void GenerateNewFigure()
    {
        Fig = AvailableFigures[random() % AvailableFigures.size()];
        // TODO better initial position, hidden rows
        figCol = FIELD_WIDTH / 2;
        figRow = FIELD_HEIGHT - 4;
    }

    void RotateFigure()
    {
        Fig.Rotate();
        if (!IsCorrectFigurePosition(figCol, figRow))
            Fig.RotateBack();
    }

    void InitAvailableFigures()
    {
        int z[2][FIG_SIZE][FIG_SIZE] = {
            {{0, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}},

            {{0, 1, 1, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 0},
             {0, 0, 0, 0}}
        };
        AvailableFigures.push_back(Figure(z, 2, sf::Color::Green));
        int s[2][FIG_SIZE][FIG_SIZE] = {
            {{0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}},

            {{0, 0, 1, 1},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}}
        };
        AvailableFigures.push_back(Figure(s, 2, sf::Color::Blue));
        int o[2][FIG_SIZE][FIG_SIZE] = {
            {{0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}},
        };
        AvailableFigures.push_back(Figure(o, 1, sf::Color::Red));
        int i[2][FIG_SIZE][FIG_SIZE] = {
            {{0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},

            {{0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0},
             {0, 0, 0, 0}}
        };
        AvailableFigures.push_back(Figure(i, 2, sf::Color::Yellow));
        int l[4][FIG_SIZE][FIG_SIZE] = {
            {{0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},

            {{0, 0, 0, 0},
             {0, 1, 1, 1},
             {0, 1, 0, 0},
             {0, 0, 0, 0}},

            {{0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}},

            {{0, 0, 0, 0},
             {0, 0, 1, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0}},
        };
        AvailableFigures.push_back(Figure(l, 4, sf::Color::Cyan));

        int j[4][FIG_SIZE][FIG_SIZE] = {
            {{0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}},

            {{0, 0, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 1, 1},
             {0, 0, 0, 0}},

            {{0, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}},

            {{0, 0, 0, 0},
             {0, 1, 1, 1},
             {0, 0, 0, 1},
             {0, 0, 0, 0}},
        };
        AvailableFigures.push_back(Figure(j, 4, sf::Color(25, 75, 150)));

        int t[4][FIG_SIZE][FIG_SIZE] = {
            {{0, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}},

            {{0, 0, 0, 0},
             {0, 1, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0}},

            {{1, 0, 0, 0},
             {1, 1, 0, 0},
             {1, 0, 0, 0},
             {0, 0, 0, 0}},

            {{1, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}},
        };
        AvailableFigures.push_back(Figure(t, 4, sf::Color::Magenta));
    }

    void ClearLines()
    {
        std::vector< std::vector<Square> >::iterator it = field.begin();
        while (it != field.end()) {
            bool isLineFull = true;
            for (int j = 0; j < it->size(); j++) {
                isLineFull &= it->at(j).IsOccupied;
            }
            if (isLineFull) {
                field.erase(it);
                field.push_back(std::vector<Square>(FIELD_WIDTH));
                it = field.begin();
            } else {
                ++it;
            }
        }
    }

    bool IsCorrectFigurePosition(int col, int row)
    {
        for (int r = 0; r < FIG_SIZE; ++r) {
            for (int c = 0; c < FIG_SIZE; ++c) {
                if (Fig.GetSquare(r, c).IsOccupied && ((col + c) >= FIELD_WIDTH || (col + c) < 0 || (row + r) >= FIELD_HEIGHT || (row + r) < 0 || field[row + r][col + c].IsOccupied)) {
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
    Figure Fig;
    std::vector<Figure> AvailableFigures;
    int figRow;
    int figCol;
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
    const int tickSize = 250;
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
