#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// Repeat text function
// https://reactgo.com/cpp-repeat-string-n-times
string repeat(string s, int n)
{
    string repeat;

    for (int i = 0; i < n; i++)
        repeat += s;

    return repeat;
}

// Create new line and bold custom messages
// https://stackoverflow.com/questions/29997096
string message(string s)
{
    cout << "\n\e[1m" << s << "\e[0m\n"
         << endl;

    return s;
}

// Clear terminal screen
int ClearScreen()
{
#if defined(_WIN32)
    return std::system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    return std::system("clear");
#endif
}

// Pause the terminal
int Pause()
{
#if defined(_WIN32)
    return std::system("pause");
#elif defined(__linux__) || defined(__APPLE__)
    return std::system(R"(read -p "Press any key to continue . . . " dummy)");
#endif
}

// Delay terminal screen
void sleep(float seconds)
{
    // Converting milliseconds to seconds
    int milliSeconds = 1000 * seconds;

    // Storing start time
    clock_t startTime = clock();

    // Looping until time is reached
    while (clock() < startTime + milliSeconds)
        ;
}

// This function clears the screen and then waits for 1 second.
void load()
{
    ClearScreen(); // Call to a function that clears the screen.
    sleep(0.1);      // Wait for 1 second.
}

// This function displays an error message when the user provides invalid input.
void invalid()
{
    load();                              // Call to the load function to display a loading message before the error message.
    message("ERROR\t:\tInvalid input!"); // Call to a function that displays an error message.
}

// Turn negative value to positive
int absoluteValue(int n)
{
    if (n < 0)
        n *= -1;

    return n;
}

// Compare distance between two objects
int compareDistance(int d1, int d2)
{
    int distance = absoluteValue(d1 - d2);

    return distance;
}

// Function prototype
void instructions();
void commands();
void game(int Column, int Row, int num_zombie);
void settings();
void index();

