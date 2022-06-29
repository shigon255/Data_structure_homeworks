#include "insertion.h"

class quick{
public:
    quick(vector<int> v_): v(v_){}

    vector<int> sort(){
        quick_sort_partation(0, v.size()-1);
        return v;
    }

    vector<int> sort_print(){
        cout<<"Before: "<<endl;
        print_v();
        quick_sort_partation_print(0, v.size()-1);
        return v;
    }

    void quick_sort_partation_print(int front, int rear){
        // TODO :
        if(rear-front < 1) return;
        cout<<front<<"-"<<rear<<" :"<<endl;
        int left = front,right = rear;
        while(left < right){
            if( v[left+1] <= v[left] ){
                swap(v[left+1],v[left]);
                left++;
            }
            else{
                swap(v[left+1],v[right]);
                right--;
            }
        }
        print_v();
        quick_sort_partation_print(front,left-1);
        quick_sort_partation_print(left+1,rear);
        return;
    }

    void quick_sort_partation(int front, int rear){
        // TODO :
        if(rear-front < 1) return;
        int left = front,right = rear;
        while(left < right){
            if( v[left+1] <= v[left] ){
                swap(v[left+1],v[left]);
                left++;
            }
            else{
                swap(v[left+1],v[right]);
                right--;
            }
        }
        quick_sort_partation(front,left-1);
        quick_sort_partation(left+1,rear); 
        return;
    }

    void print_v(){
        for(auto vi : v){
            cout << vi << ",";
        }
        cout << endl;
    }

public:
    vector<int> v;
};