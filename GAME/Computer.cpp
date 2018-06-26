#include "Computer.h"
#include "smartstr.h"
#include "Game.h"
#include <algorithm>

Computer::Computer(bool dupes):
Decrypter(dupes),
total(0),
mySmartString(0)
{

}

Computer::~Computer()
{

}

void Computer::GenerateAGuess()
{

    bool ok = true;
    int counter = 0;
    int start = time( NULL );
    do
    {
        counter++;
        total++;
        if ( counter % 10000 == 0 )
            cout << ".";

        ok = mySmartString->GetNext();

        if ( !ok )
        {
            cout << "Something went wrong!";
            cout << " Please start over\n";
            round = 0;
            delete mySmartString;
            mySmartString = new SmartString(duplicates);
            ShowHistory();
            cout << "\n\n";
            history.clear();
            continue;
        }

    } while ( !IsConsistent(mySmartString->GetString()) );

    int end = time( NULL );
    int seconds = end - start;
    cout << "(" << counter ;
    cout << " strings eliminated this round; ";
    cout << total << " total.)";
    if ( seconds > 1 )
        cout << " [" << seconds << " seconds]";
    cout << "\n";
}

// Handle any flag from user (-?,-q, etc.)
bool Computer::HandleFlag(char flag)
{
    bool quit = false;
    switch (flag)
    {
    case 's':
        ShowHistory();
        break;
    case '?':
        ShowHelp();
        break;
    case 'q':
        quit = true;
        break;
    default:
        cout << "\nUnknown flag. Ignored.\n";
        break;
    }
    return quit;
}

bool Computer::IsConsistent(vector<char> theGuess)
{


    if ( ! duplicates)
    {
        for (
            vector<char>::const_iterator it =
                theGuess.begin();
            it != theGuess.end();
            it++
            )
        {
            int HowMany =
                count(theGuess.begin(), theGuess.end(),*it);
            if ( HowMany > 1 )
                return false;
        }
    }

    bool isValid = true;

    int correct;
    int position;

    for (
        vector<Guess>::const_iterator it =
            history.begin();
        it != history.end();
        it++
        )
    {

        vector <char> temp = it->GetString();

        correct = 0;
        position = 0;
	  int i;
        for ( i = 0; i < Game::howManyLetters; i++)
        {
            int howManyInGuess =
                count (
                    theGuess.begin(),
                    theGuess.end(),
                    alpha[i]
                    );
            int howManyInAnswer =
                count (temp.begin(), temp.end(), alpha[i]);
            correct += min(howManyInGuess, howManyInAnswer);
        }

        for (  i = 0; i < Game::howManyPositions; i++)
        {
            if ( theGuess[i] == temp[i] )
                position++;
        }

        if ( correct != it->GetScore().first ||
                position != it->GetScore().second )
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

Guess Computer::OfferGuess()
{
    vector<char> theGuess =
        mySmartString->GetString();
    round++;
    int numCorrect, numInPosition;
    cout << "\n";
    Display(theGuess);
    cout << "Round " << round << ". ";
    cout << "Please record score. \t";
    cout << "How many correct?: ";
    cin >> numCorrect;
    cout << "\t\t\tHow many in position?: ";
    cin >> numInPosition;
    Guess thisGuess(theGuess,numCorrect,numInPosition);

    return thisGuess;

}

void Computer::Play()
{

    if ( ! mySmartString )
        mySmartString = new SmartString(duplicates);

    vector<char> theGuess;
    history.clear();

    bool deletedCharacters = false;

    for ( ;; )
    {
        Guess theGuess = OfferGuess();
        history.push_back(theGuess);

        if ( theGuess.GetScore().second ==
            Game::howManyPositions )
            break;

        if (
            ! mySmartString->CanEliminateCharacters(theGuess) ||
            ! IsConsistent(mySmartString->GetString())
            )
                GenerateAGuess();
    };
}

// result of pressing -?
void Computer::ShowHelp()
{
    cout << "\t-s Show history\n\t-? Help\n";
    cout << "\t-q quit\n" << endl;
}