class Board
{
private:
    vector<vector<char>> map_;                                              // 2D vector of board map
    int Column_, Row_;                                                      // Dimension X and Y of board map
    char objects_[10] = {' ', ' ', ' ', '^', '>', 'v', '<', 'h', 'r', 'p'}; // Haven't done the pod yet
    int numObjects = sizeof(objects_);                                      // number of objects in the array

public:
    Board(int Column = 15, int Row = 5) // Set dimension of board map
    {
        mapObjects(Column, Row);
    }
    int getColumn() const // To return value of Column
    {
        return Column_;
    }
    int getRow() const // To return value of Row
    {
        return Row_;
    }
    char getObject(int x, int y) const // To return type of object at (x,y)
    {
        return map_[Row_ - y][x - 1];
    }
    void setColumn(int x) // To set the value of column
    {
        Column_ = x;
    }
    void setRow(int y) // To set the value of row
    {
        Row_ = y;
    }
    void setObject(int x, int y, char ch) // To set an object at (x,y)
    {
        map_[Row_ - y][x - 1] = ch;
    }
    void display() const // To display the board map
    {
        load();
        cout << "\n";
        cout << "    *******************************" << endl;
        cout << "            Alien vs Zombies       " << endl;
        cout << "    *******************************" << endl;

        // For every row
        for (int i = 0; i < Row_; i++)
        {
            // Display upper border of the row
            cout << "    ";
            for (int j = 0; j < Column_; j++)
            {
                cout << "+-";
            }
            cout << "+" << endl;

            // Display row number
            cout << setw(3) << (Row_ - i);

            // Display cell content and border of each column
            cout << " |";
            for (int j = 0; j < Column_; j++)
            {
                cout << map_[i][j] << "|";
            }
            cout << endl;
        }

        // Display lower border of the last row
        cout << "    ";
        for (int j = 0; j < Column_; j++)
        {
            cout << "+-";
        }
        cout << "+" << endl;

        // Display column number
        cout << "    ";
        for (int j = 0; j < Column_; j++)
        {
            int col = (j + 1) / 10;
            cout << " ";
            if (col == 0)
                cout << " ";
            else
                cout << col;
        }
        cout << endl;
        cout << "    ";
        for (int j = 0; j < Column_; j++)
        {
            cout << " " << (j + 1) % 10;
        }
        cout << endl
             << endl;
    }
    void mapObjects(int Column, int Row) // To create the board map
    {
        Column_ = Column;
        Row_ = Row;

        // To create dynamic 2D arry using vector
        map_.resize(Row_); // create empty rows
        for (int i = 0; i < Row_; i++)
        {
            map_[i].resize(Column_); // resize each row
        }

        // To put random characters into the vector array
        for (int i = 0; i < Row_; ++i)
        {
            for (int j = 0; j < Column_; ++j)
            {
                int objNo = rand() % numObjects;
                map_[i][j] = objects_[objNo];
            }
        }
    }
    bool isInsideMap(int x, int y) const // To determine if (x,y) is inside map
    {
        return (x > 0 && x <= Column_ && y > 0 && y <= Row_);
    }
    void item_arrow(int x, int y, string direction) // Item arrow
    {
        if (direction == "up")
            setObject(x, y, '^');
        else if (direction == "right")
            setObject(x, y, '>');
        else if (direction == "down")
            setObject(x, y, 'v');
        else if (direction == "left")
            setObject(x, y, '<');
    }
    int item_h(int health) // Item health pack
    {
        // If the health is between 80 and 100 (inclusive), the player's health will be increased by 1 for 20 times until it reaches 100
        if (health > 80 && health < 100)
        {
            for (int i = 0; i < 20; i++)
            {
                health += 1;
                if (health == 100)
                    break;
            }
        }
        // If the health is less than or equal to 80, the player's health will be increased by 20
        else if (health <= 80)
        {
            health += 20;
        }

        return health;
    }
    void item_r(int x, int y) // Item rock
    {
        int objNo = rand() % (numObjects - 1);
        map_[Row_ - y][x - 1] = objects_[objNo];
    }
    void noTrail() // Trails after alien or zombie move
    {
        // Loop through every coordinate and place a random object if it is a trail
        for (int y = 1; y <= Row_; y++)
        {
            for (int x = 1; x <= Column_; x++)
            {
                if (getObject(x, y) == '.')
                {
                    int objNo = rand() % numObjects;
                    map_[Row_ - y][x - 1] = objects_[objNo];
                }
            }
        }
        display();
        // Inform the player that the trails have been reset
        cout << "  "
             << "The trail is resetted." << endl;
        cout << "  ";
        system("pause");
    }
};

class Alien
{
private:
    char ch_ = 'A';       // Alien symbol
    int x_, y_;           // Alien coordinates
    int attack_, health_; // Alien attack, health

public:
    Alien(int attack = 0, int health = 100) // Set alien attack, health
    {
        attack_ = attack;
        health_ = health;
    }
    int getX() const // To return value x
    {
        return x_;
    }
    int getY() const // To return value y
    {
        return y_;
    }
    int getAttack() const // To return value attack
    {
        return attack_;
    }
    int getHealth() const // To return value health
    {
        return health_;
    }
    char getChar() const // To return value char
    {
        return ch_;
    }
    void setAttack(int attack) // To set the value attack
    {
        attack_ = attack;
    }
    void setHealth(int health) // To set the value Health
    {
        health_ = health;
    }
    void newX(int x) // Adds the given integer value to the current x-coordinate of alien
    {
        x_ += x;
    }
    void newY(int y) // Adds the given integer value to the current y-coordinate of alien
    {
        y_ += y;
    }
    void newAttack(int attack) // Increases the attack power of Alien by 20.
    {
        attack_ += 20;
    }
    void newHealth(int health) // To change the health of the alien when its been damage by a zombie
    {
        health_ -= health;
    }
    void spawn(Board &board) // Spawn the alien on the map
    {
        // Ensure alien spawn in the middle of the map
        x_ = board.getColumn() / 2 + 1;
        y_ = board.getRow() / 2 + 1;

        // Set the alien's position on the board
        board.setObject(x_, y_, ch_);
    }
    void displayStats() const // Display alien stats
    {
        cout << "    ";
        cout << "Alien    : "
             << "Health " << setw(3) << health_
             << ", Attack " << setw(3) << attack_ << endl;
    }
};

