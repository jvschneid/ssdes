#include "ssdes.h"
#include <iostream>
#include <fstream>

SBox::SBox(int lines, int columns, string filename) {
	ifstream arq(filename);

	this->lines = lines;
	this->columns = columns;

	matrix = new int*[lines];

	for (int i = 0; i < lines; i++) {
		matrix[i] = new int[columns];
		for (int j = 0; j < columns; j++) {
			arq >> matrix[i][j];
		}
	}

	arq.close();

}

SBox::~SBox() {
	for (int i = 0; i < lines; i++) {
		delete(matrix[i]);
	}
	delete(matrix);
}

int SBox::get(int position) {
	return matrix[(position & 8) >> 3][position & 7];
}

int E(int Di_minus_one) {
	int first_two_bits = (Di_minus_one & 48) >> 4; 
	int third_bit = (Di_minus_one & 8) >> 3;
	int fourth_bit = (Di_minus_one & 4) >> 2;
	int last_two_bits = Di_minus_one & 3;
	return (first_two_bits << 6) | (fourth_bit << 5) | (third_bit << 4) | (fourth_bit << 3) | (third_bit << 2) | (last_two_bits);
}

int F(int Di_minus_one, int Ki, SBox &s1, SBox &s2) {
	int expanded = E(Di_minus_one);
	int pre_SBoxes = expanded ^ Ki;
	
	return (s1.get((pre_SBoxes & 240) >> 4) << 3) | (s2.get(pre_SBoxes & 15));
}

int round(int block, int Ki, SBox &s1, SBox &s2) {
	int Ei_minus_one = (block & 4032) >> 6;
	int Di_minus_one = (block & 63);

	int Ei = Di_minus_one;
	int Di = Ei_minus_one ^ F(Di_minus_one, Ki, s1, s2);

	return (Ei << 6) | Di;
}