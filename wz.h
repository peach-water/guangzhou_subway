#include "subway.h"

struct vertex
{
	size_t dis;
	int id;
	int path;
	bool known;
	bool popped;
	vertex() : dis(inf), known(false), popped(false) {}
};

void subway_system::read_name(string a){
	filename = a;
}

void subway_system::makeGraph(vector<edge> &edges)
{
	list<pair<int, int>> tmp;
	for (int i = Stations.size(); i--;)
		E.push_back(tmp);
	for (auto &item : edges)
	{
		E[item.u].push_back(pair<int, int>(item.v, item.w));
		E[item.v].push_back(pair<int, int>(item.u, item.w));
	}
}

vector<int> subway_system::getShortestPath(int origin, int destination)
{
	vector<int> res;
	set<int> known;
	//initialize
	vector<vertex> nodes;
	nodes.resize(n);
	for (size_t i = 0; i < nodes.size(); i++)
	{
		nodes[i].id = i;
		nodes[i].path = i;
		nodes[i].popped = false;
	}
	nodes[origin].dis = 0;
	nodes[origin].known = true;
	nodes[origin].path = origin;
	known.insert(origin);
	//calculating

	for (int i = 0; i < n; i++)
	{
		int index = 0, min_dis = inf;
		for (auto &entry : known)
			if (!(nodes[entry].popped) && nodes[entry].dis < min_dis)
			{
				min_dis = nodes[entry].dis;
				index = entry;
			}
		if (index == destination)
			break;

		//cout << station_name[index] << " ";

		nodes[index].popped = true;
		vertex u = nodes[index];

		for (auto &entry : E[u.id])
		{
			if (!nodes[entry.first].known)
			{
				if (nodes[entry.first].dis > nodes[u.id].dis + entry.second)
				{
					nodes[entry.first].known = true;
					known.insert(entry.first);
					nodes[entry.first].dis = nodes[u.id].dis + entry.second;
					nodes[entry.first].path = u.id;
				}
			}
		}
	}
	if (nodes[destination].known == false)
	{
		return res;
	}
	//get path
	stack<int> temp;
	int cur = destination;
	temp.push(cur);
	while (nodes[cur].path != origin)
	{
		cur = nodes[cur].path;
		temp.push(cur);
	}
	temp.push(origin);
	while (!temp.empty())
	{
		res.push_back(temp.top());
		temp.pop();
	}
	return res;
}

subway_system::subway_system()
{
	vector<edge> edges;
	readData(edges);
	// for (int i = 0; i < edges.size(); i++)
	// {
	// 	int u = edges[i].u, v = edges[i].v, w = edges[i].w;
	// 	cout << station_name[u] << ' ' << station_name[v] << ' ' << w << endl;
	// }
	// std::system("pause");
	makeGraph(edges);
}

void subway_system::readData(vector<edge> &edges)
{
	// string a;
	// cin >> a;
	ifstream fin("srcdata");
	int distance, ID[2];
	string line, station[2];

	while (fin >> line >> station[0] >> station[1] >> distance)
	{
		for (int i = 0; i < 2; i++)
		{
			auto findStation = Stations.find(station[i]);
			//查找不到这个站点
			if (findStation == Stations.end())
			{
				ID[i] = Stations.size();
				Stations[station[i]] = pair<string, int>(line, ID[i]);
				station_name.push_back(station[i]);
			}
			//该站点在本线路上已经出现
			else if ((*findStation).second.first == line)
				ID[i] = Stations[station[i]].second;
			//这是一个换乘站点
			else
			{
				string it_name = station[i];
				char c = '1';
				station[i] += c;
				auto it = Stations.find(station[i]);
				while (it != Stations.end() && (*it).second.first != line)
				{
					station[i].back() = ++c;
					it = Stations.find(station[i]);
				}
				if (it == Stations.end())
				{
					ID[i] = Stations.size();
					Stations[station[i]] = pair<string, int>(line, ID[i]);
					station_name.push_back(station[i]);
				}
				else
					ID[i] = Stations[station[i]].second;
				edges.push_back(edge(ID[i], Stations[it_name].second, 0));
			}
		}

		edges.push_back(edge(ID[0], ID[1], distance));
	}
	n = edges.size() + 1;
	fin.close();
	//cout << Stations.size() << endl;
	return;
}

string del_num(string a)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] >= '0' && a[i] <= '9')
		{
			a.erase(i, 1);
		}
	}
	return a;
}

string subway_system::checkLine(string oringin, string destination)
{
	vector<int> result;
	vector<string> output;
	vector<pair<string,string>> transform;
	int ori = this->Stations[oringin].second;
	int dest = this->Stations[destination].second;
	result = getShortestPath(ori, dest);
	if (result.size() == 0)
	{
		cout << "不存在这样一条道路" << endl;
		string a;
		return a;
	}

	for (int i = 0; i < result.size(); i++)
	{	
		output.push_back(del_num(station_name[result[i]]));
		
	}
	for (int i = 2; i < result.size();i++){
		if (del_num(station_name[result[i]]) == del_num(station_name[result[i - 1]]) && i > 1)
		{
			pair<string, string> temp(Stations[station_name[result[i]]].first, Stations[station_name[result[i - 1]]].first);
			transform.push_back(temp);
		}
	}
	cout << output[0] << "->";
	int j = 0;
	string all_in_one;
	for (int i = 1; i < output.size(); i++)
	{
		if (i == output.size() - 1)
		{
			all_in_one.append( output[i]);
			all_in_one.append("\n");
		}
		else if (output[i] != output[i - 1])
		{
			all_in_one.append(output[i]);
			all_in_one.append("->");
			// cout << output[i] << "->";
		}
		else
		{
			all_in_one.append("从");
			all_in_one.append(transform[j].second);
			all_in_one.append("号线转到");
			all_in_one.append(transform[j++].first);
			all_in_one.append("号线\n");
			// cout <<"从"<< transform[j++].first <<"号线转到"<<transform[j-1].second<<"号线" << endl;
		}
	}
	cout << all_in_one;
	
	std::system("pause");
	return all_in_one;
}