class Zombie
{
private:
    int num_;            // Zombie quantity
    vector<char> ch_;    // Zombie symbol
    vector<int> x_;      // Zombie x coordinate
    vector<int> y_;      // Zombie y coordinate
    vector<int> attack_; // Zombie attack
    vector<int> health_; // Zombie health
    vector<int> range_;  // Zombie range

public:
    Zombie(int num = 2) // Set number of Zombie
    {
        num_ = num;
    }
    int getNum() // To return number of zombie
    {
        return num_;
    }
    int getX(int i) // To return value of x
    {
        return x_[i];
    }
    int getY(int i) // To return value of y
    {
        return y_[i];
    }
    int getHealth(int i) // To return value health
    {
        return health_[i];
    }
    void setNUM(int num) // To set the number of zombie
    {
        num_ = num;
    }
    void setHealth(int i, int health) // To set the value health
    {
        health_[i] = health;
    }
    void newHealth(int i, int damage) // To change the health of the alien when its been damage by alien
    {
        health_[i] -= damage;
    }
    void spawn(Board &board) // Spawn the Zombie on the map
    {
        // Get map dimensions
        int Column = board.getColumn();
        int Row = board.getRow();

        // Resize vector by quantity of Zombie
        x_.resize(num_);
        y_.resize(num_);
        ch_.resize(num_);

        // Set Zombie symbol
        for (int i = 0; i < num_; i++)
        {
            ch_[i] = '1' + i;
        }

        // Set Zombie coordinates
        for (int i = 0; i < num_; i++)
        {
            // To avoid spawning in the middle of the map
            do
            {
                x_[i] = rand() % Column + 1;
                y_[i] = rand() % Row + 1;
                board.setObject(x_[i], y_[i], ch_[i]);
            } while ((x_[i] == (Column / 2 + 1)) && (y_[i] == (Row / 2 + 1)));

            // To avoid spawning at the same place as the zombie before
            if (i == 0)
            {
                continue;
            }
            else
            {
                if (x_[i] == x_[i - 1] && y_[i] == y_[i - 1])
                    i--;
            }
        }
    }
    void stats(Board &board, int num) // Set Zombie stats
    {
        // Get map dimensions
        int numColumns = board.getColumn();
        int numRows = board.getRow();

        // Set maximum range
        int maxRange = min(numColumns, numRows) - 1;

        // Initialise types of attack, health, range
        const int numAttackTypes = 4;
        int attackType[numAttackTypes] = {5, 10, 15, 20};
        const int numHealthTypes = 4;
        int healthType[numHealthTypes] = {50, 100, 150, 200};
        vector<int> rangeType(maxRange);
        for (int i = 0; i < maxRange; i++)
        {
            rangeType[i] = i + 1;
        }

        // Resize vector array to number of zombie
        attack_.resize(num);
        health_.resize(num);
        range_.resize(num);

        // Set attack, health, range to zombie
        for (int i = 0; i < num; i++)
        {
            attack_[i] = attackType[rand() % numAttackTypes];
            health_[i] = healthType[rand() % numHealthTypes];
            range_[i] = rangeType[rand() % maxRange];
        }
    }
    void displayStats() const // Display Zombie stats
    {
        for (int i = 0; i < num_; i++)
        {
            cout << "    ";
            cout << "Zombie " << i + 1 << " :"
                 << " Health " << setw(3) << health_[i] << ", Attack "
                 << setw(3) << attack_[i] << ", Range " << setw(2) << range_[i] << endl;
        }
    }
    void move(Board &board, Alien &alien) // Display Zombie movement
    {
        for (int i = 0; i < num_; i++)
        {
            if (health_[i] <= 0)
                continue;

            int x = 0, y = 0;
            bool OutsideMap;
            char Object;
            bool Obstacle;
            int alien_x = alien.getX();
            int alien_y = alien.getY();
            int range_x, range_y, total_distance;

            // Ensure Zombie move inside the map and not into obstacles
            do
            {
                int go = rand() % 4 + 1;

                // Zombie move direction
                if (go == 1)
                {
                    x = 1;
                    y = 0;
                }
                else if (go == 2)
                {
                    x = -1;
                    y = 0;
                }
                else if (go == 3)
                {
                    x = 0;
                    y = 1;
                }
                else if (go == 4)
                {
                    x = 0;
                    y = -1;
                }

                OutsideMap = !board.isInsideMap(x_[i] + x, y_[i] + y);

                if (board.isInsideMap(x_[i] + x, y_[i] + y))
                    Object = board.getObject(x_[i] + x, y_[i] + y);

                Obstacle = Object == 'r' || Object == 'A';
                for (int i = 0; i < num_; i++)
                {
                    if (Object == ch_[i])
                        Obstacle = true;
                }

            } while (OutsideMap || Obstacle);

            // Display Zombie movement
            board.setObject(x_[i], y_[i], '.');
            board.setObject(x_[i] + x, y_[i] + y, ch_[i]);

            range_x = compareDistance(x_[i], alien_x);
            range_y = compareDistance(y_[i], alien_y);
            total_distance = range_x + range_y;

            if (range_[i] >= total_distance)
                alien.newHealth(attack_[i]);

            board.display();
            alien.displayStats();
            displayStats();
            cout << endl;
            if (x == 1)
                cout << "  "
                     << "Zombie " << i + 1 << " move to the right." << endl;
            else if (x == -1)
                cout << "  "
                     << "Zombie " << i + 1 << " move to the left." << endl;
            else if (y == 1)
                cout << "  "
                     << "Zombie " << i + 1 << " move upwards." << endl;
            else if (y == -1)
                cout << "  "
                     << "Zombie " << i + 1 << " move downwards." << endl;

            if (range_[i] >= total_distance)
            {
                cout << "  "
                     << "The alien is in zombie " << i + 1 << " range." << endl
                     << "  "
                     << "Zombie " << i + 1 << " will attack the alien." << endl;

                if (alien.getHealth() <= 0)
                {
                    cout << "  "
                         << "The alien is dead." << endl;
                    cout << "  ";
                    message("GAME OVER!");
                    system("pause");
                    break;
                }
                else
                {
                    cout << "  "
                         << "The alien is still alive." << endl;
                }
            }
            else
            {
                cout << "  "
                     << "The alien is out of zombie " << i + 1 << " range." << endl
                     << "  "
                     << "Zombie " << i + 1 << " does not attack the alien." << endl;
            }

            system("pause");
            board.noTrail();
            x_[i] += x;
            y_[i] += y;
        }
    }
};

