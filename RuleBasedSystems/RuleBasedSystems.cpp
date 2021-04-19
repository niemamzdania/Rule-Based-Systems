#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdlib>

#define UNKNOWN -10;
#define INFINITY 1000;

using namespace std;

struct plik
{
	int column;
	int row;
	int value;
};

struct matrix
{
	int first;
	int second;
	vector <int> numbers;
};

struct lem
{
	int column;
	int row;
	int value;
	int decision;
};

struct valuesobj
{
	int value;
	int object;
};

struct rules
{
	int column;
	int value;
	vector <int> objects;
};

vector <plik> Read(string name);

void Algorithm(vector <plik> &data2);

vector <matrix> Create_matrix(vector <plik> &data);

void Rules_exhaustive(vector <matrix> &data, vector <plik> &data2);

void LEM2(vector <plik> &data2);

int main()
{
	vector <plik> data2 = Read("SystemDecyzyjny.txt");

	Algorithm(data2);

	vector <plik> data3 = Read("SystemDecyzyjny2.txt");

	vector <matrix> data = Create_matrix(data3);

	Rules_exhaustive(data, data3);

	vector <plik> data4 = Read("SystemDecyzyjny3.txt");

	LEM2(data4);

}

//---Read from file
vector <plik> Read(string name)
{
	vector <plik> content;

	fstream file;
	file.open(name, ios::in);

	if (file.good())
	{
		string line; //line from file
		string digit; //single value from file

		int number_of_row = 0;

		while (!file.eof())
		{
			getline(file, line);

			int counter = 0;

			for (int i = 0; i <= line.length(); i++)
			{
				if (line[i] == ' ' || line[i] == '\0')
				{
					int number = stod(digit);

					plik structure;
					structure.column = counter;
					structure.row = number_of_row;
					structure.value = number;

					content.push_back(structure);

					digit = "";
					counter++;
				}
				else
				{
					digit += line[i];
				}
			}

			number_of_row++;
		}
	}
	else
	{
		cout << "Blad pliku. Sprawdz katalog z projektem" << endl;
		system("PAUSE");
		exit(0);
	}

	return content;
}

//###################################################---Sequential covering algorithm
void Algorithm(vector <plik> &data2)
{

	const int number_of_rows = 8;
	const int number_of_columns = 7;

	int data[number_of_columns][number_of_rows];

	//convert data as board
	int k = 0;

	for (int i = 0; i < number_of_rows; i++)
	{
		for (int j = 0; j < number_of_columns; j++)
		{
			data[j][i] = data2[k].value;
			k++;
		}
	}

	//create rules of length one

	int decision;
	int value;
	int attribute;
	vector <int> values;
	vector <int> ignored_objects;
	bool flag = true;
	bool flag2 = true;


	cout << "Sequential covering algorithm" << endl << endl;
	cout << "Rules of length one: " << endl << endl;

	for (int i = 0; i < number_of_rows; i++)
	{

		if (ignored_objects.size() > 0)
		{
			for (int m = 0; m < ignored_objects.size(); m++)
			{
				if (i == ignored_objects[m])
				{
					flag = false;
					break;
				}
			}
		}

		if (flag == false)
		{
			flag = true;
			continue;
		}


		for (int j = 0; j < number_of_columns - 1; j++)
		{
			value = data[j][i];
			decision = data[6][i];
			attribute = j + 1;

			//checking rules for non-contradiction
			for (int k = 0; k < number_of_rows; k++)
			{
				if ((data[j][k] == value) && (data[6][k] != decision))
				{
					flag = false;
					break;
				}
			}

			if (flag == false)
			{
				flag = true;
				continue;
			}


			for (int l = 0; l < number_of_rows; l++)
			{
				if (data[j][l] == value)
				{
					values.push_back(value);

					//if (ignored_objects.size() > 0)
					//{
					//	for (int p=0; p<ignored_objects.size(); p++)
					//	{
					//		if (l == ignored_objects[p])
					//			flag2 = false;
					//	}
					//}

					//if(flag2 == true)
					ignored_objects.push_back(l);
				}
			}
			break;
		}

		if (values.size() > 0)
		{
			cout << "(a_" << attribute << " = " << value << ") => (d = " << decision << ")";
			if (values.size() > 1)
				cout << " [" << values.size() << "]" << endl;
			else cout << endl;


			//cout << "We ignore objects ";
			//for (int o = 0; o < ignored_objects.size(); o++)
			//	cout << ignored_objects[o] + 1 << " ";
			values.clear();
		}


		flag = true;
	}
	cout << endl;
}

