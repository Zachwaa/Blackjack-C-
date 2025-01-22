#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>

class deck;

struct card
{
    int val;
    std::string house;
    std::string name;
    inline std::string fullName()
    {
        return (name + " of " + house);
    }
};

class player
{
private:
    int cards_sum;
    std::vector<card> cards;

public:
    bool bust;

    virtual void move(deck *d) = 0;

    ~player() {}

    inline std::vector<card> _getCards()
    {
        return cards;
    }

    int _sumCards()
    {
        int sum = 0;
        for (card i : cards)
        {
            sum += i.val;
        }
        return sum;
    }

    void _addCard(card c) { cards.push_back(c); }

    void _removeCard() { cards.pop_back(); }

    void _clearHand() { cards.clear(); }

    bool _bust()
    {
        bool busted = false;
        for (card &i : cards)
        {
            if ((i.name == "Ace"))
            {
                i.val = 11;
                if (_sumCards() > 21)
                {
                    i.val = 1;
                }
            }
        }
        if (_sumCards() > 21)
        {
            busted = true;
        }

        return busted;
    }
};

class deck
{
public:
    size_t size = 52;
    std::deque<card> Cards;

    deck()
    {
        create_deck();
    }

    void create_deck()
    {

        std::string houses[4] = {"Diamond", "Clubs", "Hearts", "Spades"};
        for (std::string p : houses)
        {
            for (int i = 1; i <= 13; i++)
            {

                std::string l;
                int k;
                switch (i)
                {
                case 11:
                    l = "Jack";
                    k = 10;
                    break;
                case 12:
                    l = "Queen";
                    k = 10;
                    break;
                case 13:
                    l = "King";
                    k = 10;
                    break;
                case 1:
                    l = "Ace";
                    k = 11;
                    break;
                default:
                    l = std::to_string(i);
                    k = i;
                }

                struct card c;
                c.house = p;
                c.name = l;
                c.val = k;

                Cards.push_back(c);
            }
        }
    }

    void _drawCard(player *subject)
    {
        card active;
        active = Cards.front();
        Cards.pop_front();
        subject->_addCard(active);
    }

    void _shuffleDeck()
    {
        std::random_shuffle(Cards.begin(), Cards.end());
    }

    void _deleteDeck() { Cards.clear(); }
};

class player1 : public player
{
public:
    int cash;
    player1(int cash)
    {
        cash = cash;
    }

    void move(deck *d) override
    {

        if (!_bust())
        {
            std::string x;
            std::cout << "Hit or Stick: ";
            std::cin >> x;

            if (x == "hit")
            {
                d->_drawCard(this);
                card newCard = this->_getCards()[this->_getCards().size() - 1];
                std::cout << newCard.fullName() << '\n';
                move(d);
            }
            else if (x == "stick")
            {
                std::cout << "STICKING on " << _sumCards() << '\n';
            }
            else
            {
                std::cout << "INCORRECT\n";
            }
        }
        else
        {
            std::cout << "BUST\n";
        }
    }
};

class dealer : public player
{
public:
    void move(deck *d) override
    {
        if (!_bust())
        {
            if (_sumCards() < 17)
            {
                d->_drawCard(this);
                card newCard = this->_getCards()[this->_getCards().size() - 1];
                std::cout << newCard.fullName() << '\n';
                move(d);
            }
            else
            {
                std::cout << "Dealers sticking on" << _sumCards() << '\n';
            }
        }
        else
        {
            std::cout << "Dealers bust\n";
        }
    }
};

class ai : public player
{
public:
    void move(deck *d) override
    {
    }
};

class Game
{
public:
    std::vector<player *> players;
    deck Deck;
    deck *d = &Deck;

    void run(int x)
    {

        initalise(x);

        // Create main game loop
        while (running)
        {
            frame();
        }

        //   Shutdown();
    };

private:
    void initalise(int x)
    {
        std::vector<player *> totalPlayers;
        totalPlayers.push_back(new player1(5000));
        int y = 1;

        try
        {
            if (0 <= x <= 3)
            {
                while (y <= x)
                {
                    totalPlayers.push_back(new ai);
                    y++;
                }
            }
            else
            {
                throw x;
            }
        }
        catch (int x)
        {
            std::cout << "Integer for len of ai must be <= 3 \n";
            std::cout << "Error Number:" << x;
        }

        totalPlayers.push_back(new dealer);

        players = totalPlayers;
    };
    void Shutdown(){};
    void frame()
    {
        d->_shuffleDeck();
        std::vector<card> playerCards;

        for (player *p : players)
        {
            d->_drawCard(p);
            d->_drawCard(p);
        }

        playerCards = players[0]->_getCards();

        std::cout << "Dealer Cards: " << players[players.size() - 1]->_getCards()[0].fullName() << '\n';

        std::cout << "Your Cards: " << playerCards[0].fullName() << " , ";
        std::cout << playerCards[1].fullName() << '\n';

        std::vector<int> scores;

        for (int i = 0; i < players.size(); i++)
        {
            players[i]->move(d);
        }

        running = false;
    };

    bool running = true;
};

int main()
{
    srand(time(0));
    Game game;
    game.run(0);
    return 0;
}