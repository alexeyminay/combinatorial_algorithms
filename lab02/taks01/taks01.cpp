#include <iostream>
#include <boost/timer.hpp>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <stack>
#include <unordered_map>

std::vector<std::vector<int>> read_file(std::string path)
{
	std::ifstream file;
	file.open(path);
	std::vector<std::vector<int>> graph;
	int thing_count = 0;
	if (file.is_open())
	{
		std::string input;
		int read_line = 0;
		while (!file.eof())
		{
			std::getline(file, input);
			std::vector<std::string> split_input;
			boost::split(split_input, input, boost::is_any_of(" "));
			if (read_line == 0)
			{
				int node_count = stoi(split_input[0]);
				for (int i = 0; i < node_count; i++)
				{
					std::vector<int> row;
					for (int k = 0; k < node_count; k++)
					{
						row.push_back(0);
					}
					graph.push_back(row);
				}
			}
			else
			{
				int from_node = stoi(split_input[0]);
				int to_node = stoi(split_input[1]);
				graph[from_node - 1][to_node - 1] = 1;
				graph[to_node - 1][from_node - 1] = 1;
			}
			read_line++;
		}
	}
	return graph;
}

void println(std::vector<std::vector<int>> matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int k = 0; k < matrix.size(); k++)
		{
			std::cout << matrix[i][k] << " ";
		}
		std::cout << std::endl;
	}
}

template <typename T>
void println(std::vector<T> vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		std::cout << vector[i] << " ";
	}
	std::cout << std::endl;
}

struct TreeNode
{
	int value;
	int parent;
	int enter;
	int exit;
	std::vector<TreeNode*> children;
	std::vector<TreeNode*> back_edge;

	TreeNode(int value, int parent, int enter)
	{
		this->value = value;
		this->enter = enter;
		this->parent = parent;
		exit = -1;
	}

	~TreeNode()
	{
		for (int i = 0; i < children.size(); i++)
		{
			delete children[i];
		}
	}

};

void printTreeNode(TreeNode *node)
{
	std::cout << "[" << node->value + 1 << "] - ";
	std::cout << "{" << node->enter + 1 << "," << node->exit + 1 << "}";

	if (node->back_edge.size() > 0)
	{
		std::cout << " - back_edges - (";
	}
	for (int i = 0; i < node->back_edge.size(); i++)
	{
		std::cout << node->back_edge[i]->value + 1 << "{"; 
		std::cout << node->back_edge[i]->enter + 1 << ", " << node->back_edge[i]->exit + 1 << "}";
		if (i < node->back_edge.size() - 1)
		{
			std::cout << ", ";
		} 
		else
		{
			std::cout << ");";
		}
	}
	std::cout << std::endl;

	for (int i = 0; i < node->children.size(); i++)
	{
		printTreeNode(node->children[i]);
	}
}

bool has_back_edge(TreeNode* node, int enter_time)
{
	std::stack<TreeNode*> dfs_stack;
	dfs_stack.push(node);
	while (dfs_stack.size() != 0)
	{
		TreeNode* top = dfs_stack.top();
		dfs_stack.pop();
		for (int i = 0; i < top->back_edge.size(); i++)
		{
			TreeNode* back_edge = top->back_edge[i];
			if (back_edge->enter < enter_time)
			{
				return true;
			}
		}
		for (int i = 0; i < top->children.size(); i++)
		{
			dfs_stack.push(top->children[i]);
		}
	}
	return false;
}

bool is_articulation_point(TreeNode* node)
{
	if (node->children.size() == 0)
	{
		return false;
	}
	bool is_articulation_point = false;
	for (int i = 0; i < node->children.size(); i++)
	{
		is_articulation_point = is_articulation_point || !has_back_edge(node->children[i], node->enter);
	}
	return is_articulation_point;
}

std::vector<int> find_articulation_points(TreeNode *root)
{
	std::vector<int> articulation_points;
	if (root->children.size() > 1)
	{
		articulation_points.push_back(root->value);
	}
	int back_paths_count = 0;
	std::stack<TreeNode*> dfs_stack;
	for (int i = 0; i < root->children.size(); i++)
	{
		dfs_stack.push(root->children[i]);
	}
	while (dfs_stack.size() != 0)
	{
		TreeNode* top = dfs_stack.top();
		dfs_stack.pop();
		if (is_articulation_point(top)) 
		{
			articulation_points.push_back(top->value + 1);
		}
		for (int i = 0; i < top->children.size(); i++)
		{
			dfs_stack.push(top->children[i]);
		}
	}
	return articulation_points;
}

template <typename T>
bool contains(std::vector<T> vec, const T& elem)
{
	bool result = false;
	if (std::find(vec.begin(), vec.end(), elem) != vec.end())
	{
		result = true;
	}
	return result;
}

void print_result(TreeNode *tree_root, std::vector<std::vector<int>> adjacency_matrix)
{
	std::cout << "----ADJACENCY MATRIX----" << std::endl;
	println(adjacency_matrix);
	std::cout << std::endl;
	std::cout << "----DFS TREE----" << std::endl;
	printTreeNode(tree_root);
	std::cout << std::endl;
	std::cout << "----ARICULATION POINTS----" << std::endl;
	println(find_articulation_points(tree_root));
	std::cout << std::endl;
}

int main()
{
	std::vector<std::vector<int>> adjacency_matrix = read_file("test/input.txt");
	std::unordered_map<int, TreeNode*> visited_tree_node_map;
	std::stack<TreeNode*> dfs_stack;
	int curent_node = 0;
	int time = 0;
	int start_node_value = 0;
    TreeNode* tree_root = new TreeNode(start_node_value, -1, time);
	visited_tree_node_map[start_node_value] = tree_root;
	dfs_stack.push(tree_root);
	while (dfs_stack.size() != 0)
	{
		TreeNode* top = dfs_stack.top();
		for (int i = 0; i < adjacency_matrix.size(); i++)
		{
			if (adjacency_matrix[top->value][i] == 1)
			{
				if (visited_tree_node_map[i] == nullptr) 
				{
					time++;
					TreeNode* child = new TreeNode(i, top->value, time);
					//std::cout << top->value + 1 << "-" << child->value + 1 << std::endl;
					dfs_stack.push(child);
					visited_tree_node_map[i] = child;
					top->children.push_back(child);
					break;
				}
				else
				{
					TreeNode* back_edge = visited_tree_node_map[i];
					bool isNotChildren = !contains(top->children, back_edge);
					bool isNotBackEdgeBackPath = !contains(back_edge->back_edge, top);
					bool isNotBackPath = top->parent != i;
					if (isNotChildren && isNotBackEdgeBackPath && isNotBackPath)
					{
						top->back_edge.push_back(back_edge);
						//std::cout << top->value + 1 << "-" << back_edge->value + 1 << std::endl;
					}
				}
			}
			if (i == adjacency_matrix.size() - 1)
			{
				//std::cout << "pop " << top->value + 1 << std::endl;
				time++;
				top->exit = time;
				dfs_stack.pop();
			}
		}
	}
	print_result(tree_root, adjacency_matrix);
	delete tree_root;
    return 0;
}