//---Create indiscernibility matrix
vector <matrix> Create_matrix(vector <plik> &data)
{
	const int number_of_rows = 8;
	const int number_of_columns = 7;

	int decide_ob1;
	int decide_ob2;

	vector <int> same;

	matrix str;

	vector <matrix> matrix2;

	for (int i = 0; i < number_of_rows; i++)
	{
		for (int j = 0; j < number_of_rows; j++)
		{
			//if (i >= j)
			//{
			//	continue;
			//}

			if (i == j)
				continue;

			for (int l = 0; l < data.size(); l++)
			{
				if (data[l].column == 6 && data[l].row == i)
					decide_ob1 = data[l].value;
				else if (data[l].column == 6 && data[l].row == j)
					decide_ob2 = data[l].value;

			}

			if (decide_ob1 == decide_ob2)
				continue;

			for (int k = 0; k < data.size(); k++)
			{
				for (int m = 0; m < data.size(); m++)
				{
					if (data[k].row == i && data[m].row == j)
					{
						if (data[k].column == data[m].column)
						{
							if (data[k].column != number_of_columns - 1)
							{
								if (data[k].value == data[m].value)
								{
									same.push_back(data[k].column);
								}
							}
						}
					}
				}
			}

			if (same.size() > 0)
			{
				str.first = i;
				str.second = j;
				str.numbers = same;
				same.clear();
				matrix2.push_back(str);
			}
		}
	}

	return matrix2;
}

