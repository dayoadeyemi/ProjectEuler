#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>

std::string rank_name[10] = 
{
    "High Card",
    "One Pair",
    "Two Pairs",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush",
    "Royal Flush",
};

std::map<char, int> card_value_map;
char card_value_rep[13] = 
{ 
    'A', 'K', 'Q', 'J', 'T', 
    '9', '8', '7', '6', '5', 
    '4', '3', '2' 
};

void InitCardValueMap()
{
    for (int i = 0; i < 13; i++)
    {
        card_value_map[card_value_rep[i]] = i;
    }
}

struct Card 
{
    char rep[3];
    int value;
    char suit;

    void Normalize()
    {
        value = card_value_map[rep[0]];
        suit = rep[1];
    }
};

struct PokerHand
{
    Card cards[5];
    int rank;
    int high_card_count;
    int high_card_value[5];

    bool operator > (const PokerHand& other) const
    {
        if (rank != other.rank)
        {
            return rank > other.rank;
        }

        for (int i = 0; i < high_card_count; i++)
        {
            if (high_card_value[i] != other.high_card_value[i])
            {
                return high_card_value[i] < other.high_card_value[i];
            }
        }
        return false;
    }

    void Normalize()
    {
        Init();
        if (!TryToSetStraightOrFlushRank())
        {
            SetOtherRank();
        }        
    }

    void Init()
    {
        for (int i = 0; i < 5; i++)
        {
            cards[i].Normalize();
        }
        rank = -1;
        high_card_count = -1;
    }

    bool TryToSetStraightOrFlushRank()
    {
        bool is_straight = IsStraight();
        bool is_flush = IsFlush();
        if (is_straight)
        {
            rank = is_flush ? 8 : 4;
            BuildStraightHighCards();
            return true;
        }
        else if (is_flush)
        {
            rank = 5;
            BuildHighCardHighCards();
            return true;
        }
        return false;
    }

    void SetOtherRank()
    {
        int card_value_freq[13] = {};
        for (int i = 0; i < 5; i++)
        {
            card_value_freq[cards[i].value]++;
        }

        std::vector<int> four;
        std::vector<int> three;
        std::vector<int> pair;
        for (int i = 0; i < 13; i++)
        {
            if (card_value_freq[i] == 4)
            {
                four.push_back(i);
            }
            else if (card_value_freq[i] == 3)
            {
                three.push_back(i);
            }
            else if (card_value_freq[i] == 2)
            {
                pair.push_back(i);
            }
        }

        if (four.size())
        {
            rank = 7;
            high_card_count = 1;
            high_card_value[0] = four[0];
        }
        else if (three.size())
        {
            rank = (pair.size()) ? 6 : 3;
            high_card_count = 1;
            high_card_value[0] = three[0];
        }
        else if (pair.size() == 2)
        {
            rank = 2;
            high_card_count = 3;
            if (pair[0] < pair[1])
            {
                high_card_value[0] = pair[0];
                high_card_value[1] = pair[1];
            }
            else
            {
                high_card_value[0] = pair[1];
                high_card_value[1] = pair[0];
            }
            for (int i = 0; i < 13; i++)
            {
                if (card_value_freq[i] == 1)
                {
                    high_card_value[2] = i;
                }
            }
        }
        else if (pair.size() == 1)
        {
            rank = 1;
            high_card_count = 4;
            high_card_value[0] = pair[0];
            int pos = 1;
            for (int i = 0; i < 13; i++)
            {
                if (card_value_freq[i] == 1)
                {
                    high_card_value[pos] = i;
                    pos++;
                }
            }
        }
        else
        {
            rank = 0;
            high_card_count = 5;
            int pos = 0;
            for (int i = 0; i < 13; i++)
            {
                if (card_value_freq[i] == 1)
                {
                    high_card_value[pos] = i;
                    pos++;
                }
            }
        }
    }

    bool IsStraight() const 
    {
        bool visited[13] = {};
        for (int i = 0; i < 5; i++)
        {
            visited[cards[i].value] = true;
        }

        int pos = 0;
        for (; !visited[pos]; pos++);

        for (int i = pos; i < pos + 5; i++)
        {
            if (!visited[i])
            {
                return false;
            }
        }
        return true;
    }

    bool IsFlush() const
    {
        int common_suit = cards[0].suit;
        for (int i = 1; i < 5; i++)
        {
            if (cards[i].suit != common_suit)
            {
                return false;
            }
        }
        return true;
    }

    void BuildStraightHighCards()
    {
        high_card_count = 1;
        high_card_value[0] = cards[0].value;
        for (int i = 1; i < 5; i++)
        {
            if (cards[i].value < high_card_value[0])
            {
                high_card_value[0] = cards[i].value;
            }
        }
    }

    void BuildHighCardHighCards()
    {
        high_card_count = 5;
        for (int i = 0; i < 5; i++)
        {
            high_card_value[i] = cards[i].value;
        }
        std::sort(high_card_value, high_card_value + 5);
    }

    void Debug() const 
    {
        std::cout << "Hand: ";
        for (int i = 0; i < 5; i++)
        {
            std::cout << cards[i].rep << " ";
        }
        std::cout << std::endl;
        std::cout << "Rank: " << rank_name[rank] << std::endl;
        std::cout << "High Cards: ";
        for (int i = 0; i < high_card_count; i++)
        {
            std::cout << card_value_rep[high_card_value[i]] << " ";
        }
        std::cout << std::endl;
    }
};

// -----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    InitCardValueMap();
    PokerHand player[2];

    int count = 0;
    while (std::cin >> 
        player[0].cards[0].rep >> player[0].cards[1].rep >> 
        player[0].cards[2].rep >> player[0].cards[3].rep >> player[0].cards[4].rep >> 
        player[1].cards[0].rep >> player[1].cards[1].rep >> 
        player[1].cards[2].rep >> player[1].cards[3].rep >> player[1].cards[4].rep)
    {
        player[0].Normalize();
        player[1].Normalize();

        if (player[0] > player[1])
        {
            count++;
        }
    }
    std::cout << count << std::endl;

    return 0;
}