// Item Pod
void item_p(int x, int y, Zombie &zombie)
{
    // Get the number of zombies
    int numZom = zombie.getNum();

    // Create vectors to store distance data
    vector<int> distance_x(numZom);
    vector<int> distance_y(numZom);
    vector<int> total_distance(numZom);

    // Calculate distances for all zombies
    for (int i = 0; i < numZom; i++)
    {
        distance_x[i] = (x > zombie.getX(i)) ? (x - zombie.getX(i)) : (zombie.getX(i) - x);
        distance_y[i] = (y > zombie.getY(i)) ? (y - zombie.getY(i)) : (zombie.getY(i) - y);
        total_distance[i] = distance_x[i] + distance_y[i];
    }

    // Determine the closest zombie
    int closest_zombie = 0;
    for (int i = 1; i < numZom; i++)
    {
        if (zombie.getHealth(i) == 0)
            continue;

        if (total_distance[i] < total_distance[closest_zombie])
        {
            closest_zombie = i;
        }
        else if (total_distance[i] == total_distance[closest_zombie])
        {
            // Randomly choose between the two zombies
            if (rand() % 2 == 0)
            {
                closest_zombie = i;
            }
        }
    }

    // Decrease the health of the closest zombie by 10
    zombie.newHealth(closest_zombie, zombie.getHealth(closest_zombie) - 10);
}

