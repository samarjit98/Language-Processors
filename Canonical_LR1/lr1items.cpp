#include <bits/stdc++.h>
using namespace std;

vector<pair<char, string> > productions;
vector<vector<pair<char, pair<string, char> > > > itemsets;
map<pair<int, char>, int> goto1;
map<pair<int, char>, string> action;
set<char> terms;
map<char, set<char> > follows;
vector<pair<char, string> > closure(vector<pair<char, string> > kernel_item);
void build_items();
void find_first(string arg, set<char> &first_set);
void build_first();
void build_actions();

ofstream fout("output.txt", ios::out);

/*
vector<pair<char, pair<string, char> > > closure(vector<pair<char, pair<string, char> > > kernel_item){
	vector<pair<char, pair<string, char> > > ret;

	stack<pair<char, pair<string, char> > > s;
	
	for(int i=0; i<kernel_item.size(); i++)s.push(kernel_item[i]);

	vector<bool> added(productions.size(), false);
	for(int i=0; i<productions.size(); i++){
		for(int j=0; j<kernel_item.size(); j++){
			if(make_pair(kernel_item[j].first, kernel_item[j].second.first)==productions[i])added[i]=true;
		}
	}

	while(!s.empty()){
		pair<char, pair<string, char> > curr = s.top(); s.pop();
		ret.push_back(curr);

		int pos = -1;
		for(int i=0; i<curr.second.first.size(); i++){
			if(curr.second.first[i]=='.'){
				pos = i; break;
			}
		}

		if(pos==curr.second.first.size()-1)continue;

		char lhs = curr.second.first[pos+1];

		string str;
		for(int i=pos+2; i<curr.second.first.size(); i++)str+=curr.second.first[i];
		set<char> first_set;
		find_first(str, first_set);
		if(first_set.size()==0)first_set.insert('$');
		set<char>::iterator it;

		for(int i=0; i<productions.size(); i++){
			if(productions[i].first==lhs && !added[i]){
				for(it=first_set.begin(); it!=first_set.end(); it++){
					pair<char, string> curr_prod = productions[i];
					added[i] = true;
					curr_prod.second.insert(curr_prod.second.begin(), '.');
					s.push(make_pair(curr_prod.first, make_pair(curr_prod.second, *it)));
				}
			}
		}
	}

	return ret;
}
*/

vector<pair<char, pair<string, char> > > closure(vector<pair<char, pair<string, char> > > kernel_item){
	vector<pair<char, pair<string, char> > > ret;

	stack<pair<char, pair<string, char> > > s;
	
	for(int i=0; i<kernel_item.size(); i++)s.push(kernel_item[i]);

	set<pair<char, pair<string, char> > > changes;

	while(!s.empty()){
		pair<char, pair<string, char> > curr = s.top(); s.pop();
		changes.insert(curr);
		ret.push_back(curr);

		int pos = -1;
		for(int i=0; i<curr.second.first.size(); i++){
			if(curr.second.first[i]=='.'){
				pos = i; break;
			}
		}

		if(pos==curr.second.first.size()-1)continue;

		char lhs = curr.second.first[pos+1];

		string str;
		for(int i=pos+2; i<curr.second.first.size(); i++)str+=curr.second.first[i];
		set<char> first_set;
		find_first(str, first_set);
		if(first_set.size()==0)first_set.insert(curr.second.second);
		set<char>::iterator it;

		for(int i=0; i<productions.size(); i++){
			if(productions[i].first==lhs){
				for(it=first_set.begin(); it!=first_set.end(); it++){
					pair<char, string> curr_prod = productions[i];
					curr_prod.second.insert(curr_prod.second.begin(), '.');
					if(changes.find(make_pair(curr_prod.first, make_pair(curr_prod.second, *it)))==changes.end())s.push(make_pair(curr_prod.first, make_pair(curr_prod.second, *it)));
				}
			}
		}
	}

	return ret;
}

