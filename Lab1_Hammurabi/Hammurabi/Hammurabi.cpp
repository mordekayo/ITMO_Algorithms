#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <random>

class Game
{
public:
    Game();
    void StartGame();
private:
    void SaveGame();
    void LoadGame();
    void ShowCurrentParams() const;
    void CatchPlayerInput();
    void CalculateForNextTurn();
    [[nodiscard]] std::string GetStringInput() const;
    [[nodiscard]] int GetIntegerInput() const;
    int Year = 1000;

    std::default_random_engine RandomGenerator;
    std::uniform_int_distribution<int> AcrePriceGen;
    std::uniform_int_distribution<int> WheatPerAcreGen;
    std::uniform_real_distribution<float> WheatEatedByRatsFactorGen;
    std::uniform_int_distribution<int> PlagueProbGen;
    const int WheatSeedsToSownAcre = 5;
    const int AcresOfLandFeasibleForOneMan = 10;
    const int WheatEatenByCitizenInYear = 20;

    std::vector<float> DiedFromStarvingFactorByYear;
    int Citizens = 100;
    int Wheat = 2800;
    int Lands = 1000;
    int AcrePrice = 0;
    
    int DiedStarving = 0;
    int Newcomers = 0;
    bool Plague = false;
    int NewWheat = 0;
    int WheatPerAcre = 0;
    int WheatEatedByRats = 0;
    int WheatEatenByCitizens = 0;
    int LandsCommandedToBeBought = 0;
    int WheatCommandedToEat = 0;
    int LandsCommandedToBeSown = 0;

    bool bLose = false;
};

Game::Game()
{
    RandomGenerator.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    AcrePriceGen = std::uniform_int_distribution<int>(17, 26);
    WheatPerAcreGen = std::uniform_int_distribution<int>(1, 6);
    WheatEatedByRatsFactorGen = std::uniform_real_distribution<float>(0.0f, 0.07f);
    PlagueProbGen = std::uniform_int_distribution<int>(1,100);

    DiedFromStarvingFactorByYear.reserve(10);
}

void Game::StartGame()
{
    std::cout << "Do you want to load existing save?" << std::endl;
    bool CorrectLoadSaveInput = true;
    std::string LoadOrNotStr;
    bool LoadOrNot = false;
    do
    {
        LoadOrNotStr = GetStringInput();
        if(LoadOrNotStr._Equal("yes"))
        {
            LoadOrNot = true;
        }
        else if (LoadOrNotStr._Equal("no")) 
        {
            LoadOrNot = false;
        }
        else
        {
            std::cout << "Incorrect input, please try again" << std::endl;
            CorrectLoadSaveInput = false;
        }
    }
    while (!CorrectLoadSaveInput);
    if(LoadOrNot)
    {
        LoadGame();
    }
    while(true)
    {
        ShowCurrentParams();
        std::cout << "Do you want save and exit?" << std::endl;
        bool CorrectSaveOrNotInput = true;
        std::string SaveOrNotStr;
        bool SaveOrNot = false;
        do
        {
            SaveOrNotStr = GetStringInput();
            if(SaveOrNotStr._Equal("yes"))
            {
                SaveOrNot = true;
            }
            else if (SaveOrNotStr._Equal("no")) 
            {
                SaveOrNot = false;
            }
            else
            {
                std::cout << "Incorrect input, please try again" << std::endl;
                CorrectSaveOrNotInput = false;
            }
        }
        while (!CorrectSaveOrNotInput);
        if(SaveOrNot)
        {
            SaveGame();
            break;
        }
        if(!bLose && Year < 1010)
        {
            CatchPlayerInput();
            CalculateForNextTurn();
            system("CLS");
            Year += 1;
        }
        else
        {
            if(Year >= 1010)
            {
                float MediumDiedFromStarvingFactor = 0.0f;
                for(auto i : DiedFromStarvingFactorByYear)
                {
                    MediumDiedFromStarvingFactor += i;
                }
                MediumDiedFromStarvingFactor /= DiedFromStarvingFactorByYear.size();
                const int LandsPerCitizen = Lands/Citizens;
                
                if(MediumDiedFromStarvingFactor < 0.03f && LandsPerCitizen > 10)
                {
                    std::cout << "Fiction! Charlemagne, Disraeli and Jefferson couldn't have done better together." << std::endl;
                }
                else if(MediumDiedFromStarvingFactor < 0.10f && LandsPerCitizen > 9)
                {
                    std::cout << "You did quite well, you certainly have detractors, but many would like to see you at the head of the city again." << std::endl;
                }
                else if(MediumDiedFromStarvingFactor < 0.33f || LandsPerCitizen > 7)
                {
                    std::cout << "You ruled with an iron fist, like Nero and Ivan the Terrible." << std::endl
                    <<" The people breathed a sigh of relief, and no one else wants to see you as ruler." << std::endl;
                }
                else
                {
                    std::cout << "Because of your incompetence in government," << std::endl <<
                                 " the people have started a riot and driven you out of the city." << std::endl
                    <<" Now you are forced to drag out a miserable existence in exile." << std::endl;
                }
            }
            break;
        }
    }
}

