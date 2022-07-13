#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

constexpr auto MAX_TREE_HT = 256;

map<char, string> codes;
map<char, int> freq;

struct MinHeapNode
{
    char data;			
    int freq;			
    MinHeapNode* left, * right; 

    MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

void storeCodes(struct MinHeapNode* root, string str)
{
    if (root == NULL)
        return;
    if (root->data != '$')
        codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}
priority_queue<MinHeapNode*, vector<MinHeapNode*>> minHeap;
void HuffmanCodes(int size)
{
    struct MinHeapNode* left, * right, * top;
    for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
    while (minHeap.size() != 1)
    {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    storeCodes(minHeap.top(), "");
}
void calcFreq(string str, int n)
{
    for (int i = 0; i < str.size(); i++)
        freq[str[i]]++;
}

string decode_file(struct MinHeapNode* root, string s)
{
    string ans = "";
    struct MinHeapNode* curr = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;
    }
    return ans + '\0';
}


string setStringtoASCII(string str)
{
    int N = int(str.size());
    if (N % 8 != 0)
        return "Not Possible!";

    string res = "";

    for (int i = 0; i < N; i += 8)
    {
        int decimal_value = stoi((str.substr(i, 8)), 0, 2);
        res += char(decimal_value);
    }
    return res;
}
void encoder(string filename, string cov)
{
    ifstream input(filename, ios::binary);
    string buffer(istreambuf_iterator<char>(input), {});

    string binary = "";
    for (size_t i = 0; i < buffer.size(); ++i)
        binary += bitset<8>(buffer.c_str()[i]).to_string();

    int pos = filename.find_last_of(".");
    string ext = filename.substr(++pos);

    vector<int> deci;
    for (int i = 0; i < binary.length(); i += 3)
        deci.push_back(stoi(binary.substr(i, 3), 0, 2));
    Mat cover = imread(cov);
    Mat hidden = imread(cov);
    if (deci.size() > (static_cast<unsigned long long>(cover.size().area()) * 3) / 2)
    {
        cout << "File size too big to be encoded in this image";
        exit(0);
    }
    int k = 0;
    for (int i = 0; i < cover.rows; i++)
    {
        for (int j = 0; j < cover.cols; j++)
        {
            if (k >= deci.size())
                break;
            int b = cover.at<Vec3b>(i, j)[0];
            if (b < 250)
            {
                b -= (b % 10);
                b += deci[k];
                hidden.at<Vec3b>(i, j)[0] = b;
                k++;
            }
            if (k >= deci.size())
                break;
            int g = cover.at<Vec3b>(i, j)[1];
            if (g < 250)
            {
                g -= (g % 10);
                g += deci[k];
                hidden.at<Vec3b>(i, j)[1] = g;
                k++;
            }
            if (k >= deci.size())
                break;
            int r = cover.at<Vec3b>(i, j)[2];
            if (r < 250)
            {
                r -= (r % 10);
                r += deci[k];
                hidden.at<Vec3b>(i, j)[2] = r;
                k++;
            }
        }
    }
    string name = ext + to_string(binary.length()) + ".png";
    imwrite(name, hidden);
    imshow("Your Image", cover);
    imshow("Modified Image", hidden);
    waitKey();
    cout << "File Saved Locally with name: " << name;
}
void decoder(string filename)
{
    Mat decoder = imread(filename);
    string bin_file = "";
    vector<int> deco;
    string ext = filename.substr(0, 3);
    filename.erase(0, 3);
    filename.erase(filename.size() - 4, 4);
    int size = stoi(filename);
    int it = 0;
    for (int i = 0; i < decoder.rows; i++)
    {
        for (int j = 0; j < decoder.cols; j++)
        {
            if (it >= size)
                break;
            int b = decoder.at<Vec3b>(i, j)[0];
            if (b < 250)
            {
                deco.push_back(b % 10);
                bin_file += bitset<3>(b % 10).to_string();
                it += 3;
            }
            if (it >= size)
                break;
            int g = decoder.at<Vec3b>(i, j)[1];
            if (g < 250)
            {
                deco.push_back(g % 10);
                bin_file += bitset<3>(g % 10).to_string();
                it += 3;
            }
            if (it >= size)
                break;
            int r = decoder.at<Vec3b>(i, j)[2];
            if (r < 250)
            {
                deco.push_back(r % 10);
                bin_file += bitset<3>(r % 10).to_string();
                it += 3;
            }
        }
    }
    it -= size;
    if (it > 0)
    {
        string a = bin_file.substr(bin_file.length() - 3, 3);
        bin_file.erase(bin_file.length() - 3, 3);
        int d = stoi(a, 0, 2);
        it = 3 - it;
        if (it == 1)
        {
            if (d == 1)
                bin_file += '1';
            else
                bin_file += '0';
        }
        else
            bin_file += bitset<2>(d).to_string();
    }
    string decoded = "decoded." + ext;
    string buffer2 = setStringtoASCII(bin_file);
    ofstream f2(decoded, fstream::trunc | fstream::binary);
    for (auto i : buffer2)
        f2 << i;
    cout << "Decoded File saved locally";
}

int main()
{
    ios_base::sync_with_stdio(false);
    int choice = 0;
    cin.tie(NULL);

    system("cls");
    cout << "MENU: \n";
    cout << "1. ENCODE\n";
    cout << "2. DECODE\n";
    cout << "Enter Choice: ";
    cin >> choice;
    system("cls");
    string filename = "";
    switch (choice)
    {
    case 1:
    {
        string cover;
        cout << "Enter File to be encrypted: ";
        cin >> filename;
        cout << "Enter Filename for Cover Image: ";
        cin >> cover;
        encoder(filename, cover);
        exit(0);
    }
    case 2:
    {
        cout << "Enter File name: ";
        cin >> filename;
        decoder(filename);
        exit(0);
    }
    default:
    {
        cout << "Wrong Choice";
        exit(0);
    }
    }

}
