#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

/*
* ========== Naming Convention Guideline ==========
* Class names: PascalCase
* Function names : camelCase
* Variable names : lower_snake_case
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

using namespace std;

bool goForward(const string& word, const map<string, bool>& dictionary);

vector<string> findValidCombinations(const vector<char>& letters,
const map<string, bool>& dictionary);

void generateCombination(
    const vector<char>& letters,
    const map<string, bool> &dictionary,
    vector<bool>& used,
    string& current,
    vector<string>& result
);

void processQuery(map<string, bool>& dictionary);
void loadDictionary(map<string, bool>& dictionary, int n);
void runGame();

int main()
{
    runGame();
    return 0;
}

bool goForward(const string &word, const map<string, bool> &dictionary)
{
    for(const auto &p : dictionary)
    {
        const string &dict_word = p.first;

        if(dict_word.size() < word.size())
            continue;

        bool ok = true;
        for(int i = 0; i < word.size(); i++)
        {
            if(dict_word[i] != word[i])
            {
                ok = false;
                break;
            }
        }

        if(ok)
            return true;
    }

    return false;
}


vector<string> findValidCombinations(const vector<char> &letters, const map<string, bool> &dictionary)
{
    vector<string> result;
    
    vector<bool> used(letters.size() , false);
    string current = "";

    generateCombination(letters, dictionary, used, current, result);

    sort(result.begin() , result.end());
    result.erase(unique(result.begin(), result.end()), result.end());

    return result;
}


void generateCombination(
    const vector<char> &letters,
    const map<string, bool> &dictionary, 
    vector<bool> &used, 
    string &current, 
    vector<string>& result)
{
    if(!current.empty() && goForward(current , dictionary))
        result.push_back(current);

    for(int i = 0 ; i < letters.size() ; i++)
    {
        if(!used[i])
        {
            used[i] = true;
            current.push_back(letters[i]);
            generateCombination(letters, dictionary, used, current, result);

            //Backtracking
            current.pop_back();
            used[i] = false;
        }
    }
}


void processQuery(map<string, bool> &dictionary)
{
    int q;
    cin >> q;
    cin.ignore();
    vector<vector<string>> built_words;
     built_words.resize(q);
    
    for(int i = 0 ; i < q ; i++)
    {
        string line;
        getline(cin , line);

        vector<char> letters;

        for(char ch : line)
        {
            if(ch != ' ')
                letters.push_back(ch);
        }
        sort(letters.begin() , letters.end());

        built_words[i] = findValidCombinations(letters , dictionary); 
    }

    for(int i = 0 ; i < built_words.size() ; i++)
    {
        cout << built_words[i].size() << endl;
        
        for(int j = 0 ; j < built_words[i].size() ; j++)
            cout << built_words[i][j] << endl;
    }
}


void loadDictionary(map<string, bool> &dictionary, int n)
{
    for(int i = 0 ; i < n ; i++)
    {
        string word;
        cin >> word;
        dictionary[word] = false;
    }
}

void runGame()
{
    int n;
    cin >> n;

    map <string , bool> dictionary;
    loadDictionary(dictionary , n);
    processQuery(dictionary);
}