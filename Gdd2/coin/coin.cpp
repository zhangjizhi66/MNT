#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct coin{
    int ex;
    int ey;
};

int main(int argc, char** argv)
{
    ifstream ifs("coin.txt",ios::in);
    vector<coin> vcoin;
    char c;
    string s;
    int ex,ey;
    while (getline(ifs,s)) {
        getline(ifs,s);
        if ( s != "" ){
            stringstream ss;
            ss<<s;
            while (ss>>ex>>c>>ey>>c){
                coin c;
                c.ex = ex;
                c.ey = ey;
                vcoin.push_back(c);
            }
        }
        getline(ifs,s);
    }
    
    int ec;
    int ew = 2;
    cout<<"Enter gate energy: ";
    while (cin>>ec){
        cout<<"gated on "<<ec<<" keV:"<<endl;
        for (int i=0; i<(int)vcoin.size(); i++){
            if (vcoin[i].ex>ec-ew && vcoin[i].ex<ec+ew)
                cout<<vcoin[i].ey<<endl;
            else if (vcoin[i].ey>ec-ew && vcoin[i].ey<ec+ew)
                cout<<vcoin[i].ex<<endl;
        }
        cout<<'\n'<<"Enter gate energy: ";
    }
}