#include "ply.h"

int loadFile(const string file) 
{
    fstream fopen;

    if (!file.empty()) 
    {
        fopen.open(file, ios::in);
    } 
    else 
    {
        cout << "请输入文件名" << endl;
        return -1;
    }

    if (!fopen.is_open())
    {
        cout << "ply文件读取失败" << endl;
        return -1;
    } 

    string data;
    while (!fopen.eof()) 
    {
        fopen >> data;

        if (data == "ply" || data == "PLY" || data == "forment" || data == "comment" || data == "property") 
        {
            getline(fopen, data, '\n'); 
        } 
        else if (data == "element") 
        {
            fopen >> data;
            if (data == "vertex") 
            {
                fopen >> rabbitVertexNum;
                cout << "顶点个数为" << rabbitVertexNum << endl;
                getline(fopen, data, '\n');
            } 
            else if (data == "face") 
            {
                fopen >> rabbitFaceNum;
                cout << "面片个数为" << rabbitFaceNum << endl;
                getline(fopen, data, '\n');
            } 
            else 
            {
                cout << "无效的element信息" << endl;
                getline(fopen, data, '\n');
            }
        } 
        else if (data == "end_header") 
        {
            getline(fopen, data, '\n');

            int i = 0, n = 0;
            int nums;
            float confidence, intensity;

            cout << "读取顶点数据" << endl;

            for (int vnum = 0; vnum < rabbitVertexNum; vnum++)
            {
                fopen >> rabbitVertices[i++];
                fopen >> rabbitVertices[i++];
                fopen >> rabbitVertices[i++];

                fopen >> confidence;
                fopen >> intensity;
            }

            cout << "读取面片数据" << endl;

            glm::vec3 o, r1, r2;

            for (int fnum = 0; fnum < rabbitFaceNum; fnum++)
            {
                fopen >> nums;

                if (nums == 3) 
                {
                    fopen >> rabbitIndices[n];  o.x = rabbitVertices[rabbitIndices[6 * n]];  o.y = rabbitVertices[rabbitIndices[6 * n + 1]];  o.z = rabbitVertices[rabbitIndices[6 * n + 2]];  n++;
                    fopen >> rabbitIndices[n];  r1.x = rabbitVertices[rabbitIndices[6 * n]] - o.x;  r1.y = rabbitVertices[rabbitIndices[6 * n + 1]] - o.y;  r1.z = rabbitVertices[rabbitIndices[6 * n + 2]] - o.z;  n++;
                    fopen >> rabbitIndices[n];  r2.x = rabbitVertices[rabbitIndices[6 * n]] - o.x;  r2.y = rabbitVertices[rabbitIndices[6 * n + 1]] - o.y;  r2.z = rabbitVertices[rabbitIndices[6 * n + 2]] - o.z;  n++;

                    glm::vec3 normal = glm::cross(r1, r2);
                    addEdge(n - 1, normal);
                    addEdge(n - 2, normal);
                    addEdge(n - 3, normal);
                }
            }
        }
    }

    cout << "ply文件读取完毕" << endl;
    return 0;
}

void addEdge(int vertexIndex, glm::vec3 normal)
{
    if (edgeTable[vertexIndex] == nullptr) {
        edgeTable[vertexIndex]->edge = normal;
        edgeTable[vertexIndex]->next = nullptr;
        return;
    }

    edgeNode p = edgeTable[vertexIndex];
    while (p != nullptr) {
        p = p->next;
    }

    p->edge = normal;
    p->next = nullptr;
}

void calNormal()
{
    glm::vec3 normal;
    edgeNode p;
    int num;

    for (int i = 0; i < rabbitVertexNum; i++) {
        normal = glm::vec3(0.0f);
        p = edgeTable[i];
        num = 0;

        while (p != nullptr) {
            normal += p->edge;
            p = p->next;
            num++;
        }
        normal /= num;

        rabbitVertices[rabbitIndices[6 * i + 3]] = normal.x;
        rabbitVertices[rabbitIndices[6 * i + 4]] = normal.y;
        rabbitVertices[rabbitIndices[6 * i + 5]] = normal.z;
    }
}