// Alien movements
void move(Board &board, Alien &alien, Zombie &zombie, string command)
{
    int x = 0, y = 0;
    int Column = board.getColumn();
    int Row = board.getRow();
    bool Border;

    // Determine the direction of movement based on the command parameter
    if (command == "up")
    {
        // Check if the movement would take the alien outside the bounds of the game board
        y = 1;
        if (!board.isInsideMap(alien.getX() + x, alien.getY() + y))
        {
            y = 0;
            Border = true;
        }
    }
    else if (command == "down")
    {
        y = -1;
        if (!board.isInsideMap(alien.getX() + x, alien.getY() + y))
        {
            y = 0;
            Border = true;
        }
    }
    else if (command == "left")
    {
        x = -1;
        if (!board.isInsideMap(alien.getX() + x, alien.getY() + y))
        {
            x = 0;
            Border = true;
        }
    }
    else if (command == "right")
    {
        x = 1;
        if (!board.isInsideMap(alien.getX() + x, alien.getY() + y))
        {
            x = 0;
            Border = true;
        }
    }

    do
    {
        // Move the alien to the new location
        board.setObject(alien.getX(), alien.getY(), '.');
        alien.newX(x);
        alien.newY(y);

        // Check if the new location contains a special item
        if (board.getObject(alien.getX(), alien.getY()) == '^')
        {
            x = 0;
            y = 1;
            alien.newAttack(20);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has encountered an arrow." << endl;
            cout << "  "
                 << "The Alien gains 20 attack." << endl;
            cout << "  "
                 << "The Alien will continue to move upwards." << endl;
            system("pause");
        }
        else if (board.getObject(alien.getX(), alien.getY()) == '>')
        {
            x = 1;
            y = 0;
            alien.newAttack(20);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has encountered an arrow." << endl;
            cout << "  "
                 << "The Alien gains 20 attack." << endl;
            cout << "  "
                 << "The Alien will continue to move to the right." << endl;
            system("pause");
        }
        else if (board.getObject(alien.getX(), alien.getY()) == 'v')
        {
            x = 0;
            y = -1;
            alien.newAttack(20);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has encountered an arrow." << endl;
            cout << "  "
                 << "The Alien gains 20 attack." << endl;
            cout << "  "
                 << "The Alien will continue to move downwards." << endl;
            system("pause");
        }
        else if (board.getObject(alien.getX(), alien.getY()) == '<')
        {
            x = -1;
            y = 0;
            alien.newAttack(20);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has encountered an arrow." << endl;
            cout << "  "
                 << "The Alien gains 20 attack." << endl;
            cout << "  "
                 << "The Alien will continue to move to the left." << endl;
            system("pause");
        }
        // If the object at the Alien's current position is a health pod
        else if (board.getObject(alien.getX(), alien.getY()) == 'h')
        {
            // Increase Alien's health by 20
            alien.setHealth(board.item_h(alien.getHealth()));
            // Update the game board and display the current game state
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            // Inform the player about the health gain and pause the game
            cout << endl;
            cout << "  "
                 << "The Alien has encountered a health pod." << endl;
            cout << "  "
                 << "The Alien heals 20 health." << endl;
            system("pause");
        }
        // Check if the next position contains a rock or the current position contains a rock
        else if (board.isInsideMap(alien.getX() + x, alien.getY() + y) && board.getObject(alien.getX() + x, alien.getY() + y) == 'r' || board.getObject(alien.getX(), alien.getY()) == 'r')
        {
            // If next position contains a rock
            if (board.getObject(alien.getX() + x, alien.getY() + y) == 'r')
            {
                // Destroy the rock and reveal the object underneath
                board.item_r(alien.getX() + x, alien.getY() + y);
                board.setObject(alien.getX(), alien.getY(), alien.getChar());
                alien.setAttack(0);
                board.display();
                alien.displayStats();
                zombie.displayStats();
                cout << endl;
                cout << "  "
                     << "The Alien has encountered a rock." << endl;
                cout << "  "
                     << "The rock is destroyed and the object " << board.getObject(alien.getX() + x, alien.getY() + y) << " is revealed." << endl;
                cout << "  "
                     << "The Alien has stopped moving." << endl;

                cout << "test1: " << alien.getX() << " " << alien.getY();
            }
            // If current position contains a rock
            else if (board.getObject(alien.getX(), alien.getY()) == 'r')
            {
                // Destroy the rock, move back to the previous position, and reveal the object underneath
                board.item_r(alien.getX(), alien.getY());
                alien.newX(-x);
                alien.newY(-y);
                board.setObject(alien.getX(), alien.getY(), alien.getChar());
                alien.setAttack(0);
                board.display();
                alien.displayStats();
                zombie.displayStats();
                cout << endl;
                cout << "  "
                     << "The Alien has encountered a rock." << endl;
                cout << "  "
                     << "The rock is destroyed and the object " << board.getObject(alien.getX() + x, alien.getY() + y) << " is revealed." << endl;
                cout << "  "
                     << "The Alien has stopped moving." << endl;

                cout << "test2: " << alien.getX() << " " << alien.getY();
            }
            // Pause and exit the loop
            system("pause");
            break;
        }
        // If the Alien has encountered a pod (item_p), update game board and stats
        else if (board.getObject(alien.getX(), alien.getY()) == 'p')
        {
            item_p(alien.getX(), alien.getY(), zombie);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            // Loop through Zombies to check if any of their health has fallen to zero
            for (int i = 0; i < zombie.getNum(); i++)
            {
                if (zombie.getHealth(i) <= 0)
                    zombie.setHealth(i, 0);
            }
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has encountered a pod." << endl;
            cout << "  "
                 << "The pod inflicts 10 damage to the nearest Zombie." << endl;
            system("pause");
        }

        int back;
        int ch = '1';
        for (int i = 0; i < zombie.getNum(); i++)
        {
            // Check if there is a zombie in the new location of the alien
            if (board.isInsideMap(alien.getX() + x, alien.getY() + y) && (board.getObject(alien.getX() + x, alien.getY() + y) == ch + i || board.getObject(alien.getX(), alien.getY()) == ch + i))
            {
                // If the zombie is in the same location as the alien, move the alien back to its previous location
                if (board.getObject(alien.getX(), alien.getY()) == i + 1)
                {
                    alien.newX(-x);
                    alien.newY(-y);
                }
                // Reduce the health of the zombie based on the attack value of the alien
                zombie.newHealth(i, alien.getAttack());

                // If the zombie is killed, remove it from the board and update the alien's position
                if (zombie.getHealth(i) <= 0)
                {
                    zombie.setHealth(i, 0);
                    board.setObject(alien.getX(), alien.getY(), '.');
                    alien.newX(x);
                    alien.newY(y);
                    board.setObject(alien.getX(), alien.getY(), alien.getChar());
                    cout << "test1"
                         << " " << alien.getX() << " " << alien.getY();
                }
                // If the zombie is not killed, update the board and the alien's attack value
                else
                {
                    board.setObject(alien.getX(), alien.getY(), alien.getChar());
                    alien.setAttack(0);
                    back = 1;
                }
                // Display the board and the stats of the alien and zombie
                board.display();
                alien.displayStats();
                zombie.displayStats();

                // Show appropriate messages based on whether the zombie is killed or not
                if (zombie.getHealth(i) <= 0)
                {
                    cout << endl;
                    cout << "  "
                         << "The Alien has encountered Zombie " << i + 1 << "." << endl;
                    cout << "  "
                         << "Alien attacks Zombie " << i + 1 << " and killed the Zombie." << endl;
                    cout << "  "
                         << "The Alien continues moving." << endl;
                    system("pause");
                }
                else if (zombie.getHealth(i) > 0)
                {
                    cout << endl;
                    cout << "  "
                         << "The Alien has encountered a Zombie." << endl;
                    cout << "  "
                         << "Alien attacks Zombie " << i + 1 << " but did not kill the Zombie." << endl;
                    cout << "  "
                         << "The Alien has stopped moving." << endl;
                    system("pause");
                    break;
                }
            }
        }
        // If the alien moved successfully without encountering any zombies, set back to 1 to exit the loop
        if (back == 1)
            break;

        if (!board.isInsideMap(alien.getX() + x, alien.getY() + y) || Border)
        {
            alien.setAttack(0);
            board.setObject(alien.getX(), alien.getY(), alien.getChar());
            board.display();
            alien.displayStats();
            zombie.displayStats();
            cout << endl;
            cout << "  "
                 << "The Alien has hit a border. The Alien stopped its movement." << endl;
            system("pause");
            break;
        }

    } while (board.isInsideMap(alien.getX() + x, alien.getY() + y));

    // Check if all zombies are killed
    int back2 = 0;
    for (int i = 0; i < zombie.getNum(); i++)
    {
        // If any zombie still has health, break out of loop
        if (zombie.getHealth(i) == 0)
        {
            back2 = 1;
        }
        else
        {
            back2 = 0;
            break;
        }
    }
    // If all zombies are killed, display win message and return to index
    if (back2 == 1)
    {
        cout << "  " << endl
             << "All the zombies have been killed." << endl;
        message("WIN!!");
        system("pause");
        index();
    }
    // If zombies still exist, clear trail on board
    else
    {
        board.noTrail();
    }
}