void build_items(){
	pair<char, string> curr_production = productions[0];
	curr_production.second.insert(curr_production.second.begin(), '.');
	
	vector<pair<char, pair<string, char> > > init; init.push_back(make_pair(curr_production.first, make_pair(curr_production.second, '$'))); itemsets.push_back(closure(init)); 
	stack<vector<pair<char, pair<string, char> > > > s; s.push(closure(init)); 

	while(!s.empty()){
		vector<pair<char, pair<string, char> > > now; now = s.top(); s.pop();

		int index = 0;

		for(int i=0; i<itemsets.size(); i++){
			if(now==itemsets[i]){
				index = i; break;
			}
		}

		map<char, vector<pair<char, pair<string, char> > > > kernels;

		for(int i=0; i<now.size(); i++){
			string rhs = now[i].second.first;
			for(int j=0; j<rhs.size(); j++){
				if(rhs[j]=='.' && j!=rhs.size()-1){
					swap(now[i].second.first[j], now[i].second.first[j+1]);
					kernels[now[i].second.first[j]].push_back(now[i]);
				}
			}
		}

		map<char, vector<pair<char, pair<string, char> > > >::iterator it;

		for(it=kernels.begin(); it!=kernels.end(); it++){
			vector<pair<char, pair<string, char> > > curr_close = closure(it->second);
			int flag = 0;
			for(int i=0; i<itemsets.size(); i++){
				if(itemsets[i]==curr_close){
					goto1[make_pair(index, it->first)] = i; flag = 1; break;
				}
			}

			if(flag==0){
				goto1[make_pair(index, it->first)] = itemsets.size();
				itemsets.push_back(curr_close);
				s.push(curr_close);
			}
		}
	}
}

void find_first(string arg, set<char> &first_set){
	if(arg.size()==0)return;
	if(arg[0]=='0'){
		first_set.insert(arg[0]); return;
	}
	else{
		for(int i=0; i<arg.size(); i++){
			int flag=0;
			if(!(arg[i]<='Z' && arg[i]>='A')){
				first_set.insert(arg[0]); return;
			}
			else{
				set<char> tmp_first_vec;
				vector<pair<char, string> > lhs_matched;

				for(int j=0; j<productions.size(); j++){
					if(productions[j].first==arg[i] && productions[j].second!=arg){
						lhs_matched.push_back(productions[j]);
					}
				}

				for(int j=0; j<lhs_matched.size(); j++){
					set<char> tmp_first_set;
					find_first(lhs_matched[j].second, tmp_first_set);
					set<char>::iterator it1;
					for(it1=tmp_first_set.begin(); it1!=tmp_first_set.end(); it1++)tmp_first_vec.insert(*it1);
				}

				if(tmp_first_vec.find('0')==tmp_first_vec.end())flag=1;

				set<char>::iterator it;

				for(it=tmp_first_vec.begin(); it!=tmp_first_vec.end(); it++){
					first_set.insert(*it);
				}
			}
			if(flag==1)break;
		}
	}
}

void build_first(){
	fout<<"\n\n";
	for(int i=0; i<productions.size(); i++){
		set<char> first_set;
		find_first(productions[i].second, first_set);
		fout<<"First for: "<<productions[i].second<<" - ";
		set<char>::iterator it;
		for(it=first_set.begin(); it!=first_set.end(); it++){
			fout<<" "<<*it;
		}
		fout<<endl;
	}
}

