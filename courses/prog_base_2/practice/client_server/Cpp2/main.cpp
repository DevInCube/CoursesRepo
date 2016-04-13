#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cctype>

using namespace std;

struct student {
    char name[100];
    char surname[100];
    int age;
    double score;
};


int main() {
    student st[3];
    st[0].age = 4;
    int st1age = st[0].age;


    return 0;
}