// The instructions on how to play
void instructions()
{
    cout << endl
         << "\t How to Play?" << endl;
    cout << "\t 1.  Alien and Zombies are game characters. A character is defeated when its health becomes zero." << endl;
    cout << "\t 2.  The attack attribute indicates the damage a character can inflict on its opponent's health in one hit." << endl;
    cout << "\t 3.  The Alien's attack always starts from ZERO (0) at each turn. This is because Alien's attack is" << endl;
    cout << "\t     accumulated by collecting the Arrow objects in the game board and is reset after each turn." << endl;
    cout << "\t 4.  Unlike Alien, Zombie has the range attribute, which defines how far Zombie's attack can reach." << endl;
    cout << "\t 5.  Alien moves by continuously thrusting in one of the FOUR (4) directions (up, down, left, right)" << endl;
    cout << "\t     and attacks Zombie when encountering it during its move. It is stopped by one of the following events:" << endl;
    cout << "\t\t  - It hits the border of the game board." << endl;
    cout << "\t\t  - It hits the Rock object." << endl;
    cout << "\t\t  - It hits and attacks Zombie, but Zombie survives the attack." << endl;
    cout << "\t 6.  Zombie can only move one step at a time (up, down, left, right). After a move, Zombie will attack" << endl;
    cout << "\t     Alien if Alien is within its attack range." << endl;
    cout << "\t 7.  When Alien encounters an arrow, the alien will change direction during its move according to the";
    cout << "\t     direction of the arrow and it will add 20 attack to the Alien." << endl;
    cout << "\t 8.  When Alien finds a health pod, the Alien health is healed by 20 points." << endl;
    cout << "\t 9.  When Alien finds a pod, the pod will target the closest Zombie." << endl;
    cout << "\t 10. When Alien encounters a rock, it stops the Alien from moving and reveals a hidden game object." << endl;
    cout << endl;
    cout << "  ";
    system("pause");
}

