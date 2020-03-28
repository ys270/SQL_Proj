#include "query_funcs.h"
#include <climits>

void sql_execute(connection *C, string s){
  work W(*C);
  W.exec(s);
  W.commit();
}

void add_player(connection *C, int team_id, int jersey_num, string first_name,
		string last_name, int mpg, int ppg, int rpg, int apg,
		double spg, double bpg) {
  work W(*C);
  stringstream ss;
  ss << "INSERT INTO player (team_id, uniform_num, first_name, last_name, "
             "mpg, ppg, rpg, apg, spg, bpg) VALUES ("
      << team_id << ", " << jersey_num << ", " << W.quote(first_name) << ", "
      << W.quote(last_name) << ", " << mpg << ", " << ppg << ", " << rpg << ", "
      << apg << ", " << spg << ", " << bpg << ");";
  W.exec(ss.str());
  W.commit();
  }


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
  stringstream ss;
  ss<<"INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES("<<"\'"<<name<<"\', "<<state_id<<", "<<color_id<<", "<<wins<<", "<<losses<<");";
  sql_execute(C,ss.str());
}


void add_state(connection *C, string name)
{
  stringstream ss;
  ss <<"INSERT INTO STATE (NAME) VALUES(\'"<<name<<"\');";
  sql_execute(C,ss.str());
}


void add_color(connection *C, string name)
{
  stringstream ss;
  ss<<"INSERT INTO COLOR (NAME) VALUES(\'"<<name<<"\');";
  sql_execute(C,ss.str());
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  if(!use_mpg){
    min_mpg = INT_MIN;
    max_mpg = INT_MAX;
  }
  if(!use_ppg){
    min_ppg = INT_MIN;
    max_ppg = INT_MAX;
  }
  if(!use_rpg){
    min_rpg = INT_MIN;
    max_rpg = INT_MAX;
  }
  if(!use_apg){
    min_apg = INT_MIN;
    max_apg = INT_MAX;
  }
  if(!use_spg){
    min_spg = INT_MIN;
    max_spg = INT_MAX;
  }
  if(!use_bpg){
    min_bpg = INT_MIN;
    max_bpg = INT_MAX;
  }
  stringstream ss;
  ss<<"SELECT * FROM PLAYER WHERE (MPG BETWEEN "<<min_mpg<<" AND "<<max_mpg<<") AND "<<"(PPG BETWEEN "<<min_ppg<<" AND "<<max_ppg<<") AND "<<"(RPG BETWEEN "<<min_rpg<<" AND "<<max_rpg<<") AND "<<"(APG BETWEEN "<<min_apg<<" AND "<<max_apg<<") AND "<<"(SPG BETWEEN "<<min_spg<<" AND "<<max_spg<<") AND "<<"(BPG BETWEEN "<<min_bpg<<" AND "<<max_bpg<<");";
  nontransaction N(*C);
  result R(N.exec(ss.str()));
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
  for(result::iterator it=R.begin();it!=R.end();++it){
    cout<<it[0]<<" "<<it[1]<<" "<<it[2]<<" "<<it[3]<<" "<<it[4]<<" "<<it[5]<<" "<<it[6]<<" "<<it[7]<<" "<<it[8]<<" "<<fixed<<setprecision(1)<<it[9].as<double>()<<" "<<it[10].as<double>()<<endl;
  }
}


void query2(connection *C, string team_color)
{
  string s = "SELECT TEAM.NAME FROM TEAM, COLOR WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME = \'"+team_color+"\';";
  nontransaction N(*C);
  result R(N.exec(s));
  cout<<"NAME"<<endl;
  for(result::iterator it=R.begin();it!=R.end();++it){
    cout<<it[0]<<endl;
  }
}


void query3(connection *C, string team_name)
{
  string s = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = \'"+team_name+"\'"+" ORDER BY PPG DESC;";
  nontransaction N(*C);
  result R(N.exec(s));
  cout<<"FIRST_NAME LAST_NAME"<<endl;
  for(result::iterator it=R.begin();it!=R.end();++it){
    cout<<it[0]<<" "<<it[1]<<endl;
  }
}


void query4(connection *C, string team_state, string team_color)
{
  string s = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM FROM PLAYER, TEAM, STATE, COLOR WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND STATE.NAME = \'"+ team_state + "\' AND COLOR.NAME = \'"+team_color+"\';";
  nontransaction N(*C);
  result R(N.exec(s));
  cout<<"FIRST_NAME LAST_NAME UNIFORM_NUM"<<endl;
  for(result::iterator it=R.begin();it!=R.end();++it){
    cout<<it[0]<<" "<<it[1]<<" "<<it[2]<<endl;
  }
}


void query5(connection *C, int num_wins)
{
  stringstream ss;
  ss<<"SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS > "<<num_wins<<";";
  nontransaction N(*C);
  result R(N.exec(ss.str()));
  cout<<"FIRST_NAME LAST_NAME NAME WINS"<<endl;
  for(result::iterator it=R.begin();it!=R.end();++it){
    cout<<it[0]<<" "<<it[1]<<" "<<it[2]<<" "<<it[3]<<endl;
  }
}
