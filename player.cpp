#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int num_ques = 15;

class Player;
class Character;
class GameEntity;

class Combatant {
public:
    void attackgame(Player& player);
};

class Scholar {
public:
    void quizguard(Player& player, int m = 10);
protected:
    static string ques[num_ques];
    static char answers[num_ques];
};

class Explorer {
public:
    void treasurehunt(Player& player);
};

class GameEntity {
protected:
    int entityID;
    static int nextID;
public:
    GameEntity() {
        entityID = nextID++;
    }
    int getID() const { return entityID; }
};
int GameEntity::nextID = 1;

class Character : public GameEntity {
protected:
    int score;
    int losses;

    void checkLoss(bool lost) {
        if (lost) {
            losses++;
            cout << "Consecutive failures: " << losses << '\n';
            if (losses >= 3) {
                -(*this);
            }
        } else {
            losses = 0;
        }
    }
public:
    Character() {
        score = 0;
        losses = 0;
    }
    void operator-() {
        score = 0;
        losses = 0;
        cout << "Character's score has been reset due to 3 consecutive failures!\n";
    }
};

class Player : public Character, public Combatant, public Scholar, public Explorer {
    friend class Combatant;
    friend class Scholar;
    friend class Explorer;

protected:
    int game;
    int qs;
    
public:
    static int totalPlayers;
    static int gamesPlayed[3];

    Player() {
        game = 0;
        qs = 0;

    }

    Player(int s, int g, int q) {
        score = s;
        game = g;
        qs = q;
        if (g >= 1 && g <= 3) {
             gamesPlayed[g - 1]++; 
            }
    }

    Player(const Player &p) {
        score = p.score;
        game = p.game;
        qs = p.qs;
        losses = p.losses;
    }
    
    int getGameType() const { return game; }
    friend void topscores(Player ps[], int n);

    void updatescore();
    void updatescore(int bonus);
    void updatescore(const string& special);
    void displayscore();
    void play();
    void reset(int g, int q);

    Player operator+(const Player& other) {
        Player temp;
        temp.score = this->score + other.score;
        temp.game = 0;
        temp.qs = 0;
        return temp;
    }

    static void displayStats() {
        cout << "\n--- Game Statistics ---\n";
        cout << "Total players: " << totalPlayers << "\n";
        cout << "'Defeat the Enemy' played: " << gamesPlayed[0] << " times\n";
        cout << "'Quiz Guard' played: " << gamesPlayed[1] << " times\n";
        cout << "'Treasure Hunt' played: " << gamesPlayed[2] << " times\n";
        cout << "-----------------------\n";
    }
};

int Player::totalPlayers = 0;
int Player::gamesPlayed[3] = {0, 0, 0};

string Scholar::ques[num_ques] = {
    "What is the capital of Karnataka? \n(a) Bangalore \n(b) Delhi \n(c) Hyderabad",
    "What is the powerhouse of the cell? \n(a) Nucleus \n(b) Ribosome \n(c) Mitochondria",
    "What is 5 * 6? \n(a) 11 \n(b) 30\n(c) 56", "Which planet is known as the Red Planet? \n(a) Earth \n(b) Mars \n(c) Venus",
    "How many continents are there? \n(a) Six \n(b) Five \n(c) Seven", "Which language is used for Android apps? \n(a) Java \n(b) Python \n(c) Swift",
    "What does CPU stand for? \n(a) Central Performance Unit \n(b) Central Processing Unit \n(c) Computer Personal Unit",
    "Which ocean is largest? \n(a) Atlantic \n(b) Indian \n(c) Pacific", "Who wrote 'Romeo and Juliet'? \n(a) Dickens \n(b) Shakespeare \n(c) Tolstoy",
    "Symbol for gold? \n(a) Gd \n(b) Au \n(c) Ag", "Planet with most moons? \n(a) Saturn \n(b) Mars \n(c) Jupiter",
    "Currency of Japan? \n(a) Won \n(b) Yuan \n(c) Yen", "Egypt is in? \n(a) Asia \n(b) Africa \n(c) Europe",
    "Bees collect? \n(a) Dew \n(b) Pollen \n(c) Nectar", "Father of Computers? \n(a) Alan Turing \n(b) Steve Jobs \n(c) Charles Babbage"
};
char Scholar::answers[num_ques] = {'a','c','b','b','c','a','b','c','b','b','a','c','b','c','a'};

