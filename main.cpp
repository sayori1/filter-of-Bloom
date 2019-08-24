#include <iostream>
#include <ctime>
#include <vector>
#include <string.h>
#include <math.h>
#include <tgmath.h>
#include <fstream>
using namespace std;

class bits_field{
public:
    vector<int> value;
    bits_field(){
        value = vector<int>();
        value.push_back(0);
    }
    void set(int index){
        if((index / 16) + 1 > value.size()){
                for(int i = value.size(); i < index / 16; i++){
                    value.push_back(0);
                }
        }
        value[index / 16] = (1 << (index%16)) | value[index/16];
    }
    bool get(int index){
        return (value[index/16] >> (index % 16)) & 1;
    }
    void print(){
        for(int i = value.size() * 16; i>= 0; i--){
            cout <<get(i);
        }
    }
};
int gen(int key, const char* str){
    int result = 1;
    for(int i = 0; i < strlen(str); i++){
            result = (key * result + (int)str[i]) & 0xFFFFFFFF;
    }
    return result;
}
class bloom{
public:
    bits_field field;
    vector<int> keys;
    int size;
    bloom(int size, int n){
        field = bits_field();
        keys = vector<int>();
        this->size = size;
        for(int i = 0; i<n; i++){
            keys.push_back((rand() % 128) + 32);
        }
    }
    void add(const char* str){
        for(int i = 0; i < keys.size(); i++){
            int val = gen(keys[i], str) % size;
            val = val < 0? -val: val;
            field.set(val);
        }
    }
    bool has(const char* str){
        for(int i = 0; i < keys.size();i++){
            int val = gen(keys[i], str) % size;
            val = val < 0? -val: val;
            if(!field.get(val)){
                return false;
            }
        }
        return true;
    }
};
void generat(const char* path, int n){
    ofstream out = ofstream();
    out.open(path, ios_base::openmode::_S_in);
    for(int i = 0; i < n; i++){
        int len = (rand()%4) + 5;
        char* str = new char[len+1];
        for(int j = 0; j < len; j++){
                int n = (rand() % 23) + 98;
                str[j] = (char)n;
        }
        out << str << endl;
        if(i % 10000 == 0){
           cout << (((float)i) / ((float)n)) * 100 << endl;
           system("cls");
        }
    }
    out.close();
}
bloom* gen_optimal_bloom(int n, float interupt){
    int size = -(n * log(interupt)) / (log(2) * log(2));
    int count = (((float)size) / ((float)n)) * log(2);
    if(count == 0)
        count = 1;
    cout << "count of data - " << size << endl;
    cout << "count of hash - " << count << endl;

        int a;
    cin >> a;
    bloom* bl = new bloom(size, count);
    return bl;
}

void load(const char* path, bloom* bl){
    ifstream in = ifstream();
    in.open(path, ios::openmode::_S_out);
    string s;
    while(getline(in, s)){
            bl -> add(s.c_str());
    }
    in.close();
}

int main()
{
    srand(time(0));
    bloom* bl = gen_optimal_bloom(2000000, 0.01f); //For instance, we will have setted 2 000 000 values
    generat("C:\\o.txt", 2000000); //Generate a lot of random strings and load to a text file
    load("C:\\o.txt", bl);
    while(true){
        string str;
        getline(cin, str);
        cout << bl->has(str.c_str()) << endl;
    }
    return 0;
}



