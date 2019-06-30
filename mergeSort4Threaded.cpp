#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void printList (vector<int> list1) {    //print the list
    cout<<"[";
    for (int num: list1) {
        cout<<num<<",";
    }
    cout<<"\b]"<<endl;
}

void merge(vector<int> &list1, int beg, int mid, int last) {
    int n1 = mid - beg + 1;
    int n2 =  last - mid;

    vector<int> L, R;

    //Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L.push_back(list1[beg + i]);
    for (int j = 0; j < n2; j++)
        R.push_back(list1[mid + 1+ j]);

    //Merge the temp arrays back
    int i = 0; // Initial index of Left
    int j = 0; // Initial index of Right
    int k = beg; // Initial index of merged Vector
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { //compares the data
            list1[k] = L[i];
            i++;
        }
        else {
            list1[k] = R[j];
            j++;
        }
        k++;
    }

    //Copy the remaining elements of L, if there are any
    while (i < n1) {
        list1[k] = L[i];
        i++;
        k++;
    }

    //Copy the remaining elements of R, if there are any */
    while (j < n2) {
        list1[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int> &list1, int beg, int last) {
    int i=0;
    if (beg < last) { //check if last element
        int mid = beg+(last-beg)/2; //get the middle of list

        mergeSort(list1, beg, mid); //sort the first half
        mergeSort(list1, mid+1, last);  //sort the second half

        merge(list1, beg, mid, last);
    }
}

void getData (string fileName, vector<int> &list1) {
    string line;
    ifstream theFile (fileName.c_str(), ios::in);
    while (getline(theFile, line)) {
        istringstream buf(line);
        for(string word; buf >> word; ) {
            list1.push_back(stoi(word));    //place all elements into vector
        }
    }
}

int main(){
    string fileName;
    while (fileName!="exit"){
        vector<int> list1;
        //get data from file
        cout<<"input file name: ";
        cin>>fileName;
        getData(fileName, list1);
        //----------------------- Sorting ------------------------//
        auto start = chrono::steady_clock::now();   //mark start time
        cout<<"unsorted list: "<<endl;
        printList(list1);   //output unsorted list
        //slit the process into 4 threads
        thread t1{&mergeSort, ref(list1), 0, list1.size()/4};                   //thread for the 1st quartile
        thread t2{&mergeSort, ref(list1), list1.size()/4+1, list1.size()/2};    //thread for the 2nd quartile
        thread t3{&mergeSort, ref(list1), list1.size()/2+1, list1.size()*3/4};  //thread for the 3rd quartile
        thread t4{&mergeSort, ref(list1), list1.size()*3/4+1, list1.size()-1};  //thread for the 4th quartile
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        merge(list1, 0, list1.size()/4, list1.size()/2);    //join the 1st and 2nd quartile
        merge(list1, list1.size()/2+1, list1.size()*3/4, list1.size()-1);   //join the 3rd and 4th quartile
        merge(list1, 0, list1.size()/2, list1.size()-1);    // join the 2 half of the list
        cout<<"sorted list: "<<endl;
        printList(list1);   //output sorted list
        auto end = chrono::steady_clock::now(); //mark end time
        auto diff = end - start;    //get the difference
        cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    }
    return 0;
}