void Game::LoadGame()
{
    std::fstream f;
    f.open("save.txt", std::fstream::in);
    assert(f.is_open());
    std::string s;
    std::getline(f, s);
    Year = std::stoi(s);
    std::getline(f, s);
    Citizens = std::stoi(s);
    std::getline(f, s);
    Wheat = std::stoi(s);
    std::getline(f, s);
    Lands = std::stoi(s);
    std::getline(f, s);
    AcrePrice = std::stoi(s);
    std::getline(f, s);
    DiedStarving = std::stoi(s);
    std::getline(f, s);
    Newcomers = std::stoi(s);
    std::getline(f, s);
    Plague = std::stoi(s);
    std::getline(f, s);
    NewWheat = std::stoi(s);
    std::getline(f, s);
    WheatPerAcre = std::stoi(s);
    std::getline(f, s);
    WheatEatedByRats = std::stoi(s);
    std::getline(f, s);
    WheatEatenByCitizens = std::stoi(s);
    std::getline(f, s);
    LandsCommandedToBeBought = std::stoi(s);
    std::getline(f, s);
    WheatCommandedToEat = std::stoi(s);
    std::getline(f, s);
    LandsCommandedToBeSown = std::stoi(s);
    DiedFromStarvingFactorByYear.clear();
    for(auto i : DiedFromStarvingFactorByYear)
    {
        std::getline(f, s);
        DiedFromStarvingFactorByYear.push_back(std::stoi(s));
    }
    f.close();
}

void Game::SaveGame()
{
    std::string Save = "";
    Save.append(std::to_string(Year));
    Save.append("\n");
    Save.append(std::to_string(Citizens));
    Save.append("\n");
    Save.append(std::to_string(Wheat));
    Save.append("\n");
    Save.append(std::to_string(Lands));
    Save.append("\n");
    Save.append(std::to_string(AcrePrice));
    Save.append("\n");
    Save.append(std::to_string(DiedStarving));
    Save.append("\n");
    Save.append(std::to_string(Newcomers));
    Save.append("\n");
    Save.append(std::to_string(Plague));
    Save.append("\n");
    Save.append(std::to_string(NewWheat));
    Save.append("\n");
    Save.append(std::to_string(WheatPerAcre));
    Save.append("\n");
    Save.append(std::to_string(WheatEatedByRats));
    Save.append("\n");
    Save.append(std::to_string(WheatEatenByCitizens));
    Save.append("\n");
    Save.append(std::to_string(LandsCommandedToBeBought));
    Save.append("\n");
    Save.append(std::to_string(WheatCommandedToEat));
    Save.append("\n");
    Save.append(std::to_string(LandsCommandedToBeSown));
    Save.append("\n");
    for(auto i : DiedFromStarvingFactorByYear)
    {
        Save.append(std::to_string(i));
        Save.append("\n");
    }
    std::fstream f;
    f.open("save.txt", std::fstream::out);
    f.write(Save.c_str(), Save.size());
    f.close();
}

