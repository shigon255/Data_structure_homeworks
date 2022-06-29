#include<iostream>
#include<stack>
#include<ctime>
#include<vector>
#include<cstdlib>
#include<iomanip>
#define landing_queue_capacity 1000
#define takeoff_queue_capacity 1000

using namespace std;

int land_index = 1,takeoff_index = 0,total_plane = 0,landing_waiting_time = 0,takeoff_waiting_time = 0,emergency = 0,crash = 0,fuel_saved = 0,crash_takeoff = 0,landing_plane = 0,takeoff_plane = 0;

class plane{
public:
    plane():id(-1){};
    plane(int id_):id(id_){}
    plane(int id_,int in_time_):id(id_),in_time(in_time_){}
    plane(int id_,int fuel_,int in_time_):id(id_),fuel(fuel_),in_time(in_time_){}
    int get_id(){return id;}
    int get_fuel(){return fuel;}
    int get_in_time(){return in_time;}
    void timePass(){fuel--;}
private:
    int id;
    int fuel;
    int in_time; //the time it enter the queue
};

plane void_plane(){
    return plane(-1,-1,-1);
}

int force_planes = 0;
plane force[landing_queue_capacity];


bool in_force(plane p){
    for(int i=0;i<force_planes;i++){
        if(force[i].get_id() == p.get_id()) return true;
    }
    return false;
}

void print_force(){
    for(int i=0;i<force_planes;i++){
        cout<<"("<<force[i].get_id()<<", "<<force[i].get_fuel()<<"), ";
    }
    cout<<endl;
}

void pop_force(plane p){
    for(int i=0;i<force_planes;i++){
        if(force[i].get_id() == p.get_id()){
            force_planes--;
            for(int j=i;j<force_planes;j++){
                force[j] = force[j+1];
            }
            force[force_planes] = void_plane();
        }
    }
}
//for debug
vector<plane> crash_plane;
vector<plane> crash_takeoff_plane;


class runwayF{
public:
    runwayF():now(void_plane()),takeoff_planes(0){
        for(int i=0;i<takeoff_queue_capacity;i++){
            takeoff[i] = void_plane();
        }
    };
    plane getnow(){return now;}

    //takeoff part

    //determine that if there is still place for new takeoff plane
    bool can_insert_takeoff(){return takeoff_planes < 6;}

    //check if there is no plane in takeoff queue
    bool no_plane(){
        return takeoff_planes == 0;
    }

    //push plane into takeoff queue
    void push_takeoff(plane new_plane){
        takeoff[takeoff_planes] = new_plane;
        takeoff_planes++;
    }

    //emergency part

    //help emergency planes
    void help(plane new_plane){
        now = new_plane;
    }

    //choose part

    //choose land or takeoff, return -1 => insert failed
    int choose(){
        if(now.get_id()!=-1) return 1;
        else{
            if(for_takeoff()) return 0;
            return -1;
        }
    }


    //put plane from takeoff queue to now
    bool for_takeoff(){
        if(takeoff_planes == 0) return false;
        now = takeoff[0];
        takeoff_planes--;
        for(int i=0;i<takeoff_planes;i++){
            takeoff[i] = takeoff[i+1];
        }
        takeoff[takeoff_planes] = void_plane();
        return true;
    }

    //update and clear

    void print_takeoff(){
        for(int i=0;i<takeoff_planes;i++){
            cout<<"("<<takeoff[i].get_id()<<"), ";
        }
        cout<<endl;
    }

    void reset_now(){now = void_plane();}

    int total_takeoff_planes(){return takeoff_planes;}

private:
    plane takeoff[takeoff_queue_capacity];
    int takeoff_planes;
    plane now;
};

runwayF runwayf;


class runwayN{
public:
    runwayN():now(void_plane()),takeoff_planes(0){
        for(int i=0;i<2;i++){
            max_waiting_time_unit[i] = -1;
            land_planes[i] = 0;
            for(int j=0;j<landing_queue_capacity;j++){
                land[i][j] = void_plane();
            }
        }
        for(int i=0;i<10;i++){
            land_order[i] = void_plane();
        }
        for(int i=0;i<takeoff_queue_capacity;i++){
            takeoff[i] = void_plane();
        }
    };
    plane getnow(){return now;};