// Commands
void commands()
{
    cout << endl;
    cout << "  up    => Alien to move up" << endl;
    cout << "  down  => Alien to move down" << endl;
    cout << "  left  => Alien to move left" << endl;
    cout << "  right => Alien to move right" << endl;
    cout << "  arrow => Switch the direction of an arrow object in the game board" << endl;
    cout << "  help  => List and describe the commands that the player can use in the game" << endl;
    cout << "  quit  => Quit the game while still in play" << endl;
    cout << endl;
    cout << "  ";
    system("pause");
}

// To display the game
void game(int Column, int Row, int numZom)
{
    Board board(Column, Row);
    Alien alien;
    Zombie zombie(numZom);

    alien.spawn(board);
    zombie.stats(board, numZom);
    zombie.spawn(board);

    string input;
    int back = 1;
    do
    {
        board.display();
        alien.displayStats();
        zombie.displayStats();

        cout << endl
             << "  <command> ";
        cin >> input;

        if (input == "arrow")
        {
            int x, y;
            string direction;

            do
            {
                cout << "  "
                     << "X: ";
                cin >> x;
                cout << "  "
                     << "Y: ";
                cin >> y;
                cout << "  "
                     << "Direction: ";
                cin >> direction;

                if (board.getObject(x, y) == '^' || board.getObject(x, y) == '>' || board.getObject(x, y) == 'v' || board.getObject(x, y) == '<')
                {
                    board.item_arrow(x, y, direction);

                    if (x > 0 && y > 0 && x <= Column && y <= Row && (direction == "up" || direction == "down" || direction == "left" || direction == "right"))
                        continue;

                    invalid();
                }
                else
                {
                    break;
                }

            } while (x <= 0 || y <= 0 || x > Column || y > Row || (direction != "up" && direction != "down" && direction != "left" && direction != "right"));
        }
        else if (input == "right" || input == "left" || input == "up" || input == "down")
        {
            move(board, alien, zombie, input);
            zombie.move(board, alien);
            if (alien.getHealth() <= 0)
            {
                back = 0;
            }
        }
        else if (input == "help")
        {
            commands();
        }
        else if (input == "quit")
        {
            char yn;

            cout << "  Are you sure you want to quit? (y/n) => ";
            cin >> yn;

            if (yn == 'y')
            {
                back = 0;
            }
            else
            {
                cout << "  Back to the game..." << endl;
                cout << "  ";
                system("pause");
            }
        }
        else
        {
            invalid();
        }

    } while (back);
    load();
    index();
}

