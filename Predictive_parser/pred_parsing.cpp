#include <bits/stdc++.h>
using namespace std;

vector<pair<char, string> > productions;
map<pair<char, char>, pair<char, string> > table;
map<char, set<char> > follows;

ofstream fout("output.txt", ios::out);

void find_first(string arg, set<char> &first_set){
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

void find_follow(char nont, set<char> &follow_set, vector<bool> &visited){
	if(nont==productions[0].first){
		follow_set.insert('$'); 
	}

	for(int i=0; i<productions.size(); i++){
		if(!visited[i]){
			visited[i]=true;
			for(int j=0; j<productions[i].second.size(); j++){
				if(nont==productions[i].second[j]){
					if(j==productions[i].second.size()-1){
						if(nont!=productions[i].first){
							if(follows.find(productions[i].first)==follows.end()){
								set<char> tmp_follow_set;
								find_follow(productions[i].first, tmp_follow_set, visited);
								follows[productions[i].first] = tmp_follow_set;
								set<char>::iterator it;
								for(it=tmp_follow_set.begin(); it!=tmp_follow_set.end(); it++){
									follow_set.insert(*it);
								}
							}
							else{
								set<char>::iterator it;
								for(it=follows[productions[i].first].begin(); it!=follows[productions[i].first].end(); it++){
									follow_set.insert(*it);
								}
							}
						}
					}
					else{
						set<char> first_set;
						string beta;
						for(int k=j+1; k<productions[i].second.size(); k++)beta+=productions[i].second[k];
						find_first(beta, first_set);

						set<char>::iterator it;
						for(it=first_set.begin(); it!=first_set.end(); it++)if(*it!='0')follow_set.insert(*it);

						if(first_set.find('0')!=first_set.end()){
							if(nont!=productions[i].first){
								if(follows.find(productions[i].first)==follows.end()){
									set<char> tmp_follow_set;
									find_follow(productions[i].first, tmp_follow_set, visited);
									follows[productions[i].first] = tmp_follow_set;
									for(it=tmp_follow_set.begin(); it!=tmp_follow_set.end(); it++){
										follow_set.insert(*it);
									}
								}
								else{
									for(it=follows[productions[i].first].begin(); it!=follows[productions[i].first].end(); it++){
										follow_set.insert(*it);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void build_follow(){
	fout<<"\n\n";
	for(int i=0; i<productions.size(); i++){
		set<char> follow_set;
		vector<bool> visited(productions.size(), false);
		find_follow(productions[i].first, follow_set, visited);
		follows[productions[i].first] = follow_set;
		fout<<"Follow for: "<<productions[i].first<<" - ";
		set<char>::iterator it;
		for(it=follow_set.begin(); it!=follow_set.end(); it++){
			fout<<" "<<*it;
		}
		fout<<endl;
	}
}

void build_table(){
	for(int i=0; i<productions.size(); i++){
		set<char> first_set;
		find_first(productions[i].second, first_set);
		set<char>::iterator it;

		for(it=first_set.begin(); it!=first_set.end(); it++){
			if(*it!='0')table[make_pair(productions[i].first, *it)] = productions[i];
		}

		if(first_set.find('0')!=first_set.end()){
			set<char> follow_set = follows[productions[i].first];
			for(it=follow_set.begin(); it!=follow_set.end(); it++)table[make_pair(productions[i].first, *it)] = productions[i];
		}
	}
}

void parse(){
	string w;
	cin>>w;

	int i=0;
	stack<char> s; s.push('$'); s.push('E');

	while(s.top()!='$'){
		if(s.top()==w[i]){
			s.pop(); fout<<"match "<<w[i]<<endl; i++;
		}
		else if(!(s.top()<='Z' && s.top()>='A') && s.top()!='$'){
			fout<<"Error1()"<<endl; return;
		}
		else if(i<w.size() && table.find(make_pair(s.top(), w[i]))==table.end()){
			fout<<"Error2()"<<endl; return;
		}
		else{
			pair<char, string> curr_prod;
			if(i<w.size())curr_prod = table[make_pair(s.top(), w[i])];
			else curr_prod = table[make_pair(s.top(), '$')];
			fout<<curr_prod.first<<"->"<<curr_prod.second<<endl;
			s.pop();
			if(curr_prod.second!="0"){
				reverse(curr_prod.second.begin(), curr_prod.second.end());
				for(int k=0; k<curr_prod.second.size(); k++)s.push(curr_prod.second[k]);
			}
		}
	}
}

int main(){
	ifstream fin("input.txt", ios::in);
	int n; 
	cout<<"Enter number of productions:"; fin>>n;

	while(n--){
		char lhs; string rhs; 
		cout<<"Enter lhs:"; fin>>lhs;
		cout<<"Enter rhs:"; fin>>rhs;
		productions.push_back(make_pair(lhs, rhs));
		cout<<endl;
	}
	fin.close();
	
	build_first();
	build_follow();
	build_table();

	fout<<"\nParsing table:\n\n";

	map<pair<char, char>, pair<char, string> >::iterator it2;

	for(it2=table.begin(); it2!=table.end(); it2++){
		fout<<"\tENTRY["<<(it2->first).first<<", "<<(it2->first).second<<"] = "<<(it2->second).first<<"->"<<(it2->second).second<<endl;
	}

	fout<<"\n\n";
	parse();
	return 0;
}

/*

Enter number of productions:8
Enter lhs:E
Enter rhs:TA

Enter lhs:A
Enter rhs:+TA

Enter lhs:A
Enter rhs:0

Enter lhs:T
Enter rhs:FB

Enter lhs:B
Enter rhs:*FB

Enter lhs:B
Enter rhs:0

Enter lhs:F
Enter rhs:(E)

Enter lhs:F
Enter rhs:d`

8
E
TA
A
+TA
A
0
T
FB
B
*FB
B
0
F
(E)
F
d
*/
