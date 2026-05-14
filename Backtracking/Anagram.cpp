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

bool goForward(const string& word, const vector<char> &letters, vector<bool>& used, int index);
vector<string> findValidCombinations(const vector<char>& letters, const string& word);
void processQuery(map<string, bool>& dictionary);
void loadDictionary(map<string, bool>& dictionary, int n);
void runGame();

int main()
{
    runGame();
    return 0;
}

bool goForward(const string &word, const vector<char> &letters, vector<bool>& used, int index)
{
    if(index == word.size())
        return true;
    
    for(int i = 0 ; i < letters.size() ; i++)
    {
        if(!used[i] && letters[i] == word[index])
        {
            used[i] = true;
            if(goForward(word, letters, used, index+1))
                return true;
            used[i] = false;            
        }
    }
    return false;
}

vector<string> findValidCombinations(const vector<char> &letters, const string& word)
{
    vector<string> result;
    
    vector<bool> used(letters.size() , false);

    if(goForward(word, letters, used, 0))
    {
        result.push_back(word);
    }

    sort(result.begin() , result.end());
    result.erase(unique(result.begin(), result.end()), result.end());

    return result;
}



void processQuery(map<string, bool>& dictionary)
{
    int q;
    cin >> q;
    cin.ignore();

    vector<vector<string>> built_words(q);

    for(int i = 0; i < q; i++)
    {
        string line;
        getline(cin, line);

        vector<char> letters;

        for(char ch : line)
        {
            if(ch != ' ')
                letters.push_back(ch);
        }

        sort(letters.begin(), letters.end());

        for(auto& word : dictionary)
        {
            if(word.second)
                continue;

            vector<string> temp = findValidCombinations(letters, word.first);

            for(const auto& w : temp)
            {
                built_words[i].push_back(w);
                word.second = true;
            }
        }

        for(auto& word : dictionary)
        {
            word.second = false;
        }
    }

    for(int i = 0; i < built_words.size(); i++)
    {
        cout << built_words[i].size() << endl;

        for(int j = 0; j < built_words[i].size(); j++)
        {
            cout << built_words[i][j] << endl;
        }
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