    //landing part

    //check if the landing can insert into one of the landing patterns
    bool can_insert_in_land(int fuel){
        int index = choose_landing_pattern();
        for(int i=max_waiting_time_unit[index]+1;i<10&&i<fuel;i++){
            if(land_order[i].get_id() == -1) return true;
        }
        return false;
    }

    //choose the landing pattern which has lower max_waiting_time_unit
    int choose_landing_pattern(){
        return max_waiting_time_unit[0]<max_waiting_time_unit[1]?0:1;
    }

    //push into landing pattern
    void push_landing(plane new_plane){
        int land_pattern_index = choose_landing_pattern();
        land[land_pattern_index][land_planes[land_pattern_index]] = new_plane;
        land_planes[land_pattern_index]++;
        bool can_insert_far = false;
        for(int i=max_waiting_time_unit[land_pattern_index]+2;i<new_plane.get_fuel()&&i<10;i++){
            if(land_order[i].get_id() == -1){
                land_order[i] = new_plane;
                can_insert_far = true;
                break;
            }
        }
        if(!can_insert_far){
            for(int i=new_plane.get_fuel()-1;i>max_waiting_time_unit[land_pattern_index];i--)
            if(land_order[i].get_id() == -1){
                land_order[i] = new_plane;
                break;
            }
        }
        calculate_waitingtime();
    }

    //distribute excess landing plane into one of pattern
    void push_excess_force(plane new_plane){
        int index = land_planes[0]<land_planes[1]?0:1;
        land[index][land_planes[index]] = new_plane;
        land_planes[index]++;
        force[force_planes] = new_plane;
        force_planes++;
    }

    //takeoff part

    //push plane into takeoff queue
    void push_takeoff(plane new_plane){
        //add plane to takeoff queue
        takeoff[takeoff_planes] = new_plane;
        takeoff_planes++;
    }

    //check if the runway have no plane
    bool no_plane(){
        return total_land_planes()+total_takeoff_planes() == 0;
    }

    //emergency part

    //check if land order has plane that has no fuel
    bool check_emergency(){
        for(int i=0;i<2;i++){
            for(int j=0;j<land_planes[i];j++){
                if(land[i][j].get_id()!=-1&&land[i][j].get_fuel() == 0) return true;
            }
        }
        return false;
    }

    //print emergency plane in land order
    int print_emergency(){
        int sum = 0;
        for(int i=0;i<2;i++){
            for(int j=0;j<land_planes[i];j++){
                if(land[i][j].get_id()!=-1&&land[i][j].get_fuel() == 0){
                    sum++;
                    cout<<"("<<land[i][j].get_id()<<", "<<land[i][j].get_fuel()<<"), ";
                }
            }
        }
        return sum;
    }

    //check if planes in patterns has no fuel. If yes 1. insert into land order [0], 2. insert into emergency_plane
    void need_help(stack<plane>* s){
        for(int i=0;i<2;i++){
            for(int j=0;j<land_planes[i];j++){
                if(land[i][j].get_fuel() == 0){
                    (*s).push(land[i][j]);
                    pop_plane(land[i][j].get_id());
                }
            }
        }
    }

    //remove the specified plane in land patterns
    void pop_plane(int id){
        for(int i=0;i<2;i++){
            for(int j=0;j<land_planes[i];j++){
                if(land[i][j].get_id() == id){
                    land_planes[i]--;
                    for(int k=j;k<land_planes[i];k++){
                        land[i][k] = land[i][k+1];
                    }
                    land[i][land_planes[i]] = void_plane();
                    return;
                }
            }
        }
    }

    //take emergency plane to now plane
    bool help(plane new_plane){
        if(land_order[0].get_id()==-1){
            now = new_plane;
            land_planes[0]++;
            return true;
        }
        else return false;
    }

    //choose part

    //choose which plane to be put into now
    int choose(){
        if(now.get_id()==-1){
            if(land_order[0].get_id()!=-1){
                for_landing();
                return 1;
            }
            else{
                if(takeoff_planes){
                    for_takeoff();
                    return 2;
                }
                else{
                    if(for_landing_instead()) return 3;
                    else return -1;
                }
            }
        }
        else{
            land_planes[0]--;
            return 0;
        }
    }

