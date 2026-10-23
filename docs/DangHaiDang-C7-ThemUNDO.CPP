#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <windows.h>
#include "mylib.h"

using namespace std;
const int MAXLIST = 2000;

const int so_item = 11;
const int dong = 4;
const int cot = 10;
const int Up = 72; // Extended code 
const int Down = 80;

char thucdon [so_item][50] = {"1. Nhap danh sach hoc vien  ",
			                  "2. Liet ke danh sach        ",
			                  "3. Tim kiem hoc vien theo ma",
			                  "4. Xoa hoc vien theo ma so  ",
			                  "5. Xoa hoc vien theo ten    ",
			                  "6. Ghi danh sach            ",
			                  "7. Mo  danh sach            ",
			                  "8. Them sv co thu tu theo ma",
			                  "9. Hieu chinh ho ten theo ma",
			                  "10. Undo (Phuc hoi T/X/S)   ",
			                  "11. Ket thuc chuong trinh   "};

struct Sinhvien {
  int maso;
  char ho [50];
  char ten[10];
};

struct list {
  int n=0;
  Sinhvien *nodes [MAXLIST];
};

// ================= CHỨC NĂNG UNDO (T/X/S) =================
struct UndoNode {
  char type;     // 'T': Them, 'X': Xoa, 'S': Sua
  Sinhvien sv;   // Luu thong tin sinh vien (de phuc hoi hoac thu hoi)
  int vitri;     // Luu vi tri goc trong danh sach
  UndoNode *next;
};

struct UndoStack {
  UndoNode *top = NULL;
  int count = 0;
};

void PushUndo(UndoStack &st, char type, const Sinhvien &sv, int vitri = -1) {
  UndoNode *p = new UndoNode;
  p->type = type;
  p->sv = sv;
  p->vitri = vitri;
  p->next = st.top;
  st.top = p;
  st.count++;
}

bool PopUndo(UndoStack &st, UndoNode &out) {
  if (st.top == NULL) return false;
  UndoNode *p = st.top;
  out = *p;
  st.top = st.top->next;
  delete p;
  st.count--;
  return true;
}

void ClearUndo(UndoStack &st) {
  while (st.top != NULL) {
    UndoNode *p = st.top;
    st.top = st.top->next;
    delete p;
  }
  st.count = 0;
}
// ===============================================================================

void Normal () {
	SetColor(15); // white 
	SetBGColor(0);// black
}
void HighLight () {
	SetColor(15);
	SetBGColor(1); // blue
}

int MenuDong(char td [so_item][50]){
  Normal(); system("cls");
  int chon =0;
  int i; 
  for ( i=0; i< so_item ; i++)
  { gotoxy(cot, dong +i);
    cout << td[i];
  }
  HighLight();
  gotoxy(cot,dong+chon);
  cout << td[chon];
  char kytu;
  do {
  kytu = getch();
  if (kytu==0) kytu = getch();
  switch (kytu) {
    case Up :if (chon >0)
  			  {
  		        Normal(); 
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon --;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  			  }
  			  break;
  	case Down :if (chon <so_item-1)
  			  {
  		        Normal(); 
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon ++;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  			  }
  			  break;
  	case Enter : return chon+1;
  }  // end switch
  } while (1);
}

int Search(list &ds, int x) {
  for (int i =0; i < ds.n ; i++)
     if (ds.nodes[i]->maso == x) return i;
  return -1;
}

void BaoLoi (const char *s){
  int x=wherex() , y=wherey();
  gotoxy (10,24);
  cout <<s;
  Sleep(2000);
  gotoxy(10,24);
  clreol();
  gotoxy(x,y);
}

void NhapChuoi (const char *tieude, char *S) {
	cout << tieude ; 
	int x=wherex(); gotoxy(x, wherey()+1);  fflush(stdin);
    do {
	  gotoxy(x, wherey()-1) ;
	  gets(S); 
    }
    while (strcmp(S,"")==0)  ;	
}