void Game::ShowCurrentParams() const
{
    std::cout << "My Lord, let me tell you" << std::endl;
    std::cout << "in the " << Year << " year of your reign" << std::endl;
    if (DiedStarving > 0)
    {
        std::cout << DiedStarving << " Died from starving last year";
        if(Newcomers > 0)
        {
            std::cout << ",but";
        }
        std:: cout<< std::endl;
    }
    if(Newcomers > 0)
    {
        std::cout << Newcomers << " newcomers arrived in your domain;" << std::endl;
    }
    if(Plague)
    {
        std::cout << "The plague killed half the population;" << std::endl;
    }
    else
    {
        std::cout << "The plague has passed your domain;" << std::endl;
    }
    std::cout << "Current population is " << Citizens << " people;" << std::endl;
    if(Year > 1000)
    {
        std::cout << "We gathered " << NewWheat <<
       " bushels of wheat, " << WheatPerAcre << " bushels per acre;" << std::endl;   
    }
    if(WheatEatedByRats > 0)
    {
        std::cout << "The rats destroyed " << WheatEatedByRats << " bushels of wheat." << std::endl;
    }
    std::cout << Wheat << " bushels of wheat leaving in barns;" << std::endl;
    std::cout << "The city now covers " << Lands << " acres;" << std::endl;
    std::cout << "1 acre of lands now costs " << AcrePrice << " bushels of wheat." << std::endl;

    if(bLose)
    {
        std::cout << "YOU LOST! Too many people died starving." << std::endl;
    }
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
                std::cout << "How many acres of land you command to sell?" << std::endl;
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
        WheatToEat = GetIntegerInput();
        std::cout << "How many acres you command to be sown with wheat?" << std::endl;
        LandsToBeSown = GetIntegerInput();

        auto First = (WheatToEat + LandsToBuy * AcrePrice + LandsToBeSown * WheatSeedsToSownAcre) > Wheat;
        auto Second = LandsToBeSown > Citizens * AcresOfLandFeasibleForOneMan || LandsToBeSown > Lands + LandsToBuy;
        if(First || Second)
        {
            CorrectInput = false;
            std::cout << "My Lord, we only have" << std::endl
                      << Citizens << " people;" << std::endl
                      << Wheat << " bushels of wheat;" << std::endl
                      << Lands << " acres of land;" << std::endl;
        }
    }
    while (!CorrectInput);

    LandsCommandedToBeBought = LandsToBuy;
    WheatCommandedToEat = WheatToEat;
    LandsCommandedToBeSown = LandsToBeSown;
}

std::string Game::GetStringInput() const
{
    std::string PlayerInput;
    do
    {
        std::getline(std::cin, PlayerInput);
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
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    Plague = false;
    AcrePrice = AcrePriceGen(RandomGenerator);
    WheatPerAcre = WheatPerAcreGen(RandomGenerator);
    if(Year > 1000)
    {
        Lands += LandsCommandedToBeBought;
        NewWheat = LandsCommandedToBeSown * WheatPerAcre;
        Wheat += NewWheat;
        WheatEatedByRats = static_cast<int>(WheatEatedByRatsFactorGen(RandomGenerator) * Wheat);
        Wheat -= std::clamp(WheatEatedByRats, 0, INT_MAX);
        Wheat -= WheatCommandedToEat;
        DiedStarving = Citizens - WheatCommandedToEat / WheatEatenByCitizenInYear;
        DiedFromStarvingFactorByYear.push_back(static_cast<float>(DiedStarving) / static_cast<float>(Citizens));
        Citizens -= DiedStarving;
        if(DiedFromStarvingFactorByYear.back() > 0.45f)
        {
            bLose = true;
        }

        auto Plg = PlagueProbGen(RandomGenerator);
        if(Plg > 85)
        {
            Plague = true;
        }
        Newcomers = std::clamp(static_cast<int>(static_cast<float>(DiedStarving) / 2 + static_cast<float>((5 - WheatPerAcre) * Wheat) / 600 + 1.0f), 0, 50);
        if(Plague)
        {
            Citizens /= 2;
        }
        Citizens += Newcomers;
    }
}


int main(int argc, char* argv[])
{
    Game game;
    game.StartGame();
    return 0;
}
