#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// Το ζητούμενο ντετερμινιστικό αυτόματο είναι μία 7-άδα

// Μ = (Q, S, G, q0, Z0, d, F)
// 'Οπου:
// Q = σύνολο καταστάσεων
// S = αλφάβητο συμβόλων εισόδου
// G = αλφάβητο συμβόλων στοίβασ
// q0 = αρχική κατάσταση
// Z0 = αρχικό σύμβολο σωρού
// d = συνάρτηση μετάβασης
// F = σύνολο τελικών καταστάσεων

// Q = {q0,q1,q2}
// S = {x,y}
// G = {Z0, x}
// q0 = q0
// Z0
// d = [
// 1. d(q0,e,Z0)= (q0,Z0)
// 2. d(q0,0,Z0)= (q0,0Z0)
// 3. d(q0,0,0)= (q0,00)
// 4. d(q0,1,0)= (q0,e)
// 5. d(q1,1,0)= (q1,e)
// 6. d(q1,e,Z0)= (q2,Z0)
// ]
// F={q2}

class transferFunction
{
public:
    string currentState;
    string inputChar;
    string currentTopStackChar;

    string nextState;
    string outputChar;
};

class DAS
{
public:
    vector<string> states;
    vector<string> inputSymbols;
    vector<string> stackSymbols;
    string currentState;
    string initialState;
    string initialStackChar;
    vector<transferFunction *> transferFuncs;
    vector<string> finalStates;
    stack<string> dasStack;
    void CommitTransferFunction(transferFunction *);
    transferFunction *GetTransferFunc(DAS *, string);
};

void DAS::CommitTransferFunction(transferFunction *transferFunc)
{
    string inputChar = transferFunc->inputChar;
    // Check the input symbol
    bool foundInStack = (find(stackSymbols.begin(), stackSymbols.end(), inputChar) != stackSymbols.end());
    bool foundInInput = (find(inputSymbols.begin(), inputSymbols.end(), inputChar) != inputSymbols.end());
    // cout << "input char:" << inputChar << "\n";
    // cout << "found in stack:" << foundInStack << "\n";
    // cout << "found in input:" << foundInInput << "\n";

    currentState = transferFunc->nextState;
    // If it is in the stackSymbol list then append it to the stack.
    if (foundInStack)
    {
        cout << "pushed char: " << inputChar << "\n";
        dasStack.push(inputChar);
    }
    // If it is in the inputSymbol but not in stackSymbols then pop from the stack.
    else if (foundInInput)
    {
        if (dasStack.top() == initialStackChar)
        {
            cout << "stack is empty and y found, string rejected!"
                 << "\n";
        }
        dasStack.pop();
        cout << "y found and item popped from stack.Top item of stack :" << dasStack.top() << "\n";
    }
}

transferFunction *DAS::GetTransferFunc(DAS *das, string input)
{
    for (int i = 0; i < das->transferFuncs.size(); i++)
    {
        transferFunction *trans = das->transferFuncs[i];
        if (trans->currentState == das->currentState && trans->currentTopStackChar == das->dasStack.top() && trans->inputChar == input)
        {
            return trans;
        }
    }
    return NULL;
    // transferFunction *func = new transferFunction();
    // func->currentState = das->currentState;
    // func->currentTopStackChar = das->dasStack.top();
    // func->inputChar = input;
}

transferFunction *initializeTransferFunc(string currentState, string inputChar, string currentTopChar, string nextState, string outputChar)
{
    transferFunction *new_transf = new transferFunction();
    new_transf->currentState = currentState;
    new_transf->inputChar = inputChar;
    new_transf->currentTopStackChar = currentTopChar;
    new_transf->nextState = nextState;
    new_transf->outputChar = outputChar;
    return new_transf;
}

bool isAccepted(DAS *das)
{
    // Check if is in final state
    bool isInFinal = (find(das->finalStates.begin(), das->finalStates.end(), das->currentState) != das->finalStates.end());

    if (das->dasStack.top() == "z0" && isInFinal)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// IF its called with bool false it prints the state before the trans. function
void printStatus(DAS *das, bool b, string input)
{
    if (b == false)
    {
        cout << "----Before the function---- \n ";
        cout << "Current State :" << das->currentState << "\n";
        cout << "Stack top item :" << das->dasStack.top() << "\n";
        cout << "Input char :" << input << "\n";
        cout << "--------------------------- \n ";
    }
    else
    {
        cout << "----After the function---- \n ";
        cout << "Current State :" << das->currentState << "\n";
        cout << "Stack top item :" << das->dasStack.top() << "\n";
        cout << "--------------------------- \n ";
    }
}

int main()
{
    // Initialize the DAS
    DAS *new_das = new DAS();
    vector<string> states{"q0", "q1", "q2"};
    vector<string> inputSymbols{"x", "y"};
    vector<string> stackSymbols{"z0", "x"};
    string initialState = "q0";
    string initialSymbol = "z0";
    vector<string> finalStates = {"q2"};

    new_das->states = states;
    new_das->inputSymbols = inputSymbols;
    new_das->stackSymbols = stackSymbols;
    new_das->finalStates = finalStates;
    new_das->initialState = initialState;
    new_das->currentState = initialState;
    new_das->initialStackChar = initialSymbol;

    stack<string> dasStack;
    dasStack.push("z0");
    new_das->dasStack = dasStack;

    // Initialize Transfer Functions

    vector<transferFunction *> transferFuncs;

    transferFuncs.push_back(initializeTransferFunc("q0", "e", "z0", "q0", "z0"));
    transferFuncs.push_back(initializeTransferFunc("q0", "x", "z0", "q0", "x"));
    transferFuncs.push_back(initializeTransferFunc("q0", "x", "x", "q0", "x"));
    transferFuncs.push_back(initializeTransferFunc("q0", "y", "x", "q1", "e"));
    transferFuncs.push_back(initializeTransferFunc("q1", "y", "x", "q1", "e"));
    transferFuncs.push_back(initializeTransferFunc("q1", "e", "z0", "q2", "z0"));

    new_das->transferFuncs = transferFuncs;

    string inputString;
    cout << "Type your string"
         << "\n";
    cin >> inputString;
    for (int i = 0; i <= inputString.length(); i++)
    {
        string input = string(1, inputString[i]);
        // Check if the next input is null
        if (inputString[i] == '\0')
        {
            input = string(1, 'e');
        }
        printStatus(new_das, false, input);
        transferFunction *func = new_das->GetTransferFunc(new_das, input);
        if (func != NULL)
        {
            new_das->CommitTransferFunction(func);
        }
        else
        {
            break;
        }

        printStatus(new_das, true, input);
    };
    // Check for final state
    bool ok = isAccepted(new_das);
    // We are at final state
    if (ok)
    {
        cout << "Your string is accepted!";
    }
    else
    {
        cout << "Your string is not accepted!";
    }
}