// Hàm nhập hỗ trợ Enter để giữ nguyên thông tin cũ
void NhapChuoiSua (const char *tieude, const char *cu, char *moi, int maxLen) {
    cout << tieude << " [Cu: " << cu << "] (Enter giu nguyen): ";
    char buffer[200] = "";
    int len = 0;
    while (kbhit()) getch(); // Xoa cac phim con ton dong trong bo dem
    
    while (true) {
        char ch = _getch();
        if (ch == 13 || ch == 10) { // Enter
            buffer[len] = '\0';
            cout << "\n";
            break;
        } else if (ch == 8) { //Backspace
            if (len > 0) {
                len--;
                cout << "\b \b";
            }
        } else if (ch == 27) { // ESC: Giu nguyen va ket thuc Field
            buffer[0] = '\0';
            len = 0;
            cout << "\n";
            break;
        } else if (len < maxLen && ((unsigned char)ch >= 32)) {
            buffer[len++] = ch;
            putchar(ch);
        }
    }
    
    // Giu nguyen thong tin cu
    if (len == 0) {
        strcpy(moi, cu);
    } else {
        strcpy(moi, buffer);
    }
}

int NhapSV ( list &ds, Sinhvien &sv){
while (true){
   cout << "Nhap ma so (<=0 la ket thuc) : " ;  
   cin >>sv.maso;                                
   if (sv.maso <=0) return 0;                    
   if (Search(ds, sv.maso)>=0){
       BaoLoi ("Ma so sinh vien bi trung");
       continue;
   }
    NhapChuoi (  "Nhap Ho: ", sv.ho);
    NhapChuoi (  "Nhap Ten:", sv.ten);
    return 1;
}
}

void NhapDSSV (list &ds, int flag, UndoStack &st){
 Sinhvien sv;
 
 if (flag==0) { // nhap moi tu dau
 	while (ds.n> 0){
 	   delete ds.nodes[ds.n-1];
       ds.n-- ;
    } 
    ClearUndo(st);
 }
  
 while (ds.n <MAXLIST)
 {  
    if (NhapSV(ds, sv)==0)  return;
    ds.nodes[ds.n]= new Sinhvien;
    *ds.nodes [ds.n]=sv;
    PushUndo(st, 'T', sv, ds.n); // Ghi nhan Undo Them
    ds.n++;
 }
 if (ds.n==MAXLIST) BaoLoi ("Danh sach day");
}

void LietKe (list &ds) {
 system("cls") ;
 printf ("                     DANH SACH SINH VIEN \n");
 printf ("   Ma so       Ho   va ten \n");
 for (int i =0 ; i < ds.n ; i++)
  printf ("%8d %-30s%-10s\n", ds.nodes[i]->maso, 
     ds.nodes[i]->ho, ds.nodes[i]->ten);
 printf("So luong sinh vien :%d\n",ds.n);
 printf("\nNhan phim bat ky de tiep tuc...");
 getch();
}

void ThongTinSv ( list &ds, int maso ) {
  int i = Search (ds,maso ) ;
  if (i==-1) BaoLoi("Ma so sinh vien khong co trong danh sach");
  else { printf( " Ho ten sv : %30s%10s", ds.nodes[i]->ho, ds.nodes[i]->ten); 
          getch(); }
}

void SaveFile(list &ds, const char *filename) {
 FILE * f; 
 if ((f=fopen(filename,"wb"))==NULL)
 {  BaoLoi ("Loi mo file de ghi"); return ;
 }
  
 for (int i=0; i < ds.n; i++)
   fwrite (ds.nodes[i], sizeof (Sinhvien), 1, f);
 fclose(f);
 BaoLoi ("Da ghi xong danh sach vao file");
}

void OpenFile(list &ds, const char *filename, UndoStack *st = NULL) {
 FILE * f; Sinhvien sv;
 if ((f=fopen(filename,"rb"))==NULL)
 {  BaoLoi ("Loi mo file de doc"); return ;
 }
  
 while (ds.n > 0) {
     delete ds.nodes[ds.n - 1];
     ds.n--;
 }
 if (st != NULL) ClearUndo(*st);

 while  (fread (&sv, sizeof (Sinhvien), 1, f)!=0) {
 	ds.nodes[ds.n]=new Sinhvien;
 	*ds.nodes[ds.n]=sv;
 	ds.n ++;
 }
    
 fclose(f);
 BaoLoi ("Da load xong danh sach vao bo nho");
}

void DeleteItem (list &ds, int i){
	delete  ds.nodes[i]; // chi dung trong mang con tro
    for (int j=i+1; j <ds.n; j++)
       ds.nodes[j-1]=ds.nodes[j];
    ds.n--; 
}

int XoaSv(list &ds, int maso, UndoStack &st){
   int i = Search (ds,maso ) ;
   if (i==-1) return 0;
   PushUndo(st, 'X', *ds.nodes[i], i); // Ghi nhan Undo Xoa
   DeleteItem (ds,  i); 
   return 1;
}

