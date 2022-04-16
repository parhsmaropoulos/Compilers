#include <stack>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// Η γραμματική είναι
// <E> ::= (<Y>)
// <Y> ::= <A><B>
// <A> ::= v|<E>
// <B> ::=-<Y>|+<Y>|e

// Μπορεί να οριστεί απο την τετράδα G= {S,P,N,T}
// όπου:
// S σύμβολο εκκίνησης
// P σύνολο κανόνων
// N μη τερματικά σύμβολα
// T τερματικά σύμβολα

class Rule
{
public:
    string leftSymbol;
    vector<string> rightSymbols;
};

class Grammar
{
public:
    string startSymbol;
    vector<string> nonTerminalItems;
    vector<string> terminalItems;
    vector<Rule *> rules;
    bool isTerminal(string);
    Rule *getRule(string);
};

Rule *initializeRule(string leftSymbol, vector<string> rightSymbols)
{
    Rule *new_rule = new Rule();
    new_rule->leftSymbol = leftSymbol;
    new_rule->rightSymbols = rightSymbols;
    return new_rule;
};

string applyRule(Rule *rule, string currentString)
{
    // Get the pos of the first notTerminal char in our current string
    size_t pos = currentString.find(rule->leftSymbol);
    // Now get a random replace string from the right side of our rule
    int size = rule->rightSymbols.size();
    int randomIndex = rand() % size;
    cout << "choice index:" << randomIndex << "\n";
    string replaceStr = rule->rightSymbols[randomIndex];

    // Replace our string
    currentString.replace(pos, 3, replaceStr);
    return currentString;
}

Rule *Grammar::getRule(string currentString)
{
    // Loop through all nonTerminalItems of the grammar and check if string contain any of them
    // If it finds one then returns the Rule with left symbol equal to the one it found
    for (int i = 0; i < nonTerminalItems.size(); i++)
    {
        string item = nonTerminalItems[i];
        if (currentString.find(item) != string::npos)
        {
            for (int j = 0; j < rules.size(); j++)
            {
                Rule *rule = rules[j];
                if (rule->leftSymbol == item)
                {
                    return rule;
                }
            }
        }
    }
}

bool Grammar::isTerminal(string currentString)
{
    // Loop through all nonTerminalItems of the grammar and check if string contain any of them
    // If not its at terminal state, else we can change the string
    for (int i = 0; i < nonTerminalItems.size(); i++)
    {
        string item = nonTerminalItems[i];
        if (currentString.find(item) != string::npos)
        {
            return false;
        }
    }
    return true;
}

void printString(string currentString, bool b)
{
    // if b is false then the string is printed before the rule is applied
    if (b == false)
    {
        cout << "----Before rule is applied----"
             << "\n";
        cout << currentString << "\n";
        cout << "------------------------------"
             << "\n";
    }
    else
    {
        cout << "----After rule is applied----"
             << "\n";
        cout << currentString << "\n";
        cout << "------------------------------"
             << "\n";
    }
}

int main()
{
    // Initialize our Grammar
    Grammar *new_grammar = new Grammar();

    vector<string> nonTerminalItems{"<E>", "<Y>", "<A>", "<B>"};
    vector<string> terminalItems{"n", "e"};

    new_grammar->nonTerminalItems = nonTerminalItems;
    new_grammar->terminalItems = terminalItems;
    string input;
    cout << "Select starting symbol: {<E>, <Y>, <A>, <B>}"
         << "\n";
    cin >> input;
    new_grammar->startSymbol = input;

    // Initialize our Rules
    vector<Rule *> rules;
    rules.push_back(initializeRule("<E>", vector<string>{"(<Y>)"}));
    rules.push_back(initializeRule("<Y>", vector<string>{"<A><B>"}));
    rules.push_back(initializeRule("<A>", vector<string>{"n", "<E>"}));
    rules.push_back(initializeRule("<B>", vector<string>{"-<Y>", "+<Y>", "e"}));
    new_grammar->rules = rules;

    string currentString = new_grammar->startSymbol;

    int maxiterations;
    cout << "Select max iterations (1-100)"
         << "\n";
    while (true)
    {
        cin >> maxiterations;
        if (maxiterations < 0 || maxiterations > 100)
        {
            cout << "Give a valid number! \n";
        }
        else
        {
            break;
        }
    };
    int i;
    for (i = 0; i < maxiterations; i++)
    {
        printString(currentString, false);
        // Check if terminal
        bool ok = new_grammar->isTerminal(currentString);
        if (!ok)
        {
            Rule *rule = new_grammar->getRule(currentString);
            string new_string = applyRule(rule, currentString);
            currentString = new_string;
        }
        else
        {
            cout << "Found terminal state \n";
            break;
        }
        printString(currentString, true);
    }
    if (i >= maxiterations)
    {
        cout << "Reached maximum iterations! \n";
    }
}