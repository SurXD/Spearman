#pragma once
#include <iostream>
#include <cmath>
#include<graphics.h>

using namespace std;

typedef struct button{ // ������
   int  x, y, dx, dy;
   IMAGE * bmp;
} Button;
void draw_menu(); //��������� ������ ����
int menu(int st); // ����� ������ ����
void init_menu(); // ������������� ����

void draw_LoseOrWin(int x); // �������� ���������� ������

extern int b; //����
extern IMAGE * bmpEnemy[7]; // ������ ��� ����������� ������
extern IMAGE * bmpITEMS[9]; // ������ ��� ����������� ��������� � ������ ���������
extern IMAGE * bmpBASIC[20]; // ������ ��� ��������� ������� ����������� 