int XoaSvTheoTen(list &ds, char *ten, UndoStack &st){
int dem=0;
for(int i=0; i<ds.n;   )
if(strcmp(ds.nodes[i]->ten,ten)==0)
{
    PushUndo(st, 'X', *ds.nodes[i], i); // Ghi nhan Undo Xoa
	DeleteItem (ds,  i);
	dem++;
}  else i++;
return dem;
}

bool TestIncrease (list &ds){
	for (int i=0; i <ds.n-1; i++)
	  if (ds.nodes [i]->maso > ds.nodes [i+1]->maso) return false;
	return true;
}

int InsertOrder (list &ds, Sinhvien &sv, UndoStack &st){
	int i, k;
	if (ds.n ==MAXLIST ) return 0;
    for ( i=0;i < ds.n && ds.nodes[i]->maso < sv.maso ; i++);
 
	for ( k = ds.n-1; k >=i  ; k--) 
	   ds.nodes[k+1]= ds.nodes[k];
	ds.nodes[i]= new Sinhvien;
	*ds.nodes[i]=sv; 
    ds.n++;
    PushUndo(st, 'T', sv, i); // Ghi nhan Undo Them
	return 1;
}

bool compareSV(Sinhvien* sv1, Sinhvien* sv2) {
	int compareTen = stricmp(sv1->ten, sv2->ten);
	
	if (compareTen < 0) {
        return true;
    } else if (compareTen > 0) {
        return false;
    } else {
        return stricmp(sv1->ho, sv2->ho) < 0;
    }
}

void sort(list &list) {
	int n = list.n;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (!compareSV(list.nodes[j], list.nodes[j + 1])) {
                swap(list.nodes[j], list.nodes[j + 1]);
            }
        }
    }
}

// ================= CHỨC NĂNG 1: HIỆU CHỈNH HỌ TÊN SINH VIÊN =================
void HieuChinhSV(list &ds, UndoStack &st) {
    int maso;
    cout << "\nNhap ma so sinh vien can hieu chinh: ";
    cin >> maso;
    if (cin.fail()) {
        cin.clear();
        fflush(stdin);
        BaoLoi("Ma so khong hop le");
        return;
    }
    int pos = Search(ds, maso);
    if (pos == -1) {
        BaoLoi("Ma so sinh vien khong co trong danh sach");
        return;
    }
    
    // Thong tin sinh vien hien tai
    cout << "\n========= THONG TIN SINH VIEN HIEN TAI =========" << endl;
    cout << "  Ma so : " << ds.nodes[pos]->maso << endl;
    cout << "  Ho    : " << ds.nodes[pos]->ho << endl;
    cout << "  Ten   : " << ds.nodes[pos]->ten << endl;
    cout << "================================================\n" << endl;
    
    char hoMoi[50];
    char tenMoi[10];
    NhapChuoiSua("Nhap Ho moi ", ds.nodes[pos]->ho, hoMoi, 49);
    NhapChuoiSua("Nhap Ten moi", ds.nodes[pos]->ten, tenMoi, 9);
    
    // Kiem tra thong tin co thay doi khong
    if (strcmp(ds.nodes[pos]->ho, hoMoi) != 0 || strcmp(ds.nodes[pos]->ten, tenMoi) != 0) {
        // Ghi nhan truoc khi cap nhat
        PushUndo(st, 'S', *ds.nodes[pos], pos);
        
        strcpy(ds.nodes[pos]->ho, hoMoi);
        strcpy(ds.nodes[pos]->ten, tenMoi);
        
        cout << "\n=> Hieu chinh thanh cong!";
        cout << "\n   Thong tin moi: " << ds.nodes[pos]->ho << " " << ds.nodes[pos]->ten << endl;
        Sleep(1500);
    } else {
        cout << "\n=> Khong co thong tin nao thay doi (giu nguyen)." << endl;
        Sleep(1500);
    }
}

