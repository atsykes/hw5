#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <deque>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void addStarsToFloating(std::string&, std::string&); // Adds stars to the floating string to represent the missing letters
void permuteLetters(std::string&, std::deque<char>&, std::vector<std::string>&, size_t); // Permutes the characters in the floating string to the missing letters
void makeDeque(const std::string&, std::deque<char>&, size_t); // Adds the characters of the floating string into a deque
void wordleReplaceStars(const std::vector<std::string>&, std::set<std::string>&, const std::set<std::string>&); // Replaces the stars with possible letters and then adds valid words into a set
void wordleReplaceStarsHelper(const std::string, std::set<std::string>&, const std::set<std::string>&, size_t); // Helper to wordleReplaceStars

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::string s = in;
    std::string f = floating;
    std::set<std::string> dest;
    std::deque<char> d;
    std::vector<std::string> vec;
    addStarsToFloating(s, f);
    makeDeque(f, d, 0);
    permuteLetters(s, d, vec, 0);
    wordleReplaceStars(vec, dest, dict);
    
    return dest;
}

// Define any helper functions here
void addStarsToFloating(string& s, string& floating)
{
    size_t cnt = 0;
    // Counts how many empty spots there are
    for (size_t i=0; i < s.size(); ++i)
    {
        if (s[i] == '-') cnt++;
    }
    // Finds how many letters are missing and adds stars
    cnt -= floating.size();
    for (size_t j=0; j < cnt; ++j)
    {
        floating.push_back('*');
    }
}

void makeDeque(const string& s, deque<char>& d, size_t depth)
{
    if (depth == s.size()) return;
    d.push_back(s[depth]);
    makeDeque(s, d, depth+1);
}

void permuteLetters(string& s, deque<char>& floating, vector<string>& vec, size_t depth)
{
    if (depth == s.size()) 
    {
        vec.push_back(s);
        return;
    }

    // Looks for empty spots and permutes all of the floating characters
    if (s[depth] == '-')
    {
        for (size_t k=0; k < floating.size(); ++k)
        {
            char assign = floating[0];
            s[depth] = assign;
            floating.pop_front();
            permuteLetters(s, floating, vec, depth+1);
            floating.push_back(assign);
            s[depth] = '-';
        }
    }
    else permuteLetters(s, floating, vec, depth+1);
}


void wordleReplaceStars(const vector<string>& v, set<string>& dest, const set<string>& dict)
{
    for (size_t i=0; i < v.size(); ++i)
    {
        wordleReplaceStarsHelper(v[i], dest, dict, 0);
    }
}

void wordleReplaceStarsHelper(string s, set<string>& dest, const set<string>& dict, size_t depth)
{
    if (s.find('*') == string::npos)
    {
        if (dict.find(s) == dict.end()) return;

        dest.insert(s);
        return;
    }

    // Replaces the stars with possible characters
    if (s[depth] == '*')
    {
        for (char h='a'; h < '{'; ++h)
        {
            s[depth] = h;
            wordleReplaceStarsHelper(s, dest, dict, depth+1);
            s[depth] = '-';
        }
    }
    else wordleReplaceStarsHelper(s, dest, dict, depth+1);
}
