#include <string>
#include <vector>
#include <tuple>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

// Η ζητούμενη γραμματική είναι μια τετράδα {N,T,P,S}
// όπου:
// T = {(,),a,b,-,+,*}
// N = {S, X, Y, Z}
// P = [
// S->(Y),
// X->YX
// Y->a|b|S
// Z->*X|-X|+X|e
// ]
// S = (Y)

// Σύνολα FIRST:
// FIRST(S) = {()}
// FIRST(X) = {a,b,(}
// FIRST(Y) = {a,b,(}
// FIRST(Z) = {-,+,*,e}

// Σύνολα FOLLOW:
// FOLLOW(S) = {$,*,-,+,)}
// FOLLOW(X) = {)}
// FOLLOW(Y) = {*,-,+,)}
// FOLLOW(Z) = {)}

// Πίνακας LOOKAHEAD
// Παραγωγή   |   Σύνολα
// S->(X)     |{(}
// X->YZ      |{a,b,(}
// Y->a       |{a}
// Y->b       |{b}
// Y->S       |{(}
// Z->*X      |{*,a,b,(}
// Z->-X      |{-,a,b,(}
// Z->+X      |{+,a,b,(}
// Z->e       |{)}

// Άρα είναι γραμματική LL(1)
// Για τον συντακτικό αναλυτή θα χρειαστούμε τον συντακτικό πίνακα Μ

// Πίνακας Μ
// N\T  |  (   |  )  |  *  |  +  |  -  |  a  |  b  |  $|
// S    |S->(X)|     |     |     |     |     |     |   |
// X    |X->YZ |     |     |     |     |X->YZ|x->YZ|   |
// Y    |Y->S  |     |     |     |     |Y->a |Y->b |   |
// Z    |      |Z->e |Z->*X|Z->+X|Z->-X|     |     |   |


class Rule
{
    public:
        string leftSymbol;
        string rightSymbol;
};

class STableItem
{
    public:
        string terminalItem;
        string nonTerminalItem;
        Rule* rule;
};

Rule* initializeRule(string leftsymbol, string rightSymbol)
{
    Rule* new_rule = new Rule();
    new_rule->leftSymbol = leftsymbol;
    new_rule->rightSymbol = rightSymbol;
    return new_rule;
};

STableItem* initializeSTableItem(string terminalSymbol, string nonTerminalSymbol, Rule* rule)
{
    STableItem* new_item = new STableItem();
    new_item->terminalItem = terminalSymbol;
    new_item->nonTerminalItem = nonTerminalSymbol;
    new_item->rule = rule;
    return new_item;
};

int getIndex(vector<string> vec, string str)
{
    // cout << "looking for : "<< str << "|| \n";
    auto it = find(vec.begin(), vec.end(), str);
    int index = it - vec.begin();
    return index;
    
};

class SYA
{
    public:
        stack<string> syaStack;
        vector<string> terminalItems;
        vector<string> nonTerminalItems;
        string startingChar;
        vector<Rule *> rules;
        tuple<int,string> takeAction(string, int, STableItem*[4][8]);
        void alterStack(Rule*);
};

void SYA::alterStack(Rule* rule)
{
    // Βγαίνει το πάνω πάνω στοιχείο της στοίβας και το
    // αντικαταστούμε με το δεξί μέλος του κανόνα βάζοντας τα στοιχεία με ανάποδη σειρά
    syaStack.pop();
    // εάν το δεξί μέλος είναι το e τότε δεν μπαίνει στη στοίβα
    if (rule->rightSymbol != "e")
    {
        for(int j = rule->rightSymbol.size()-1; j>=0;j-- ){
            string item = string(1, rule->rightSymbol[j]);
            syaStack.push(item);
        }
    }

}

void printTable(SYA* sya, string input, STableItem* tableItem, bool nul )
{
    stack<string> output = sya->syaStack;
    string stackToPrint = "";
    while(!output.empty())
    {
        string str = output.top();
        stackToPrint += str;
        output.pop();
    };

    string stoixeio ="";
    string paragwgh ="";

    if (nul == false)
    {
        // stoixeio = "M("+tableItem->nonTerminalItem +","+tableItem->terminalItem+")";
        stoixeio.append("M(");
        stoixeio.append(tableItem->nonTerminalItem);
        stoixeio.append(",");
        stoixeio.append(tableItem->terminalItem);
        stoixeio.append(")");
        // paragwgh = tableItem->rule->leftSymbol+"->"+tableItem->rule->rightSymbol;
        paragwgh.append(tableItem->rule->leftSymbol);
        paragwgh.append("->");
        paragwgh.append(tableItem->rule->rightSymbol);
    };

    cout << "  Stoiva  |  Eisodos  |  Stoixeio Pinaka  |  Paragwgh  \n";
    cout << stackToPrint << "  |  "<< input << "  |  "<< stoixeio << "  |  " << paragwgh <<"\n";
    return;
}


