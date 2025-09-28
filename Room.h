#pragma once
#include"Textures.h"

struct Enemy{
   int xe, ye; // ���������� �����
   int eHP; // �������� �����
   double startHP; // ��������� �������� �����
   int eSpeed; // �������� �����
   IMAGE * bmpe; // ����������� �����
}; // ��������� �����

typedef struct ES{
   Enemy mas[16][4];
   int nums[16];
} ES; // ��������� �������� ������


class Room {
   private:
      Enemy* arr; // ������ ������
      int num; // ���������� ������
      int open; //  ���� �������� �����
   public:
      Room(){ // �����������
         arr = new Enemy[4];
         num = 0;
         open = 0;
         for(int i = 0; i < 4; i++){
            arr[i].xe = 0; arr[i].ye = 0; arr[i].eHP = 0; arr[i].eSpeed = 0;
         }
      }
      ~Room(){ for(int i = 0; i < 4; i++) freeimage(arr[i].bmpe); } //����������
      int get_num(){ return num; } // �������� ���������� ������
      int get_open(){ return open; } //  �������� ���� �������� �����
      Enemy* getArr(){ return arr;} // �������� ������ ������ � �������
      void initRoom(int x, ES a); // �������� �������
      void OpenRoom(); // �������� �� �������� ������ � �������
      void Moved(int x, int y); // �������� ������
      void printEnemys(); // ��������� ������
}; // ����� ��� �������


void init_ES(Enemy normal[]);// ������������� ���� ������