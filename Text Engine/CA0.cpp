#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct wordCount{
    string word;
    int count;
};

bool isPunctuation(char c);
bool isNumber(string& w);
void sanitize(vector<string>& words); //also make words lower case
bool isStopWord(string word , vector<string> stopWord);
vector<wordCount> updateWordCount(vector<wordCount> frequencies , string word);
vector<wordCount> sortWordCounts(vector<wordCount> frequencies);

int main(){
    vector<string> words;
    string word;
    vector<wordCount> frequencies;

    while(cin>>word && word != "###END###"){
        words.push_back(word);
    }

    int size = words.size(); //before any sanitization

    sanitize(words);

    //it's not possible to use normal loop here(because they're range-based)
    for(auto w=words.begin(); w!=words.end();){
        if(isNumber(*w))
            w = words.erase(w);
        else
            w++;
    }


    string forbiddenLine;
    getline(cin, forbiddenLine); //get cin(blank line)
    getline(cin, forbiddenLine);

    vector<string> stopWord;
    stringstream ss(forbiddenLine);

    while (ss >> word) {
        stopWord.push_back(word);
    }


    vector<string> filteredWords;
    
    for(string& word:words){
        if(!isStopWord(word , stopWord))
            filteredWords.push_back(word);
    }

    for(string& w:filteredWords){
        frequencies = updateWordCount(frequencies , w);
    }

    cout<<"Total words original: "<<size<<endl;
    cout<<"Total words after filter: "<<filteredWords.size()<<endl;

    frequencies = sortWordCounts(frequencies);
    
    cout<<"Word Frequencies:"<<endl;
    for(int i=0 ; i<frequencies.size() ; i++){
        cout<<frequencies[i].word<<": "<<frequencies[i].count<<endl;
    }
}


bool isPunctuation(char c)
{
    return c=='"' || c==':' || c==';' || c=='?' || c=='!' || c=='.' || c==',' || c=='\'';
}


bool isNumber(string& word)
{
    if(word.empty())
        return true; //if a complete string should be deleted here it'll fix(such as ...)
    
    int dotCount=0;
    for(char ch:word){
        if(isdigit(ch))
            continue;

        else if(ch=='.'){
            dotCount++;
            if(dotCount>1)
                return false;
        }

        else
            return false;
    }
    return true;
}

void sanitize(vector<string>& words)
{
    for(string& w:words){
        while (!w.empty() && isPunctuation(w.front()))
            w.erase(w.begin());

        while (!w.empty() && isPunctuation(w.back()))
            w.pop_back();

        //make them lower case
        for(char& ch:w){
            if(ch>='A' && ch<='Z')
                ch = ch+32;
            }
    }
}

bool isStopWord(string word, vector<string> stopWord)
{
    if(stopWord.empty())
        return false;
    for(string& w:stopWord){
        if(word==w)
            return true;
    }
    return false;
}

vector<wordCount> updateWordCount(vector<wordCount> frequencies, string word)
{
    bool found = false;
    for(auto& item:frequencies){
        if(item.word == word){
            item.count++;
            found = true;
            break;
        }
    }
    
    if(!found){
        wordCount temp;
        temp.count = 1;
        temp.word = word;
        frequencies.push_back(temp);
    }

    return frequencies;
}

vector<wordCount> sortWordCounts(vector<wordCount> frequencies)
{   
    int size = frequencies.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {

            bool shouldSwap = false;

            if (frequencies[j].count < frequencies[j+1].count) {
              shouldSwap = true;
            }
            else if (frequencies[j].count == frequencies[j+1].count &&
                frequencies[j].word > frequencies[j+1].word) {
                shouldSwap = true;
            }

            if (shouldSwap) {
              swap(frequencies[j], frequencies[j+1]);
            }
        }
    }
    return frequencies;
}