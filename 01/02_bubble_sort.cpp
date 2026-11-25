#include <iostream>
using namespace std;
int main() {
    int score[10] = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10 };
    int temp;
    for (int i = 0; i < 10 - 1; i++) {
        for (int j = 0; j < 10 - i - 1; j++) {
            if (score[j] > score[j + 1]) {
                temp = score[j];
                score[j] = score[j + 1];
                score[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        cout << score[i] << endl;
    }
   
    return 0;
}