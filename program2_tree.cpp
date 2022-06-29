#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

class node{
    public:
        node(int _weight,char _c):weight(_weight),c(_c){
            left = nullptr;
            right = nullptr;
        };
        node* left;
        node* right;
        int weight;
        char c;
};


int char_to_index(char c){
    if('A'<=c&&c<='Z') return c-'A';
    else return c-'a'+26;
}

char index_to_char(int i){
    if(0<=i&&i<=25) return (char)('A'+i);
    else return (char)('a'+(i-26));
}

struct cmp {
    bool operator()(node* a, node* b) {
        return a->weight > b->weight;
    }
};



bool compare_node(node* a,node* b){
    if(a->weight<b->weight) return false;
    if(a->weight>b->weight) return true;
    if(char_to_index(a->c)<char_to_index(b->c)) return false;
    return true;
}


int main(int argc, const char* argv[]){




    std::vector<std::string> all_args;
    all_args.assign(argv, argv + argc); // convert command line argument to string

    std::cout << "DSOO-Program2-Demo: ";
    for(auto a : all_args){
        std::cout << a << " ";
    }
    std::cout << "\n";





    if(argc != 2){


        //std::ofstream out("output.txt");
        //std::streambuf *coutbuf = std::cout.rdbuf();
        //std::cout.rdbuf(out.rdbuf());


        int WEPL = 0;
        std::vector<int> frequency = {11,5,2,3,12,3,5,6,8,1,2,4,8,7,2,2,1,9,6,2,4,1,10,10,1,1};
        std::vector<std::string> encoded(26);


        std::priority_queue<node*,std::vector<node*>,cmp> pq;
        for(int i=0;i<26;i++){
            node* create_node = new node(frequency[i],'A'+i);
            pq.push(create_node);
        }
        while(pq.size()>1){
            node* node1 = pq.top();
            pq.pop();
            node* node2 = pq.top();
            pq.pop();
            node* new_node = new node(node1->weight+node2->weight,'@');
            new_node->left = node1;
            new_node->right = node2;
            pq.push(new_node);
        }
        node* root = pq.top();
        pq.pop();


        std::queue<std::pair<node*,std::string>> q;
        q.push({root,""});
        while(!q.empty()){
            node* tmp_node = q.front().first;
            std::string now_s = q.front().second;
            q.pop();
            if(!tmp_node->left&&!tmp_node->right){
                WEPL+= now_s.size()*tmp_node->weight;
                encoded[tmp_node->c-'A'] = now_s;
            }
            else{
                if(tmp_node->left){
                    q.push({tmp_node->left,now_s+"0"});
                }
                if(tmp_node->right){
                    q.push({tmp_node->right,now_s+"1"});
                }
            }
        }

        std::cout<<std::endl;
        int count = 0;
        for(int i=0;i<26;i++){
            if(count == 10){
                std::cout<<std::endl;
                count = 0;
            }
            std::cout<<(char)('A'+i)<<" =";
            if(frequency[i]<10){
                std::cout<<"  ";
            }
            else if(frequency[i]<100){
                std::cout<<" ";
            }
            std::cout<<frequency[i];
            std::cout<<" | ";
            count++;
        }
        std::cout<<std::endl<<std::endl;

        for(int i=0;i<26;i++){
            std::cout<<(char)('A'+i)<<" : "<<encoded[i]<<std::endl;
        }
        std::cout<<std::endl;
        std::cout<<"WEPL : "<<WEPL<<std::endl;


        //std::cout.rdbuf(coutbuf);

    }else{
        std::string filename = argv[1];
        //std::cout << "read file from : " << "test/" << filename << std::endl;


        std::ifstream in(filename);
        std::streambuf *cinbuf = std::cin.rdbuf();
        std::cin.rdbuf(in.rdbuf());

        //std::ofstream out("output.txt");
        //std::streambuf *coutbuf = std::cout.rdbuf();
        //std::cout.rdbuf(out.rdbuf());

        int WEPL = 0;

        std::vector<int> frequency(52,0);
        std::vector<std::string> encoded(52);

        node* root = nullptr;
        node* now_node = root; //for decoding


        std::string decoded_string = "";
        std::string frequency_string;
        std::string encoded_string;
        std::cin>>frequency_string;
        std::cin>>encoded_string;
        for(auto c:frequency_string){
            frequency[char_to_index(c)]++;
        }


        std::priority_queue<node*,std::vector<node*>,cmp> pq;
        for(int i=0;i<52;i++){
            if(frequency[i]){
                node* create_node = new node(frequency[i],index_to_char(i));
                pq.push(create_node);
            }
        }
        while(pq.size()>1){
            node* node1 = pq.top();
            pq.pop();
            node* node2 = pq.top();
            pq.pop();
            node* new_node = new node(node1->weight+node2->weight,'@');
            new_node->left = node1;
            new_node->right = node2;
            pq.push(new_node);
        }
        root = pq.top();
        pq.pop();


        std::queue<std::pair<node*,std::string>> q;
        q.push({root,""});
        while(!q.empty()){
            node* tmp_node = q.front().first;
            std::string now_s = q.front().second;
            q.pop();
            if(!tmp_node->left&&!tmp_node->right){
                WEPL+= now_s.size()*tmp_node->weight;
                encoded[char_to_index(tmp_node->c)] = now_s;
            }
            else{
                if(tmp_node->left){
                    q.push({tmp_node->left,now_s+"0"});
                }
                if(tmp_node->right){
                    q.push({tmp_node->right,now_s+"1"});
                }
            }
        }


        now_node = root;
        for(auto c:encoded_string){
            if(c == '0'){
                now_node = now_node->left;
            }
            else{
                now_node = now_node->right;
            }
            if(!now_node->left&&!now_node->right){
                decoded_string += now_node->c;
                now_node = root;
            }
        }


       // std::cout << "DSOO-Program2-Demo: ";
       // for(auto a : all_args){
       //     std::cout << a << " ";
       // }
       // std::cout << "\n";
        std::cout<<"characters : "<<frequency_string<<std::endl<<std::endl;
        int count = 0;
        for(int i=0;i<52;i++){
            if(frequency[i]){
                if(count == 10){
                    std::cout<<std::endl;
                    count = 0;
                }
                count++;
                std::cout<<index_to_char(i)<<" =";
                if(frequency[i] < 10){
                    std::cout<<"  ";
                }
                else if(frequency[i] < 100){
                    std::cout<<" ";
                }
                std::cout<<frequency[i];
                std::cout<<" | ";
            }
        }
        std::cout<<std::endl<<std::endl;
        for(int i=0;i<52;i++){
            if(frequency[i]){
                std::cout<<index_to_char(i)<<" : "<<encoded[i]<<std::endl;
            }
        }
        std::cout<<std::endl;
        std::cout<<"Huffman code : "<<encoded_string<<std::endl;
        std::cout<<"decode : "<<decoded_string;
        if(now_node!=root) std::cout<<" -wrong decode-";
        std::cout<<std::endl;
        std::cout<<"WEPL : "<<WEPL<<std::endl;



        std::cin.rdbuf(cinbuf);
       // std::cout.rdbuf(coutbuf);

    }






    return 0;
}
