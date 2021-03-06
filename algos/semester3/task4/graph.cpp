#include "graph.h"

char name_node(char c) {return c+'a';}


Graph :: Graph(): n(11), m(10)
/*
    Пример графа.
*/
{
    LIST.resize(n);
    LIST[0].push_back(1);
    LIST[1].push_back(0);
    LIST[2].push_back(3);LIST[2].push_back(6);
    LIST[3].push_back(2);LIST[3].push_back(4);LIST[3].push_back(5);
    LIST[4].push_back(3);LIST[4].push_back(5);LIST[4].push_back(6);
    LIST[5].push_back(3);LIST[5].push_back(4);
    LIST[6].push_back(2);LIST[6].push_back(4);
    LIST[7].push_back(8);LIST[7].push_back(9);
    LIST[8].push_back(7);LIST[8].push_back(9);
    LIST[9].push_back(7);LIST[9].push_back(8);

    cout << "Пример графа\nСписок смежности:\n";
    cout << "a: -b---------\n";
    cout << "b: a----------\n";
    cout << "c: ---d--g----\n";
    cout << "d: --c-ef-----\n";
    cout << "e: ---d-fg----\n";
    cout << "f: ---de------\n";
    cout << "g: --c-e------\n";
    cout << "h: --------ij-\n";
    cout << "i: -------h-j-\n";
    cout << "j: -------hi--\n";
    cout << "k: -----------\n";
    cout << "|V| = 11, |E| = 10\n";

}


Graph :: Graph(int maxV): n(maxV), m(0)
/*
    Ввод графа на n вершинах.
*/
{
    int G[n][n];
    string s;


    // обнуление матрицы
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            G[i][j] = 0;

    cout << "\nВведите множества смежности (строки букв a до z)\n";


    // заполнение матрицы смежности
    int k = 0;
    do
    {
        cout << "v[" << name_node(k) << "] = ";
        cin >> s;
        for (auto i : s)
            if (isalpha(i))
            {
                int j = tolower(i) - 'a';
                G[k][j] = G[j][k] = 1;
            }
        k++;
    }
    while(k < n);

    cout << "\nВведенный граф\nСписок смежности:";
    // заполнение списка смежности
    m = 0;
    LIST.resize(n);
    for (int i = 0; i < n; ++i)
    {
        int f = 0;
        cout << '\n' << name_node(i) << ": ";
        for (int j = 0; j < n; ++j)
            if (G[i][j])
            {
                ++f;
                LIST[i].push_back(j);
                cout << name_node(j);
            }
            else cout << '-';
        m += f;
    }

    cout << "\n|V| = " << n << ", |E| = " << m/2 << '\n';
}


Graph :: Graph(int maxV, char c): n(maxV), m(0)
/*
    Генерация случайного графа на n вершинах.
*/
{
    int G[n][n];

    // генерация случайной матрицы смежности
    for (int i = 0; i < n; ++i)
        for (int j = i; j < n; ++j)
            if (i == j)
                G[i][j] = G[j][i] = 0;
            else
                G[i][j] = G[j][i] = rand()%2;

    cout << "\nСгенерированный граф\nСписок смежности:";
    // заполнение списка смежности
    m = 0;
    LIST.resize(n);
    for (int i = 0; i < n; ++i)
    {
        int f = 0;
        cout << '\n' << name_node(i) << ": ";
        for (int j = 0; j < n; ++j)
            if (G[i][j])
            {
                ++f;
                LIST[i].push_back(j);
                cout << name_node(j);
            }
            else cout << '-';
        m += f;
    }

    cout << "\n|V| = " << n << ", |E| = " << m/2 << '\n';
}


void Graph :: spanning_tree(int start_node, bool* visited, vector <pair<int, int>> *tree, int* m)
/*
    Поиск стягивающего дерева поиском в ширину.
*/
{
    queue<int> q;
    visited[start_node] = true;
    q.push(start_node);

    while (!q.empty())
    {
        int current_node = q.front();
        q.pop();

        for (auto adj_node : LIST[current_node])
            if (!visited[adj_node])
            {
                visited[adj_node] = true;
                q.push(adj_node);
                // cout << "Добавлено ребро: " << current_node << ' ' << adj_node << '\n';
                (*tree)[(*m)++] = (make_pair(current_node, adj_node));
            }
    }
}


Forest Graph :: spanning_forest()
/*
    Поиск стягивающего леса серией поисков в ширину.
*/
{

    Forest f(n); // т.к стягивающий лес, то количество вершин графа = количество вершин леса
    bool *visited = new bool[n]; // список посещенных вершин для обхода всех компонент

    f.m = 0;
    f.edges.resize(n-1);

    for (int node = 0; node < n; ++node)
        if (!visited[node]) // если вершина не была посещена, то
        {
            // запускаем обход из этой вершины
            spanning_tree(node, visited, &f.edges, &f.m);
        }

    f.edges.resize(f.m);
    delete [] visited;
    return f;
}


void Forest :: print()
/*
    Вывод леса.
*/
{
    cout << "\n\nШиринный стягивающий лес. \n";
    cout << "|V| = " << n << ", |E| = " << m << '\n';

    cout << "Вершины: ";
    for (int i = 0; i < n; ++i)
        cout << name_node(i) << " ";

    if (m > 0)
    {
        cout << "\nСписок ребер: \n";
        for (auto e:edges)
            cout << name_node(e.first) << " " << name_node(e.second) << '\n';
    }
    else
        cout << "\nЛес не содержит ребер.\n";
}
