#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <sstream>
#include <string>
#include "exerciser.h"

using namespace std;
using namespace pqxx;


void create_tables(connection *C, string filename){
  ifstream f;
  string s,temp;
  f.open(filename.c_str(),fstream::in);
  while(getline(f,temp)){
    s += temp;
  }
  f.close();
  work W(*C);
  W.exec(s);
  W.commit();
}

void build_state(connection *C, string filename){
  ifstream f;
  string name,temp;
  int state_id;
  f.open(filename.c_str(),fstream::in);
  while(getline(f,temp)){
    stringstream ss;
    ss.str(temp);
    ss>>state_id>>name;
    add_state(C,name);
  }
  f.close();
}

void build_color(connection *C, string filename){
  ifstream f;
  string temp,name;
  int color_id;
  f.open(filename.c_str(),fstream::in);
  while(getline(f,temp)){
    stringstream ss;
    ss.str(temp);
    ss>>color_id>>name;
    add_color(C,name);
  }
  f.close();
}

void build_team(connection *C, string filename){
  ifstream f;
  string name,temp;
  int team_id,state_id,color_id,wins,losses;
  f.open(filename.c_str(),fstream::in);
  while(getline(f,temp)){
    stringstream ss;
    ss.str(temp);
    ss>>team_id>>name>>state_id>>color_id>>wins>>losses;
    add_team(C,name,state_id,color_id,wins,losses);
  }
  f.close();  
}

void build_player(connection *C, string filename){
  ifstream f;
  string first_name,last_name,temp;
  int player_id,team_id,jersey_num,mpg,ppg,rpg,apg;
  double spg,bpg;
  f.open(filename.c_str(),fstream::in);
  while(getline(f,temp)){
    stringstream ss;
    ss.str(temp);
    ss>>player_id>>team_id>>jersey_num>>first_name>>last_name>>mpg>>ppg>>rpg>>apg>>spg>>bpg;
    add_player(C,team_id,jersey_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg);
  }
  f.close();
}

int main (int argc, char *argv[]) 
{
  connection *C;
  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }  
  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  create_tables(C,"create_tables.sql");
  build_state(C,"state.txt");
  build_color(C,"color.txt");
  build_team(C,"team.txt");
  build_player(C,"player.txt");
  exercise(C);
  //Close database connection
  C->disconnect();

  return 0;
}


