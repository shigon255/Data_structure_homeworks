#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class insertion{
public:
    insertion(vector<int> v_) : v(v_){};

    vector<int> sort(){
        for(int i=1;i<v.size();i++){
            insert(i);
        }
        return v;
    }

    vector<int> sort_print(){
        for(int i=1;i<v.size();i++){
            cout << setw(2) <<i << " : ";
            insert(i);
            print_v();
        }
        return v;
    }

    void insert(int idx){
        int temp = v[idx];
        int j = idx-1;
        for(;j>=0 && temp<v[j]; j--){
            v[j+1] = v[j];
        }
        v[j+1] = temp;
    };

    void print_v(){
        for(auto vi : v){
            cout << vi << ",";
        }
        cout << endl;
    }

public: 
    vector<int> v;
};