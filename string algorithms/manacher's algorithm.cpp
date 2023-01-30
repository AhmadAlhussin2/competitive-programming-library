#include<bits/stdc++.h>

using namespace std;

/**
* function to calculate the length of the longest palindrome centered at every index in the string
*
* @param str an input string to calculate manacher's array on it
* @returns a vector which represents the longest palindrome centered at every index in the string
*/
vector<int> manacher(const string &str) {
    // since there are strings centered at even length, and others centered at odd length we need to insert characters in the string
    // an example could be ababa
    // the extended string will be @#a#b#a#b#a#$
    // the manacher's array will be `1 0 3 0 5 0 3 0 1` (ignore '@','$' signs)
    vector<int> manacherArray(2 * str.size() + 3);
    string extendedString = "@";
    for (int i = 0; i < str.size(); i++) {
        extendedString += "#";
        extendedString += str[i];
    }
    extendedString += "#$";
    int leftBorder = 0, rightBorder = 0;
    for (int i = 1; i < extendedString.size() - 1; i++) {
        if (rightBorder > i) {
            int mirrorIndex = leftBorder + (i - leftBorder);
            cout << leftBorder << " " << i << " " << rightBorder << " " << mirrorIndex << endl;
            manacherArray[i] = min(rightBorder - i, manacherArray[mirrorIndex]);
        } else {
            manacherArray[i] = 0;
        }
        while (extendedString[i + 1 + manacherArray[i]] == extendedString[i - 1 - manacherArray[i]]) {
            manacherArray[i]++;
        }
        if (i + manacherArray[i] > rightBorder) {
            leftBorder = i;
            rightBorder = i + manacherArray[i];
        }
    }
    manacherArray = vector<int>(manacherArray.begin() + 2,
                                manacherArray.end() - 2); // get rid of the trailing charachters
    return manacherArray;
}

int main() {
    string str;
    cin >> str;
    vector<int> answer = manacher(str);
    for (int i = 0; i < answer.size(); i++) {
        cout << answer[i] << " ";
    }
    return 0;
}

