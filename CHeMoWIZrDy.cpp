/*
 * CHeMoWIZrDy
 * --------------
 * Name: jinglebot
 * Date: July 3, 2014
 * This program allows the user to enter a name and prints it using the letters
 * of the symbols of elements in the Periodic Table.
 *
 * Pseudocode :
 * - open a file of a table of elements
 * - create a set of element symbols
 * - prompt user for a name
 * - initialization:
 * - create Q1 (main queue) where all Q2's (minor queue) are stored
 * - store input name in a Q2
 * - save Q2 in Q1
 * - recursion proper:
 * - place safety for erasing prompted name from Q1 and Q2
 * - place counter for limiting recursions temporarily
 * - test first 3 letters of name if found in elements set
 * - store in Q3 (test queue per element)
 * - place safety when any of the Q's = 0
 * - store each 1- to 3-letter combo stored in Q3 in Q4(test queue per
 *      elements combo), each with a copy of Q2
 * - test if any in Q4 == name
 * - store Q4's in Q1 only for those with letters in the correct order
 * - recursion loop to add counter
 * - compare next 3 letters of the name to elements set
 * - check if each of the 3 elements in the previous set spells correctly
 *      with each of the next 3 elements in the next set by storing the
 *      complete spelling in Q3
 * - do until all the letters have been compared
 * - if name == correct element spelling combo, true
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <queue>
using namespace std;


 /* Prototypes */

struct qn
{
    queue < string > line;
};
void openTable(ifstream &f, set < string > &s);
bool isElementSpellable(string t, set < string >& s);
void testName(string n, int c, bool &p, set < string > &s, queue <qn> &mq);


/* Main Program */

int main()
{
    ifstream myfile;
    set < string > symbols;
    openTable(myfile, symbols);

    string text = " ";
    while ( text != "" )
    {
        cout << "Enter a name :  ";
        getline(cin, text);
        if ( text == "" ) break;

        bool spelled = false;
        spelled = isElementSpellable(text, symbols) ;
        if (spelled)    cout << " can be spelled" << endl << endl;
        else cout << "cannot spell " << text << endl << endl;
    }
    cout << "\nGoodbye\n";

    return 0;
}


void openTable(ifstream &myfile, set < string > &symbols)
{
    myfile.open("://table/ElementsTable.txt");

    if (myfile.is_open())
    {
        string line;
        while (getline(myfile, line))
        {
            symbols.insert(line);
            if (myfile.fail())
            {
                myfile.gcount();
                myfile.clear();
            }
        }
        myfile.close();
    }
    else
    {
        cout << "Can't open file" << endl;
        exit(1);
    }
}

bool isElementSpellable(string text, set <string>& symbols)
{
    // Initializations
    int start = 0;
    qn s_name;
    bool spelled = 0;	//false
    for (int i = 0; i < text.size(); ++i )
        text[i] = tolower( text[i] );   // all letters in lower case
    s_name.line.push(text);
    queue <qn> Q1;
    Q1.push(s_name);
    testName ( text, start, spelled, symbols, Q1 );
    return spelled;
}


void testName(string name, int start, bool &spelled, set < string > &symbols, queue <qn> &Q1)
{
    // Get a string from the Q1 front
    qn Q2 = Q1.front();
    Q1.pop();
    // Safety check if Q's == 0 at start
    if (start == 0) Q2.line.pop();

    qn Q3;
    set < string >::iterator pos = symbols.begin();
    // This will be done for every 3 sets of letters in the input name
    for (int i = 1; i <= 3; ++i)
    {
        string subname;
        subname = name.substr(start, i);
        for (int j = 0; j < subname.size(); ++j)
        {
            // Capitalize the first letter like that of elements symbols
            if (j == 0) subname[0] = toupper(subname[0]);
            else subname[j] = tolower(subname[j]);
        }
        // Check all possible elements that can be derived from the substring
        if ( ( pos = symbols.find ( subname ) ) != symbols.end() )
            Q3.line.push ( *pos );
    }

    qn Q4;
    int c = 0;
    // Safety check if Q's == 0 at start
    if ( start == 0) c = 1;
    else c = Q2.line.size();
    // Check if no elements were derived from the 3-letter substring
    if ( Q2.line.size() == 0 && Q3.line.size() == 0 )
    {
        spelled = false;
        return;
    }

    for (int k = 0; k < c;  ++k)
    {
        if ( Q3.line.size() == 0 ) Q4 = Q2;
        else
        {
            for (int l = 0; l < Q3.line.size(); ++l)
            {
                // Combine substring from current 3-letter set (Q3) with previous 3-letter set (Q2)
                string sxword;
                if (start == 0) sxword = Q3.line.front();
                else sxword = Q2.line.front() + Q3.line.front();

                // Check back to lower case
                string cxword = sxword;
                for (int j = 0; j < cxword.size(); ++j)
                    cxword[j] = tolower(cxword[j]);

                // Test each substring combo letter if matches input name
                if  ( name.substr( 0, cxword.size() ) == cxword )
                    Q4.line.push ( sxword );

                // Test if substring combo series matches name
                if ( cxword == name )
                {
                    cout << sxword << "\t";
                    spelled = true;
                    return;
                }

                // Clear for next substring in Q3
                Q3.line.push ( Q3.line.front() );
                Q3.line.pop();
            }
        }

        // Clear for next substring combos in Q2
        if (start != 0)
        {
            Q2.line.push ( Q2.line.front() );
            Q2.line.pop();
        }
    }

    // Safety in case next Q3 == 0
    if ( Q4.line.size() == 0 ) Q4 = Q2;
    Q1.push ( Q4 );

    // Test recursion end: if counter reaches the last letter of input name
    if ( start == name.size()  )
    {
        spelled = false;
        return;
    }

    testName(name, start + 1, spelled, symbols, Q1);
}