// ================= CHỨC NĂNG 2: PHỤC HỒI DỮ LIỆU UNDO (T/X/S) =================
void ThucHienUndo(list &ds, UndoStack &st) {
    if (st.top == NULL) {
        BaoLoi("Khong co thao tac nao de Undo (Stack rong)!");
        return;
    }
    UndoNode node;
    PopUndo(st, node);
    
    cout << "\n================ KET QUA PHUC HOI UNDO ================" << endl;
    if (node.type == 'T') {
        // Hoan tat -> xoa sv khoi undostack
        int pos = Search(ds, node.sv.maso);
        if (pos != -1) {
            cout << "  [UNDO - THEM]: Da thu hoi (xoa) sinh vien vua them:\n";
            cout << "  Ma so : " << node.sv.maso << " | Ho ten: " << node.sv.ho << " " << node.sv.ten << endl;
            DeleteItem(ds, pos);
        } else {
            cout << "  [UNDO - THEM]: Khong tim thay sinh vien ma " << node.sv.maso << " de thu hoi!" << endl;
        }
    } else if (node.type == 'X') {
        // Hoantat -> chen lai vao vi tri cu
        if (ds.n >= MAXLIST) {
            cout << "  [UNDO - XOA]: Danh sach da day (" << MAXLIST << "), khong the phuc hoi!" << endl;
        } else {
            int vt = node.vitri;
            if (vt < 0 || vt > ds.n) vt = ds.n;
            
            for (int k = ds.n - 1; k >= vt; k--) {
                ds.nodes[k + 1] = ds.nodes[k];
            }
            ds.nodes[vt] = new Sinhvien;
            *ds.nodes[vt] = node.sv;
            ds.n++;
            cout << "  [UNDO - XOA]: Da phuc hoi sinh vien bi xoa:\n";
            cout << "  Ma so : " << node.sv.maso << " | Ho ten: " << node.sv.ho << " " << node.sv.ten << " (tai vi tri " << vt + 1 << ")" << endl;
        }
    } else if (node.type == 'S') {
        // Hoan tat -> khoi phuc lai hoten cu
        int pos = Search(ds, node.sv.maso);
        if (pos != -1) {
            cout << "  [UNDO - SUA]: Da phuc hoi ho ten cu cua sinh vien ma " << node.sv.maso << ":\n";
            cout << "  Tu     : " << ds.nodes[pos]->ho << " " << ds.nodes[pos]->ten << "\n";
            cout << "  Ve lai : " << node.sv.ho << " " << node.sv.ten << endl;
            strcpy(ds.nodes[pos]->ho, node.sv.ho);
            strcpy(ds.nodes[pos]->ten, node.sv.ten);
        } else {
            cout << "  [UNDO - SUA]: Khong tim thay sinh vien ma " << node.sv.maso << " de phuc hoi!" << endl;
        }
    }
    cout << "=======================================================" << endl;
    cout << "Con lai " << st.count << " thao tac trong Undo Stack." << endl;
    cout << "\nNhan phim bat ky de tiep tuc...";
    getch();
}

int main (){ 
  system ("cls");
  char filename[80]="D:\\DSSV.TXT";
  char ten[20]; 
  int chon;  
  list ds;   
  int maso; 
  Sinhvien sv; 
  UndoStack undoStack;

  OpenFile(ds, filename, &undoStack); 
  do
  {
    chon = MenuDong (thucdon); 
    Normal();
    system ("cls");
    switch (chon ){
      case 1: NhapDSSV(ds, 1, undoStack); break;
      case 2: LietKe(ds); break;
      case 3: { 
          printf ("\n Ban nhap ma so sinh vien : ");
          cin >> maso;
          ThongTinSv(ds, maso); 
          break;
      };
      case 4: { 
          cout << "Ban nhap ma sinh vien muon xoa : "; 
          cin >> maso; 
          cout << "Da xoa duoc " << XoaSv(ds, maso, undoStack) << " sinh vien.";
          getch();
          break;
      }; 
      case 5: { 
          NhapChuoi ("Ban nhap ten sinh vien muon xoa : ", ten); 
          cout << "Da xoa duoc " << XoaSvTheoTen(ds, ten, undoStack) << " sinh vien.";
          getch();
          break;
      };    
      case 6: SaveFile(ds, filename); break;
      case 7: OpenFile(ds, filename, &undoStack); break;
      case 8: { 
          if (TestIncrease(ds) == false)
              BaoLoi("Danh sach sinh vien chua co thu tu tang theo ma nen khong the them");
          else if (NhapSV(ds, sv) == 1) 
              InsertOrder(ds, sv, undoStack);
          break;
      };
      case 9: {
          HieuChinhSV(ds, undoStack);
          break;
      };
      case 10: {
          ThucHienUndo(ds, undoStack);
          break;
      };
      case so_item: { 
          SaveFile(ds, filename); 
          ClearUndo(undoStack);
          while (ds.n > 0) {
              delete ds.nodes[ds.n - 1];
              ds.n--;
          }
          return 0;
      }
    }
  } while (true);
  return 0;
}