//#############################################################---Exhaustive algorithm
void Rules_exhaustive(vector <matrix> &data, vector <plik> &data2)
{

	const int number_of_rows = 8;
	const int number_of_col = 7;

	int datatab[number_of_col][number_of_rows];

	//convert data as board
	int z = 0;

	for (int i = 0; i < number_of_rows; i++)
	{
		for (int j = 0; j < number_of_col; j++)
		{
			datatab[j][i] = data2[z].value;
			z++;
		}
	}


	//---Rules of length one
	cout << endl << "Exhaustive set of rules algorithm" << endl;

	const int number_of_columns = 8;
	const int number_of_attributes = 6;

	vector <int> quantity_of_at;

	vector <int> numbers_of_at;

	vector <string> rules_l1;
	string rule_l1;

	matrix structure;


	bool flag = false; // true - attribute is on list of attributes from single column

	for (int i = 0; i < number_of_attributes; i++)
		quantity_of_at.push_back(i);

	for (int i = 0; i < number_of_columns; i++)
	{
		for (int j = 0; j < data.size(); j++)
		{
			if (data[j].first == i)
			{
				for (int k = 0; k < data[j].numbers.size(); k++)
				{
					numbers_of_at.push_back(data[j].numbers[k]);
				}
			}
		}

		for (int l = 0; l < quantity_of_at.size(); l++)
		{
			for (int m = 0; m < numbers_of_at.size(); m++)
			{
				if (numbers_of_at[m] == quantity_of_at[l])
					flag = true;
			}

			if (flag == false)
			{
				//cout << "(a_" << l + 1 << " = " << datatab[l][i] << ") => (d = " << datatab[6][i] << ")" << endl;

				//Convert rules to check coverage
				rule_l1 += (l + 1) + '0';
				rule_l1 += datatab[l][i] + '0';
				rule_l1 += datatab[6][i] + '0';

				rules_l1.push_back(rule_l1);
				rule_l1.clear();

				datatab[l][i] = UNKNOWN;
				structure.first = i;
				structure.second = UNKNOWN;
				structure.numbers.push_back(l);
				data.push_back(structure);
				structure.numbers.clear();

			}

			flag = false;
		}

		numbers_of_at.clear();
	}


	vector <int> covered_rules_l1;
	int coverage_l1 = 0;

	//-----------------------------------------------------------
	cout << endl << "Rules of length one:" << endl << endl;

	for (int i = 0; i < rules_l1.size(); i++)
	{
		flag = false;

		for (int k = 0; k < covered_rules_l1.size(); k++)
		{
			if (i == covered_rules_l1[k])
			{
				flag = true;
				break;
			}
		}

		if (flag == true)
			continue;


		for (int j = 0; j < rules_l1.size(); j++)
		{

			if (rules_l1[i] == rules_l1[j])
			{
				coverage_l1 += 1;
				covered_rules_l1.push_back(j);
			}
		}

		cout << "(a_" << rules_l1[i][0] << " = " << rules_l1[i][1] << ") => (d = " << rules_l1[i][2] << ")";
		if (coverage_l1 > 1)
			cout << " [" << coverage_l1 << "]" << endl;
		coverage_l1 = 0;
	}

	//----------------------------------------------------------------------------################################

	//-----------------------------------------------Rules of length two

	vector <string> comb;
	vector <string> all_combinations;
	vector <string> to_check_comb;
	vector <string> true_combinations;
	char to_ignore = 'q'; //random letter
	string rule;
	vector <string> rules;
	int coverage = 0;
	vector <int> covered_rules;



	for (int i = 0; i < number_of_attributes;i++)
	{
		for (int j = 0; j < number_of_attributes;j++)
		{
			if (i >= j)
				continue;

			ostringstream helping;
			helping << i;
			string letter1 = helping.str();
			ostringstream helping2;
			helping2 << j;
			string letter2 = helping2.str();
			all_combinations.push_back(letter1 + letter2);
		}
	}



	for (int i = 0; i < number_of_columns; i++)
	{
		for (int j = 0; j < data.size(); j++)
		{
			if (data[j].first == i && data[j].second != -10)
			{
				for (int k = 0; k < data[j].numbers.size(); k++)
				{
					for (int l = 0; l < data[j].numbers.size();l++)
					{
						if (k >= l)
							continue;

						ostringstream help;
						help << data[j].numbers[k];
						string letter1 = help.str();
						ostringstream help2;
						help2 << data[j].numbers[l];
						string letter2 = help2.str();

						comb.push_back(letter1 + letter2);
					}
				}
			}
		}

		//for (int a = 0;a < comb.size();a++)
		//	cout << comb[a] << endl;
		//cout << endl;


		for (int m = 0; m < all_combinations.size(); m++)
		{
			for (int n = 0; n < comb.size(); n++)
			{
				if (comb[n] == all_combinations[m])
					flag = true;
			}

			if (flag == false)
				to_check_comb.push_back(all_combinations[m]);

			flag = false;
		}


		for (int p = 0;p < data.size();p++)
		{
			if (data[p].first == i && data[p].second == -10)
			{
				to_ignore = data[p].numbers[0] + '0';
				break;
			}
		}

		//Here!
		for (int r = 0;r < to_check_comb.size();r++)
		{
			if (to_check_comb[r][to_check_comb[r].length() - 1] != to_ignore && to_check_comb[r][0] != to_ignore)
			{
				true_combinations.push_back(to_check_comb[r]);
			}
		}


		for (int s = 0;s < true_combinations.size();s++)
		{
			int nr_of_at = true_combinations[s][0] - '0';
			int nr_of_at2 = true_combinations[s][true_combinations[s].length() - 1] - '0';


			//conversion single rule as string to compare rules

			rule += (nr_of_at + 1) + '0';
			rule += datatab[nr_of_at][i] + '0';
			rule += (nr_of_at2 + 1) + '0';
			rule += datatab[nr_of_at2][i] + '0';
			rule += datatab[number_of_col - 1][i] + '0';
			rules.push_back(rule);
			rule = "";
		}

		comb.clear();
		to_check_comb.clear();
		true_combinations.clear();
		to_ignore = INFINITY;

	}



	flag = false;

	vector <string> rules_l2;

	cout << endl << endl << "Rules of length two:" << endl << endl;

	for (int i = 0; i < rules.size(); i++)
	{
		flag = false;

		for (int k = 0; k < covered_rules.size(); k++)
		{
			if (i == covered_rules[k])
			{
				flag = true;
				break;
			}
		}

		if (flag == true)
			continue;


		for (int j = 0; j < rules.size(); j++)
		{

			if (rules[i] == rules[j])
			{
				coverage += 1;
				covered_rules.push_back(j);
			}
		}

		cout << "(a_" << rules[i][0] << " = " << rules[i][1] << ") ^ (a_" << rules[i][2] << " = " << rules[i][3] << ") => (d = " << rules[i][4] << ")";

		if (coverage <= 1)
			cout << endl;
		else cout << " [" << coverage << "]" << endl;

		coverage = 0;
		rules_l2.push_back(rules[i]);
	}


	//--------------------------------------Rules of length 3
	//Erase data for next step
	all_combinations.clear();
	comb.clear();
	to_check_comb.clear();
	true_combinations.clear();
	to_ignore = 'q'; //random letter
	rule = "";
	rules.clear();
	coverage = 0;
	covered_rules.clear();
	flag = false;


	for (int i = 0; i < number_of_attributes; i++)
	{
		for (int j = 0; j < number_of_attributes; j++)
		{
			if (i >= j)
				continue;
			for (int k = 0; k < number_of_attributes; k++)
			{
				if (j >= k)
					continue;

				ostringstream helping;
				helping << i;
				string letter1 = helping.str();
				ostringstream helping2;
				helping2 << j;
				string letter2 = helping2.str();
				ostringstream helping3;
				helping3 << k;
				string letter3 = helping3.str();

				all_combinations.push_back(letter1 + letter2 + letter3);
			}
		}
	}



	for (int i = 0; i < number_of_columns; i++)
	{
		for (int j = 0; j < data.size(); j++)
		{
			if (data[j].first == i && data[j].second != -10)
			{

				for (int k = 0; k < data[j].numbers.size(); k++)
				{
					for (int l = 0; l < data[j].numbers.size(); l++)
					{
						if (k >= l)
							continue;
						for (int m = 0; m < data[j].numbers.size(); m++)
						{
							if (l >= m)
								continue;

							ostringstream help;
							help << data[j].numbers[k];
							string letter1 = help.str();
							ostringstream help2;
							help2 << data[j].numbers[l];
							string letter2 = help2.str();
							ostringstream help3;
							help3 << data[j].numbers[m];
							string letter3 = help3.str();

							//Save combinations without checking it

							comb.push_back(letter1 + letter2 + letter3);
						}
					}
				}
			}
		}

		//for (int a = 0;a < comb.size();a++)
		//	cout << comb[a] << endl;
		//cout << endl;


		for (int n = 0; n < all_combinations.size(); n++)
		{
			for (int o = 0; o < comb.size(); o++)
			{
				if (comb[o] == all_combinations[n])
					flag = true;
			}

			if (flag == false)
				to_check_comb.push_back(all_combinations[n]);

			flag = false;
		}


		for (int p = 0;p < data.size(); p++)
		{
			if (data[p].first == i && data[p].second == -10)
			{
				to_ignore = data[p].numbers[0] + '0';
				break;
			}
		}


		for (int r = 0; r < to_check_comb.size(); r++)
		{
			if (to_check_comb[r][to_check_comb[r].length() - 1] != to_ignore && to_check_comb[r][0] != to_ignore && to_check_comb[r][1] != to_ignore)
			{
				true_combinations.push_back(to_check_comb[r]);
			}
		}

		//cout << endl << "True combinations (column " << i+1 << "):" << endl;
		//for (int a = 0; a < true_combinations.size(); a++)
		//	cout << true_combinations[a] << endl;

		//GOOD!


		for (int s = 0;s < true_combinations.size();s++)
		{
			int nr_of_at = true_combinations[s][0] - '0';
			int nr_of_at2 = true_combinations[s][1] - '0';
			int nr_of_at3 = true_combinations[s][true_combinations[s].length() - 1] - '0';


			//conversion single rule as string to compare rules

			rule += (nr_of_at + 1) + '0';
			rule += datatab[nr_of_at][i] + '0';
			rule += (nr_of_at2 + 1) + '0';
			rule += datatab[nr_of_at2][i] + '0';
			rule += (nr_of_at3 + 1) + '0';
			rule += datatab[nr_of_at3][i] + '0';
			rule += datatab[number_of_col - 1][i] + '0';
			rules.push_back(rule);
			rule = "";
		}


		comb.clear();
		to_check_comb.clear();
		true_combinations.clear();
		to_ignore = INFINITY;
	}


	flag = false;


	cout << endl << "Rules of length 3:" << endl << endl;

	for (int i = 0; i < rules.size(); i++)
	{
		flag = false;

		for (int j = 0; j < rules_l2.size(); j++)
		{
			if ((rules_l2[j][0] == rules[i][0] && rules_l2[j][1] == rules[i][1] && rules_l2[j][2] == rules[i][2] && rules_l2[j][3] == rules[i][3] && rules_l2[j][4] == rules[i][6]) || (rules_l2[j][0] == rules[i][0] && rules_l2[j][1] == rules[i][1] && rules_l2[j][2] == rules[i][4] && rules_l2[j][3] == rules[i][5] && rules_l2[j][4] == rules[i][6]) || (rules_l2[j][0] == rules[i][2] && rules_l2[j][1] == rules[i][3] && rules_l2[j][2] == rules[i][4] && rules_l2[j][3] == rules[i][5] && rules_l2[j][4] == rules[i][6]))
			{
				flag = true;
				break;
			}
		}

		if (flag == false)
			cout << "(a_" << rules[i][0] << " = " << rules[i][1] << ") ^ (a_" << rules[i][2] << " = " << rules[i][3] << ") ^ (a_" << rules[i][4] << " = " << rules[i][5] << ") => (d = " << rules[i][6] << ")" << endl;
	}

}