// To change the value of column, row, and zombie
void settings()
{
    int Column, Row, numZom;

    do
    {
        load();

        cout << endl
             << "  "
             << "Board Dimension" << endl
             << "  " << repeat("*", 16) << endl;

        cout << "  "
             << "Please enter odd numbers only!" << endl;
        cout << "  "
             << "Column: ";
        cin >> Column;
        cout << "  "
             << "Row   : ";
        cin >> Row;
        if (Column > 0 && Row > 0 && Column % 2 != 0 && Row % 2 != 0)
            break;

        invalid();
    } while (Column <= 0 || Row <= 0 || Column % 2 == 0 || Row % 2 == 0);

    do
    {
        load();

        cout << endl
             << "  "
             << "Zombie Settings" << endl
             << "  " << repeat("*", 16) << endl;

        cout << "  "
             << "Please note that the maximum amount of zombies is 9." << endl;
        cout << "  "
             << "Quantity of Zombie(s): ";
        cin >> numZom;
        if (numZom > 0 && numZom <= 9)
            break;

        invalid();
    } while (numZom <= 0 || numZom > 9);

    game(Column, Row, numZom);
}

// Menu
void index()
{

    cout << repeat("*", 25) << endl;
    cout << "     Alien vs Zombies" << endl;
    cout << repeat("*", 25) << endl;
    cout << endl;
    cout << "The Defult Game Setting" << endl;
    cout << repeat("*", 23) << endl;
    cout << "Column             : 15" << endl;
    cout << "Row                : 5" << endl;
    cout << "Number of Zombie   : 2" << endl;
    cout << endl;
    cout << "[1]\t-\tStart Game" << endl;
    cout << "[2]\t-\tChange Settings" << endl;
    cout << "[3]\t-\tHow to Play?" << endl;
    cout << "[4]\t-\tIn game Commands" << endl;
    cout << "[5]\t-\tQuit" << endl;

    cout << "\nSelect your options : ";
    int option;
    cin >> option;
    cout << endl;

    if (option == 1)
    {
        load();
        int Column, Row, numZom;
        game(Column = 15, Row = 5, numZom = 2);
    }
    else if (option == 2)
    {
        load();
        settings();
    }
    else if (option == 3)
    {
        instructions();
        load();
        index();
    }
    else if (option == 4)
    {
        commands();
        load();
        index();
    }
    else if (option == 5)
    {

        // Quitting the program
        //
        // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit
        message("Exitting the program...");
        sleep(1);
        exit;
    }
    else
    {
        invalid();
        index();
    }
}

int main()
{
    system("cls");
    index();
}