    //push landing plane into now
    void for_landing(){
        //first choose the land pattern which the plane comes from
        int land_pattern_index = 1;
        if(land[0][0].get_id() == land_order[0].get_id()) land_pattern_index = 0;
        //runway is used for landing
        now = land_order[0];
        //land_pattern update
        land_planes[land_pattern_index]--;
        for(int i=0;i<land_planes[land_pattern_index];i++){
            land[land_pattern_index][i] = land[land_pattern_index][i+1];
        }
        land[land_pattern_index][land_planes[land_pattern_index]] = void_plane();
        calculate_waitingtime();
    }

    //push takeoff plane into now
    void for_takeoff(){
        //runway is used for takeoff
        now = takeoff[0];
        //update takeoff queue
        takeoff_planes--;
        for(int i=0;i<takeoff_planes;i++){
            takeoff[i] = takeoff[i+1];
        }
        takeoff[takeoff_planes] = void_plane();
    }

    //take the most front landing plane
    bool for_landing_instead(){
        if(total_land_planes() == 0) return false;
        int diff = 0;
        for(;land_order[diff].get_id()==-1;diff++);
        for(int i=0;i<10-diff;i++){
            land_order[i] = land_order[i+diff];
        }
        for(int i=10-diff;i<10;i++){
            land_order[i] = void_plane();
        }
        for_landing();
        return true;
    }

    //update and clear

    //land order move left
    void update_land_order(){
        for(int i=0;i<9;i++){
            land_order[i] = land_order[i+1];
        }
        land_order[9] = void_plane();
    }

    //land order and patterns fuel minus 1
    void update_fuel(){
        for(int i=0;i<2;i++){
            for(int j=0;j<land_planes[i];j++){
                land[i][j].timePass();
            }
        }
        for(int i=0;i<10;i++){
            land_order[i].timePass();
        }
    }

    //clear now
    void reset_now(){now = void_plane();}

    //print part


    void show_max_time_unit(){
        cout<<"Max Time 0: "<<max_waiting_time_unit[0]<<endl;
        cout<<"Max Time 1: "<<max_waiting_time_unit[1]<<endl;
    }

    void print_takeoff(){
        for(int i=0;i<takeoff_planes;i++){
            cout<<"("<<takeoff[i].get_id()<<"), ";
        }
        cout<<endl;
    }

    void print_pattern(int land_pattern_index){
        for(int i=0;i<land_planes[land_pattern_index];i++){
            cout<<"("<<land[land_pattern_index][i].get_id()<<", "<<land[land_pattern_index][i].get_fuel()<<"), ";
        }
        cout<<endl;
    }

    void print_land_order(){
        for(int i=0;i<10;i++){
            if(land_order[i].get_id()==-1) cout<<"(-1), ";
            else{
                cout<<"("<<land_order[i].get_id()<<", "<<land_order[i].get_fuel()<<"), ";
            }
        }
        cout<<endl;
    }

    //other

    //update the max waiting time unit
    void calculate_waitingtime(){
        for(int i=0;i<2;i++){
            if(land_planes[i] == 0){
                max_waiting_time_unit[i] = -1;
                continue;
            }
            int index = land_planes[i]-1;
            while(in_force(land[i][index])){
                index--;
            }
            int search_id = land[i][index].get_id();
            for(int j=0;j<10;j++){
                if(land_order[j].get_id() == search_id){
                    max_waiting_time_unit[i] = j;
                    break;
                }
            }
        }
    }

    int total_land_planes(){return land_planes[0]+land_planes[1];}

    int total_takeoff_planes(){return takeoff_planes;}

private:
    plane takeoff[takeoff_queue_capacity]; //takeoff queue
    plane land[2][landing_queue_capacity]; //landing patterns
    plane land_order[10]; //ordering
    int max_waiting_time_unit[2]; //landing pattern's max position in land order
    int land_planes[2]; //number of planes waiting for landing
    int takeoff_planes; //number of planes waiting for takeoff
    plane now;
};



runwayN runwayn[3];