void build_actions(){
	for(int i=0; i<itemsets.size(); i++){
		for(int j=0; j<itemsets[i].size(); j++){
			string tmp = itemsets[i][j].second.first;
			for(int k=0; k<tmp.size(); k++){
				if(tmp[k]=='.' && k!=tmp.size()-1){
					if(!(tmp[k+1]>='A'&&tmp[k+1]<='Z')){
						stringstream ss; ss<<goto1[make_pair(i, tmp[k+1])];
						action[make_pair(i, tmp[k+1])] = "shift "+ss.str();
					}
				}
				else if(tmp[k]=='.' && k==tmp.size()-1){
					if(itemsets[i][j].first==productions[0].first){
                        action[make_pair(i, '$')] = "accept"; break;
                    }

                    pair<char, pair<string, char> > reduce_this = itemsets[i][j];
                    reduce_this.second.first.erase(reduce_this.second.first.find('.'));
                    stringstream ss; ss<<"reduce "<<reduce_this.first<<"->"<<reduce_this.second.first;
                    action[make_pair(i, reduce_this.second.second)] = ss.str();
				}
			}
		} 
	}
}

void parse(){
	string w;
	cin>>w; w+='$';
	int i=0;
	stack<int> s; s.push(0);

	fout<<"\n\nParsing actions:\n\n";
	fout<<"String is:"<<w<<"\n\n";

	while(1){
		string acc = action[make_pair(s.top(), w[i])];
		fout << "\nACTION[" << s.top() << ", " <<w[i]<<"]"<<endl;
		if(acc[0]=='s'){
			fout<<acc<<endl;
			string tmp; 
			for(int j=6; j<acc.size(); j++)tmp+=acc[j]; 
			s.push(atoi(tmp.c_str())); i++;
		}
		else if(acc[0]=='r'){
			fout<<acc;
			char lhs=acc[7]; string rhs;
			for(int j=10; j<acc.size(); j++)rhs+=acc[j];
			int popoff=rhs.size();

			while(popoff--)s.pop();
			s.push(goto1[make_pair(s.top(), lhs)]);
			fout<<endl;
		}
		else if(acc=="accept"){
			fout<<"Accepted"<<endl; break;
		}
		else{
			fout<<"Error()\n"; break;
		}
	}
}

int main(){
	int n; 
	cout<<"Enter number of productions:"; cin>>n;

	while(n--){
		char lhs; string rhs; 
		cout<<"Enter lhs:"; cin>>lhs;
		cout<<"Enter rhs:"; cin>>rhs;
		productions.push_back(make_pair(lhs, rhs));
		cout<<endl;
		for(int i=0; i<rhs.size(); i++)if(!(rhs[i]>='A' && rhs[i]<='Z'))terms.insert(rhs[i]);
	}
	terms.insert('$');
	build_first();
	build_items();
	build_actions();

	fout<<"\nItemsets:\n\n";

	for(int i=0; i<itemsets.size(); i++){
		fout<<"\n   ---------------------------------"<<endl;
		fout << "\tI-" << i <<":\n\n";

		for(int j=0; j<itemsets[i].size(); j++){
			fout<<"\t  "<<itemsets[i][j].first<<" -> "<<itemsets[i][j].second.first<<" , "<<itemsets[i][j].second.second<<endl;
		}
		fout<<"   ---------------------------------"<<endl;
		fout<<endl;
	}

	fout<<"\nGo to computations:\n\n";

	map<pair<int, char>, int>::iterator it;

	for(it=goto1.begin(); it!=goto1.end(); it++){
		fout<<"\tGOTO["<<(it->first).first<<", "<<(it->first).second<<"] = "<<it->second<<endl;
	}

	fout<<"\nActions in parsing table:\n\n";

	map<pair<int, char>, string>::iterator it2;

	for(it2=action.begin(); it2!=action.end(); it2++){
		fout<<"\tACTION["<<(it2->first).first<<", "<<(it2->first).second<<"] = "<<it2->second<<endl;
	}

	fout<<"\n\n";
	parse();
	fout.close();
	return 0;
}

/*

Enter number of productions:4
Enter lhs:A
Enter rhs:S

Enter lhs:S
Enter rhs:CC

Enter lhs:C
Enter rhs:cC

Enter lhs:C
Enter rhs:d

*/