//#############################################---LEM2
void LEM2(vector <plik> &data2)
{
	cout << endl << endl << "LEM2 Algorithm" << endl << endl << "Rules: " << endl << endl;

	const int number_of_symbols = 2;
	const int number_of_columns = 6;
	const int number_of_rows = 7;

	//---Convert data as vector of lem structures
	vector <lem> data;
	lem structure;
	vector <int> numbers_of_objects;

	for (int k = 1; k <= number_of_symbols; k++)
	{
		for (int i = 0; i < data2.size(); i++)
		{
			if (data2[i].column == (number_of_columns - 1) && data2[i].value == k)
			{
				numbers_of_objects.push_back(data2[i].row);
			}

		}
		for (int l = 0; l < numbers_of_objects.size(); l++)
		{
			for (int j = 0; j < data2.size(); j++)
			{
				if ((data2[j].row == numbers_of_objects[l]) && (data2[j].column != (number_of_columns - 1)))
				{
					structure.column = data2[j].column;
					structure.row = data2[j].row;
					structure.value = data2[j].value;
					structure.decision = k;
					data.push_back(structure);
				}

			}
		}
		numbers_of_objects.clear();
	}
	//---.

	//The most common descriptor
	const int quantity_col_lem = 5;
	const int quantity_row_lem = 7;


	int i = 1;
	valuesobj valobj;
	rules rul;
	vector <rules> ruls;
	vector <rules> ruls2;
	vector <valuesobj> values;
	vector <int> objects_rul;
	int quantity_rule = 0;
	bool flag = false;
	int max = 0;
	int nr_reg = 0;
	vector <int> to_ignore_obj;
	string rule;
	vector <string> rules;
	vector <int> to_check_obj;
	bool flag2 = true;
	bool flag3 = false;
	bool flag4 = false;
	bool flagpom = false;
	vector <int> to_ignore_col;

	while (i <= number_of_symbols)
	{
		max = 0;

		for (int j = 0; j < quantity_col_lem; j++)
		{
			flag3 = false;
			//flag4 = false;

			if (to_ignore_col.size() > 0)
			{
				for (int k = 0; k < to_ignore_col.size(); k++)
				{
					if (j == to_ignore_col[k])
						flag3 = true;
				}
			}

			if (flag3 == true)
				continue;

			for (int k = 0; k < data.size(); k++)
			{
				flag2 = true;
				flag3 = false; //not necessary
				flag4 = false;

				if (to_check_obj.size() == 0)
					flag2 = false;

				for (int l = 0; l < to_check_obj.size(); l++)
				{
					if (data[k].row == to_check_obj[l])
						flag2 = false;
				}

				//if (to_ignore_obj.size() == 0)
				//	flag4 = false;

				for (int w = 0; w < to_ignore_obj.size(); w++)
				{
					if (data[k].row == to_ignore_obj[w])
						flag4 = true;
				}

				if (data[k].column == j && data[k].decision == i && flag2 == false && flag4 == false)
				{
					valobj.object = data[k].row;
					valobj.value = data[k].value;
					values.push_back(valobj);


				}
			}

			//Here is good! :)

			for (int k = 0; k < values.size(); k++)
			{
				flag = false;

				for (int l = 0; l < objects_rul.size(); l++)
				{
					if (values[k].object == objects_rul[l])
					{
						flag = true;
						break;
					}
				}

				if (flag == true)
					continue;


				for (int m = 0; m < values.size(); m++)
				{

					if (values[k].value == values[m].value)
					{
						objects_rul.push_back(values[m].object);
					}
				}

				rul.column = j;
				rul.objects = objects_rul;
				rul.value = values[k].value;
				ruls.push_back(rul);

				objects_rul.clear();

			}


			for (int k = 0; k < ruls.size(); k++)
			{
				flag = false;

				for (int l = 0; l < ruls2.size(); l++)
				{
					if (ruls2[l].column == ruls[k].column && ruls2[l].value == ruls[k].value && ruls2[l].objects == ruls[k].objects)
					{
						flag = true;
						break;
					}
				}
				if (flag == false)
					ruls2.push_back(ruls[k]);
			}



			ruls.clear();
			objects_rul.clear();
			values.clear();
		}

		for (int k = 0; k < ruls2.size();k++)
		{
			if (ruls2[k].objects.size() > max)
			{
				max = ruls2[k].objects.size();
			}
		}

		for (int k = 0; k < ruls2.size(); k++)
		{
			if (ruls2[k].objects.size() == max)
			{
				nr_reg = k;
				break;
			}
		}

		flag = false;

		//---checking contradiction of max desciptor
		for (int k = 0; k < data.size(); k++)
		{
			if (ruls2[nr_reg].column == data[k].column && ruls2[nr_reg].value == data[k].value && data[k].decision != i)
			{
				flag = true;
			}
		}

		if (flag == false)
		{
			to_check_obj.clear();
			to_ignore_col.clear();

			rule += "(a_";
			rule += (ruls2[nr_reg].column + 1) + '0';
			rule += " = ";
			rule += ruls2[nr_reg].value + '0';
			rule += ") => (d = ";
			rule += i + '0';
			rule += ")";

			if (ruls2[nr_reg].objects.size() > 1)
			{
				rule += " [";
				rule += (ruls2[nr_reg].objects.size()) + '0';
				rule += "]";
			}

			rules.push_back(rule);

			for (int k = 0; k < ruls2[nr_reg].objects.size(); k++)
				to_ignore_obj.push_back(ruls2[nr_reg].objects[k]);

			rule = "";

		}

		if (flag == true)
		{
			to_check_obj.clear();

			rule += "(a_";
			rule += (ruls2[nr_reg].column + 1) + '0';
			rule += " = ";
			rule += ruls2[nr_reg].value + '0';
			rule += ") ^ ";

			to_ignore_col.push_back(ruls2[nr_reg].column);

			for (int j = 0; j < ruls2[nr_reg].objects.size(); j++)
				to_check_obj.push_back(ruls2[nr_reg].objects[j]);

		}

		ruls2.clear();
		nr_reg = 0;

		if (i == 2)
		{
			if (to_ignore_obj.size() >= 3)
				i += 1;
		}

		if (i == 1)
		{
			if (to_ignore_obj.size() >= 4)
			{
				i += 1;
				to_ignore_obj.clear();
			}
		}
	}


	//Print rules
	for (int i = 0; i < rules.size(); i++)
		cout << rules[i] << endl;


}