//decide which the runway should landing plane enter
int landing_runway_index(int fuel){
    int ans = -1;
    int nowland = -1;
    int nowtotal = -1;
    for(int i=0;i<3;i++){
        if(!runwayn[i].can_insert_in_land(fuel)){
            continue;
        }
        else{
            if(ans == -1){
                ans = i;
                nowland = runwayn[i].total_land_planes();
                nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
            }
            else{
                if(nowland < runwayn[i].total_land_planes()){
                    continue;
                }
                else{
                    if(nowland > runwayn[i].total_land_planes()){
                        ans = i;
                        nowland = runwayn[i].total_land_planes();
                        nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
                    }
                    else if(nowtotal > runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes()){
                        ans = i;
                        nowland = runwayn[i].total_land_planes();
                        nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
                    }
                }
            }
        }
    }
    return ans;
}

//distribute plane to either of runwayns
void distribute_force_plane(plane new_plane){
    int dis_index = 0;
    int now_max_total = runwayn[0].total_land_planes()+runwayn[0].total_takeoff_planes();
    for(int i=1;i<3;i++){
        if(runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes() < now_max_total){
            dis_index = i;
            now_max_total = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
        }
    }
    runwayn[dis_index].push_excess_force(new_plane);
}


//determine which runway should takeoff plane enter
int takeoff_runway_index(){
    int ans = -1;
    if(runwayf.no_plane()) return ans;
    for(int i=0;i<3;i++){
        if(runwayn[i].no_plane()){
            ans = i;
            return ans;
        }
    }
    if(runwayf.can_insert_takeoff()){
        return ans;
    }
    int nowtake = 0,nowtotal = 0;
    for(int i=0;i<3;i++){
        if(nowtake == 0){
            ans = i;
            nowtake = runwayn[i].total_takeoff_planes();
            nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
        }
        else{
            if(nowtake < runwayn[i].total_takeoff_planes()){
                continue;
            }
            else{
                if(nowtake > runwayn[i].total_takeoff_planes()){
                    ans = i;
                    nowtake = runwayn[i].total_takeoff_planes();
                    nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
                }
                else if(nowtotal < runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes()){
                    ans = i;
                    nowtake = runwayn[i].total_takeoff_planes();
                    nowtotal = runwayn[i].total_land_planes()+runwayn[i].total_takeoff_planes();
                }
            }
        }
    }
    if(nowtake > runwayf.total_takeoff_planes()) ans = -1;
    return ans;
}


//clear now in runwayN ans runwayf
void runway_clear(){
    for(int i=0;i<3;i++){
        if(in_force(runwayn[i].getnow())) pop_force(runwayn[i].getnow());
        runwayn[i].reset_now();
    }
    if(in_force(runwayf.getnow())) pop_force(runwayf.getnow());
    runwayf.reset_now();
}

//print current status
void show_status(){
    //print current status
    cout<<endl;
    cout<<"Runway1("<<runwayf.getnow().get_id()<<")"<<endl;
    cout<<"L1:\nL2:\nT: ";
    runwayf.print_takeoff();
    cout<<endl;
    for(int i=0;i<3;i++){
        cout<<"Runway"<<i+2<<"("<<runwayn[i].getnow().get_id()<<")"<<endl;
        cout<<"L1: ";
        runwayn[i].print_pattern(0);
        cout<<"L2: ";
        runwayn[i].print_pattern(1);
        cout<<"T: ";
        runwayn[i].print_takeoff();
        cout<<endl;
    }
}




