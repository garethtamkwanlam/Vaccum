#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <iterator>
#include <stdlib.h>
using namespace std;
const int WEST_I = 0;
const int EASTI = 2;
const int WESTI = 4;
const int SOUTHI = 3;
const int NORTHI = 1;
const int NORTH_I = 5;
const int HOME = 99;
const int desHome = 200;
const char EAST = 'E';
const char WEST = 'W';
const char SOUTH = 'S';
const char NORTH = 'N';
const int BLOCK = 100;
const int OPEN = 103;
const int VISITED = 101;
const int UNKNOWN = 105;
int goHome = true;
int tracex = 20; 
int tracey = 20;
int countd = 0;
const int range = 40;
int destinationx = -99;
int destinationy = -99;
char maindir = NORTH;
queue<char> priorityactionqueue;
queue<char> parsequeue;
queue <string> actionqueue;
queue <vector<int> > sstepqueue;
queue <vector<int> > sdestqueue;
vector<vector<int> > vdest;
bool razer_l, razer_f, razer_r, dirt, home;
int roommap[range][range];
bool checkfordone(){
	bool done = true;
	for ( int i = 0 ;i < range; i++) {
		for ( int j = 0; j < range; j++ ){
			if (roommap[i][j] == OPEN){
				done = false;
			}
		}
	}
	return done;
}
char convertDirToChar(int dirr){
	char n ;
	if (dirr == 0 || dirr == 4){
		n = WEST;
	}else if ( dirr == 2){
		n = EAST;
	}else if ( dirr == 3) {
		n = SOUTH;
	}else if (dirr == 1 || dirr == 5){
		n = NORTH;
	}
	return n;
}
int convertCharToDir(char dirr){
	int n ;
	if (dirr == NORTH){
		n = NORTHI;
	}else if ( dirr == EAST){
		n = EASTI;
	}else if ( dirr == SOUTH) {
		n = SOUTHI;
	}else if (dirr == WEST){
		n = WESTI;
	}
	return n;
}
bool block(int x, int y){
	bool pass = false;
	if (roommap[x][y] == BLOCK){
		pass = true;
	}
	return pass;
}
bool isHome(int x, int y){
//	bool pass = false;
	if ( goHome == true){
		return false;
	}else {
		return roommap[x][y] == HOME;
	}
}
bool checkstring(string s ){
        bool passed = false;
        //Regex regexf = "
        if (s.find("RL") == string::npos
                && s.find("LLL") == string::npos
                && s.find("LR") == string::npos
                && s.find("RRR") == string::npos
                && s.find("FRRF") == string::npos
                && s.find("FLLF") == string::npos
                && s.find("FRFRFRFR") == string::npos
                && s.find("FLFLFLFL") == string::npos
                && s.find("FFRRFFFRRF") == string::npos
                && s.find("FFLLFFFLLF") == string::npos
                && s.find("FLFLF") == string::npos
                ){
                passed = true;
        }
        return passed;
}
void changeForward(char maindir, string output){
	  if (output == "forward" && !razer_f){
		if (maindir == NORTH){
			tracey = tracey + 1;
		}else if (maindir == EAST){
			tracex = tracex + 1;
		}else if (maindir == SOUTH){
			tracey = tracey - 1;
		}else if (maindir == WEST){
			tracex = tracex - 1;
		}
	}
	if (roommap[tracex][tracey] != HOME){
			roommap[tracex][tracey] = VISITED;
	}
	cout << "#  status on roommap[" << tracex << "][" << tracey << "] : " <<  roommap[tracex][tracey] << endl;
	cout << "#  tracex : " << tracex << " tracey : "  << tracey << endl;
}
void updatedir ( char direction, string output){
	if (direction == EAST ) {
		if ( output == "right" ) {
			maindir = SOUTH;
		}else if (output == "left"){
			maindir = NORTH;
		}
	}
	else if (direction == SOUTH){
		if (output == "right"){
			maindir = WEST;
		} else if (output == "left"){
			maindir = EAST;
		} 
	}
	else if (direction == WEST){
		if (output == "right"){
			maindir = NORTH;
		}else if (output == "left"){
			maindir = SOUTH;
		}
	}
	else if (direction == NORTH){
		if (output == "right"){
			maindir = EAST;
		}else if (output == "left"){
			maindir = WEST;
		}
	}
}
void emptysset(){
	int qsize = sstepqueue.size();
	int qtemp2 = sstepqueue.size();
	while (qtemp2 > 0){
		sstepqueue.pop();
		qtemp2 = qtemp2 - 1;
	}
}
void emptysdest(){
	int qsize = sdestqueue.size();
	//int qtemp2 = sdestqueue.size();
	while (qsize > 0){
		sdestqueue.pop();
		qsize = qsize - 1;
	}
}
void emptyact(){
	int qsize = actionqueue.size();
	int qtemp2 = actionqueue.size();
	while (qtemp2 > 0){
		actionqueue.pop();
		qtemp2 = qtemp2 - 1;
	}
}
int getabsoluate(int big, int small){
	int sbol;
	if (big >= small){
		sbol = big - small;
	}else {
		int mi = big - small;
		sbol = mi * -1;
	}
	return sbol;
}
bool searchForLeftOver(){
	bool found = false;
	int minx = 100;
	int miny = 100;
	cout << " # found : " << found << endl;
	for ( int f = 0; f < range; f++){
		for ( int d = 0; d<range; d++ ){
		//	cout << " # found : " << found << endl;
			if (roommap[f][d] == OPEN){
				found = true;
				cout << " # set new destination " << "x " << f << " y " << d << endl;
				if ( ( getabsoluate(tracex, f) < minx ) &&  ( getabsoluate(tracey,d ) < miny ) ){
					minx = f;
					miny = d;
				}
				
			}
		}
		
	}
	destinationx = minx;
	destinationy = miny;
	//break;
	cout << " # is it found ? " << found << endl;
	return found;
}
bool deplicateDest(vector<int> vv){
	bool found = false;
	int xx = vv[0];
	int yy = vv[1];
	int zz = vv[2];
	for ( int i = 0 ; i < sdestqueue.size(); i++){
		vector<int> test = sdestqueue.front();
		sdestqueue.pop();
		// if (test[0] == xx && test[1] == yy && test[2] == zz){
		// 	found = true;
		// }
		if (test[0] == xx && test[1] == yy ){
			found = true;
		}
		//int te[] = { test[0], test[1], test[2] };
		vector<int> vv ;
		vv.push_back(test[0]);
		vv.push_back(test[1]);
		vv.push_back(test[2]);
		sdestqueue.push(vv);
	}
	return found;
}
void searchDest(){
	int atx = tracex;
	int aty = tracey;
	char atdir = maindir;
	int idir = convertCharToDir(atdir);
	cout << "# idir : " << idir << endl;
	cout << " # tracex : " << atx << endl;
	cout << " # tracey : " << aty << endl;
	int q[3] = {atx, aty, idir};
	std::vector<int> qint ;//( begin(q), end(q));
	qint.push_back(atx);
	qint.push_back(aty);
	qint.push_back(idir);
	// int xx1, yy1, dd1;
	sdestqueue.push(qint);
	bool run = true;
	cout << " # maindir is " << maindir << endl;
	int counter = 0;
	while (run){
			int xx, yy, dd;
		vector<int> qdest = sdestqueue.front();
		xx = qdest[0];
		yy = qdest[1];
		dd = qdest[2];
		char cdd = convertDirToChar(dd);
		cout << " # pcurrent : " << xx << "," << yy << "," << dd << endl;
		
		if (roommap[xx][yy] == OPEN){
			cout << " ## found destination " << endl;
			destinationx = xx;
			destinationy = yy;
			emptysdest();
			run = false;
			break;
		}else 
		if ( counter < 400 )
		{
			sdestqueue.pop();
			if (dd == NORTH_I){
				dd = NORTHI;
			}else if (dd == WEST_I){
				dd = WESTI;
			}
			//F
			int fy  = yy;
			int fx = xx;
			if (dd == WESTI){
				fx = xx - 1;
			}else if (dd == EASTI){
				fx = xx + 1;
			}else if (dd == SOUTHI){
				fy = yy - 1;
			}else if ( dd == NORTHI){
				fy = yy + 1;
			}
			vector<int> qfdest;//(begin(vvf), end(vvf));
			qfdest.push_back(fx);
			qfdest.push_back(fy);
			qfdest.push_back(dd);
			//R
			int dl = dd - 1;
			if (dl == NORTH_I){
				dl = NORTHI;
			}else if (dl == WEST_I){
				dl = WESTI;
			}
			
			vector<int> qldest;//(begin(vvr), end(vvr));
			qldest.push_back(xx);
			qldest.push_back(yy);
			qldest.push_back(dl);
			int dr = dd + 1;
			if (dr == NORTH_I){
				dr = NORTHI;
			}else if (dr == WEST_I){
				dr = WESTI;
			}
			vector<int> qrdest;//(begin(vvl), end(vvl));
			qrdest.push_back(xx);
			qrdest.push_back(yy);
			qrdest.push_back(dr);
			bool addforward, addright, addleft;
			if ( !deplicateDest(qfdest) && !block(qfdest[0],qfdest[1])  && !isHome(qfdest[0],qfdest[1])){
			sdestqueue.push(qfdest);
			}
			if (!deplicateDest(qrdest)){
			sdestqueue.push(qrdest);
			}
			if (!deplicateDest(qldest)){	
			sdestqueue.push(qldest);
			}
		}
		  else if ( counter >= 400){
		  	cout << " # hard to find " << endl;
		  	if ( searchForLeftOver() == true ) {
		  		emptysdest();
		  		break;
		  }else {
		 	emptysdest();
		  	destinationx = 20;
		  	destinationy = 20;
		  	goHome = true;
		  	break;
		  }
		}
		  counter++;
	}
}
bool deplicate(vector<int> vv){
	bool found = false;
	int xx = vv[0];
	int yy = vv[1];
	int zz = vv[2];
	for ( int i = 0 ; i < sstepqueue.size(); i++){
		vector<int> test = sstepqueue.front();
		sstepqueue.pop();
		if (test[0] == xx && test[1] == yy && test[2] == zz){
			found = true;
		}
		vector<int> vv ;//(begin(te), end(te));
		vv.push_back(test[0]);
		vv.push_back(test[1]);
		vv.push_back(test[2]);
		sstepqueue.push(vv);
	}
	return found;
}
void searchPath(){
	int atx = tracex;
	int aty = tracey;
	char atdir = maindir;
	int idir = convertCharToDir(atdir);
	cout << " # idir : " << idir << endl;
	std::vector<int> qint ;//( begin(q), end(q));
	qint.push_back(atx);
	qint.push_back(aty);
	qint.push_back(idir);
	sstepqueue.push(qint);
	if ( (destinationx == -99 && destinationy == -99 )  
			|| priorityactionqueue.size() == 0 || parsequeue.size() == 0){
		cout << " # destination reset " << endl ;
		searchDest();
	}
	
	int pathcounter = 0;
	while (true){
		int xx, yy, dd;
	vector<int> qaction = sstepqueue.front();
	xx = qaction[0];
	yy = qaction[1];
	dd = qaction[2];
	
	// if (print == true){
	// 	cout << " # dd " << qaction[2] << endl;
	// 	print = false;
	// }
	char cdd = convertDirToChar(dd);
	cout << " # ppath : " << xx << "," << yy << "," << dd << endl;
	if (xx == destinationx && yy == destinationy){
		cout << " ## found destination " << endl;
		if (actionqueue.size() != 0 && actionqueue.front() != ""){
			string opath = actionqueue.front();
			cout << " # found the path " << opath << endl;
			int strsize = opath.size(); 
			int strstemp = strsize;
			for ( int i = 0; i < strsize; i++){
				parsequeue.push(opath.at(i));
			}
		}
		emptyact();
		emptysset();
		break;
	}else 
		sstepqueue.pop();
		string opath = "";
		if ( actionqueue.size() != 0 && actionqueue.front() != ""){
			opath = actionqueue.front();
			actionqueue.pop();
		}
		
		if (dd == NORTH_I){
			dd = NORTHI;
		}else if (dd == WEST_I){
			dd = WESTI;
		}
		//F
		int fy  = yy;
		int fx = xx;
		if (dd == WESTI){
			fx = xx - 1;
		}else if (dd == EASTI){
			fx = xx + 1;
		}else if (dd == SOUTHI){
			fy = yy - 1;
		}else if ( dd == NORTHI){
			fy = yy + 1;
		}
		int vvf[] = { fx,fy,dd };
		vector<int> qf;//(begin(vvf), end(vvf));
		qf.push_back(fx);
		qf.push_back(fy);
		qf.push_back(dd);
		string f = "F";
		string fpath = opath + f;
		//R
		int dl = dd - 1;
		if (dl == NORTH_I){
			dl = NORTHI;
		}else if (dl == WEST_I){
			dl = WESTI;
		}
		
		//int vvr[] = { xx,yy,dl};
		vector<int> ql;//(begin(vvr), end(vvr));
		ql.push_back(xx);
		ql.push_back(yy);
		ql.push_back(dl);
		//std::vector<int> ql (fx, fy, dl);
		string l = "L";
		string lpath = opath + l;
		
		//L
		string r= "R";
		int dr = dd + 1;
		if (dr == NORTH_I){
			dr = NORTHI;
		}else if (dr == WEST_I){
			dr = WESTI;
		}
		//int vvl[] = { xx,yy,dr};
		vector<int> qr;//(begin(vvl), end(vvl));
		qr.push_back(xx);
		qr.push_back(yy);
		qr.push_back(dr);
		string rpath = opath + r;
		// cout << " # out rpath  " << rpath << endl;
		// cout << " # out lpath " << lpath << endl;
		// cout << " # fpath : " << fx << "," << fy << "," << dd << endl;
		// cout << " # lpath : " << fx << "," << fy << "," << dl << endl;
		// cout << " # rpath : " << fx << "," << fy << "," << dr << endl;
		 if ( !deplicate(qf) && !block(qf[0],qf[1])  && !isHome(qf[0],qf[1]) && checkstring(fpath)){
                sstepqueue.push(qf);
                actionqueue.push(fpath);
                }
                rpath = opath + r;
                if (!deplicate(qr)  && checkstring(rpath)){
                sstepqueue.push(qr);
                        actionqueue.push(rpath);
                }
                lpath = opath + l;
                if (!deplicate(ql) && checkstring(lpath)){
                sstepqueue.push(ql);
                        actionqueue.push(lpath);
                }
        }
}
void populatePriorityQueue(){
	if (parsequeue.size() == 0){
		cout << " # nothing I can get, searching for the Path " << endl;
		searchPath();
	}
	cout << " # there are " << parsequeue.size() << " item in the queue " << endl;
	char first, second, third;
	if (parsequeue.size() >= 3){
	first = parsequeue.front();
	parsequeue.pop();
	second = parsequeue.front();
	parsequeue.pop();
	third = parsequeue.front();
	parsequeue.pop();
	}else if (  parsequeue.size() == 2){
		first = parsequeue.front();
	parsequeue.pop();
	second = parsequeue.front();
	parsequeue.pop();
	}else if ( parsequeue.size() == 1){
		first = parsequeue.front();
	parsequeue.pop();
	}
	cout << " # first is " << first << " second is " << second << " third is " << third << endl;
		priorityactionqueue.push(first);
		priorityactionqueue.push(second);
		priorityactionqueue.push(third);
}
int main(){
	string output;
	cout << " # maindirection " << maindir << endl; //assuming that every is maindir
	//set everybox to be unknown
	for ( int s = 0 ; s < 40; s++){
		for (int g = 0; g< 40; g++){
			roommap[s][g] = UNKNOWN;
		}
	}
	bool init = true;
	char rl, rf, rr, dt, h;
	roommap[20][20] = HOME;
	while(1){
	
			string str;
			string output;
			getline(cin, str);
			rl = str[0];
			rf = str[2];
			rr = str[4];
			dt = str[6];
			h = str[8];
			if (rl == '1') {razer_l = true;} else { razer_l = false;};
			if (rf == '1') {razer_f = true;} else { razer_f = false;};
			if (rr == '1') {razer_r = true;} else { razer_r = false;};
			if (dt == '1') {dirt = true;} else { dirt = false;};
			if (h == '1')  {home = true;} else { home = false;};
			/*-----------------------------------------Update KB-----------------------------------------------------*/
				int yy = tracey + 1;
				int xx = tracex + 1;
				int xm = tracex - 1;
				int ym = tracey - 1;
				cout << "# maindir is " << maindir << endl;
			if (maindir == NORTH){
				
				if (razer_f){
					//cout << "#it is true" << endl;
					roommap[tracex][yy] = BLOCK;
				}else if (roommap[tracex][yy] > VISITED) {
					roommap[tracex][yy] = OPEN;
				}
				if (razer_r){
					roommap[xx][tracey] = BLOCK;
				}else if (roommap[xx][tracey] > VISITED){
					roommap[xx][tracey] = OPEN;
				}
				if (razer_l){
					roommap[xm][tracey] = BLOCK;
				}else if (roommap[xm][tracey] > VISITED){
					roommap[xm][tracey] = OPEN;
				}
			}else if (maindir == WEST){
				if (razer_f){
					roommap[xm][tracey] = BLOCK;
				}else if (roommap[xx][tracey] > VISITED){
					roommap[xm][tracey] = OPEN;
				}
				if (razer_r){
					roommap[tracex][yy] = BLOCK;
				}else if (roommap[tracex][yy] > VISITED){
					roommap[tracex][yy] = OPEN;
				}
				if (razer_l){
					roommap[tracex][ym] = BLOCK;
				}else if (roommap[tracex][ym] > VISITED){
					roommap[tracex][ym] = OPEN;
				}
			}else if (maindir == SOUTH ){
				if (razer_f){
					roommap[tracex][ym] = BLOCK;
				}else if (roommap[tracex][ym] > VISITED){
					roommap[tracex][ym] = OPEN;
				}
				if (razer_r){
					roommap[xm][tracey] = BLOCK;
				}else if (roommap[xm][tracey] > VISITED){
					roommap[xm][tracey] = OPEN;
				}
				if (razer_l){
					roommap[xx][tracey] = BLOCK;
				}else if (roommap[xx][tracey] > VISITED){
					roommap[xx][tracey] = OPEN;
				}
			}else if (maindir == EAST ){ // East
				if (razer_f){
					roommap[xx][tracey] = BLOCK;
				}else if (roommap[xx][tracey] > VISITED){
					roommap[xx][tracey] = OPEN;
				}
				if (razer_r){
					roommap[tracex][ym] = BLOCK;
				}else if (roommap[tracex][ym] > VISITED){
					roommap[tracex][ym] = OPEN;
				}
				if (razer_l){
					roommap[tracex][yy] = BLOCK;
				}else if (roommap[tracex][yy] > VISITED){
					roommap[tracex][yy] = OPEN;
				}
			}
			/*-----------------------------------------base case-----------------------------------------------------*/
			if (init == true && home){
				 init = false;
				 goHome = false;
				if (priorityactionqueue.size() == 0){
					populatePriorityQueue();
				}
				char re = priorityactionqueue.front();
				if (re == 'R'){
						output = "right";
						updatedir(maindir, output);
					}else if (re == 'L'){
						output = "left";
						updatedir(maindir, output);
					}else {
						output = "forward";
					}
				priorityactionqueue.pop();
				changeForward(maindir, output);
				cout << output << endl;
				continue;
			}
			else if ( goHome == true && home  ){
				cout << "off" << endl;
				continue;
			}
			else if (dirt){
				cout << "vacuum" << endl;
				continue;
			} 
			else {
					if (priorityactionqueue.size() == 0){
						cout << " # nothing come up, populating the priority Queue" << endl;
						populatePriorityQueue();
					}
					char re = priorityactionqueue.front();
				if (re == 'R'){
						output = "right";
						updatedir(maindir, output);
					}else if (re == 'L'){
						output = "left";
						updatedir(maindir, output);
					}else {
						output = "forward";
					}
				cout << " # size of the priorityactionqueue is " << priorityactionqueue.size() << endl;
				priorityactionqueue.pop();
				changeForward(maindir, output);
				cout << output << endl;
				continue;
				}
			}
}
