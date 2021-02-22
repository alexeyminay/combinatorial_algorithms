
#include <iostream>
#include <vector>



std::vector<int> create_first(int n, int k)
{
    std::vector<int> v;
    for (int i = 1; i < n + 1; ++i)
    {
        v.push_back(i);
    }
    return v;
}

void create_all(int n, int k, void(*output)(std::vector<int>))
{
    std::vector<int> v = create_first(n, k);
    int max_value = v[v.size() - 1];
    int cursor = v.size() - 1;
    while (v[0] < k - n + 1)
    {
        output(v);
        int next = v[cursor] + 1;
        while (next > k - n + cursor + 1)
        {
			cursor--;
            next = v[cursor] + 1;
        }
        while (cursor != v.size() - 1)
        {
			v[cursor] = next;
            cursor++;
            next++;
        }
        v[cursor] = next;
    }
    output(v);
}

void print(std::vector<int> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    void(*print_result)(std::vector<int>) = print;
    create_all(5, 9, print_result);
}

