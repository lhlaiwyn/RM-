#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class ImageMatrix {
private:
    vector<vector<int>> matrix;
    int width, height;

public:
  
    void readFile(const string& filename) {
        ifstream infile(filename);
      
        matrix.clear();
        string line;
        while (getline(infile, line)) {
            vector<int> row;
            int value;
            istringstream iss(line);
            while (iss >> value) {
                row.push_back(value);
            }
            matrix.push_back(row);
        }

        height = matrix.size();
        width = matrix[0].size();
        infile.close();
    }

  
    void display() const {
        for (const auto& row : matrix) {
            for (const auto& pixel : row) {
                cout << pixel << " ";
            }
            cout << endl;
        }
    }

   
    void binaryDisplay() const {
        for (const auto& row : matrix) {
            for (const auto& pixel : row) {
                if (pixel == 0)
                    cout << ".";
                else
                    cout << "O";
            }
            cout << endl;
        }
    }

    void threshold(int thr) {
        for (auto& row : matrix) {
            for (auto& pixel : row) {
                if (pixel <= thr)
                    pixel = 0;
            }
        }
    }

    void flip() {
        for (auto& row : matrix) {
            reverse(row.begin(), row.end());
        }
    }

    void rotate() {
        vector<vector<int>> rotated(width, vector<int>(height));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                rotated[j][height - i - 1] = matrix[i][j];
            }
        }
        matrix = rotated;
        swap(width, height);


    }
};

int main() {
   
    ImageMatrix img;
    img.readFile("C:/path/to/your/image_matrix.txt.txt");

    img.readFile("C:/path/to/your/image_matrix.txt.txt");
    cout << "\n图像\n";
    img.display();
    
    cout << "\n阈值化处理\n";
    img.threshold(100);  
    img.display();

    cout << "\n翻转\n";
    img.flip();
    img.display();

    cout << "\n旋转\n";
    img.rotate();
    img.display();

    cout << "\n二值化\n";
    img.binaryDisplay();

    return 0;
}