void Combatant::attackgame(Player& player) {
    player.score = 0;
    Player::gamesPlayed[0]++;
    cout << "Defeat the enemy to move ahead!\nEnemy HP: 100\nYou have 7 chances.\n";
    for (int i = 1; i <= 7; i++) {
        cout << "Chance " << i << ": ";
        string action;
        cin >> action;
        if (action == "attack") {
            player.score += 20;
            cout << "Hit! Enemy HP: " << 100 - player.score << "\n";
            player.checkLoss(false);
        } else {
            cout << "Miss!\n";
            player.score -= 10;
            cout << "Enemy HP healed: " << 100 - player.score << "\n";
            player.checkLoss(true);
        }
        cout << "Score: " << player.score << "\n";
    }
}

void Scholar::quizguard(Player& player, int m) {
    player.score = 0;
    player.qs = m;
    Player::gamesPlayed[1]++;
    for (int i = 0; i < m; i++) {
        if (i >= num_ques) {
            cout << "No more questions available.\n";
            break;
        }
        cout << "Q" << i+1 << ":\n" << ques[i] << "\nYour answer: ";
        char ans;
        cin >> ans;
        if (ans == answers[i]) {
            cout << "Correct!\n";
            player.score += 2;
            player.checkLoss(false);
        } else {
            cout << "Wrong!\n";
            player.score -= 1;
            player.checkLoss(true);
        }
    }
}

void Explorer::treasurehunt(Player& player) {
    player.score = 0;
    Player::gamesPlayed[2]++;
    cout << "You enter the ancient dungeon seeking treasure...\n";
    string options[3] = {"north", "east", "west"};
    string correctPaths[5];
    int shift = (Player::totalPlayers + Player::gamesPlayed[2]) % 3;
    for (int i = 0; i < 5; i++) {
        correctPaths[i] = options[(i + shift) % 3];
        shift = (shift + 1) % 3;
    }
    for (int room = 0; room < 5; room++) {
        cout << "\nRoom " << room+1 << ": Choose (north/east/west): ";
        string choice;
        cin >> choice;
        if (choice == correctPaths[room]) {
            cout << "Treasure found! (+5 points)\n";
            player.score += 5;
            player.checkLoss(false);
        } else {
            cout << "Trap triggered! (-3 points)\n";
            player.score -= 3;
            player.checkLoss(true);
        }
    }
}

void Player::updatescore() {
    int c;
    cout << "Choose a game to play \n";
    cout << "1. Defeat the enemy \n";
    cout << "2. Quiz Guard \n";
    cout << "3. Treasure Hunt \n";
    cout << "Enter your choice (1, 2, or 3): ";
    cin >> c;
    game = c;
    if (c == 1)
    { attackgame(*this); }
    else if (c == 2) {
        cout << "How many questions do you want to answer? (Max " << num_ques << "): ";
        cin >> this->qs;
        if (this->qs >= 1 && this->qs <= num_ques) 
        { quizguard(*this, this->qs); }
        else {
            cout << "Invalid input. Defaulting to 10 questions.\n";
            quizguard(*this);
        }
    }
    else if (c == 3)
    { treasurehunt(*this); }
    else {
        cout << "Invalid choice. No game played.\n";
        score = 0;
    }
}

void Player::updatescore(int bonus) {
    cout << "\nPower-Up! You received a bonus of " << bonus << " points!\n";
    score += bonus;
}

void Player::updatescore(const string& special) {
    if (special == "critical_hit") {
        cout << "\nBOOM! CRITICAL HIT! +35 bonus points!\n";
        score += 35;
    } else if (special == "perfect_quiz") {
        cout << "\nYou charmed the guard into giving you extra points. +10 bonus points!\n";
        score += 10;
    } else if (special == "secret_gem") {
        cout << "\nYou got lucky and FOUND A SECRET GEM! +15 bonus points!\n";
        score += 15;
    }
}

void Player::play() {
    if (game == 1)
    { attackgame(*this); }
    else if (game == 2)
    {
        if (qs < 1 || qs > num_ques) qs = 10;
        quizguard(*this, qs);
    } 
    else if (game == 3)
    { treasurehunt(*this); }
    else { cout << "No valid game selected.\n"; }
}

void Player::reset(int g, int q) {
    score = 0;
    losses = 0;
    game = g;
    qs = q;
    if (game >= 1 && game <= 3)
    { gamesPlayed[game - 1]++; }
}

