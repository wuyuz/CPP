#include<iostream>
using namespace std;

int main() {

	int x2 = 1, x = 1, max, num_max = 0;			// 定义 num_max 为计数器，用于判断有多少个最大值
	cout << "-----------Задача 3 : Сколько элементов этой последовательности равены её наибольшему элементу-----------" << endl;

	while (x != 0) {
		cout << "Вводит целое число : "; cin >> x;
		max = x >= x2 ? x : x2;  // 将最大值赋值到 max 当中 

		if (x > x2) {					// 重新定义 计数器(num_max) 如果新输入的值大于之前的最大值
			num_max = 0;
		}

		if (x == max) {			// 如果输入的x 以前输入的最大值相等，则计数器 + 1
			num_max++;
		}

		x2 = max;					// 将最大值赋值给x2, x2 作为输入新 x 之前的最大值，并且 x2 将用于判断之后输入的 x 值的大小
	}

	cout << "Программа перествает работать!" << endl;
	cout << "В этой последовательности " << num_max << " равены её наибольшему элементу.";

	cin.get();
	return 0;
}