//
// Created by didi on 2022/7/4.
//

#include "iostream"
#include "Utils.h"
#include "string"

using namespace std;

//数组传递值的第一种办法
void testArray (float * testArray,int length)
{
    int sizeoftestArray = sizeof (testArray);
    int lengthComputer = sizeof(testArray)/ sizeof(float );
    cout << "testArray length sizeof :" <<sizeoftestArray <<endl;
    cout << "lengthComputer  :" <<lengthComputer <<endl;
    for(int i = 0;i<length;i++){
        cout << "testArray :" << testArray[i] <<endl;
    }
}
void testPointer(float * array,int *intArray){
    cout<<"testPointer float array pointer length : "<< sizeof(array)<<endl;
    cout<<"testPointer int array pointer length : "<< sizeof(intArray)<<endl;
}

int main() {
    cout<< "testMain"<<endl;
    float  test1[]  = {
            1.0f,23.0f
    };
    int test2[] = {
            1,2,3,4,5
    };
    cout<<"float array:" <<sizeof (test1)<< endl;
    cout<<"int array:" <<sizeof (test2)<< endl;

    testPointer(test1,test2);

//    testArray(test1,2);

    Utils utils;
    string path = "D:\\code\\clion_workspace\\openGlLearingForCPlusPlus\\glsl\\cpp_vertex.glsl";
    string result = utils.readShaderFromFile(const_cast<char *>(path.c_str()));
    cout <<"result:" << result<<endl;
}