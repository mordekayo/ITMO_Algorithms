#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <random>

class Game
{
    struct CityParams
    {
        int Citizens = 100;
        int Wheat = 2800;
        int Lands = 1000;
        int AcrePrice = 0;
    };
    struct CityTidings
    {
        int DiedStarwing = 0;
        int Newcomers = 0;
        bool Plague = false;
        int NewWheat = 0;
        int WheatPerAcre = 0;
        int WheatEatedByRats = 0;
    };
public:
    Game();
    void Turn();
private:
    void ShowCurrentParams() const;
    void CatchPlayerInput();
    void CalculateForNextTurn();
    std::string GetStringInput() const;
    int GetIntegerInput() const;
    int Year = 0;
    CityParams CurrentParams;
    CityTidings LastYearTidings;

    std::default_random_engine RandomGenerator;
    std::uniform_int_distribution<int> AcrePriceGen;
    std::uniform_int_distribution<float> WheatEatedByRatsFactorGen;
    std::uniform_int_distribution<int> PlagueProbGen;
    const int WheatSeedsToSownAcre = 5;
    const int AcresOfLandFeasibleForOneMan = 10;
    const int WheatEatenByCitizenInYear = 20;
};

Game::Game()
{
    AcrePriceGen = std::uniform_int_distribution<int>(1, 6);
    WheatEatedByRatsFactorGen = std::uniform_int_distribution<float>(0, 0.07);
    PlagueProbGen = std::uniform_int_distribution<int>(1,100);
}

void Game::Turn()
{
    ShowCurrentParams();
    CatchPlayerInput();
    CalculateForNextTurn();
}

void Game::ShowCurrentParams() const
{
    std::cout << "My Lord, let me tell you" << std::endl;
    std::cout << "in the " << Year << " people year of your reign" << std::endl;
    if (LastYearTidings.DiedStarwing > 0)
    {
        std::cout << LastYearTidings.DiedStarwing << " Died from starving last year";
        if(LastYearTidings.Newcomers > 0)
        {
            std::cout << ",but";
        }
        std:: cout<< std::endl;
    }
    if(LastYearTidings.Newcomers > 0)
    {
        std::cout << LastYearTidings.Newcomers << " newcomers arrived in your domain;" << std::endl;
    }
    if(LastYearTidings.Plague)
    {
        std::cout << "The plague killed half the population;" << std::endl;
    }
    else
    {
        std::cout << "The plague has passed your domain;" << std::endl;
    }
    std::cout << "Current population is " << CurrentParams.Citizens << " people;" << std::endl;
    std::cout << "We gathered " << LastYearTidings.NewWheat <<
        "bushels of wheat, " << LastYearTidings.WheatPerAcre << " bushels per acre;" << std::endl;
    if(LastYearTidings.WheatEatedByRats > 0)
    {
        std::cout << "The rats destroyed " << LastYearTidings.WheatEatedByRats << " bushels of wheat, leaving" <<
            CurrentParams.Wheat << " bushels of wheat in barns;" << std::endl;
    }
    std::cout << "The city now covers " << CurrentParams.Lands << " acres;" << std::endl;
    std::cout << "1 acre of lands now costs " << CurrentParams.AcrePrice << " bushels of wheat." << std::endl;
}

void Game::CatchPlayerInput()
{
    bool CorrectInput = true;
    // true - buy, false - sell
    std::string BuyOrSellStr;
    int LandsToBuy = 0;
    int WheatToEat = 0;
    int LandsToBeSown = 0;
    
    std::cout << "What do you wish, my Lord?" << std::endl;
    do
    {
        std::cout << "You command to buy new lands, or sell them, or do nothing?" << std::endl;
        bool CorrectBuySellInput = true;
        do
        {
            BuyOrSellStr = GetStringInput();
            if(BuyOrSellStr._Equal("buy"))
            {
                std::cout << "How many acres of land you command to buy?" << std::endl;
                LandsToBuy = GetIntegerInput();
            }
            else if (BuyOrSellStr._Equal("sell")) 
            {
                std::cout << "How many acres of land you command to buy?" << std::endl;
                LandsToBuy = GetIntegerInput()*-1;
            }
            else if (BuyOrSellStr._Equal("do nothing"))
            {
                LandsToBuy = 0;
            }
            else
            {
                std::cout << "Incorrect input, please try again" << std::endl;
                CorrectBuySellInput = false;
            }
        }
        while (!CorrectBuySellInput);
        std::cout << "How many bushels of wheat you command to be eaten by your people?" << std::endl;
        std::cin >> WheatToEat;
        std::cout << "How many acres you command to be sown with wheat?" << std::endl;
        std::cin >> LandsToBeSown;

        if((WheatToEat + LandsToBuy * CurrentParams.AcrePrice + LandsToBeSown * WheatSeedsToSownAcre) <= CurrentParams.Wheat
            || LandsToBeSown > CurrentParams.Citizens * AcresOfLandFeasibleForOneMan)
        {
            CorrectInput = false;
            std::cout << "My Lord, we only have" << std::endl
                      << CurrentParams.Citizens << " people;" << std::endl
                      << CurrentParams.Wheat << " bushels of wheat;" << std::endl
                      << CurrentParams.Lands << " acres of land;" << std::endl;
        }
    }
    while (!CorrectInput);   
}

std::string Game::GetStringInput() const
{
    std::string PlayerInput;
    do
    {
        std::cin >> PlayerInput;
        if (PlayerInput.empty())
        {
            std::cout << "Please enter a non-empty string" << std::endl;
        }
        std::ranges::transform(PlayerInput.begin(), PlayerInput.end(), PlayerInput.begin(),[](unsigned const char c){ return std::tolower(c); });
    }
    while (PlayerInput.empty());
    return PlayerInput;
}

int Game::GetIntegerInput() const
{
    std::string PlayerInput;
    int Output = INT32_MIN;
    do
    {
        std::cin >> PlayerInput;
        try
        {
            Output = std::stoi(PlayerInput); 
        }
        catch (std::invalid_argument const& ex)
        {
            std::cout << "Please enter an integer" << std::endl;
        }
        catch (std::out_of_range const& ex)
        {
            std::cout << "Please enter integer in correct range" << std::endl;
        }
        if (Output < 0)
        {
            std::cout << "Please enter non-negative number" << std::endl;
        }
    }
    while (Output < 0);
    return Output;
}

void Game::CalculateForNextTurn()
{
    LastYearTidings.WheatPerAcre = AcrePriceGen(RandomGenerator);
    CurrentParams.Wheat += CurrentParams.Lands * LastYearTidings.WheatPerAcre;
    LastYearTidings.WheatEatedByRats = WheatEatedByRatsFactorGen(RandomGenerator) * CurrentParams.Wheat;
    if(CurrentParams.Citizens * WheatEatenByCitizenInYear - CurrentParams.Wheat < 0)
    {
        CurrentParams.Citizens -= (CurrentParams.Wheat - CurrentParams.Citizens * WheatEatenByCitizenInYear)*(-1) ; 
    }
}

int main(int argc, char* argv[])
{
    return 0;
}