void Player::displayscore() {
    if (game == 1) {
        cout << "Game: Defeat the Enemy - Score: " << score << "/100\n";
        if (score >= 100)
        { cout << "You annihilated the enemy!\n"; }
        else
        { cout << "You just had to copy down a word mate.\n"; }
    }
    else if (game == 2) {
        cout << "Game: Quiz Guard - Score: " << score << "/" << (qs*2) << "\n";
        if (score >= qs*2)
        { cout << "Your intellect vaporized the guard and the door opened itself\n"; }
        else if (score >= qs)
        { cout << "The guard was satisfied and opened the door for you\n"; }
        else
        { cout << "the guard couldn't fathom your stupidity, hence he incenerated you\n"; }
    }
    else if (game == 3) {
        cout << "Game: Treasure Hunt - Score: " << score << "/25\n";
        if (score >= 17)
        { cout << "Congrats! You're rich now\n"; }
        else if (score >= 10)
        { cout << "At least you can pay some of your bill now\n"; }
        else
        { cout << "You left the dungeon empty handed and hungry.\n"; }
    }
    else
    { cout << "Combined Team Score: " << score << "\n"; }
}

void topscores(Player ps[], int count) {
    int topA = -1, topQ = -1, topT = -1;
    int scoreA = -1000, scoreQ = -1000, scoreT = -1000;
    for (int i = 0; i < count; i++) {
        if (ps[i].game == 1 && ps[i].score > scoreA)
        { scoreA = ps[i].score; topA = i; }
        if (ps[i].game == 2 && ps[i].score > scoreQ)
        { scoreQ = ps[i].score; topQ = i; }
        if (ps[i].game == 3 && ps[i].score > scoreT)
        { scoreT = ps[i].score; topT = i; }
    }
    cout << "\n--- Top Scorers ---\n";
    if (topA != -1)
    { cout << "'Defeat the Enemy': Player " << topA+1 << " (" << scoreA << " points)\n"; }
    if (topQ != -1)
    { cout << "'Quiz Guard': Player " << topQ+1 << " (" << scoreQ << " points)\n"; }
    if (topT != -1)
    { cout << "'Treasure Hunt': Player " << topT+1 << " (" << scoreT << " points)\n"; }
}

int main() {
    srand(time(0));
    int n;
    Player p[10];
    cout << "\nWelcome to the Dungeon RPG!\nEnter no. of Players (1-10): ";
    cin >> n;
    Player::totalPlayers = n;
    if (n < 1 || n > 10)
    { cout << "Invalid number of players.\n"; return 0; }

    bool teamMode = false;
    if (n > 1 && n % 2 == 0) {
        char choice;
        cout << "Even number of players detected. Do you want to play in TEAMS of 2? (y/n): ";
        cin >> choice;
        teamMode = (choice == 'y');
    }

    if (!teamMode) {
        for (int i = 0; i < n; i++) {
            cout << "\n--- Player " << i+1 << " (ID: " << p[i].getID() << ") ---\n";
            p[i].updatescore();
            int bonusChance = rand() % 10;
            if (bonusChance < 2) { p[i].updatescore(20); }
            else if (bonusChance == 5) {
                int gameType = p[i].getGameType();
                if (gameType == 1)
                { p[i].updatescore("critical_hit"); }
                else if (gameType == 2)
                { p[i].updatescore("perfect_quiz"); }
                else
                { p[i].updatescore("secret_gem"); }
            }
            char restart;
            cout << "\nRestart? (y/n): ";
            cin >> restart;
            if (restart == 'y') {
                int g, q = 0;
                cout << "Which game to restart? (1=Attack, 2=Quiz, 3=Treasure): ";
                cin >> g;
                if (g == 2) {
                    cout << "How many quiz questions? (Max " << num_ques << "): ";
                    cin >> q;
                }
                p[i].reset(g, q);
                p[i].play();
            }
        }
        cout << "\n--- Final Scores ---\n";
        for (int i = 0; i < n; i++) {
            cout << "\nPlayer " << i+1 << ": ";
            p[i].displayscore();
        }
        topscores(p, n);
    } else {
        cout << "\n Play similar games to keep it fair.\nEach pair is a team.\nEach player plays their own game.\n";
        Player teams[5];
        int teamCount = n / 2;
        for (int i = 0; i < n; i++) {
            cout << "\n--- Player " << i+1 << " (ID: " << p[i].getID() << ") ---\n";
            p[i].updatescore();
        }
        cout << "\n--- Forming Teams ---\n";
        for (int t = 0; t < teamCount; t++) {
            teams[t] = p[2*t] + p[2*t+1];
            cout << "Team " << t+1 << " formed from Player " << 2*t+1 << " + Player " << 2*t+2 << "\n";
        }
        cout << "\n--- Team Scores ---\n";
        for (int t = 0; t < teamCount; t++) {
            cout << "Team " << t+1 << ": ";
            teams[t].displayscore();
        }
    }

    Player::displayStats();
    cout << "\nPress 'Enter' key to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}