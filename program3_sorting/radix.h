#include "quick.h"
#include <math.h>


class radix{
public:
    radix(vector<int> v_) : v(v_){
        // maybe ?
        bucket.resize(10);
    }

    vector<int> sort(){
        // TODO
        for(int i=0;i<3;i++){
            for(auto n:v){
                bucket[get_digit(n,i)].push_back(n);
            }
            int index = 0;
            for(int j=0;j<10;j++){
                for(auto n:bucket[j]){
                    v[index] = n;
                    index++;
                }
            }
            clear_bucket();
        }
        return v;
    }

    vector<int> sort_print(){
        // TODO
        for(int i=0;i<3;i++){
            for(auto n:v){
                bucket[get_digit(n,i)].push_back(n);
            }
            int index = 0;
            for(int j=0;j<10;j++){
                for(auto n:bucket[j]){
                    v[index] = n;
                    index++;
                }
            }
            print_pass(i);
            clear_bucket();
        }
        return v;
    }
    // 可任意新增需要的 member funciton

    int get_digit(int num,int d){
        for(int i=0;i<d;i++) num/=10;
        return num%10;
    }

    void print_pass(int i){
        if(i == 0) cout<<"First-pass:"<<endl;
        else if(i == 1) cout<<"Second-pass:"<<endl;
        else cout<<"Thrid-pass: "<<endl;
        for(int j=0;j<10;j++){
            cout<<j<<" |";
            for(auto n:bucket[j]){
                cout<<" --> "<<n;
            }
            cout<<endl;
        }
        cout<<"resulting chain : ";
        print_v();
        cout<<endl;
    }

    void clear_bucket(){
        for(int i=0;i<10;i++) bucket[i].clear();
    }

    void print_v(){
        for(auto vi : v){
            cout << vi << ",";
        }
        cout << endl;
    }

public:
    vector<int> v;
    vector<vector<int>> bucket;
    // maybe ?
};