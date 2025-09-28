#pragma once

class Stage{
   private:
      int stage[6][6] = {0}; // ��������� �����
      int xi, yj; // ���������� ������ �� �����
      int stagemap[6][6] = {0}; // ����-����� �����
      int ItemID; // ID ��������
      int f[4] = {0}; //������ ������������� �������� ������
   public:
      int getRoom(){ return stage[xi][yj];} // �������� ��� �������
      int getItemID() {return ItemID;} // �������� ID ��������
      int* getF() {return f;} // �������� ������ ������������� �������� ������
      void printMap(); // ��������� ����-�����
      void initStage(); // �������� �����
      void printDoorsAndItems(int x); // ��������� ������ � ��������� 
      int StageMove(int x); // ����������� ������ �� ����� � ������ ��������
}; // ����� ��� �����