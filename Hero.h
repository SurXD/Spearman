#pragma once
#include"Textures.h"
#include"Room.h"

class Hero {
   private:
      int fqq;
      int x, y; // ���������� ������
      int Damage; // ���� ������
      int HP; // �������� ������
      int LenghtSpear; // ����� �����
      int frameSpear; // ����� ����� ����� �����
      int directionSpear; // ����������� �����
      int frameNoDamage; // ����� ����� ������������
      int Speed; // �������� ������
      IMAGE * Items[4]; // ������ ������ ��������� ������
      int nextItem; // ����� ���������� ��������
   public:
      Hero(){ // �����������
         fqq = 0;
         x = 298; 
         y = 300;
         HP = 3;
         Speed = 12;
         Damage = 10;
         LenghtSpear = 50;
         frameSpear = 0;
         directionSpear = 1;
         frameNoDamage = 0;
         nextItem = 0;
      }
      ~Hero(){for(int i = 0; i < 4; i++) freeimage(Items[i]);} //����������
      int get_x(){ return x;} //�������� ���������� x ������
      int get_y(){ return y;} //�������� ���������� y ������
      int get_HP(){ return HP;} //�������� �������� ������
      void DamageMe(Enemy arr[4], int num); // �������� �� ��������� ����� ������
      void DamageEnemy(Enemy arr[4]); // �������� �� ��������� ����� ������
      void Move(); // �������� ������
      void SpearAttack(); // ����� ������
      void DrawHero(); // ��������� ������ � ��� �����
      void DrawStats(); // ��������� ������������� ������
      int HeroDoor(int f[5], int ID); // ����������� ������ ����� ��������� � ������ ��������
      void DrawItems(); // ��������� ������ ��������� ������
}; // ����� ������


void DrawStat(int x, int y, int stat); // ��������� ����� ��������������