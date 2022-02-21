#include<iostream>

using namespace std;

class PPG {
   int size;

   public:
   int **ary = new int*[size];
   PPG (int k);//constructor
   bool addEdge (int x, int y);
   bool deleteEdge (int x, int y);
};
PPG::PPG (int k)  {
   size = k;
   for (int i = 0; i < k; i++) {
      ary[i] = new int[k];
   }
   for (int i = 0; i < k; i++) {
      for (int j = 0; i < k; i++) {
         ary[i][j] = 0;
      }
   }
}
bool PPG::addEdge  (int x, int y)   {
   ary[x][y] = 1;
   return true;
}
bool PPG::deleteEdge  (int x, int y)   {
   ary[x][y] = 0;
   return true;
}

int main(int argc, char** argv) {

   cout << "HELLO, WORLD!" << endl << endl;
   
   cout << "This program has " << argc << " arguments:" << endl;
   for (int i = 0; i < argc; ++i) {
     cout << i + 1 << ": " << argv[i] << endl;
   }
   cout << endl;
   return 0;
}