int main(void){
    srand(time(NULL));
    for(int i=0;i<landing_queue_capacity;i++){
        force[i] = void_plane();
    }
    int t;
    cout<<"Test times: ";
    cin>>t;
    int max_takeoff_plane;
    cout<<"Please enter the maximum number of takeoff planes per time unit: ";
    cin>>max_takeoff_plane;
    for(int _t=0;_t<t;_t++){
        cout<<"Simulation time: "<<_t+1<<endl;
        show_status();

        //landing
        int n;
        n = rand()%5;
        cout<<"Step 1:"<<endl;
        cout<<"landing plane: ";
        for(int i=0;i<n;i++){
            int tmp_index = land_index;
            land_index += 2;
            int tmp_fuel;
            tmp_fuel = rand()%10+1;
            cout<<"("<<tmp_index<<" ,"<<tmp_fuel<<"), ";
            int target_index = landing_runway_index(tmp_fuel);
            if(target_index == -1){
                distribute_force_plane(plane(tmp_index,tmp_fuel,_t));
            }
            else{
                runwayn[target_index].push_landing(plane(tmp_index,tmp_fuel,_t));
            }
        }
        show_status();

        //takeoff
        cout<<"Step 2:"<<endl;
        cout<<"takeoff planes: ";
        n = rand()%(max_takeoff_plane+1);
        for(int i=0;i<n;i++){
            int tmp_index = takeoff_index;
            takeoff_index += 2;
            int target_index = takeoff_runway_index();
            cout<<"("<<tmp_index<<"), ";
            if(target_index == -1){
                //push to runwayF's takeoff queue
                runwayf.push_takeoff(plane(tmp_index,_t));
            }
            else if(target_index!=-2){
                //push to runwayN's takeoff queue
                runwayn[target_index].push_takeoff(plane(tmp_index,_t));
            }
            else{
                crash_takeoff_plane.push_back(plane(tmp_index,_t));
            }
        }
        cout<<endl;
        show_status();

        //emergency
        cout<<"Step 3:"<<endl;
        cout<<"emergency plane: ";
        for(int i=0;i<3;i++){
            if(runwayn[i].check_emergency()){
                emergency+=runwayn[i].print_emergency();
            }
        }
        stack<plane> emergency_plane;
        for(int i=0;i<3;i++){
            runwayn[i].need_help(&emergency_plane);
        }
        if(emergency_plane.size()){
            runwayf.help(emergency_plane.top());
            emergency_plane.pop();
        }
        if(emergency_plane.size()){
            for(int i=0;i<3;i++){
                if(runwayn[i].help(emergency_plane.top())){
                    emergency_plane.pop();
                }
                if(!emergency_plane.size()) break;
            }
        }
        if(emergency_plane.size()){
            //cout<<"Crash"<<endl;
            while(emergency_plane.size()){
                crash_plane.push_back(emergency_plane.top());
                emergency_plane.pop();
                crash++;
            }
        }
        cout<<endl;
        show_status();

        //choose
        cout<<"Step 4:"<<endl;
        int tmp = runwayf.choose();
        if(tmp == 0){
            takeoff_waiting_time += _t-runwayf.getnow().get_in_time();
        }
        if(tmp != -1){
            if(runwayf.getnow().get_id()%2){
                landing_plane++;
            }
            else{
                takeoff_plane++;
            }
            total_plane++;
        }
        //determine which plane should go through the runway if no force plane is landed
        for(int i=0;i<3;i++){
            int result = runwayn[i].choose();
            if(result == 1||result == 3){
                landing_waiting_time += _t-runwayn[i].getnow().get_in_time();
                fuel_saved += runwayn[i].getnow().get_fuel();
            }
            else if(result == 2){
                takeoff_waiting_time += _t-runwayn[i].getnow().get_in_time();
            }
            if(result != -1){
                if(runwayn[i].getnow().get_id()%2){
                    landing_plane++;
                }
                else{
                    takeoff_plane++;
                }
                total_plane++;
            }
        }
        show_status();

        //clear and update
        for(int i=0;i<3;i++){
            runwayn[i].update_land_order();
            runwayn[i].calculate_waitingtime();
            runwayn[i].update_fuel();
        }
        runway_clear();
    }
    cout<<fixed<<setprecision(3);
    cout<<"How many time you want to simulate: "<<t<<endl;
    cout<<"average landing waiting time: "<<((float)landing_waiting_time/(float)landing_plane)<<"(s)"<<endl;
    cout<<"average takeoff waiting time: "<<((float)takeoff_waiting_time/(float)takeoff_plane)<<"(s)"<<endl;
    cout<<"average fuel saved: "<<((float)fuel_saved/landing_plane)<<"(s)"<<endl;
    cout<<"total plane in emergency: "<<emergency<<endl;
    cout<<"total plane crashed: "<<crash<<endl;
    return 0;
}