tuple<int,string> SYA::takeAction(string currentString, int nextIndex,STableItem* table[4][8] )
{
    // First read next char
    string nextChar = string(1,currentString[0]);
    if(currentString.empty())
    {
        cout << "here";
        nextChar ="e";
    }
    string currentStackTopChar = syaStack.top();
    string newString = currentString;
    int resp_id;
    // 4 περιπτώσεις:
    // Αρχικά ελεγχουμε αν είναι τερματικό ή όχι
    bool isTerminal = (find(terminalItems.begin(), terminalItems.end(), currentStackTopChar) != terminalItems.end());
    if(isTerminal == true)
    {
        STableItem* nulltableItem = table[0][7];

        // Εάν είναι τερματικό τότε έχουμε 2 υποπεριπτώσεις
        // α) αν τρέχον σύμβολο είναι ίσο με το σύμβολο στη κορυφή της στοίβα και διαφορετικό του κενού συμβόλου τότε βγαίνει απο την στοίβα και αναγνωρίζετ.
        // β) αν τρέχον σύμβολο είναι ίσο με το σύμβολο στη κορυφή της στοίβα και είναι και τα δύο ίσα με τοκενό τότε ο αναλυτή έχει δεχθεί την συμβολοσειρά και εχει τελειώσει.
        if(currentStackTopChar == nextChar && nextChar != "e")
        {
            syaStack.pop();
            newString = newString.substr(1, newString.size() - 1);
            printTable(this,newString,nulltableItem, true);
            // newString.pop_back();
            // Remove first char
            // return "Accepted";
            resp_id= 0;
            return make_tuple(resp_id,newString);
        }else if (currentStackTopChar == nextChar && nextChar ==  "e")
        {

            printTable(this,newString,nulltableItem, true);
            resp_id= 1;
            // return "Finished"
            return make_tuple(resp_id,newString);
        }
    }else{
        // Εάν δεν είναι τερματικό τότε ανατρέχουμε στον συντακτικό πίνακα.
        int nonTerminalIndex = getIndex(nonTerminalItems, currentStackTopChar);
        int inputIndex = getIndex(terminalItems, nextChar);
        // cout << "terminal"<< terminalIndex+"\n";
        // cout << "nonterminal" << inputIndex;
        STableItem* tableItem = table[nonTerminalIndex][inputIndex];
        Rule* rule = tableItem->rule;
        if (rule == NULL)
        {
            // γ) εάν δεν υπάρχει κάτι στη θέσητου πίνακα τότε η συμβολοσειρά δεν αναγνωρίζεται και επιστρέφει μήνυμα λάθους
            printTable(this,newString,table[0][7], true);

            resp_id= 2;
            // return "Error"
            return make_tuple(resp_id,newString);

        }else{
            printTable(this,newString,tableItem, false);
            alterStack(rule);
            resp_id= 3;
            // return "Ok"
            return make_tuple(resp_id,newString);

        }
    }
    
};

int main()
{
    // Initialize our SYA   
    SYA *new_sya = new SYA();

    vector<string> nonTerminalItems = {"S", "X", "Y", "Z"};
    vector<string> terminalItems = {"(", ")", "*", "+","-","a","b","e"};
    string initialChar = "S";
    stack<string> syaStack;
    syaStack.push("e");
    syaStack.push(initialChar);

    new_sya->syaStack = syaStack;
    new_sya->terminalItems = terminalItems;
    new_sya->nonTerminalItems = nonTerminalItems;

    // Initialize our rules
    vector<Rule*> rules;
    rules.push_back(initializeRule("S",  "(X)"));
    rules.push_back(initializeRule("X",  "YZ"));
    rules.push_back(initializeRule("Y",  "a"));
    rules.push_back(initializeRule("Y",  "b"));
    rules.push_back(initializeRule("Y",  "S"));
    rules.push_back(initializeRule("Z",  "e"));
    rules.push_back(initializeRule("Z",  "+X"));
    rules.push_back(initializeRule("Z",  "-X"));
    rules.push_back(initializeRule("Z",  "*X"));
    new_sya->rules = rules;

    // Initialize our syntactic table;
    STableItem* synTable[4][8];
    synTable [0][0]= initializeSTableItem("(","S",rules[0]);
    synTable [1][0]= initializeSTableItem("(","X",rules[1]);
    synTable [1][5]= initializeSTableItem("a","X",rules[1]);
    synTable [1][6]= initializeSTableItem("b","X",rules[1]);
    synTable [2][0]= initializeSTableItem("(","Y",rules[4]);
    synTable [2][5]= initializeSTableItem("a","Y",rules[2]);
    synTable [2][6]= initializeSTableItem("b","Y",rules[3]);
    synTable [3][1]= initializeSTableItem("e","Z",rules[5]);
    synTable [3][2]= initializeSTableItem("*X","Z",rules[8]);
    synTable [3][3]= initializeSTableItem("+X","Z",rules[6]);
    synTable [3][4]= initializeSTableItem("-X","Z",rules[7]);

    typedef enum {
        Accepted,
        Finished,
        Error,
        Ok
    } response;
    string currentString="((b-a)*(a+b))";
    cout << "Give your string: \n";
    cin >> currentString;
    bool flag =true;
    string output="";
    string newString;
    int response_id;
    while(flag)
    {
       tie(response_id, newString) =  new_sya->takeAction(currentString, 0, synTable);
       currentString = newString;
       switch(response_id)
       {
            case Accepted:
                break;
            case Finished:
                flag=false;
                output="String accepted!";
                break;
            case Error:
                flag=false;
                output="String rejected!";
                break;
            case Ok:
                break;
       }
    }
    cout << "output: " << output;
}